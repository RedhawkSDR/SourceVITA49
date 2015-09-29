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
#include "multicast.h"
#include <errno.h>
/* it is probably desirable to convert to C++ and throw exceptions instead. */
static inline void verify_ (int condition, const char* message, const char* condtext, const char* file, int line) {
    if (!condition) {
        char msg[100];
        sprintf(msg, "Verify failed '%s' at line %d: %s (%s)\n", file, line, message, condtext);
        fprintf(stderr, msg);
        perror("perror");
        throw(BadParameterError(msg));
    }
}
#define verify(CONDITION, MESSAGE) verify_(CONDITION, MESSAGE, #CONDITION, __FILE__, __LINE__)


static multicast_t multicast_open_ (const char* iface, const char* group, int port)
{
    unsigned int ii;

    multicast_t multicast = { socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) };
    verify(multicast.sock >= 0, "create socket");
    int one = 1;
    verify(setsockopt(multicast.sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == 0, "reuse address");

    /* Enumerate all the devices. */
    struct ifconf devs = {0};
    devs.ifc_len = 512*sizeof(struct ifreq);
    devs.ifc_buf = (char*)malloc(devs.ifc_len);
    verify(devs.ifc_buf != 0, "memory allocation");
    verify(ioctl(multicast.sock, SIOCGIFCONF, &devs) >= 0, "enum devices");
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
                verify(ioctl(multicast.sock, SIOCGIFFLAGS, &dev) >= 0, "get flags");
                verify(dev.ifr_flags & IFF_UP, "interface up");
                verify(!(dev.ifr_flags & IFF_LOOPBACK), "not loopback");
                verify(dev.ifr_flags & IFF_MULTICAST, "must be multicast");

                verify(ioctl(multicast.sock, SIOCGIFINDEX, &dev) == 0, "get index");
                struct ip_mreqn mreqn;
                memset(&mreqn, 0, sizeof(mreqn));
                verify(inet_aton(group, &mreqn.imr_multiaddr), "convert string to group");
                mreqn.imr_address = ((struct sockaddr_in*)(&dev.ifr_addr))->sin_addr;
                mreqn.imr_ifindex = dev.ifr_ifindex;
                verify(setsockopt(multicast.sock, IPPROTO_IP, IP_MULTICAST_IF, &mreqn, sizeof(struct ip_mreqn)) == 0, "set device");

                multicast.addr.sin_family = AF_INET;
                multicast.addr.sin_addr.s_addr = mreqn.imr_multiaddr.s_addr;
                multicast.addr.sin_port = htons(port);
                verify(bind(multicast.sock, (struct sockaddr*)&multicast.addr, sizeof(struct sockaddr_in)) == 0, "socket bind");
                if (!((multicast.addr.sin_addr.s_addr & 0x000000FF) < 224) || ((multicast.addr.sin_addr.s_addr & 0x000000FF) > 239)) {
                    verify(setsockopt(multicast.sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqn, sizeof(struct ip_mreqn)) == 0, "igmp join");
                }

                free(devs.ifc_buf);
                return multicast;
            }catch(...){};
        }
    }
    printf("Closing multicast.sock\n");

    /* If we get here, we've failed. */
    close(multicast.sock);
    free(devs.ifc_buf);
    multicast.sock = -1;

    return multicast;
}


multicast_t multicast_client (const char* iface, const char* group, int port)
{
    multicast_t client = multicast_open_(iface, group, port);
    if (client.sock != -1) {
        int size = 128*1024*1024;
        verify(setsockopt(client.sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int)) == 0, "set recvbuf size");
    }
    return client;
}


ssize_t multicast_receive (multicast_t client, void* buffer, size_t bytes, unsigned int to_in_msecs)
{
    size_t bytes_read = 0;

    int flags = multicast_poll_in(client,to_in_msecs);
    if (flags)
        bytes_read = recv(client.sock, buffer, bytes, flags);
    return bytes_read;
}


multicast_t multicast_server (const char* iface, const char* group, int port)
{
    multicast_t server = multicast_open_(iface, group, port);
    if (server.sock != -1) {
        uint8_t ttl = 32;
        verify(setsockopt(server.sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) == 0, "set ttl");
    }
    return server;
}


ssize_t multicast_transmit (multicast_t server, const void* buffer, size_t bytes)
{
    return sendto(server.sock, buffer, bytes, 0, (struct sockaddr*)&server.addr, sizeof(server.addr));
}


void multicast_close (multicast_t socket)
{
    close(socket.sock);
}


int multicast_poll_in (multicast_t client, int to_in_msecs)
{
    int _flags = 0;
    if( to_in_msecs > 0){
        struct pollfd pfd;
        pfd.fd = client.sock;
        pfd.events = POLLIN;
        int rval =0;
        do{
            rval= poll(&pfd, 1, (int)to_in_msecs);
        } while(rval < 0 && errno==EINTR);

        if (rval > 0)
            _flags = MSG_DONTWAIT;
        else if( rval < 0)
        {
            throw BadParameterError("Error configuring poll");
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
