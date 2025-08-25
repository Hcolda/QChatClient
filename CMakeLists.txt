cmake_minimum_required(VERSION 3.19)
project(New2QChatClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network LinguistTools)

qt_standard_project_setup()

qt_add_executable(New2QChatClient
    WIN32 MACOSX_BUNDLE
    QingLiaoClient_zh_CN.ts
    main.cpp
    connectserverwidget.cpp
    connectserverwidget.h
    connectserverwidget.ui
    network.h network.cpp
    DataPackage.h
    loginwiget.h loginwiget.cpp loginwiget.ui
    registerwidget.h registerwidget.cpp registerwidget.ui
)

target_link_libraries(New2QChatClient
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)
target_link_libraries(New2QChatClient PRIVATE Qt6::Core)

include(GNUInstallDirs)

install(TARGETS New2QChatClient
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET New2QChatClient
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
