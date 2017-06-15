#include "login.h"
#include "ui_login.h"
#include "QPixmap"
#include "QString"

#include "registerpage.h"
#include "chatpage.h"
#include "protocol.h"
#include "QCryptographicHash"
#include "QHostAddress"
#include "showresultpage.h"
#include "sqlite3.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    int w, h;


    //添加logo图片  和 头像
    QString *logoPath = new \
            QString("/var/ftp/pub/apue/qq/imgs/lg.png");
    QPixmap *logoMap = new QPixmap(*logoPath);
    w = ui->logoLabel->width();
    h = ui->logoLabel->height();
    *logoMap = logoMap->scaled(w, h);
    ui->logoLabel->setPixmap(*logoMap);

    QString *headPath = new \
            QString("/var/ftp/pub/apue/qq/imgs/men.png");
    QPixmap *headMap = new QPixmap(*headPath);
    w = ui->headLabel->width();
    h = ui->headLabel->height();
    *headMap =headMap->scaled(w, h);
    ui->headLabel->setPixmap(*headMap);

    socket = new QUdpSocket();
    socket->bind(0);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readDataHandler()));

    isRemember = false;
    //创建数据库
    sqlite3_open("/var/ftp/pub/apue/qq/remberpasswd.sqlite", &db);
}

void Login::readDataHandler(void)
{
    struct login_st rcvbuf;
    ChatPage *chat;
    ShowResultPage *resultPage;

    socket->readDatagram((char *)&rcvbuf, sizeof(rcvbuf));

    switch(rcvbuf.status) {
    case LOGIN_STATUS_OK:
        chat = new ChatPage();
        chat->setAttribute(Qt::WA_DeleteOnClose);
        chat->chatPageShow(QString(rcvbuf.count));

        if (isRemember)
            rememberPasswd();

        break;
    case LOGIN_STATUS_ERROR:
        resultPage = new ShowResultPage();
        resultPage->setAttribute(Qt::WA_DeleteOnClose);
        resultPage->setResultLineEdit(QString("登录失败！请检查"));
        resultPage->setCommitPushButton(QString("重新登录"));
        resultPage->setCancelPushButton(QString("我已放弃"));
        resultPage->show();
        break;
    default:
        break;
    }

}


Login::~Login()
{
    sqlite3_close(db);
    delete ui;
}
//注册按钮的槽函数
void Login::on_registerPushButton_clicked()
{
    RegisterPage * newPage = new RegisterPage();
    //关闭界面时自动销毁
    newPage->setAttribute(Qt::WA_DeleteOnClose);
    newPage->show();
}

//登录按钮的槽函数
void Login::on_loginPushButton_clicked()
{
    struct login_st sndbuf;

    //将登录帐号和密码发送给服务器
    QString cntstr = ui->countLineEdit->text();
    QString pwdstr = ui->passwdLineEdit->text();

    QByteArray ba1 = cntstr.toAscii();
    const char *c = ba1.data();
    qsnprintf(sndbuf.count, COUNT_SIZE, "%s", c);

    QByteArray ba2 = pwdstr.toAscii();
    ba2 = QCryptographicHash::hash(ba2, QCryptographicHash::Md5);
    QString pstr = ba2.toHex();
    QByteArray ba3 = pstr.toAscii();
    const char *p = ba3.data();
    qsnprintf(sndbuf.passwd, COUNT_SIZE, "%s", p);

    qDebug() << sndbuf.count << sndbuf.passwd << endl;

    QHostAddress *address = new QHostAddress(QString(LOGIN_SERVER_IP));

    socket->writeDatagram((const char *)&sndbuf, sizeof(sndbuf), *address, \
                          LOGIN_SERVER_PORT);

#if 0

#endif
}

//记住密码
int Login::rememberPasswd(void)
{
    const char *sql;
    char *errmsg = NULL;
    sqlite3_stmt *stmt = NULL;
    QByteArray ba1;
    const char *pw;
    int ret;
    //创建保存密码的表
    sql = "create table if not exists rmpasswd(count text not null, \
            passwd text not null)";
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (ret != SQLITE_OK){
        fprintf(stderr, "sqlite3_exec():%s\n", errmsg);
        return -1;
    }

    //查询rmpasswd表中是否有输入帐号的密码
    //有则更新，没有则插入
    sql = "select * from rmpasswd where count=?";
    ret = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (ret != SQLITE_OK)
        return -1;
    QByteArray ba =  ui->countLineEdit->text().toAscii();
    const char *cnt = ba.data();
    sqlite3_bind_text(stmt, 1, cnt, -1, NULL);
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE && ret != SQLITE_ROW)
           goto step_error;
    if (ret == SQLITE_ROW) {
        //更新密码
        sql = "update rmpasswd set passwd=? where count=?";
    }  else {
        //密码未曾记住
        sql = "insert into rmpasswd(passwd, count) values(?,?)";
    }
    sqlite3_finalize(stmt);
    ret = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (ret != SQLITE_OK)
        return -1;
    ba1 =  ui->passwdLineEdit->text().toAscii();
    pw = ba1.data();
    sqlite3_bind_text(stmt, 1, pw, -1, NULL);
    sqlite3_bind_text(stmt, 2, cnt, -1, NULL);
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
        goto step_error;

    sqlite3_finalize(stmt);
    return 0;
    step_error:
    sqlite3_finalize(stmt);
    return -1;
}

void Login::on_remembPwRadioButton_clicked()
{
    isRemember =  ui->remembPwRadioButton->isChecked();
}

//帐号在改变，查找记住密码数据库是否有记录，arg1就是lineEdit中的文本
void Login::on_countLineEdit_textChanged(const QString &arg1)
{
    const char *sql;
    sqlite3_stmt *stmt = NULL;
    int ret;

    sql = "select * from rmpasswd where count=?";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    QByteArray ba = arg1.toAscii();
    const char *c = ba.data();
    sqlite3_bind_text(stmt, 1, c, -1, NULL);
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        //找到记住的密码，显示
        const char *pw = (const char * )sqlite3_column_text(stmt, 1);
        ui->passwdLineEdit->setText(QString(pw));
    } else {
        ui->passwdLineEdit->clear();
    }

    sqlite3_finalize(stmt);
}
