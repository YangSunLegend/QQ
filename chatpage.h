#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QDialog>
#include "QTimer"
#include "QUdpSocket"
#include "QModelIndex"

namespace Ui {
class ChatPage;
}

class ChatPage : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChatPage(QWidget *parent = 0);
    ~ChatPage();
    void chatPageShow(QString str);
     QString myCount;

private slots:
    void timerHandler();//为时钟信号声明槽函数

    void on_addPushButton_clicked();

    void readDataHandler();

    void on_friendListTreeWidget_doubleClicked(const QModelIndex &index);

    void on_sendPushButton_clicked();

private:
    Ui::ChatPage *ui;
    QTimer *tm;
    QUdpSocket *socket;
};

#endif // CHATPAGE_H
