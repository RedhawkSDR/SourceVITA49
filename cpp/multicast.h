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

#ifndef MULTICAST_H_
#define MULTICAST_H_

#include <arpa/inet.h>
#include <stdexcept>

class BadParameterError : public std::runtime_error {
public:
    BadParameterError(const std::string& what_arg) : std::runtime_error(what_arg) {
    }
};

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int sock;
    struct sockaddr_in addr;
} multicast_t;

multicast_t multicast_client (const char* iface, const char* group, int port);
ssize_t multicast_receive (multicast_t client, void* buffer, size_t bytes, unsigned int to_in_msecs= 0);
multicast_t multicast_server (const char* iface, const char* group, int port);
ssize_t multicast_transmit (multicast_t server, const void* buffer, size_t bytes);
int multicast_poll_in (multicast_t client, int timeout);
void multicast_close(multicast_t socket);

#ifdef __cplusplus
}
#endif



#endif /* MULTICAST_H_ */
