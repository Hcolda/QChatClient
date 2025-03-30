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

#include "dataManager.h"

#include <atomic>
#include <unordered_map>
#include <shared_mutex>

#include "../network/dataPackage.h"
#include "../factory/factory.h"
#include "../parser/Json.h"

extern qingliao::Factory clientFactory;

namespace qingliao
{
    struct DataManagerImpl
    {
        std::atomic<bool> is_login = false;


    };


    DataManager::DataManager() :
        m_impl(std::make_shared<DataManagerImpl>()) {}

    DataManager::~DataManager() {}

    bool DataManager::signUp(const std::string& email, const std::string& password, long long& user_id)
    {
        qjson::JObject json(qjson::JValueType::JDict);
        json["function"] = "register";
        json["parameters"]["email"] = email;
        json["parameters"]["password"] = password;

        auto network = clientFactory.getNetwork();
        auto pack = network->send_data_with_result_n_option(qjson::JWriter::fastWrite(json), [](std::shared_ptr<DataPackage>& pack) { pack->type = 1; });
        try
        {
            qjson::JObject rejson = qjson::JParser::fastParse(pack->getData());
            if (rejson["state"].getString() != "success") return false;
            user_id = rejson["user_id"].getInt();
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    bool DataManager::signIn(long long user_id, const std::string& password)
    {
        qjson::JObject json(qjson::JValueType::JDict);
        json["function"] = "login";
        json["parameters"]["user_id"] = user_id;
        json["parameters"]["password"] = password;

        auto network = clientFactory.getNetwork();
        auto pack = network->send_data_with_result_n_option(qjson::JWriter::fastWrite(json), [](std::shared_ptr<DataPackage>& pack) { pack->type = 1; });
        try
        {
            qjson::JObject rejson = qjson::JParser::fastParse(pack->getData());
            if (rejson["state"].getString() != "success") return false;
        }
        catch (...)
        {
            return false;
        }

        m_impl->is_login = true;
        return true;
    }

    bool DataManager::addPrivateRoom(long long user_id)
    {
        return false;
    }

    bool DataManager::romovePrivateRoom(long long user_id)
    {
        return false;
    }

    bool DataManager::addGroupRoom(long long room_id)
    {
        return false;
    }

    bool DataManager::removeGroupRoom(long long room_id)
    {
        return false;
    }

    void DataManager::addPrivateRoomMessage(long long user_id, MessageType type, const std::string& message)
    {
    }

    bool DataManager::removePrivateRoomMessage(size_t index)
    {
        return false;
    }

    void DataManager::addGroupRoomMessage(long long group_id, long long sender_id, MessageType type, const std::string& message)
    {
    }

    bool DataManager::removeGroupRoomMessage(size_t index)
    {
        return false;
    }

}