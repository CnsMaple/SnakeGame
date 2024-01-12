
#include <QApplication>
#include <QWidget>

#include  "GameShowWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameShowWidget gameShowWidget;
    gameShowWidget.show();

    return app.exec();
}
