#include "net.h"

// 构造函数，初始化 QTcpSocket 对象
Net::Net(QObject* parent)
    : QObject(parent) {
    socket = new QTcpSocket; // 创建一个新的 QTcpSocket 对象
}

/**
 * @brief Net::connectServer, 与服务器建立 tcp 连接
 *                            如果服务器返回信息，则通过 Net::onReadyRead() 函数处理
 * @param ip
 * @param port
 * @return 连接是否成功
 */
bool Net::connectServer(QString ip, QString port) {
    socket->connectToHost(QHostAddress(ip), port.toShort());

    if (!socket->waitForConnected(3000)) {
        qDebug() << "Connection failed!";
        return false;
    }
    qDebug() << "Connected to Server";
    connect(socket, &QTcpSocket::readyRead, this, &Net::onReadyRead);
    return true;
}
/**
 * @brief Net::onReadyRead 处理服务器发送来的信息，发送信号 updateLabel 以更新 ui
 */
void Net::onReadyRead() {
    QByteArray data = socket->readAll();
    // 发射信号更新 UI
    QString receivedText = QString::fromUtf8(data);
    qDebug() << "received Text: " << receivedText;
    emit updateLabel(receivedText);
}
/**
 * @brief Net::sendMessage 向服务器发送信息
 * @param msg: 要发送的信息
 */
void Net::sendMessage(QString msg) {
    socket->write(msg.toStdString().c_str());
    socket->flush();
}
