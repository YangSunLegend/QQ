#include "showresultpage.h"
#include "ui_showresultpage.h"

ShowResultPage::ShowResultPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowResultPage)
{
    ui->setupUi(this);
}

ShowResultPage::~ShowResultPage()
{
    delete ui;
}

void ShowResultPage::setResultLineEdit(QString str)
{
    ui->resultLineEdit->setText(str);
}
void ShowResultPage::setCommitPushButton(QString str)
{
    ui->commitPushButton->setText(str);
}
void ShowResultPage::setCancelPushButton(QString str)
{
    ui->cancelPushButton->setText(str);
}

void ShowResultPage::on_commitPushButton_clicked()
{
    close();
}

void ShowResultPage::on_cancelPushButton_clicked()
{
    close();
}
