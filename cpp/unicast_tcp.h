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

#ifndef UNICAST_TCP_H_
#define UNICAST_TCP_H_

#include <arpa/inet.h>
#include <stdexcept>

class BadParameterError4 : public std::runtime_error {
       	public:
       		BadParameterError4(const std::string& what_arg) : std::runtime_error(what_arg) {
	        }
    };

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
  int sock;
  struct sockaddr_in addr;
} unicast_tcp_t;

unicast_tcp_t unicast_tcp_accept (unicast_tcp_t server);
unicast_tcp_t unicast_tcp_client (const char* iface, const char* group, int port);
ssize_t unicast_tcp_receive (unicast_tcp_t client, void* buffer, size_t bytes, unsigned int to_in_msecs= 0);
unicast_tcp_t unicast_tcp_server (const char* iface, const char* group, int port);
ssize_t unicast_tcp_transmit (unicast_tcp_t server, const void* buffer, size_t bytes);
int unicast_tcp_poll_in (unicast_tcp_t client, int timeout);
void unicast_tcp_close(unicast_tcp_t socket);

#ifdef __cplusplus
}
#endif



#endif /* UNICAST_TCP_H_ */
