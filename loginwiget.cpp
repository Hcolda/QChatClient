#include "loginwiget.h"
#include "ui_loginwiget.h"

LoginWiget::LoginWiget(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWiget) {
  ui->setupUi(this);

  connect(ui->loginPushButton, &QPushButton::clicked, this,
          &LoginWiget::loginSlot);
  connect(ui->registerPushButton, &QPushButton::clicked, this,
          &LoginWiget::registerSlot);
}

LoginWiget::~LoginWiget() { delete ui; }

void LoginWiget::login(const QString &account, const QString &password) {}

void LoginWiget::loginSlot() {
  QString account = ui->idLineEdit->text();
  QString password = ui->passwordLineEdit->text();

  login(account, password);
}

void LoginWiget::registerSlot() {}
