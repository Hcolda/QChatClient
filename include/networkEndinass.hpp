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

#ifndef NETWORK_ENDINASS_HPP
#define NETWORK_ENDINASS_HPP

#include <concepts>

namespace qingliao
{
    /*
    * @brief 判断本地序是否为大端序
    * @return true 为大端序 | false 为小端序
    */
    constexpr inline bool isBigEndianness()
    {
        union u_data
        {
            unsigned char   a;
            unsigned int    b;
        } data;

        data.b = 0x12345678;

        return data.a == 0x12;
    }

    /*
    * @brief 端序转换
    * @param value 数据 (整数型)
    * @return 转换端序后的数据
    */
    template<typename T>
        requires std::integral<T>
    constexpr inline T swapEndianness(T value) {
        T result = 0;
        for (size_t i = 0; i < sizeof(value); ++i) {
            result = (result << 8) | ((value >> (8 * i)) & 0xFF);
        }
        return result;
    }

    /*
    * @brief 本地序与网络序互转
    * @param value 数据 (整数型)
    * @return 转换端序后的数据
    */
    template<typename T>
        requires std::integral<T>
    constexpr inline T swapNetworkEndianness(T value)
    {
        if (!isBigEndianness())
            return swapEndianness(value);
        else
            return value;
    }
}

#endif // !NETWORK_ENDINASS_HPP
