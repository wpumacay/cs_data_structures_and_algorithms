
// #define TEST_MATRIX

#ifndef TEST_MATRIX


    #define APP
    // #define USE_QT
    // #define USE_GL
    #define NO_GRAPHICS

    #ifndef NO_GRAPHICS

    #include <QApplication>
    #include <QPushButton>
    #include "engine/qt/lwindow.h"

    #endif

#include "engine/LSolver.h"
#include "utils/LConsoleMenu.h"

#else

#include "engine/matrix/LMatrix.h"

#endif

int main( int argc, char **argv )
{
    int _ret = 0;

#ifdef APP

    #ifdef USE_QT
    QApplication app( argc, argv );

    engine::qtgp::LWindow _window;
    _window.show();

    engine::LSolver::createSolver( engine::options::optimizer::BS_GRADIENT_DESCENT,
                                   engine::options::intensifier::VND );

    _ret = app.exec();

    #else 

        #ifdef NO_GRAPHICS

    engine::LSolver::createSolver( engine::options::optimizer::BS_GRADIENT_DESCENT,
                                   engine::options::intensifier::VND );

    LConsoleMenu _menu;
    
    _menu.showMainMenu();

        #endif

    #endif

#else

    engine::mat::LMatD A = engine::mat::LMat<double>::simple( 2, 2, 
                                                              engine::mat::fill::ONES );
    engine::mat::LMatD B = engine::mat::LMat<double>::simple( 2, 2,
                                                              engine::mat::fill::ZEROS );

    B( 0, 0 ) = 1;
    B( 0, 1 ) = 2;
    B( 1, 0 ) = 3;
    B( 1, 1 ) = 4;

    engine::mat::LMatD C = A * B;

    A.print();
    B.print();
    C.print();

#endif

    return _ret;
}
