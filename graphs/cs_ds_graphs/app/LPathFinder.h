
#pragma once

#include "../graph/LGraph.h"
#include "../common.h"

#include <cmath>
#include <vector>
#include <map>

namespace app
{


	namespace graph
	{




		class LPathFinder
		{

			private :

			DS::LGraph<int, double>* m_graphRef;

			public :

			int id;

			LPathFinder( DS::LGraph<int, double>* pGraph, int pId )
			{
				m_graphRef = pGraph;
				id = pId;
			}

			virtual void calculatePath()
			{
				
			}



		};



	}






}
