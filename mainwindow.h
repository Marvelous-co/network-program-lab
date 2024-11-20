#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "net.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendMsgButton_clicked();

    void on_connectButton_clicked();

    void handleLabelUpdate(const QString& text); // 自定义更新 UI 的槽函数
private:
    Ui::MainWindow *ui;
    Net* client;
    bool isConnected = false;
};
#endif // MAINWINDOW_H
