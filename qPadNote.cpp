#include <QApplication>
#include <QDesktopWidget>
#include "Backend.h"
#include "TodoWindow.h"
#include "TodoBlock.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Backend b(QApplication::desktop()->screenGeometry(),&app);

    return app.exec();
}
