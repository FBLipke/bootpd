/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

namespace bootp
{
    class IPacket;

    class IBootpd
    {
    public:
        virtual _BOOL Init(const _INT32 &argc, const char *argv[]) = 0;
        virtual _BOOL Start() = 0;
        virtual void HeartBeat() = 0;
        virtual void Close() = 0;

        // Statische SubSysteme - für alle Manager zugänglich
        static std::map<_STRING, std::unique_ptr<IBootpd>> _subSystems;
        std::function<void(const _STRING &server_id, const _STRING &socket_id, const std::shared_ptr<bootp::Network::IPacket> &request, const _IPADDR &ip, const _USHORT &port, const _STRING &)> Handle_Manager_Request;
        std::function<void(const _STRING &server_id, const _STRING &socket_id, const std::shared_ptr<bootp::Network::IPacket> &request, const _IPADDR &ip, const _USHORT &port, const _STRING &)> Handle_Manager_Response;
    };
}