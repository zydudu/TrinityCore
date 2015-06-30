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

#include "Commands.h"
#include "ByteBuffer.h"

ByteBuffer& operator>>(ByteBuffer& msg, IPCHeader& header)
{
    msg >> header.Channel;
    msg >> header.Command;
    return msg;
}

ByteBuffer& operator>>(ByteBuffer& msg, Battlenet::RealmHandle& realm)
{
    msg >> realm.Region;
    msg >> realm.Battlegroup;
    msg >> realm.Index;
    return msg;
}

ByteBuffer& operator>>(ByteBuffer& msg, Battlenet::Header& header)
{
    msg >> header.Ipc;
    msg >> header.Realm;
    return msg;
}

ByteBuffer& operator>>(ByteBuffer& msg, Battlenet::ToonHandle& toonHandle)
{
    msg >> toonHandle.AccountId;
    msg >> toonHandle.GameAccountId;
    msg >> toonHandle.Guid;
    msg >> toonHandle.Name;
    return msg;
}

ByteBuffer& operator<<(ByteBuffer& msg, IPCHeader const& header)
{
    msg << header.Channel;
    msg << header.Command;
    return msg;
}

ByteBuffer& operator<<(ByteBuffer& msg, Battlenet::RealmHandle const& realm)
{
    msg << realm.Region;
    msg << realm.Battlegroup;
    msg << realm.Index;
    return msg;
}

ByteBuffer& operator<<(ByteBuffer& msg, Battlenet::Header const& header)
{
    msg << header.Ipc;
    msg << header.Realm;
    return msg;
}

ByteBuffer& operator<<(ByteBuffer& msg, Battlenet::ToonHandle const& toonHandle)
{
    msg << toonHandle.AccountId;
    msg << toonHandle.GameAccountId;
    msg << toonHandle.Guid;
    msg << toonHandle.Name;
    return msg;
}
