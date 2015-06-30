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

#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "Define.h"
#include <string>

class ByteBuffer;

enum Channels
{
    IPC_CHANNEL_BNET,

    MAX_IPC_CHANNELS,
};

enum BnetCommands
{
    BNET_CHANGE_TOON_ONLINE_STATE,

    IPC_BNET_MAX_COMMAND
};

struct IPCHeader
{
    uint8 Channel;
    uint8 Command;
};

namespace Battlenet
{
    struct RealmHandle
    {
        uint8 Region;
        uint8 Battlegroup;
        uint32 Index;
    };

    struct Header
    {
        IPCHeader Ipc;
        RealmHandle Realm;
    };

    struct ToonHandle
    {
        uint32 AccountId;
        uint32 GameAccountId;
        uint64 Guid;
        std::string Name;
    };
}

ByteBuffer& operator>>(ByteBuffer& msg, IPCHeader& header);
ByteBuffer& operator>>(ByteBuffer& msg, Battlenet::RealmHandle& realm);
ByteBuffer& operator>>(ByteBuffer& msg, Battlenet::Header& header);
ByteBuffer& operator>>(ByteBuffer& msg, Battlenet::ToonHandle& toonHandle);

ByteBuffer& operator<<(ByteBuffer& msg, IPCHeader const& header);
ByteBuffer& operator<<(ByteBuffer& msg, Battlenet::RealmHandle const& realm);
ByteBuffer& operator<<(ByteBuffer& msg, Battlenet::Header const& header);
ByteBuffer& operator<<(ByteBuffer& msg, Battlenet::ToonHandle const& toonHandle);

#endif // _COMMANDS_H
