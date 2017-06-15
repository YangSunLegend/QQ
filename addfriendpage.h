#ifndef ADDFRIENDPAGE_H
#define ADDFRIENDPAGE_H

#include <QDialog>
#include "QUdpSocket"
#include "QTreeWidget"

namespace Ui {
class AddFriendPage;
}

class AddFriendPage : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddFriendPage(QWidget *parent = 0);
    ~AddFriendPage();
    QString myCount;//记录主动添加的帐号
    QTreeWidget *friendWidget;
    
private slots:
    void on_pushButton_clicked();
    void readAddFriendData();

private:
    Ui::AddFriendPage *ui;
    QUdpSocket *socket;
};

#endif // ADDFRIENDPAGE_H
