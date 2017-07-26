
#pragma once

#include "LGraph.h"
#include <vector>

using namespace std;

namespace DS
{

	template<class G>
	class LNode
	{
		public :

		typedef typename G::NodeData N;
		typedef typename G::Edge Edge;
		
		N data;
		vector<Edge*> edges;

		double x;
		double y;

        LNode( N node_data, double x, double y )
		{
			data = node_data;
            this->x = x;
            this->y = y;
		}
	};
	
}
