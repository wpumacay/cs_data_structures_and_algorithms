#include <QApplication>
#include <QPushButton>
#include "engine/qt/lwindow.h"
#include "engine/LSolver.h"

#define USE_QT
// #define USE_GL
// #define NO_GRAPHICS

int main( int argc, char **argv )
{
    int _ret = 0;

    #ifdef USE_QT
    QApplication app( argc, argv );

    engine::qtgp::LWindow _window;
    _window.show();

    engine::LSolver::createSolver( engine::options::optimizer::BS_GRADIENT_DESCENT,
                                   engine::options::intensifier::VND );

    _ret = app.exec();

    #elif NO_GRAPHICS

    engine::LSolver::createSolver( engine::options::optimizer::BS_GRADIENT_DESCENT,
                                   engine::options::intensifier::VND );

    engine::LSolver::instance->init( engine::circleInstance::INST_r_i );
    // engine::LSolver::instance->solve();
    #endif

    return _ret;
}
