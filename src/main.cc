#include "main_window.h"
#include "qapplication.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWidget w;
    w.show();
    w.showMaximized();

    return app.exec();
}