
#include "common.h"

#ifdef RENDER_QT

#include "mainwindow.h"
#include <QApplication>

#else

#include "app/LAppGraph.h"

#endif

#ifdef APP_TEST

#include "app/LCommonPathFinding.h"

#endif

int main(int argc, char *argv[])
{
#ifdef APP_TEST

	DS::LNode<DS::LGraph<int, double>>* _n1 = new DS::LNode<DS::LGraph<int, double>>( 1, 0, 0, 0 );
	DS::LNode<DS::LGraph<int, double>>* _n2 = new DS::LNode<DS::LGraph<int, double>>( 10, 0, 0, 1 );
	DS::LNode<DS::LGraph<int, double>>* _n3 = new DS::LNode<DS::LGraph<int, double>>( 100, 0, 0, 2 );

	_n1->d = 1;
	_n2->d = 10;
	_n3->d = 100;

	app::graph::LNodePriorityQueueDijkstra _pq;
	//app::graph::LNodeVectPriorityQueue _pq;
	_pq.push( _n1 );
	_pq.push( _n2 );
	_pq.push( _n3 );

	while ( !_pq.empty() )
	{
		DS::LNode<DS::LGraph<int, double>>* _n = _pq.top();
		cout << "id: " << _n->id << " d: " << _n->d << endl;
		_pq.pop();
	}

	return 0;

#elif RENDER_QT

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
