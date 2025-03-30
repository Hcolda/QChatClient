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

#ifndef MANAGER_H
#define MANAGER_H

#include <memory>
#include <string>

#include "../network/network.h"
#include "../mainWindow/baseMainWindow.h"

namespace qingliao
{
    struct ManagerImpl;
    
    class Manager
    {
    public:
        Manager(std::weak_ptr<qingliao::BaseNetwork>);
        ~Manager();

        bool addMainWindow(const std::string&, qingliao::BaseMainWindow*);
        bool removeMainWindow(const std::string&);

    protected:
        void connected_callback();
        void disconnected_callback();
        void connected_error_callback(std::error_code);
        void received_message(std::string);

    private:
        std::shared_ptr<ManagerImpl> m_manager_impl;
    };
}

#endif // !MANAGER_H
