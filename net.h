#ifndef NET_H
#define NET_H
#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>

class Net : public QObject
{
    Q_OBJECT;
public:
    Net(QObject* parent = nullptr);

public:
    // 与服务器建立连接
    bool connectServer(QString ip, QString port);
    void sendMessage(QString msg);
private slots:
    void onReadyRead();
public:
    QTcpSocket* socket;
    QString ip;
    quint16 port;
signals:
    void updateLabel(const QString& text);
};

#endif // NET_H
