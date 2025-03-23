/**
*    This is a lightweight chat client.
*    Copyright (C) 2022-2025  氢聊-Hcolda.com
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef BASE_MAIN_WINDOW_H
#define BASE_MAIN_WINDOW_H

#include <functional>
#include <string>
#include <system_error>

namespace qingliao
{
    using SendPrivateRoomMessageFunc = std::function<void(long long, const std::string&)>;
    using SendGroupRoomMessageFunc = std::function<void(long long, const std::string&)>;
    using SendCommonMessageFunc = std::function<void(const std::string&)>;

    enum class MessageType
    {
        DEFAULT_MESSAGE = 0,
        TIP_MESSAGE
    };

    class BaseMainWindow
    {
    public:
        BaseMainWindow() = default;
        virtual ~BaseMainWindow() = default;

        virtual void run() {}

        virtual bool addPrivateRoom(long long user_id) { return false; }
        virtual bool romovePrivateRoom(long long user_id) { return false; }
        virtual bool addGroupRoom(long long room_id) { return false; }
        virtual bool removeGroupRoom(long long room_id) { return false; }

        virtual void addPrivateRoomMessage(long long user_id, MessageType type, const std::string& message) {}
        virtual bool removePrivateRoomMessage(size_t index) { return false; }
        virtual void addGroupRoomMessage(long long group_id, long long sender_id, MessageType type, const std::string& message) {}
        virtual bool removeGroupRoomMessage(size_t index) { return false; }

        virtual void setPrivateRoomMessageCallback(SendPrivateRoomMessageFunc func) {}
        virtual void setGroupRoomMessageCallback(SendGroupRoomMessageFunc func) {}
        virtual void setCommonMessageCallback(SendCommonMessageFunc func) {}

        virtual void connected_callback() {}
        virtual void disconnected_callback() {}
        virtual void connected_error_callback(std::error_code) {}
    };
}

#endif // !BASE_MAIN_WINDOW_H
