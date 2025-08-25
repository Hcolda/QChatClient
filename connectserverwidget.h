#ifndef CONNECTSERVERWIDGET_H
#define CONNECTSERVERWIDGET_H

#include <QWidget>
#include <QSslSocket>
#include <QList>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class ConnectServerWidget;
}
QT_END_NAMESPACE

class ConnectServerWidget : public QWidget
{
    Q_OBJECT

public:
    ConnectServerWidget(QWidget *parent = nullptr);
    ~ConnectServerWidget();

    void closeEvent(QCloseEvent* closeEvent);

protected slots:
    void connectToServer();
    void encrypted();
    void sslErrors(const QList<QSslError> &errors);
    void errorOccurred(QAbstractSocket::SocketError socketError);

private:
    Ui::ConnectServerWidget *ui;
};
#endif // CONNECTSERVERWIDGET_H
