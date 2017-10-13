
#include "common.h"

#ifdef RENDER_QT

#include "mainwindow.h"
#include <QApplication>

#else

#include "app/LAppGraph.h"

#endif

int main(int argc, char *argv[])
{
#ifdef RENDER_QT

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

#else

    app::graph::LAppGraph::create();
    app::graph::LAppGraph::instance->loop();
    app::graph::LAppGraph::destroy();

    return 0;

#endif
}
