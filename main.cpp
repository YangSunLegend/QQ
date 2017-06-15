#include <QApplication>
#include "login.h"
#include "QTextCodec"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置编码格式 支持中文
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);

    Login w;
    w.show();
    
    return a.exec();
}
