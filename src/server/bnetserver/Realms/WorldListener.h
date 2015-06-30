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

#ifndef WorldListener_h__
#define WorldListener_h__

#include "ZmqSocket.h"
#include "Commands.h"

class WorldListener
{
public:
    explicit WorldListener(uint16 worldListenPort);
    ~WorldListener();
    void Run(boost::asio::io_service& ioService);
    void End();

private:
    void Dispatch(ByteBuffer& msg);

    typedef void(WorldListener::*PacketHandler)(Battlenet::RealmHandle const& realm, ByteBuffer& msg) const;
    class HandlerTable
    {
    public:
        HandlerTable();

        struct HandlerInfo
        {
            PacketHandler Handler;
            char const* Name;
        };

        HandlerInfo const& operator[](uint8 opcode) const { return _handlers[opcode]; }

    private:
        HandlerInfo _handlers[IPC_BNET_MAX_COMMAND];
    };

    void HandleToonOnlineStatusChange(Battlenet::RealmHandle const& realm, ByteBuffer& msg) const;

    std::shared_ptr<ZmqSocket> _worldSocket;
    uint16 _worldListenPort;
    static HandlerTable const _handlers;
};

#endif // WorldListener_h__
