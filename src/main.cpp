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

#include "factory/factory.h"
#include "mainWindow/mainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSharedMemory>

#include <memory>

// 全局变量
qingliao::Factory clientFactory;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "QingLiaoClient_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    /*QSharedMemory shared("QingLiaoClient");
    if (!shared.create(1))
    {
        return 0;
    }*/

    std::shared_ptr<qingliao::BaseMainWindow>
        mainWindow(clientFactory.createNewMainWindow());
    try
    {
        mainWindow->run();
    }
    catch (...)
    {
        return 0;
    }

    return a.exec();
}
