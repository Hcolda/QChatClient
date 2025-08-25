#include "network.h"

#include <cstring>

Network::Network(QObject *parent)
    : QObject{parent},
    m_has_connected{false},
    m_can_read{false}
{
    m_ssl_socket.setProtocol(QSsl::TlsV1_3);
#ifdef QT_DEBUG
    m_ssl_socket.setPeerVerifyMode(QSslSocket::VerifyNone);
#else
    m_ssl_socket.setPeerVerifyMode(QSslSocket::VerifyPeer);
#endif
    // connect
    connect(&m_ssl_socket, &QSslSocket::encrypted, this, &Network::encrypted);
    connect(&m_ssl_socket, &QSslSocket::encrypted, this, &Network::encryptedSlot);
#ifndef QT_DEBUG
    connect(&m_ssl_socket, &QSslSocket::sslErrors, this, &Network::sslErrors);
#endif
    connect(&m_ssl_socket, &QAbstractSocket::errorOccurred, this, &Network::errorOccurred);
    connect(&m_ssl_socket, &QSslSocket::readyRead, this, &Network::readyReadSlot);
    connect(&m_ssl_socket, &QSslSocket::disconnected, this, &Network::disconnectedSlot);
}

Network& Network::getSingleton()
{
    static Network network;
    return network;
}

Network::~Network()
{}

void Network::connectToServer(const QString &host, quint16 port)
{
    QMutexLocker locker(&m_mutex);
    if (m_has_connected) {
        locker.unlock();
        m_ssl_socket.close();
        locker.relock();
    }
    m_has_connected = false;
    m_can_read = false;
    m_ssl_socket.connectToHostEncrypted(host, port);
}

void Network::disconnectFromServer()
{
    QMutexLocker locker(&m_mutex);
    if (!m_has_connected) {
        return;
    }
    m_has_connected = false;
    m_can_read = false;
    locker.unlock();
    m_ssl_socket.disconnectFromHost();
}

QSharedPointer<DataPackage> Network::read()
{
    QMutexLocker locker(&m_mutex);
    if (!m_has_connected || !m_can_read) {
        return {};
    }
    DataPackage::LengthType size{};
    std::memcpy(&size, m_buffer.data(), sizeof(DataPackage::LengthType));
    if constexpr(std::endian::native == std::endian::little) {
        size = swapEndianness(size);
    }
    QSharedPointer<DataPackage> pack =
        DataPackage::stringToPackage({m_buffer.data(), size});
    qsizetype ret_size = m_buffer.size() - size;
    std::memmove(m_buffer.data(), m_buffer.data() + size, ret_size);
    m_buffer.resize(ret_size);
    std::memcpy(&size, m_buffer.data(), sizeof(DataPackage::LengthType));
    if constexpr(std::endian::native == std::endian::little) {
        size = swapEndianness(size);
    }
    if (m_buffer.size() > size) {
        m_can_read = true;
        emit readyRead();
    }
    else {
        m_can_read = false;
    }
    return pack;
}

bool Network::canRead() const
{
    QMutexLocker locker(&m_mutex);
    return m_can_read;
}

bool Network::isConnected() const
{
    QMutexLocker locker(&m_mutex);
    return m_has_connected;
}

bool Network::waitForConnected(int msecs)
{
    return m_ssl_socket.waitForConnected(msecs);
}

void Network::encryptedSlot()
{
    QMutexLocker locker(&m_mutex);
    m_has_connected = true;
}

void Network::readyReadSlot()
{
    QMutexLocker locker(&m_mutex);
    QByteArray array = m_ssl_socket.readAll();
    m_buffer.resize(m_buffer.size() + array.size());
    std::memcpy(m_buffer.data() + m_buffer.size(), array.data(), array.size());
    if (m_buffer.size() > sizeof(DataPackage::LengthType)) {
        DataPackage::LengthType size{};
        std::memcpy(&size, m_buffer.data(), sizeof(DataPackage::LengthType));
        if constexpr(std::endian::native == std::endian::little) {
            size = swapEndianness(size);
        }
        if (m_buffer.size() > size) {
            m_can_read = true;
            emit readyRead();
        }
    }
}

void Network::disconnectedSlot()
{
    QMutexLocker locker(&m_mutex);
    m_has_connected = false;
    m_can_read = false;
    emit disconnected();
}
