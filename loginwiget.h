#ifndef LOGINWIGET_H
#define LOGINWIGET_H

#include <QWidget>

namespace Ui {
class LoginWiget;
}

class LoginWiget : public QWidget {
  Q_OBJECT

public:
  explicit LoginWiget(QWidget *parent = nullptr);
  ~LoginWiget();

protected slots:
  void loginSlot();
  void registerSlot();

protected:
  void login(const QString &account, const QString &password);

private:
  Ui::LoginWiget *ui;
};

#endif // LOGINWIGET_H
