
#pragma once

#include "LCommonPathFinding.h"

using namespace std;
using namespace engine;





namespace app
{


	namespace graph
	{




		class LPathFinderInterface
		{

			protected :

			pthread_t m_threadHandle;
			LPathFinderWorkData m_wData;

			DS::LGraph<int, double>* m_graphRef;

			DS::LNode<DS::LGraph<int, double> >* m_start;
			DS::LNode<DS::LGraph<int, double> >* m_end;
			DS::LNode<DS::LGraph<int, double> >* m_pathNode;

			public :

			




		};


	}

}