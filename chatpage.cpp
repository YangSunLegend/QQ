#include "chatpage.h"
#include "ui_chatpage.h"
#include "QPixmap"
#include "QString"
#include "QDateTime"
#include "addfriendpage.h"
#include "QTreeWidgetItem"
#include "protocol.h"

ChatPage::ChatPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);

    QString *headPath = new \
            QString("/var/ftp/pub/apue/qq/imgs/women.png");
    QPixmap *headMap = new QPixmap(*headPath);
    *headMap = headMap->scaled(ui->headLabel->size());
    ui->headLabel->setPixmap(*headMap);

    QString *setupPath = new \
            QString("/var/ftp/pub/apue/qq/imgs/setup.png");
    QPixmap *setupMap = new QPixmap(*setupPath);
    *setupMap = setupMap->scaled(ui->setupPushButton->size());
    ui->setupPushButton->setIcon(QIcon(*setupMap));

    QString *addPath = new \
            QString("/var/ftp/pub/apue/qq/imgs/add.png");
    QPixmap *addMap = new QPixmap(*addPath);
    *addMap = addMap->scaled(ui->addPushButton->size());
    ui->addPushButton->setIcon(QIcon(*addMap));

    //创建时钟对象
    tm = new QTimer();
    //链接信号与槽
    connect(tm, SIGNAL(timeout()), this, SLOT(timerHandler()));
    tm->start(1000); //alarm(1)

    socket = new QUdpSocket();
    socket->bind(0);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readDataHandler()));
}

void ChatPage::readDataHandler()
{
    struct chat_st rcvbuf;
    QTreeWidgetItem *item;
    QString sndmsg;

    socket->readDatagram((char *)&rcvbuf, sizeof(rcvbuf));

    qDebug() << rcvbuf.status << endl;
    switch(rcvbuf.status) {
    case CHAT_STATUS_LIST:
        //remmeber address server 发送的好友列表
        item = new QTreeWidgetItem(ui->friendListTreeWidget);
        item->setIcon(0, QIcon("/var/ftp/pub/apue/qq/imgs/women.png"));
        item->setText(0, QString(rcvbuf.dst_count));
        break;
    case CHAT_STATUS_MSG:
        //聊天服务器发送的聊天消息
        //[one >>> two]
        qDebug() << "the msg coming" << endl;
        sndmsg.clear();
        sndmsg.append("<font size=5 color=blue>");
        sndmsg.append("[");
        sndmsg.append(rcvbuf.src_count);
        sndmsg.append(" >>> ");
        sndmsg.append(rcvbuf.dst_count);
        sndmsg.append("] ");
        sndmsg.append(rcvbuf.msg);
        sndmsg.append("</font>");

        ui->chatTextEdit->append(sndmsg);

        break;
    default:
        break;
    }

}

void ChatPage::timerHandler()
{
    QDateTime date = QDateTime::currentDateTime();
    QString str = date.toString(QString("yy-MM-dd hh:mm:ss"));

    ui->lcdNumber->display(str);
}

//重写show方法Str
void ChatPage::chatPageShow(QString curcount)
{
    struct remember_st sndbuf;
    myCount = curcount;
/*
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->friendListTreeWidget);
    item->setIcon(0, QIcon("/var/ftp/pub/apue/qq/imgs/women.png"));
    item->setText(0, curcount);
*/
    ui->cntLabel->setText(myCount);

    //将聊天用户的帐号和地址发送记住地址服务器
    QByteArray ba = curcount.toAscii();
    const char *c = ba.data();
    qsnprintf(sndbuf.count, COUNT_SIZE, "%s", c);

    QHostAddress *address = new\
            QHostAddress(QString(REMEMBER_ADDR_IP));
    socket->writeDatagram((const char *)&sndbuf, sizeof(sndbuf),\
                          *address, REMEMBER_ADDR_PORT);

    timerHandler();
    show();
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::on_addPushButton_clicked()
{
    AddFriendPage *add = new AddFriendPage();
    add->setAttribute(Qt::WA_DeleteOnClose);
    add->myCount = myCount;
    add->friendWidget = ui->friendListTreeWidget;
    add->show();

}

void ChatPage::on_friendListTreeWidget_doubleClicked(const QModelIndex &index)
{
    QTreeWidgetItem *item= ui->friendListTreeWidget->currentItem();
    ui->cntLabel->setText(item->text(0));
}

void ChatPage::on_sendPushButton_clicked()
{
    QString sndmsg;
    QString msg = ui->inputTextEdit->toPlainText();

    //[one >>> two]
    sndmsg.clear();
    sndmsg.append("<font size=5 color=red>");
    sndmsg.append("[");
    sndmsg.append(myCount);
    sndmsg.append(" >>> ");
    sndmsg.append(ui->cntLabel->text());
    sndmsg.append("] ");
    sndmsg.append(msg);
    sndmsg.append("</font>");

     ui->chatTextEdit->append(sndmsg);
     ui->inputTextEdit->clear();

    //将聊天内容发送给服务器
     struct chat_st sndbuf;

     QByteArray ba = myCount.toAscii();
     const char *c = ba.data();
     qsnprintf(sndbuf.src_count, COUNT_SIZE, "%s", c);

     QByteArray ba1 = ui->cntLabel->text().toAscii();
     const char *d = ba1.data();
     qsnprintf(sndbuf.dst_count, COUNT_SIZE, "%s", d);

     QByteArray ba2 = msg.toAscii();
     const char *s = ba2.data();
     qsnprintf(sndbuf.msg, COUNT_SIZE, "%s", s);

     QHostAddress *address = new QHostAddress\
             (QString(CHAT_SERVER_IP));

     socket->writeDatagram((const char *)&sndbuf, sizeof(sndbuf),\
                           *address, CHAT_SERVER_PORT);

}







