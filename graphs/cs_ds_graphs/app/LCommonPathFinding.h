
#pragma once

#include "../graph/LGraph.h"
#include "../gl/core/primitives/LPrimitivesRenderer2D.h"
#include "../common.h"

#include <pthread.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <map>

namespace app
{


	namespace graph
	{


        struct LPathFinderWorkData
        {
            DS::LNode<DS::LGraph<int, double> >* start;
            DS::LNode<DS::LGraph<int, double> >* end;
            DS::LNode<DS::LGraph<int, double> >* pathNode;
            int id;
        };


	}


}