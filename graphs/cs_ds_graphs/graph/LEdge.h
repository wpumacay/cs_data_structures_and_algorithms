#pragma once

#include "LGraph.h"

namespace DS
{

	template<class G>
	class LEdge
	{
		public :

		typedef typename G::EdgeData E;
		typedef typename G::Node Node;

		E data;
		Node* nodes[2];

		int glIndx;
		bool drawable;

		LEdge( E edge_data )
		{
			data = edge_data;
			glIndx = -1;
			drawable = false;
		}
	};
	
}