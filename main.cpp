
#include <QApplication>
#include <QWidget>

#include  "GameShowWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameShowWidget *gameShowWidget = new GameShowWidget();
    gameShowWidget->show();

    return app.exec();
}
