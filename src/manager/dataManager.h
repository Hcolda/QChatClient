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

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <memory>
#include <string>

#include "../mainWindow/baseMainWindow.h"

namespace qingliao
{
	struct DataManagerImpl;

	class DataManager
	{
	public:
		DataManager();
		~DataManager();

		// 禁止复制和移动
		DataManager(const DataManager&) = delete;
		DataManager(DataManager&&) = delete;
		DataManager& operator=(const DataManager&) = delete;
		DataManager& operator=(DataManager&&) = delete;

		static bool signUp(const std::string& email, const std::string& password, long long& user_id);
		bool signIn(long long user_id, const std::string& password);

		bool addPrivateRoom(long long user_id);
		bool romovePrivateRoom(long long user_id);
		bool addGroupRoom(long long room_id);
		bool removeGroupRoom(long long room_id);

		void addPrivateRoomMessage(long long user_id, MessageType type, const std::string& message);
		bool removePrivateRoomMessage(size_t index);
		void addGroupRoomMessage(long long group_id, long long sender_id, MessageType type, const std::string& message);
		bool removeGroupRoomMessage(size_t index);

	private:
		std::shared_ptr<DataManagerImpl> m_impl;
	};
}

#endif