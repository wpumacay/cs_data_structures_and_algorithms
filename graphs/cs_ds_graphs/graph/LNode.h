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
        float ff[MAX_PARALLEL_REQUESTS];
        float gg[MAX_PARALLEL_REQUESTS];
        float hh[MAX_PARALLEL_REQUESTS];

        LNode* parent;// For A* search path reconstruction

       	float d;// For Dijkstra precalculation

       	pair<LNode*,Edge*> parentInfo[MAX_PARALLEL_REQUESTS];

        bool inOpen;

        LNode( N node_data, double x, double y, int id )
		{
			data = node_data;
            inOpen = false;
			this->glIndx = -1;
            this->x = x;
            this->y = y;
            this->id = id;
            this->f = 0;
            this->g = 0;
            this->h = 0;
            this->d = INF;
            this->parent = NULL;

       		for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
       		{
       			this->parentInfo[q].first = NULL;
       			this->parentInfo[q].second = NULL;

                this->ff[q] = 0.0f;
                this->gg[q] = 0.0f;
                this->hh[q] = 0.0f;
       		}
		}
	};
	
}