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

#ifndef ABSTRACT_ROOM_HPP
#define ABSTRACT_ROOM_HPP

#include <string>
#include <chrono>
#include <functional>
#include <memory>

namespace qingliao
{
    using RoomMessageTimePoint = std::chrono::system_clock::time_point;

    enum class RoomMessageType
    {
        Normal = 0,
        Tip
    };

    class AbstractRoomMessage
    {
    public:
        AbstractRoomMessage(size_t id, const std::string& content, RoomMessageTimePoint point) {}
        virtual ~AbstractRoomMessage() = default;

        virtual void changeContent(const std::string&) {}
        virtual const std::string& getContent() const { return std::string(); }

        virtual void removeMessage() {}

        virtual bool addChangedCallback(std::function<void(const AbstractRoomMessage&)>) {}
        virtual bool addRemovedCallback(std::function<void(const AbstractRoomMessage&)>) {}
    };

    class AbstractRoom
    {
    public:
        AbstractRoom() = default;
        virtual ~AbstractRoom() = default;

        /*
        * @brief 添加消息
        * @param message 消息
        * @return 消息id
        */
        virtual void addMessage(const std::shared_ptr<AbstractRoomMessage>& message) {}

        /*
        * @brief 删除消息
        * @param message
        */
        virtual void removeMessage(const std::shared_ptr<AbstractRoomMessage>& message) {}

        /*
        * @brief 获取消息列表
        * @return 消息列表
        */
        virtual std::vector<std::shared_ptr<AbstractRoomMessage>>
            getMessageList() const
            { return std::vector<std::shared_ptr<AbstractRoomMessage>>(); }
    };
}

#endif // !ABSTRACT_ROOM_HPP
