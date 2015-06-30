/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ZmqSocket_h__
#define ZmqSocket_h__

#include "Common.h"

#pragma warning(push)
#pragma warning(disable:4100)
#include <azmq/socket.hpp>
#pragma warning(pop)

class ZmqSocket : public std::enable_shared_from_this<ZmqSocket>
{
public:
    ZmqSocket(boost::asio::io_service& ioService, int type, bool singleThreadedIoService) : _socket(ioService, type, singleThreadedIoService),
        _closed(false)
    {
    }

    void AsyncRead(std::function<void(ByteBuffer&)> callback)
    {
        if (_closed)
            return;

        _socket.async_receive(std::bind(&ZmqSocket::ReadHandlerInternal, shared_from_this(), std::move(callback),
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    void AsyncWrite(ByteBuffer const& data)
    {
        if (_closed)
            return;

        azmq::message msg(boost::asio::buffer(data.contents(), data.size()));

        _socket.async_send(msg, [](boost::system::error_code const& /*error*/, std::size_t /*transferedBytes*/) { });
    }

    void Connect(std::string const& address)
    {
        if (_closed)
            return;

        boost::system::error_code connectError;
        _socket.connect(address, connectError);
    }

    void Bind(std::string const& address)
    {
        if (_closed)
            return;

        boost::system::error_code bindError;
        _socket.bind(address, bindError);
    }

    void CloseSocket()
    {
        if (_closed.exchange(true))
            return;

        _socket.cancel();
    }

private:
    void ReadHandlerInternal(std::function<void(ByteBuffer&)> callback, boost::system::error_code const& errorCode, azmq::message& msg, size_t /*bytesTransferred*/)
    {
        if (errorCode || _closed)
            return;

        if (size_t size = msg.size())
        {
            ByteBuffer buf;
            buf.resize(size);
            msg.buffer_copy(boost::asio::buffer(buf.contents(), size));

            callback(buf);
        }

        AsyncRead(std::move(callback));
    }

    azmq::socket _socket;
    std::atomic<bool> _closed;
};

#endif // ZmqSocket_h__
