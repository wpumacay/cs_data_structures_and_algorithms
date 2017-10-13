

#include "LAppGraph.h"

#include <iostream>

using namespace std;

namespace app
{

	namespace graph
	{

		void LAppGraph::create()
		{
			cout << "creating graph app" << endl;

			if ( LAppGraph::instance != NULL )
			{
				delete LAppGraph::instance;
			}

			LAppGraph::instance = new LAppGraph();
			LAppGraph::instance->initialize();
		}

		void LAppGraph::createWorld()
		{
			cout << "creating graph-world" << endl;

			m_world = new LGraphWorld( 4000.0f, 2000.0f, 
                                       APP_WIDTH, APP_HEIGHT, 
                                       1.0f );
			m_stage->addChildScene( m_world->scene() );
		}

	}

}