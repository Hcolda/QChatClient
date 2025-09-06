#ifndef NETWORK_H
#define NETWORK_H

#include <QByteArray>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QSharedPointer>
#include <QSslError>
#include <QSslSocket>
#include <QString>

#include <atomic>

#include "DataPackage.h"

class Network : public QObject {
  Q_OBJECT
protected:
  explicit Network(QObject *parent = nullptr);

public:
  static Network &getSingleton();
  ~Network();

  void connectToServer(const QString &host, quint16 port);
  void disconnectFromServer();
  QSharedPointer<DataPackage> read();
  bool canRead() const;
  bool isConnected() const;
  bool waitForConnected(int msecs = 30000);

signals:
  void sslErrors(const QList<QSslError> &errors);
  void encrypted();
  void readyRead();
  void disconnected();
  void errorOccurred(QAbstractSocket::SocketError socketError);

protected slots:
  void encryptedSlot();
  void readyReadSlot();
  void disconnectedSlot();

private:
  QSslSocket m_ssl_socket;
  mutable QMutex m_mutex;
  QByteArray m_buffer;
  bool m_has_connected;
  bool m_can_read;
};

#endif // NETWORK_H
