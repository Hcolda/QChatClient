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

#include "manager.h"

#include <functional>
#include <unordered_map>
#include <shared_mutex>

#include "../factory/factory.h"
#include "../mainWindow/baseMainWindow.h"

extern qingliao::Factory clientFactory;

namespace qingliao
{
    struct ManagerImpl
    {
        std::weak_ptr<qingliao::BaseNetwork> network;
        std::unordered_map<std::string,
            qingliao::BaseMainWindow*>   mainWindow_map;
        std::shared_mutex           mainWindow_map_mutex;
    };

    Manager::Manager(std::weak_ptr<qingliao::BaseNetwork> network) :
        m_manager_impl(std::make_shared<ManagerImpl>())
    {
        m_manager_impl->network = std::move(network);

        auto local_network = m_manager_impl->network.lock();

        local_network->add_connected_callback("ManagerConnected",
            std::bind(&Manager::connected_callback, this));
        local_network->add_disconnected_callback("ManagerDisconnected",
            std::bind(&Manager::disconnected_callback, this));
        local_network->add_received_stdstring_callback("ManagerReceivedMessage",
            std::bind(&Manager::received_message, this, std::placeholders::_1));
        local_network->add_connected_error_callback("ManagerConnectedError",
            std::bind(&Manager::connected_error_callback, this, std::placeholders::_1));
    }

    Manager::~Manager()
    {
        auto network = m_manager_impl->network.lock();
        network->remove_connected_callback("ManagerConnected");
        network->remove_disconnected_callback("ManagerDisconnected");
        network->remove_received_stdstring_callback("ManagerReceivedMessage");
    }

    bool Manager::removeMainWindow(const std::string& name)
    {
        std::unique_lock<std::shared_mutex> lock(m_manager_impl->mainWindow_map_mutex);
        auto iter = m_manager_impl->mainWindow_map.find(name);
        if (iter == m_manager_impl->mainWindow_map.cend())
            return false;
        m_manager_impl->mainWindow_map.erase(iter);
        return true;
    }

    bool Manager::addMainWindow(const std::string& name, qingliao::BaseMainWindow* window)
    {
        std::unique_lock<std::shared_mutex> lock(m_manager_impl->mainWindow_map_mutex);
        auto iter = m_manager_impl->mainWindow_map.find(name);
        if (iter != m_manager_impl->mainWindow_map.cend())
            return false;
        m_manager_impl->mainWindow_map[name] = window;
        return true;
    }

    void Manager::connected_callback()
    {
        std::shared_lock<std::shared_mutex> sl(m_manager_impl->mainWindow_map_mutex);
        for (const auto& [_, window] : m_manager_impl->mainWindow_map)
        {
            window->connected_callback();
        }
    }

    void Manager::disconnected_callback()
    {
        std::shared_lock<std::shared_mutex> sl(m_manager_impl->mainWindow_map_mutex);
        for (const auto& [_, window] : m_manager_impl->mainWindow_map)
        {
            window->disconnected_callback();
        }
    }

    void Manager::received_message(std::string data)
    {
        std::shared_lock<std::shared_mutex> sl(m_manager_impl->mainWindow_map_mutex);
        /*for (const auto& [_, window] : m_manager_impl->mainWindow_map)
        {
            window->;
        }*/
    }

    void Manager::connected_error_callback(std::error_code ec)
    {
        std::shared_lock<std::shared_mutex> sl(m_manager_impl->mainWindow_map_mutex);
        for (const auto& [_, window] : m_manager_impl->mainWindow_map)
        {
            window->connected_error_callback(ec);
        }
    }
}
