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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <memory>

#include <QMainWindow>
#include <QtWidgets/QWidget>
#include <QPoint>
#include <QMouseEvent>

#include "baseMainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct MainWindowImpl;

class MainWindow : public QMainWindow, public qingliao::BaseMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void run();

    bool addPrivateRoom(long long user_id);
    bool romovePrivateRoom(long long user_id);
    bool addGroupRoom(long long room_id);
    bool removeGroupRoom(long long room_id);

    void addPrivateRoomMessage(long long user_id, qingliao::MessageType type, const std::string& message);
    bool removePrivateRoomMessage(size_t index);
    void addGroupRoomMessage(long long group_id, long long sender_id, qingliao::MessageType type, const std::string& message);
    bool removeGroupRoomMessage(size_t index);

    void setPrivateRoomMessageCallback(qingliao::SendPrivateRoomMessageFunc func);
    void setGroupRoomMessageCallback(qingliao::SendGroupRoomMessageFunc func);
    void setCommonMessageCallback(qingliao::SendCommonMessageFunc func);

    void connected_callback();
    void disconnected_callback();
    void connected_error_callback(std::error_code);

protected slots:
    //界面移动
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    Ui::MainWindow* ui;

    std::shared_ptr<MainWindowImpl> mainwindow_impl_;
    bool flag_;
    QPoint position_;
};

#endif // !MAIN_WINDOW_H
