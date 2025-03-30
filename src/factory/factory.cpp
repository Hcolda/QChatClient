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

#include "factory.h"

#include "../mainWindow/mainWindow.h"

namespace qingliao
{
    struct FactoryImpl
    {
        std::shared_ptr<Network>     network;
        std::shared_ptr<DataManager> dataManager;
        std::shared_ptr<Manager>     manager;

        FactoryImpl() :
            network(std::make_shared<Network>()),
            dataManager(std::make_shared<DataManager>())
        {
            manager = std::make_shared<Manager>(network);
        }
    };

    Factory::Factory() :
        m_factory_impl(std::make_shared<FactoryImpl>())
    {
    }

    Factory::~Factory() {}

    std::shared_ptr<BaseNetwork> Factory::getNetwork() const
    {
        return m_factory_impl->network;
    }

    std::shared_ptr<DataManager> Factory::getDataManager() const
    {
        return m_factory_impl->dataManager;
    }

    std::shared_ptr<Manager> Factory::getManager() const
    {
        return m_factory_impl->manager;
    }

    BaseMainWindow* Factory::createNewMainWindow(QWidget* parent)
    {
        return new MainWindow { parent };
    }

    Login* Factory::createNewLoginWidget(QWidget* parent)
    {
        return new Login { parent };
    }

    Start* Factory::createNewStartWidget(QWidget* parent)
    {
        return new Start { parent };
    }
}
