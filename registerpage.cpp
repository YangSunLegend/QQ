#include "registerpage.h"
#include "ui_registerpage.h"
#include "showresultpage.h"
#include "protocol.h"
#include "QByteArray"
#include "QCryptographicHash"
#include "QDebug"
#include "QHostAddress"

RegisterPage::RegisterPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterPage)
{
    ui->setupUi(this);

    socket = new QUdpSocket();
    //qt4 bind  --->
    socket->bind(0);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readDataHandler()));
}

//当socket可读，则调用槽函数
void RegisterPage::readDataHandler()
{
    struct register_st msgRcv;
    ShowResultPage *resultPage ;

    resultPage= new ShowResultPage();
    resultPage->setAttribute(Qt::WA_DeleteOnClose);

    socket->readDatagram((char *)&msgRcv, sizeof(msgRcv));
    switch(msgRcv.status) {
    case REGISTER_STATUS_OK:
        resultPage->setResultLineEdit(QString("恭喜你！注册成功"));
        resultPage->setCommitPushButton(QString("马上登录"));
        resultPage->setCancelPushButton(QString("再来一次"));
        break;
    case REGISTER_STATUS_ERROR:
        resultPage->setResultLineEdit(QString("很遗憾！注册失败"));
        resultPage->setCommitPushButton(QString("再来一次"));
        resultPage->setCancelPushButton(QString("还是算了吧"));
        break;
    default:
        break;
    }
    resultPage->show();

}


RegisterPage::~RegisterPage()
{
    delete ui;
}

void RegisterPage::on_pushButton_clicked()
{
    struct register_st msgData;

    QString cntstr = ui->cntLineEdit->text();
    QString pwdstr = ui->pwdLineEdit->text();

    //QString--->QByteArray--->cstring
    QByteArray ba = cntstr.toAscii();
    const char *cnt = ba.data();
    qsnprintf(msgData.count, COUNT_SIZE, "%s", cnt);

    //passwd--->crypt--->cstring
    QByteArray ba1 = pwdstr.toAscii();
    ba1 = QCryptographicHash::hash(ba1, QCryptographicHash::Md5);
    QString crypwd = ba1.toHex();
    QByteArray ba2 = crypwd.toAscii();
    const char *pwd = ba2.data();
    qsnprintf(msgData.passwd, COUNT_SIZE, "%s", pwd);

    qDebug() << cnt << pwd << endl;

    QHostAddress *addressHost = new \
            QHostAddress(QString(REGISTER_SERVER_IP));
    socket->writeDatagram((const char *)&msgData, sizeof(msgData),\
                          *addressHost, REGISTER_SERVER_PORT);

    /*

*/
}
