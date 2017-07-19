#include <QApplication>
#include <QPushButton>
#include "engine/qt/lwindow.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    engine::qtgp::LWindow _window;
    _window.show();

    return app.exec();
}
