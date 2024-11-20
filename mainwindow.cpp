#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "util.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new Net();
    // 连接信号与槽
    connect(ui->msgEdit, SIGNAL(returnPressed()), this, SLOT(on_sendMsgButton_clicked()));
    connect(client, &Net::updateLabel, this, &MainWindow::handleLabelUpdate);
    connect(client->socket, &QTcpSocket::connected, [this](){
        QMessageBox::information(nullptr, "连接提示", "连接服务器成功");
        client->ip = client->socket->localAddress().toString();
        client->port = client->socket->localPort();
        this->isConnected = true;
        ui->connectButton->setText("断开连接");
    });
    connect(client->socket, &QTcpSocket::disconnected, [this]() {
        QMessageBox::information(nullptr, "连接提示", "连接断开");
        this->ui->connectButton->setText("连接");
        this->isConnected = false;
    });
    // 设置 label 中的位置左上对齐
    ui->mySendMsg->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->otherSendMsg->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->msgEdit->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}
/**
 * 点击发送消息按钮时触发的事件
 */
void MainWindow::on_sendMsgButton_clicked()
{
    if (!this->isConnected) {
        QMessageBox::warning(nullptr, "Error", "请先与服务器建立连接, 参考服务器ip: 82.157.182.101:12345");
        return;
    }
    QString writeContent = ui->msgEdit->text();
    ui->msgEdit->clear();
    qDebug() << writeContent;

    client->sendMessage(writeContent);

    // 更新 ui 操作
    QString msg = ui->mySendMsg->text();
    msg += "\n" + client->ip + ":" + QString::number(client->port) + ":  " + writeContent;
    ui->mySendMsg->setText(msg);
}

/**
 * 点击连接按钮时触发的事件:
 * 1. 从 LineEdit 中获取 ip 地址 + 端口号
 * 2. 检查输入的 ip and port 是否合法
 * 3. 同服务器建立 tcp 连接
 */
void MainWindow::on_connectButton_clicked()
{
    QString text = ui->ipPortEdit->text();
    if (!this->isConnected) { // 还未与服务器建立连接
        QStringList list = text.split(":", Qt::SkipEmptyParts);
        if (list.size() != 2) {
            QMessageBox::critical(nullptr, "Error", "Input <ip:port> Format Error!");
            return;
        }
        QString ip = list[0], port = list[1];
        // ip:port 不合法
        if (!isValidIPAndPort(ip.toStdString(), port.toStdString())) {
            QMessageBox::critical(nullptr, "Error", "Input <ip:port> Format Error!");
            return;
        }
        // 与服务器建立 tcp 连接失败
        if (!client->connectServer(ip, port)) {
            QMessageBox::critical(nullptr, "Error", "Connect to " + text + " Error");
            return;
        }
    } else { // 与服务器建立连接后断开连接
        client->socket->disconnectFromHost();
    }
}
/**
 * 当接收到服务器推送来的消息通知时
 * 更新 ui 中 label 的值
 */
void MainWindow::handleLabelUpdate(const QString& text) {
    qDebug() << "handleLabelUpdate() : " << text;
    if (text == "waiting other clients to connected") { // 聊天的两方只有一方进行了 tcp 连接
        QString mysendMsgText = ui->mySendMsg->text();
        mysendMsgText += "⚠️";
        ui->mySendMsg->setText(mysendMsgText);
        return;
    }
    QString labelContent = ui->otherSendMsg->text();
    labelContent += text + "\n";
    ui->otherSendMsg->setText(labelContent);
}


