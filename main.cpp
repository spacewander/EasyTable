/**
*@file
*EasyTable 's main() is here
*/

//Qt 4.8.4
#include "mainwidget.h"
#include <QApplication>
#include <QTextCodec>

/**
*I bet you know what is main function
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    MainWidget w;
    w.show();
    return a.exec();
}
