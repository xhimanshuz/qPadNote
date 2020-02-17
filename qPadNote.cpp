#include <QApplication>
#include "Backend.h"
#include "TodoWindow.h"
#include "TodoBlock.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Backend b(&app);

    return app.exec();
}
