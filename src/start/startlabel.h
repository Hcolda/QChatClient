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

#ifndef START_LABEL_H
#define START_LABEL_H

#include <QWidget>
#include <QLabel>
#include <QPaintEvent>

class StartLabel : public QLabel
{
    Q_OBJECT

public:
    StartLabel(QWidget* parent = nullptr);
    ~StartLabel() = default;

protected slots:
    void paintEvent(QPaintEvent* event);
};

#endif // !START_LABEL_H
