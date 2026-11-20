#include <QApplication>
#include <QDir>

#include "../Data/Data.h"
#include "../Gui/MainWidget.h"

//---------------------------------------------------------------------------------
int main(int argc, char *argv[])
//---------------------------------------------------------------------------------
{
    QApplication app(argc, argv, true);

    // Create DataBase
    dm::Data::instance();

    gui::MainWidget *mainWidget = new gui::MainWidget();
    mainWidget->show();

    return app.exec();
}
