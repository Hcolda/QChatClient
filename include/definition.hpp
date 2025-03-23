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

#ifndef DEFINITION_HPP
#define DEFINITION_HPP

#include <format>
#include <filesystem>

#if _HAS_CXX23
// c++23 stacktrace
#include <stacktrace>
#endif

#if _HAS_CXX23
#define ERROR_WITH_STACKTRACE(errmsg) std::format("error: {}\nin file \"{}\" line {}\nstack trace: \n{}\n", errmsg, std::filesystem::path(__FILE__).filename().string(), __LINE__, std::to_string(std::stacktrace::current()))
#else
#define ERROR_WITH_STACKTRACE(errmsg) std::format("error: {}\nin file \"{}\" line {}\n", errmsg, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#endif

#endif // !DEFINITION_HPP