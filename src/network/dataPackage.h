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

#ifndef DATA_PACKAGE_H
#define DATA_PACKAGE_H

#include <string>
#include <memory>
#include <string_view>

namespace qingliao
{
    /*
    * @brief 数据包
    */
    class DataPackage
    {
    private:
#pragma pack(1)
        int                 length = 0;
    public:
        long long           requestID = 0;
        int                 type = 0;
        int                 sequence = -1;
    private:
        unsigned long long  verifyCode = 0;
        char                data[2]{ 0 };
#pragma pack()

    public:
        DataPackage() = delete;
        ~DataPackage() = default;
        DataPackage(const DataPackage&) = delete;
        DataPackage(DataPackage&& dp) = delete;

        /*
        * @brief 制作数据包
        * @param data 数据包中需要存的二进制数据
        * @return 带自动回收的数据包
        */
        static std::shared_ptr<DataPackage> makePackage(std::string_view data);

        /*
        * @brief 从string中加载数据包
        * @param data 数据包中需要存的二进制数据
        * @return 带自动回收的数据包
        */
        static std::shared_ptr<DataPackage> stringToPackage(const std::string& data);

        /*
        * @brief 将数据包转换为二进制格式数据包
        * @param dp DataPackage
        * @return 二进制格式数据包
        */
        std::string packageToString() noexcept;

        /*
        * @brief 获取数据包大小
        * @return size 数据包大小
        */
        size_t getPackageSize() noexcept;

        /*
        * @brief 获取包中二进制数据大小
        * @return size 二进制数据大小
        */
        size_t getDataSize() noexcept;

        /*
        * @brief 获取包中二进制数据
        * @return string 二进制数据
        */
        std::string getData();

        static void deleteDataPackage(DataPackage* dp);
    };
}

#endif // !DATA_PACKAGE_H
