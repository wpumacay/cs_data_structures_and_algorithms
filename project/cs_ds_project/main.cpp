
// #define TEST_MATRIX
#define TEST_GL

#if defined( TEST_MATRIX ) || defined( TEST_GL )
#define NO_TEST_APP
#endif

#ifndef NO_TEST_APP


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

    #ifdef TEST_MATRIX

        #include "engine/matrix/LMatrix.h"

    #elif defined( TEST_GL )
        #include "engine/gl/LBaseCircle2D.h"
        #include "engine/gl/LBaseObject2D.h"
        #include "engine/gl/LApp.h"

    #endif

#endif

#include <iostream>

using namespace std;

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

#elif defined( TEST_MATRIX )

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

#elif defined( TEST_GL )

    cout << "initialized gl" << endl;

    engine::gl::LApp _app;
    _app.initialize();

    engine::gl::LBaseCircle2D* _obj1 = new engine::gl::LBaseCircle2D( 0.0, 0.0 );
    engine::gl::LBaseObject2D* _obj2 = new engine::gl::LBaseObject2D( 0.2, 0.2 );
    _obj2->scale.x = 3;
    _obj2->scale.y = 3;
    engine::gl::LScene* _stage = _app.stage();
    _stage->addObject2D( _obj1 );
    _stage->addObject2D( _obj2 );

    _app.loop();
    _app.finalize();


#endif

    return _ret;
}
