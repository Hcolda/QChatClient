// #include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "test.hpp"

// ===================== 样式设置和主函数 =====================
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // 设置全局样式
  a.setStyleSheet(R"(
        QMainWindow {
            background-color: #F5F5F7;
            font-family: "Microsoft YaHei";
        }
        /* 服务器列表样式 */
        QListWidget#serverList {
            background-color: #FFFFFF;
            border: none;
            border-right: 1px solid #E0E0E0;
            font-size: 13px;
        }
        QListWidget#serverList::item {
            padding: 10px 8px;
            border-bottom: 1px solid #F0F0F0;
        }
        QListWidget#serverList::item:selected {
            background-color: #E3F2FD;
            color: #0D47A1;
        }
        /* 用户列表样式 */
        QListWidget#userList {
            background-color: #FFFFFF;
            border: none;
            border-right: 1px solid #E0E0E0;
            font-size: 13px;
        }
        QListWidget#userList::item {
            padding: 0;
            border-bottom: 1px solid #F0F0F0;
        }
        QListWidget#userList::item:hover {
            background-color: #F5F5F5;
        }
        /* 聊天区域样式 */
        QTextEdit#chatDisplay {
            background-color: #FFFFFF;
            border: none;
            padding: 10px;
        }
        QLineEdit#messageInput {
            background-color: #FFFFFF;
            border: 1px solid #E0E0E0;
            border-radius: 4px;
            padding: 8px;
            margin: 10px;
        }
        /* 工具栏样式 */
        QToolBar {
            background-color: #FFFFFF;
            border: none;
            border-bottom: 1px solid #E0E0E0;
            padding: 4px;
        }
        /* 状态栏样式 */
        QStatusBar {
            background-color: #EDEDED;
            border-top: 1px solid #D0D0D0;
            font-size: 12px;
        }
        /* 按钮样式 */
        QPushButton {
            background-color: #1E88E5;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            margin: 10px;
        }
        QPushButton:hover {
            background-color: #1976D2;
        }
        /* 分割线样式 */
        QSplitter::handle {
            background-color: #E0E0E0;
            width: 1px;
        }
        /* 搜索框样式 */
        QLineEdit {
            padding: 5px 10px;
            border: 1px solid #E0E0E0;
            border-radius: 4px;
            margin: 5px;
        }
    )");

  // 设置默认字体
  QFont defaultFont("Microsoft YaHei", 10);
  a.setFont(defaultFont);

  MainWindow w;
  w.show();

  return a.exec();
}

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);

//     QTranslator translator;
//     const QStringList uiLanguages = QLocale::system().uiLanguages();
//     for (const QString &locale : uiLanguages) {
//         const QString baseName = "QingLiaoChatClient_" +
//         QLocale(locale).name(); if (translator.load(":/i18n/" + baseName)) {
//             a.installTranslator(&translator);
//             break;
//         }
//     }
//     MainWindow w;
//     w.show();
//     return a.exec();
// }
