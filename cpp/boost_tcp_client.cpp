/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file distributed with this 
 * source distribution.
 * 
 * This file is part of REDHAWK Basic Components sourcesocket.
 * 
 * REDHAWK Basic Components sourcesocket is free software: you can redistribute it and/or modify it under the terms of 
 * the GNU Lesser General Public License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 * 
 * REDHAWK Basic Components sourcesocket is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License along with this 
 * program.  If not, see http://www.gnu.org/licenses/.
 */

#include "boost_tcp_client.h"

TCPClient::TCPClient(unsigned short port, const std::string& ip_addr) :
Debuggable(),
_port(port),
_ipAddr(ip_addr),
_ioService(),
_socket(_ioService),
_isConnected(false)
{}

bool TCPClient::connect() {

    // Don't attempt to connect if connection already established
    if (this->_isConnected) {
        this->_printDebug("TCPClient::connect - Cannot connect - Connection already exists");
        return false;
    }

    try {
        tcp::resolver resolver(this->_ioService);
        std::stringstream ss;
        ss << _port;
        tcp::resolver::query query(this->_ipAddr, ss.str());
        tcp::resolver::iterator endpoint_iter = resolver.resolve(query);
        this->_socket.connect(*endpoint_iter);
        this->_printDebug("TCPClient::connect - Successfully connected");
        this->_isConnected = true;
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        this->_printDebug("TCPClient::connect - Failed to connect");
        this->close();
    }
    return (this->_isConnected);
}

void TCPClient::close() {
    if (!this->_isConnected) {
        return;
    }

    boost::system::error_code ec;

    try {
        this->_socket.shutdown(tcp::socket::shutdown_send, ec);
        if (ec) this->_printDebug("TCPClient::close - Failed to shutdown socket");

        this->_socket.close(ec);
        if (ec) this->_printDebug("TCPClient::close - Failed to close socket");
    } catch (std::exception& ex) {
        std::cerr << "TCPClient::close - Exception occurred while closing socket: " << ex.what() << std::endl;
    }

    this->_isConnected = false;
}

bool TCPClient::isConnected() {
    return (this->_isConnected);
}

bool TCPClient::_readyToRead() {
    if (this->_isConnected == false) connect();
    if (this->_isConnected == false) {
        this->_printDebug("TCPClient::_readyToRead - isConnected? NO");
    }
    return (this->_isConnected) && (this->_socket.available() != 0);
}
