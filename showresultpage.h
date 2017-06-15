#ifndef SHOWRESULTPAGE_H
#define SHOWRESULTPAGE_H

#include <QDialog>

namespace Ui {
class ShowResultPage;
}

class ShowResultPage : public QDialog
{
    Q_OBJECT
    
public:
    explicit ShowResultPage(QWidget *parent = 0);
    ~ShowResultPage();
    void setResultLineEdit(QString str);
    void setCommitPushButton(QString str);
    void setCancelPushButton(QString str);
    
private slots:
    void on_commitPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::ShowResultPage *ui;
};

#endif // SHOWRESULTPAGE_H
