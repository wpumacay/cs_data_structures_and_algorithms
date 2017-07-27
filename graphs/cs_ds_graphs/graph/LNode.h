
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

        int id;

        double f,g,h;

        LNode* parent;

        LNode( N node_data, double x, double y, int id )
		{
			data = node_data;
            this->x = x;
            this->y = y;
            this->id = id;
            this->f = 0;
            this->g = 0;
            this->h = 0;
            this->parent = nullptr;
		}
	};
	
}
