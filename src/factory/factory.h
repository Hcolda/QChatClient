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

#ifndef FACTORY_H
#define FACTORY_H

#include <memory>

#include "../network/network.h"
#include "../mainWindow/baseMainWindow.h"
#include "../login/login.h"
#include "../start/start.h"
#include "../manager/dataManager.h"
#include "../manager/manager.h"

namespace qingliao
{
    struct FactoryImpl;

    class Factory
    {
    public:
        Factory();
        ~Factory();

        // 禁止复制和移动
        Factory(const Factory&) = delete;
        Factory(Factory&&) = delete;

        Factory& operator=(const Factory&) = delete;
        Factory& operator=(Factory&&) = delete;

        std::shared_ptr<BaseNetwork> getNetwork() const;
        std::shared_ptr<DataManager> getDataManager() const;
        std::shared_ptr<Manager> getManager() const;

        BaseMainWindow* createNewMainWindow(QWidget* parent = nullptr);
        Login* createNewLoginWidget(QWidget* parent = nullptr);
        Start* createNewStartWidget(QWidget* parent = nullptr);

    private:
        std::shared_ptr<FactoryImpl> m_factory_impl;
    };
}

#endif // !FACTORY_H
