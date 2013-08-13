//Qt 4.8.4
//coded by spacewander,welcome to reuse if you want
#include "mainwidget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    MainWidget w;
    w.show();
    return a.exec();
}
