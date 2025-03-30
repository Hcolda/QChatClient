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

#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPoint>
#include <QMouseEvent>

#include <system_error>

#include "../mainWindow/baseMainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QDialog, public qingliao::BaseMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void connected_error_singal(std::error_code);
    void connected_singal();
    void disconnected_singal();

protected slots:
    //界面移动
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void connected_error_slot(std::error_code);
    void connected_slot();
    void disconnected_slot();

protected:
    void connected_callback();
    void disconnected_callback();
    void connected_error_callback(std::error_code);

private:
    Ui::Login *ui;

    bool flag_;
    QPoint position_;
};
#endif // LOGIN_H
