﻿cmake_minimum_required(VERSION 3.24)

project(QingLiaoClient VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(OpenSSL REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(kcp CONFIG REQUIRED)

set(TS_FILES QingLiaoClient_zh_CN.ts)

set(PROJECT_SOURCES
        QingLiaoClient.qrc
        src/main.cpp

        src/login/login.cpp
        src/login/login.h
        src/login/login.ui

        src/start/startlabel.h
        src/start/startlabel.cpp
        src/start/start.h
        src/start/start.cpp
        src/start/start.ui

        src/register/register.h
        src/register/register.cpp
        src/register/register.ui

        src/messageBox/messageBox.h

        src/network/dataPackage.cpp
        src/network/dataPackage.h
        src/network/package.cpp
        src/network/package.h
        src/network/network.cpp
        src/network/network.h

        src/manager/manager.h
        src/manager/manager.cpp
        src/manager/dataManager.h
        src/manager/dataManager.cpp

        src/factory/factory.h
        src/factory/factory.cpp
        src/factory/abstractClass/abstractRoom.hpp
        src/factory/token.h
        src/factory/token.cpp

        src/mainWindow/baseMainWindow.h
        src/mainWindow/baseMainWindow.cpp
        src/mainWindow/mainWindow.h
        src/mainWindow/mainWindow.cpp
        src/mainWindow/mainWindow.ui

        src/parser/Ini.cpp
        src/parser/Ini.h
        src/parser/Json.cpp
        src/parser/Json.h

        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug")
    endif()
    if(CMAKE_BUILD_TYPE)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            qt_add_executable(QingLiaoClient
                MANUAL_FINALIZATION
                ${PROJECT_SOURCES}
            )
        else()
            qt_add_executable(QingLiaoClient WIN32
                MANUAL_FINALIZATION
                ${PROJECT_SOURCES}
            )
        endif()
    endif()
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QingLiaoClient APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(QingLiaoClient SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        if (NOT CMAKE_BUILD_TYPE)
            set(CMAKE_BUILD_TYPE "Debug")
        endif()
        if(CMAKE_BUILD_TYPE)
            if(CMAKE_BUILD_TYPE STREQUAL "Debug")
                add_executable(QingLiaoClient
                    ${PROJECT_SOURCES}
                )
            else()
                add_executable(QingLiaoClient WIN32
                    ${PROJECT_SOURCES}
                )
            endif()
        endif()
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(QingLiaoClient PUBLIC include/ src/parser/)
target_link_libraries(QingLiaoClient PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    OpenSSL::SSL
    OpenSSL::Crypto
    asio::asio)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QingLiaoClient)
endif()
