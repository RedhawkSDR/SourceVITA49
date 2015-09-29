/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK core.
 *
 * REDHAWK core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK core is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include "unicast.h"

/* it is probably desirable to convert to C++ and throw exceptions instead. */
static inline void verify_ (int condition, const char* message, const char* condtext, const char* file, int line) {
    if (!condition) {
        char msg[100];
        sprintf(msg, "Verify failed '%s' at line %d: %s (%s)\n", file, line, message, condtext);
        fprintf(stderr, msg);
        perror("perror");
        throw(BadParameterError3(msg));
    }
}
#define verify(CONDITION, MESSAGE) verify_(CONDITION, MESSAGE, #CONDITION, __FILE__, __LINE__)


static unicast_t unicast_open_ (const char* iface, const char* group, int port, int isClient)
{
    unsigned int ii;

    unicast_t unicast = { socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) };
    verify(unicast.sock >= 0, "create socket");
    int one = 1;
    verify(setsockopt(unicast.sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == 0, "reuse address");
    /* Enumerate all the devices. */
    struct ifconf devs = {0};
    devs.ifc_len = 512*sizeof(struct ifreq);
    devs.ifc_buf = (char*)malloc(devs.ifc_len);
    verify(devs.ifc_buf != 0, "memory allocation");
    verify(ioctl(unicast.sock, SIOCGIFCONF, &devs) >= 0, "enum devices");
    for (ii = 0; ii<devs.ifc_len/sizeof(struct ifreq); ii++) {
        bool any = (!*iface);
        bool any_interface_vlan_match = false;
        if(*iface && iface[0] == '.'){
            size_t len_dev = strlen(devs.ifc_req[ii].ifr_ifrn.ifrn_name);
            size_t len_iface = strlen(iface);
            if(len_dev >= len_iface && !strcmp(devs.ifc_req[ii].ifr_ifrn.ifrn_name + len_dev-len_iface,iface))
                any_interface_vlan_match = true;
        }
        bool interface_exact_match = (strcmp(iface, devs.ifc_req[ii].ifr_ifrn.ifrn_name) == 0);
        if (any || any_interface_vlan_match || interface_exact_match) {
            try{
                struct ifreq dev = devs.ifc_req[ii];
                verify(ioctl(unicast.sock, SIOCGIFFLAGS, &dev) >= 0, "get flags");
                verify(dev.ifr_flags & IFF_UP, "interface up");
                //verify(!(dev.ifr_flags & IFF_LOOPBACK), "not loopback");
                verify(ioctl(unicast.sock, SIOCGIFINDEX, &dev) == 0, "get index");

                unicast.addr.sin_family = AF_INET;
                unicast.addr.sin_addr.s_addr = inet_addr(group);//mreqn.imr_multiaddr.s_addr;
                unicast.addr.sin_port = htons(port);
                if (isClient) {
                    if (bind(unicast.sock, (struct sockaddr*)&unicast.addr, sizeof(struct sockaddr)) < 0){
                        char str[INET_ADDRSTRLEN];
                        inet_ntop(unicast.addr.sin_family, &(unicast.addr.sin_addr), str, INET_ADDRSTRLEN);
                        printf(" Unable to bind socket (%i) to address (%s) \n", unicast.sock, str);
                    }
                }

                free(devs.ifc_buf);
                return unicast;
            }catch(...){};
        }
    }
    printf("Closing unicast.sock\n");

    /* If we get here, we've failed. */
    close(unicast.sock);
    free(devs.ifc_buf);
    unicast.sock = -1;

    return unicast;
}


unicast_t unicast_client (const char* iface, const char* group, int port)
{
    unicast_t client = unicast_open_(iface, group, port, 1);
    if (client.sock != -1) {
        int size = 128*1024*1024;
        verify(setsockopt(client.sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int)) == 0, "set recvbuf size");
    }
    return client;
}


ssize_t unicast_receive (unicast_t client, void* buffer, size_t bytes, unsigned int to_in_msecs)
{
    size_t bytes_read = 0;
    int flags = unicast_poll_in(client, to_in_msecs); //0 is MSG_DONTWAIT
    if (flags)
        bytes_read = recv(client.sock, buffer, bytes, flags);
    return bytes_read;
}


unicast_t unicast_server (const char* iface, const char* group, int port)
{
    unicast_t server = unicast_open_(iface, group, port, 0);
    if (server.sock != -1) {
        uint8_t ttl = 32;
        verify(setsockopt(server.sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == 0, "set ttl");
    }
    return server;
}


ssize_t unicast_transmit (unicast_t server, const void* buffer, size_t bytes)
{
    return sendto(server.sock, buffer, bytes, 0, (struct sockaddr*)&server.addr, sizeof(server.addr));
}


void unicast_close (unicast_t socket)
{
    close(socket.sock);
}


int unicast_poll_in (unicast_t client, int to_in_msecs)
{
    int _flags = 0;
    if( to_in_msecs > 0){
        struct pollfd pfd;
        pfd.fd = client.sock;
        pfd.events = POLLIN;
        int rval = poll(&pfd, 1, (int)to_in_msecs);

        if (rval > 0)
            _flags = MSG_DONTWAIT;
        else if( rval < 0)
        {
            throw BadParameterError3("Error configuring poll");
        }
        else
        {
            //use the val returned
        }
    }
    else if( to_in_msecs == 0){
        _flags = MSG_DONTWAIT;
    }
    else
    {
        _flags = MSG_WAITALL;
    }
    return _flags;
}
