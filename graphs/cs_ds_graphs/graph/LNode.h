#pragma once

#include "LGraph.h"
#include <vector>
#include <utility>

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

		double x;// X coordinate in the scene
		double y;// Y coordinate in the scene

        int id;// Unique identifier, the same as index in graph's vector of nodes

       	int glIndx;

        double f,g,h; // For A* search

        LNode* parent;// For A* search path reconstruction
       	pair<LNode*,Edge*> parentInfo;

        LNode( N node_data, double x, double y, int id )
		{
			data = node_data;
			this->glIndx = -1;
            this->x = x;
            this->y = y;
            this->id = id;
            this->f = 0;
            this->g = 0;
            this->h = 0;
            this->parent = NULL;
           	this->parentInfo.first = NULL;
           	this->parentInfo.second = NULL;
		}
	};
	
}