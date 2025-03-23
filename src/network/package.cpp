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

#include "package.h"

#include <stdexcept>

#include "networkEndinass.hpp"

void qingliao::Package::write(std::string_view data)
{
    m_buffer += data;
}

bool qingliao::Package::canRead() const
{
    using namespace qingliao;

    if (m_buffer.size() < sizeof(int))
        return false;

    int length = 0;
    std::memcpy(&length, m_buffer.c_str(), sizeof(int));
    length = swapNetworkEndianness(length);
    if (length > m_buffer.size())
        return false;

    return true;
}

size_t qingliao::Package::firstMsgLength() const
{
    using namespace qingliao;

    if (m_buffer.size() < sizeof(int))
        return 0;

    int length = 0;
    std::memcpy(&length, m_buffer.c_str(), sizeof(int));
    length = swapNetworkEndianness(length);
    return size_t(length);
}

std::string qingliao::Package::read()
{
    if (!canRead())
        throw std::logic_error("Can't read data");
    else if (!firstMsgLength())
        throw std::logic_error("length is empty");

    std::string result = m_buffer.substr(0, firstMsgLength());
    m_buffer = m_buffer.substr(firstMsgLength());

    return result;
}

const std::string& qingliao::Package::readBuffer() const
{
    return m_buffer;
}

void qingliao::Package::setBuffer(const std::string& b)
{
    m_buffer = b;
}

std::string qingliao::Package::makePackage(std::string_view data)
{
    int lenght = static_cast<int>(data.size());
    std::string result;
    result.resize(sizeof(int));
    std::memcpy(result.data(), result.data(), sizeof(int));
    result += data;

    return result;
}
