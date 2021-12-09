#include <QApplication>
#include <QScreen>
#include "Backend.h"
#include "TodoWindow.h"
#include "TodoBlock.h"
#include <iostream>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QFile f("://stylesheet.qss");
    if(f.open(QFile::ReadOnly))
    {
        std::cout << "[!] Stylesheet Set" <<std::endl;
        app.setStyleSheet(f.readAll());
    }

    app.setWindowIcon(QIcon("://icon.png"));
    Backend b(QApplication::primaryScreen()->geometry());
    b.show();
    return app.exec();
}
