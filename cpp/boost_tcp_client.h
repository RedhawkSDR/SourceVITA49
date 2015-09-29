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

#ifndef BOOST_TCP_CLIENT_H_
#define BOOST_TCP_CLIENT_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include "debuggable.h"

using boost::asio::ip::tcp;

class TCPClient : public Debuggable
{
public:
    TCPClient(unsigned short port, const std::string& ip_addr);
    virtual ~TCPClient() { 
        close(); 
    }; 

    bool connect();
    bool isConnected();
    void close();

    template<typename T>
    int read(std::vector<char, T> & data, size_t bytesToRead=0)
    {
        int emptyReadCount = 0;
        boost::system::error_code error;
        std::stringstream ss;

        if (bytesToRead == 0 || bytesToRead > data.size()) {
            bytesToRead = data.size();
        }

        if (_readyToRead()) {
            while (bytesToRead > 0) {
                try {
                    int bytes = this->_socket.read_some(boost::asio::buffer(&data[data.size()-bytesToRead], bytesToRead), error);
                    ss << "TCPClient::read - Bytes Received by socket read: " << bytes;
                    this->_printDebug(ss.str());
                    bytesToRead -= bytes;

                    if (bytes == 0) {
                        ++emptyReadCount;
                    } else {
                        emptyReadCount = 0;
                    }

                    if (emptyReadCount == 10) {
                        std::cout << "Received 10 empty packets..." << std::endl;
                        break;
                    }
                } catch (std::exception& ex) {
                    std::cerr << "TCPClient::read - Exception occured while reading from socket: " << ex.what() << std::endl;
                    return 0;
                }
            }
        }

        // Handle read errors
        if (error == boost::asio::error::eof) {
            this->_printDebug("TCPClient::read - Connection closed by server");
            this->_isConnected = false;
        } else if (error) {
            std::cerr << "TCPClient::read - Error occured while reading: " << error << std::endl;
        }

        return (data.size() - bytesToRead);
    }

    template<typename T>
    size_t peek(std::vector<char, T> & data)
    {
        int bytesReceived=0;
        boost::system::error_code error;
        std::stringstream ss;

        if (_readyToRead()) {
            try {
                bytesReceived = this->_socket.receive(boost::asio::buffer(data), boost::asio::socket_base::message_peek, error);
            } catch (std::exception& ex) {
                std::cerr << "TCPClient::peek - Exception occured while receiving from socket: " << ex.what() << std::endl;
            }
        }

        // Handle read errors
        if (error == boost::asio::error::eof) {
            this->_printDebug("TCPClient::read - Connection closed by server");
            this->_isConnected = false;
        } else if (error) {
            std::cerr << "TCPClient::read - Error occured while reading: " << error << std::endl;
        }
        return bytesReceived;
    }

protected:
    bool _readyToRead();
    unsigned short _port;
    std::string _ipAddr;
    boost::asio::io_service _ioService;
    tcp::socket _socket;
    bool _isConnected;
};


#endif /* BOOST_TCP_CLIENT_H_ */
