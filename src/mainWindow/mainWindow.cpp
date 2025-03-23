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

#include "mainWindow.h"
#include "./ui_MainWindow.h"

#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>
#include <QWidgetAction>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>

#include <cstdlib>
#include <regex>
#include <stdexcept>

#include "src/factory/factory.h"
#include "src/manager/manager.h"
#include "src/start/start.h"
#include "src/login/login.h"

extern qingliao::Factory clientFactory;

struct MainWindowImpl
{
    qingliao::SendPrivateRoomMessageFunc sendPrivateRoomMessageFunction;
    qingliao::SendGroupRoomMessageFunc   sendGroupRoomMessageFunction;
    qingliao::SendCommonMessageFunc      sendCommonMessageFunction;
};

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    flag_(false),
    mainwindow_impl_(std::make_shared<MainWindowImpl>())
{
    ui->setupUi(this);

    //界面设置
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //关闭按钮
    QObject::connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::close);
    //最小化按钮
    QObject::connect(ui->minimizeButton, &QPushButton::clicked, this, &MainWindow::showMinimized);

    // 设置阴影
    {
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
        shadow->setOffset(0, 0);
        //设置阴影颜色
        shadow->setColor(QColor("#004D40"));
        //设置阴影圆角
        shadow->setBlurRadius(6);
        //给嵌套QWidget设置阴影
        ui->widget->setGraphicsEffect(shadow);
    }
}

MainWindow::~MainWindow()
{
    auto manager = clientFactory.getManager();
    auto network = clientFactory.getNetwork();
    manager->removeMainWindow("MainWindow");
    network->stop();
    delete ui;
}

void MainWindow::run()
{
    Start start;
    if (start.exec() != QDialog::Accepted)
    {
        auto network = clientFactory.getNetwork();
        network->stop();
        throw std::logic_error("Application closed");
    }

    Login login;
    if (login.exec() != QDialog::Accepted)
    {
        auto network = clientFactory.getNetwork();
        network->stop();
        throw std::logic_error("Application closed");
    }

    auto manager = clientFactory.getManager();
    manager->addMainWindow("MainWindow", this);
    show();
}

bool MainWindow::addPrivateRoom(long long user_id)
{
    return false;
}

bool MainWindow::romovePrivateRoom(long long user_id)
{
    return false;
}

bool MainWindow::addGroupRoom(long long room_id)
{
    return false;
}

bool MainWindow::removeGroupRoom(long long room_id)
{
    return false;
}

void MainWindow::addPrivateRoomMessage(long long user_id, qingliao::MessageType type, const std::string& message)
{
}

bool MainWindow::removePrivateRoomMessage(size_t index)
{
    return false;
}

void MainWindow::addGroupRoomMessage(long long group_id, long long sender_id, qingliao::MessageType type, const std::string& message)
{
}

bool MainWindow::removeGroupRoomMessage(size_t index)
{
    return false;
}

void MainWindow::setPrivateRoomMessageCallback(qingliao::SendPrivateRoomMessageFunc func)
{
    mainwindow_impl_->sendPrivateRoomMessageFunction = func;
}

void MainWindow::setGroupRoomMessageCallback(qingliao::SendGroupRoomMessageFunc func)
{
    mainwindow_impl_->sendGroupRoomMessageFunction = func;
}

void MainWindow::setCommonMessageCallback(qingliao::SendCommonMessageFunc func)
{
    mainwindow_impl_->sendCommonMessageFunction = func;
}

void MainWindow::connected_callback()
{
}

void MainWindow::disconnected_callback()
{
}

void MainWindow::connected_error_callback(std::error_code)
{
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    //界面移动
    int num = (event->globalPos() - this->pos()).y();
    if (event->button() == Qt::LeftButton)
    {
        flag_ = true;
        position_ = std::move(event->globalPos() - this->pos());
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    //界面移动
    if (flag_)
    {
        this->move(event->globalPos() - position_);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    //界面移动
    flag_ = false;
}