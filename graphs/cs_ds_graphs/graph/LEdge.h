
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

		LEdge( E edge_data )
		{
			data = edge_data;
		}
	};
	
}