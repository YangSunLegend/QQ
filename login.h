#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "QUdpSocket"
#include "sqlite3.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    
private slots:
    void on_registerPushButton_clicked();

    void on_loginPushButton_clicked();

    void readDataHandler();

    void on_remembPwRadioButton_clicked();

    void on_countLineEdit_textChanged(const QString &arg1);

private:
    Ui::Login *ui;
    QUdpSocket *socket;
    bool isRemember;
    sqlite3 *db;
    int rememberPasswd(void);
};

#endif // LOGIN_H
