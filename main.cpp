// Under Development..

#include "mainpadnote.h"
#include <QApplication>
#include<QDesktopWidget>
#include<QGuiApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainPadNote w(QApplication::desktop()->screenGeometry());
    w.show();

    return a.exec();
}
