#include "addfriendpage.h"
#include "ui_addfriendpage.h"
#include "protocol.h"
#include "QDebug"
#include "QTreeWidgetItem"
#include "showresultpage.h"

AddFriendPage::AddFriendPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriendPage)
{
    ui->setupUi(this);
    socket = new QUdpSocket();
    socket->bind(0);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readAddFriendData()));
}

void AddFriendPage::readAddFriendData()
{
    struct addfrd_st rcvbuf;
    QTreeWidgetItem *item;
    ShowResultPage *resultPage;

    resultPage = new ShowResultPage();
    resultPage->setAttribute(Qt::WA_DeleteOnClose);

    qDebug() << "debug"<<endl;
    socket->readDatagram((char *)&rcvbuf, sizeof(rcvbuf));
    qDebug() << rcvbuf.status << endl;
    switch(rcvbuf.status) {
    case ADD_STATUS_OK:
        item = new QTreeWidgetItem(friendWidget);
        item->setIcon(0, QIcon("/var/ftp/pub/apue/qq/imgs/men.png"));
        item->setText(0, QString(rcvbuf.dst_count));
        close();
        break;
    case ADD_STATUS_ERROR:
        resultPage->setResultLineEdit(QString("很遗憾！添加失败"));
        resultPage->setCommitPushButton(QString("重新添加"));
        resultPage->setCancelPushButton(QString("还是算了吧"));
        resultPage->show();
        break;
    case ADD_STATUS_EXISTS:
        resultPage->setResultLineEdit(QString("已经是您的好友"));
        resultPage->setCommitPushButton(QString("重新添加"));
        resultPage->setCancelPushButton(QString("开始畅聊"));
        resultPage->show();
        break;
    default:
        break;
    }

}

AddFriendPage::~AddFriendPage()
{
    delete ui;
}

void AddFriendPage::on_pushButton_clicked()
{
    struct addfrd_st sndbuf;
    //将待添加好友的帐号发送给服务器

    QByteArray ba1 = myCount.toAscii();
    const char *src = ba1.data();
    qsnprintf(sndbuf.src_count, COUNT_SIZE, "%s", src);

    QString destStr = ui->cntLineEdit->text();
    QByteArray ba2 = destStr.toAscii();
    const char *dest = ba2.data();
    qsnprintf(sndbuf.dst_count, COUNT_SIZE, "%s", dest);

    qDebug() << sndbuf.src_count << sndbuf.dst_count << endl;
    QHostAddress *address = new QHostAddress(\
                QString(ADDFRIEND_SERVER_IP));
    socket->writeDatagram((const char *)&sndbuf, sizeof(sndbuf), \
                          *address, ADDFRIEND_SERVER_PORT);


}
