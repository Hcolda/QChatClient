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

#ifndef REGEX_MATCH_HPP
#define REGEX_MATCH_HPP

#include <regex>
#include <string>

namespace qingliao
{
    class RegexMatch
    {
    public:
        RegexMatch() = default;
        ~RegexMatch() = default;

        static bool emailMatch(const std::string& email)
        {
            static std::regex re(R"((\w+\.)*\w+@(\w+\.)+[A-Za-z]+)", std::regex::optimize);
            std::smatch results;
            return std::regex_match(email, results, re);
        }

        static bool ipAddressMatch(const std::string& ip)
        {
            static std::regex re(R"((((\d{1,2})|(1\d{2})|(2[0-4]\d)|(25[0-5]))\.){3}((\d{1,2})|(1\d{2})|(2[0-4]\d)|(25[0-5])))",
                std::regex::optimize);
            std::smatch results;
            return std::regex_match(ip, results, re);
        }

        static bool phoneMatch(const std::string& phone)
        {
            static std::regex re(R"(\d{11})",
                std::regex::optimize);
            std::smatch results;
            return std::regex_match(phone, results, re);
        }
    };
}

#endif // !REGEX_MATCH_HPP
