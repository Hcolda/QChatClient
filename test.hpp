#ifndef TEST_HPP
#define TEST_HPP

#include <QAction>
#include <QApplication>
#include <QDateTime>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QIcon>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextEdit>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

// ===================== 消息模型 =====================
class ChatMessage {
public:
  enum MessageType { Text, Image, File };

  ChatMessage(const QString &content, const QString &sender, bool isSelf,
              MessageType type = Text,
              qint64 timestamp = QDateTime::currentMSecsSinceEpoch())
      : content(content), sender(sender), isSelf(isSelf), type(type),
        timestamp(timestamp), canRecall(true) {}

  QString content;
  QString sender;
  bool isSelf;
  MessageType type;
  qint64 timestamp;
  bool canRecall; // 是否可撤回

  QString timeString() const {
    return QDateTime::fromMSecsSinceEpoch(timestamp).toString("hh:mm:ss");
  }
};

// ===================== 聊天界面组件 =====================
class ChatWidget : public QWidget {
  Q_OBJECT
public:
  explicit ChatWidget(QWidget *parent = nullptr) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 聊天消息展示区域
    chatDisplay = new QTextEdit;
    chatDisplay->setReadOnly(true);
    chatDisplay->setFont(QFont("Microsoft YaHei", 10));
    chatDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chatDisplay, &QTextEdit::customContextMenuRequested, this,
            &ChatWidget::showMessageContextMenu);
    mainLayout->addWidget(chatDisplay, 1);

    // 消息输入区域
    QHBoxLayout *inputLayout = new QHBoxLayout;

    messageInput = new QLineEdit;
    messageInput->setPlaceholderText("输入消息...");
    messageInput->setFont(QFont("Microsoft YaHei", 10));
    connect(messageInput, &QLineEdit::returnPressed, this,
            &ChatWidget::sendMessage);
    inputLayout->addWidget(messageInput, 1);

    sendButton = new QPushButton("发送");
    sendButton->setFixedWidth(80);
    connect(sendButton, &QPushButton::clicked, this, &ChatWidget::sendMessage);
    inputLayout->addWidget(sendButton);

    mainLayout->addLayout(inputLayout);

    // 初始化撤回菜单
    recallMenu = new QMenu(this);
    recallAction = recallMenu->addAction("撤回消息");
    connect(recallAction, &QAction::triggered, this,
            &ChatWidget::recallSelectedMessage);
  }

  void addMessage(const ChatMessage &message) {
    messages.append(message);
    displayMessage(message);

    // 自动滚动到底部
    QScrollBar *scrollbar = chatDisplay->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
  }

  void clearChat() {
    messages.clear();
    chatDisplay->clear();
  }

signals:
  void messageSent(const ChatMessage &message);
  void messageRecalled(const ChatMessage &message);

private slots:
  void sendMessage() {
    QString text = messageInput->text().trimmed();
    if (text.isEmpty())
      return;

    ChatMessage msg(text, "我", true);
    addMessage(msg);
    emit messageSent(msg);

    messageInput->clear();
  }

  void showMessageContextMenu(const QPoint &pos) {
    QTextCursor cursor = chatDisplay->cursorForPosition(pos);
    int position = cursor.position();

    // 查找点击位置的消息
    selectedMessageIndex = -1;
    for (int i = 0; i < messages.size(); ++i) {
      if (position >= messagePositions[i].first &&
          position <= messagePositions[i].second) {
        if (messages[i].isSelf && messages[i].canRecall) {
          selectedMessageIndex = i;
          break;
        }
      }
    }

    if (selectedMessageIndex >= 0) {
      recallMenu->exec(chatDisplay->mapToGlobal(pos));
    }
  }

  void recallSelectedMessage() {
    if (selectedMessageIndex < 0 || selectedMessageIndex >= messages.size())
      return;

    ChatMessage &msg = messages[selectedMessageIndex];
    msg.canRecall = false;

    // 更新显示
    QTextCursor cursor(chatDisplay->document());
    cursor.setPosition(messagePositions[selectedMessageIndex].first);
    cursor.setPosition(messagePositions[selectedMessageIndex].second,
                       QTextCursor::KeepAnchor);

    QString recallText = QString("【%1 撤回了一条消息】").arg(msg.sender);
    if (msg.type == ChatMessage::Image) {
      recallText = QString("【%1 撤回了一张图片】").arg(msg.sender);
    } else if (msg.type == ChatMessage::File) {
      recallText = QString("【%1 撤回了一个文件】").arg(msg.sender);
    }

    cursor.insertText(recallText);

    // 发送撤回信号
    emit messageRecalled(msg);
  }

private:
  void displayMessage(const ChatMessage &message) {
    QTextCursor cursor(chatDisplay->textCursor());
    cursor.movePosition(QTextCursor::End);

    // 保存消息位置
    int startPos = cursor.position();

    // 设置消息样式
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(message.isSelf ? Qt::AlignRight : Qt::AlignLeft);
    blockFormat.setTopMargin(5);
    blockFormat.setBottomMargin(5);
    cursor.setBlockFormat(blockFormat);

    // 发送者信息
    QTextCharFormat senderFormat;
    senderFormat.setFontWeight(QFont::Bold);
    senderFormat.setForeground(message.isSelf ? QColor("#1E88E5")
                                              : QColor("#43A047"));
    cursor.insertText(message.sender + " ", senderFormat);

    // 时间信息
    QTextCharFormat timeFormat;
    timeFormat.setFontPointSize(9);
    timeFormat.setForeground(Qt::gray);
    cursor.insertText("(" + message.timeString() + ")", timeFormat);
    cursor.insertBlock();

    // 消息内容
    QTextCharFormat msgFormat;
    msgFormat.setFontPointSize(11);

    if (message.type == ChatMessage::Image) {
      msgFormat.setForeground(QColor("#0288D1"));
      cursor.insertText("[图片] " + message.content, msgFormat);
    } else if (message.type == ChatMessage::File) {
      msgFormat.setForeground(QColor("#7B1FA2"));
      cursor.insertText("[文件] " + message.content, msgFormat);
    } else {
      msgFormat.setForeground(Qt::black);
      cursor.insertText(message.content, msgFormat);
    }

    int endPos = cursor.position();
    messagePositions.append(qMakePair(startPos, endPos));

    // 添加分隔线
    cursor.insertBlock();
    cursor.insertHtml("<hr style=\"margin: 5px 0;\">");
  }

private:
  QTextEdit *chatDisplay;
  QLineEdit *messageInput;
  QPushButton *sendButton;
  QList<ChatMessage> messages;
  QList<QPair<int, int>> messagePositions; // 存储每条消息的起始和结束位置
  int selectedMessageIndex;
  QMenu *recallMenu;
  QAction *recallAction;
};

// ===================== 服务器管理组件 =====================
class ServerManager : public QWidget {
  Q_OBJECT
public:
  explicit ServerManager(QWidget *parent = nullptr) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // 服务器列表标题
    QLabel *titleLabel = new QLabel("服务器列表");
    titleLabel->setStyleSheet(
        "font-weight: bold; font-size: 14px; margin-bottom: 5px;");
    mainLayout->addWidget(titleLabel);

    // 服务器列表
    serverList = new QListWidget;
    serverList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(serverList, &QListWidget::customContextMenuRequested, this,
            &ServerManager::showServerContextMenu);
    connect(serverList, &QListWidget::itemClicked, this,
            &ServerManager::handleServerSelection);
    mainLayout->addWidget(serverList, 1); // 占据剩余空间

    // 添加服务器按钮
    QPushButton *addButton = new QPushButton("添加服务器");
    connect(addButton, &QPushButton::clicked, this, &ServerManager::addServer);
    mainLayout->addWidget(addButton);

    // 添加默认服务器
    addServerItem("主服务器 (192.168.1.100)");
    addServerItem("备用服务器 (10.0.0.5)");
    serverList->setCurrentRow(0);
  }

  QString getCurrentServer() const {
    if (serverList->currentItem()) {
      return serverList->currentItem()->text();
    }
    return "";
  }

public slots:
  void addServer() {
    bool ok;
    QString serverName = QInputDialog::getText(
        this, "添加服务器", "服务器名称:", QLineEdit::Normal, "新服务器", &ok);
    if (ok && !serverName.isEmpty()) {
      addServerItem(serverName);
    }
  }

  void removeServer() {
    if (serverList->currentRow() < 0)
      return;

    int row = serverList->currentRow();
    QListWidgetItem *item = serverList->takeItem(row);
    delete item;

    // 如果删除后列表为空，添加一个默认服务器
    if (serverList->count() == 0) {
      addServerItem("默认服务器");
    }
  }

  void editServer() {
    if (serverList->currentRow() < 0)
      return;

    bool ok;
    QString currentName = serverList->currentItem()->text();
    QString newName = QInputDialog::getText(
        this, "编辑服务器", "服务器名称:", QLineEdit::Normal, currentName, &ok);
    if (ok && !newName.isEmpty()) {
      serverList->currentItem()->setText(newName);
    }
  }

  void connectToServer() {
    if (serverList->currentRow() < 0)
      return;

    QListWidgetItem *item = serverList->currentItem();
    item->setIcon(QIcon(":/icons/connected.png"));
    emit serverConnected(item->text());
  }

  void disconnectFromServer() {
    if (serverList->currentRow() < 0)
      return;

    QListWidgetItem *item = serverList->currentItem();
    item->setIcon(QIcon());
    emit serverDisconnected(item->text());
  }

signals:
  void serverSelected(const QString &serverName);
  void serverConnected(const QString &serverName);
  void serverDisconnected(const QString &serverName);

private:
  void addServerItem(const QString &name) {
    QListWidgetItem *item = new QListWidgetItem(name, serverList);
    item->setIcon(QIcon());
    serverList->addItem(item);
  }

  void showServerContextMenu(const QPoint &pos) {
    if (serverList->currentItem() == nullptr)
      return;

    QMenu menu(this);

    QAction *connectAction = menu.addAction("连接");
    QAction *disconnectAction = menu.addAction("断开连接");
    menu.addSeparator();
    QAction *editAction = menu.addAction("编辑");
    QAction *removeAction = menu.addAction("删除");

    connect(connectAction, &QAction::triggered, this,
            &ServerManager::connectToServer);
    connect(disconnectAction, &QAction::triggered, this,
            &ServerManager::disconnectFromServer);
    connect(editAction, &QAction::triggered, this, &ServerManager::editServer);
    connect(removeAction, &QAction::triggered, this,
            &ServerManager::removeServer);

    menu.exec(serverList->viewport()->mapToGlobal(pos));
  }

  void handleServerSelection(QListWidgetItem *item) {
    emit serverSelected(item->text());
  }

private:
  QListWidget *serverList;
};

// ===================== 用户管理组件 =====================
class UserManager : public QWidget {
  Q_OBJECT
public:
  explicit UserManager(QWidget *parent = nullptr) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // 用户列表标题
    QHBoxLayout *titleLayout = new QHBoxLayout;

    QLabel *titleLabel = new QLabel("在线用户");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    titleLayout->addWidget(titleLabel);

    userCountLabel = new QLabel("(0)");
    userCountLabel->setStyleSheet("color: #666666;");
    titleLayout->addWidget(userCountLabel);
    titleLayout->addStretch();

    mainLayout->addLayout(titleLayout);

    // 用户搜索框
    QLineEdit *searchInput = new QLineEdit;
    searchInput->setPlaceholderText("搜索用户...");
    searchInput->setClearButtonEnabled(true);
    connect(searchInput, &QLineEdit::textChanged, this,
            &UserManager::filterUsers);
    mainLayout->addWidget(searchInput);

    // 用户列表
    userList = new QListWidget;
    userList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(userList, &QListWidget::itemDoubleClicked, this,
            &UserManager::startChatWithUser);
    connect(userList, &QListWidget::customContextMenuRequested, this,
            &UserManager::showUserContextMenu);
    mainLayout->addWidget(userList, 1);

    // 添加模拟用户
    addUser("Lonely微世", "在线", "#43A047");
    addUser("主菜单 高清", "在线", "#43A047");
    addUser("TextGroup", "离线", "#757575");
    addUser("启用", "忙碌", "#FB8C00");
    addUser("Turkish", "在线", "#43A047");
    addUser("karbihara", "离开", "#FDD835");
    addUser("现代C++/苹果", "在线", "#43A047");
    addUser("导航和搜索", "离线", "#757575");
    addUser("C++/视频Office", "在线", "#43A047");
    addUser("C/C++/语音之家", "忙碌", "#FB8C00");
    addUser("可控技术分析", "在线", "#43A047");
    addUser("系统", "在线", "#43A047");

    updateUserCount();
  }

  void addUser(const QString &name, const QString &status,
               const QString &color) {
    QListWidgetItem *item = new QListWidgetItem(name, userList);
    item->setData(Qt::UserRole, status);
    item->setData(Qt::UserRole + 1, color);
    updateUserItem(item);
  }

  void updateUserCount() {
    userCountLabel->setText(QString("(%1)").arg(userList->count()));
  }

signals:
  void chatRequested(const QString &userName);

private slots:
  void filterUsers(const QString &text) {
    for (int i = 0; i < userList->count(); ++i) {
      QListWidgetItem *item = userList->item(i);
      item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));
    }
  }

  void startChatWithUser(QListWidgetItem *item) {
    emit chatRequested(item->text());
  }

  void showUserContextMenu(const QPoint &pos) {
    QListWidgetItem *item = userList->itemAt(pos);
    if (!item)
      return;

    QMenu menu(this);

    QAction *chatAction = menu.addAction("发起聊天");
    QAction *infoAction = menu.addAction("查看资料");
    menu.addSeparator();
    QAction *addFriendAction = menu.addAction("添加好友");
    QAction *blockAction = menu.addAction("屏蔽用户");

    connect(chatAction, &QAction::triggered,
            [this, item]() { emit chatRequested(item->text()); });

    menu.exec(userList->viewport()->mapToGlobal(pos));
  }

private:
  void updateUserItem(QListWidgetItem *item) {
    QString name = item->text();
    QString status = item->data(Qt::UserRole).toString();
    QString color = item->data(Qt::UserRole + 1).toString();

    QString html =
        QString("<div style='margin: 5px;'>"
                "<b>%1</b><br>"
                "<span style='color: %2; font-size: small;'>%3</span>"
                "</div>")
            .arg(name)
            .arg(color)
            .arg(status);

    item->setText("");
    item->setData(Qt::DisplayRole, QVariant());    // 清除文本显示
    item->setData(Qt::DecorationRole, QVariant()); // 清除图标
    item->setData(Qt::UserRole + 2, html);         // 保存HTML

    // 设置自定义绘制
    userList->setItemWidget(item, nullptr);
    QLabel *label = new QLabel(html);
    label->setMargin(5);
    userList->setItemWidget(item, label);
  }

private:
  QListWidget *userList;
  QLabel *userCountLabel;
};

// ===================== 主窗口 =====================
class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
    setWindowTitle("QOOP 聊天客户端");

    // 创建中心部件
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 创建主分割器
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainLayout->addWidget(mainSplitter);

    // 1. 左侧服务器栏
    serverManager = new ServerManager;
    serverManager->setMinimumWidth(150);
    serverManager->setMaximumWidth(200);
    connect(serverManager, &ServerManager::serverSelected, this,
            &MainWindow::onServerSelected);
    connect(serverManager, &ServerManager::serverConnected, this,
            &MainWindow::onServerConnected);
    connect(serverManager, &ServerManager::serverDisconnected, this,
            &MainWindow::onServerDisconnected);
    mainSplitter->addWidget(serverManager);

    // 2. 中间用户栏
    userManager = new UserManager;
    userManager->setMinimumWidth(180);
    userManager->setMaximumWidth(250);
    connect(userManager, &UserManager::chatRequested, this,
            &MainWindow::onChatRequested);
    mainSplitter->addWidget(userManager);

    // 3. 右侧聊天区
    chatWidget = new ChatWidget;
    connect(chatWidget, &ChatWidget::messageSent, this,
            &MainWindow::onMessageSent);
    connect(chatWidget, &ChatWidget::messageRecalled, this,
            &MainWindow::onMessageRecalled);
    mainSplitter->addWidget(chatWidget);

    // 设置分割器比例
    mainSplitter->setSizes({150, 200, 600});

    // 创建工具栏
    createToolBars();

    // 创建状态栏
    createStatusBar();

    // 设置初始大小
    resize(1200, 800);

    // 模拟接收消息
    QTimer::singleShot(3000, this, &MainWindow::simulateIncomingMessage);
  }

private:
  void createToolBars() {
    // 主工具栏
    QToolBar *mainToolBar = addToolBar("主工具栏");
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // 连接操作
    QAction *connectAction = new QAction("连接", this);
    connect(connectAction, &QAction::triggered, serverManager,
            &ServerManager::connectToServer);

    QAction *disconnectAction = new QAction("断开", this);
    connect(disconnectAction, &QAction::triggered, serverManager,
            &ServerManager::disconnectFromServer);

    // 消息操作
    QAction *sendFileAction = new QAction("发送文件", this);
    QAction *sendImageAction = new QAction("发送图片", this);
    QAction *historyAction = new QAction("历史记录", this);

    mainToolBar->addAction(connectAction);
    mainToolBar->addAction(disconnectAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(sendFileAction);
    mainToolBar->addAction(sendImageAction);
    mainToolBar->addAction(historyAction);

    // 服务器状态指示器
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mainToolBar->addWidget(spacer);

    QLabel *serverLabel = new QLabel("服务器: ");
    mainToolBar->addWidget(serverLabel);

    serverStatus = new QLabel("未连接");
    serverStatus->setStyleSheet("color: #CC0000; font-weight: bold;");
    mainToolBar->addWidget(serverStatus);
  }

  void createStatusBar() {
    QStatusBar *statusBar = new QStatusBar;
    setStatusBar(statusBar);

    // 状态信息
    connectionStatus = new QLabel("状态: 未连接");
    statusBar->addWidget(connectionStatus);

    // 当前聊天对象
    currentChatLabel = new QLabel("聊天对象: 无");
    statusBar->addWidget(currentChatLabel);

    // 消息状态
    messageStatus = new QLabel("就绪");
    statusBar->addWidget(messageStatus);

    // 右侧状态信息
    QLabel *versionLabel = new QLabel("版本: Q3_1");
    statusBar->addPermanentWidget(versionLabel);
  }

private slots:
  void onServerSelected(const QString &serverName) {
    serverStatus->setText(serverName);
    statusBar()->showMessage("已选择服务器: " + serverName, 2000);
  }

  void onServerConnected(const QString &serverName) {
    serverStatus->setText(serverName + " (已连接)");
    serverStatus->setStyleSheet("color: #009900; font-weight: bold;");
    connectionStatus->setText("状态: 已连接");
    statusBar()->showMessage("已连接到服务器: " + serverName, 3000);
  }

  void onServerDisconnected(const QString &serverName) {
    serverStatus->setText(serverName + " (未连接)");
    serverStatus->setStyleSheet("color: #CC0000; font-weight: bold;");
    connectionStatus->setText("状态: 未连接");
    chatWidget->clearChat();
    currentChatLabel->setText("聊天对象: 无");
    statusBar()->showMessage("已断开与服务器的连接: " + serverName, 3000);
  }

  void onChatRequested(const QString &userName) {
    currentChatLabel->setText("聊天对象: " + userName);
    chatWidget->clearChat();

    // 添加欢迎消息
    ChatMessage welcomeMsg(QString("您已与 %1 开始聊天").arg(userName), "系统",
                           false);
    chatWidget->addMessage(welcomeMsg);
  }

  void onMessageSent(const ChatMessage &message) {
    messageStatus->setText("消息已发送");
    QTimer::singleShot(2000, [this]() { messageStatus->setText("就绪"); });
  }

  void onMessageRecalled(const ChatMessage &message) {
    messageStatus->setText("消息已撤回");
    QTimer::singleShot(2000, [this]() { messageStatus->setText("就绪"); });
  }

  void simulateIncomingMessage() {
    if (currentChatLabel->text() != "聊天对象: 无") {
      QStringList responses = {
          "你好！很高兴和你聊天",          "你最近在做什么项目？",
          "我这边有个关于C++的问题想请教", "你看到我发的文件了吗？",
          "这个功能实现起来有点复杂",      "我们可以下周再讨论这个",
          "你觉得Qt的哪个模块最难掌握？"};

      ChatMessage incomingMsg(responses[rand() % responses.size()],
                              currentChatLabel->text().split(": ").last(),
                              false);
      chatWidget->addMessage(incomingMsg);
    }

    // 继续模拟
    QTimer::singleShot(5000 + rand() % 10000, this,
                       &MainWindow::simulateIncomingMessage);
  }

private:
  ServerManager *serverManager;
  UserManager *userManager;
  ChatWidget *chatWidget;
  QLabel *serverStatus;
  QLabel *connectionStatus;
  QLabel *currentChatLabel;
  QLabel *messageStatus;
};

#endif // TEST_HPP
