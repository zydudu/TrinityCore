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

#include "Log.h"
#include "SessionManager.h"
#include "WoWRealmPackets.h"
#include "WorldListener.h"

WorldListener::HandlerTable const WorldListener::_handlers;

WorldListener::HandlerTable::HandlerTable()
{
#define DEFINE_HANDLER(opc, func) _handlers[opc] = { func, #opc }

    DEFINE_HANDLER(BNET_CHANGE_TOON_ONLINE_STATE, &WorldListener::HandleToonOnlineStatusChange);

#undef DEFINE_HANDLER
}

WorldListener::WorldListener(uint16 worldListenPort) : _worldListenPort(worldListenPort)
{
}

WorldListener::~WorldListener()
{
}

void WorldListener::Run(boost::asio::io_service& ioService)
{
    _worldSocket = std::make_shared<ZmqSocket>(ioService, ZMQ_PULL, false);
    _worldSocket->Bind(std::string("tcp://*:") + std::to_string(_worldListenPort));
    TC_LOG_INFO("server.ipc", "Listening on connections from worldservers...");

    _worldSocket->AsyncRead(std::bind(&WorldListener::Dispatch, this, std::placeholders::_1));
}

void WorldListener::End()
{
    _worldSocket->CloseSocket();
    _worldSocket.reset();
    TC_LOG_INFO("server.ipc", "Shutting down connections from worldservers...");
}

void WorldListener::Dispatch(ByteBuffer& msg)
{
    Battlenet::Header ipcHeader;
    msg >> ipcHeader;

    if (ipcHeader.Ipc.Channel != IPC_CHANNEL_BNET)
        return;

    if (ipcHeader.Ipc.Command < IPC_BNET_MAX_COMMAND)
        (this->*_handlers[ipcHeader.Ipc.Command].Handler)(ipcHeader.Realm, msg);
}

void WorldListener::HandleToonOnlineStatusChange(Battlenet::RealmHandle const& realm, ByteBuffer& msg) const
{
    Battlenet::ToonHandle toonHandle;
    bool online;
    msg >> toonHandle;
    msg >> online;

    if (Battlenet::Session* session = sSessionMgr.GetSession(toonHandle.AccountId, toonHandle.GameAccountId))
    {
        if (online)
        {
            if (!session->IsToonOnline())
            {
                Battlenet::WoWRealm::ToonReady* toonReady = new Battlenet::WoWRealm::ToonReady();
                toonReady->Realm.Battlegroup = realm.Battlegroup;
                toonReady->Realm.Index = realm.Index;
                toonReady->Realm.Region = realm.Region;
                toonReady->Guid = toonHandle.Guid;
                toonReady->Name = toonHandle.Name;
                session->SetToonOnline(true);
                session->AsyncWrite(toonReady);
            }
        }
        else if (session->IsToonOnline())
        {
            session->AsyncWrite(new Battlenet::WoWRealm::ToonLoggedOut());
            session->SetToonOnline(false);
        }
    }
}
