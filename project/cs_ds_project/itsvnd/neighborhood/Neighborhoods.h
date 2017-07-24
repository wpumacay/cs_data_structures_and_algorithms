
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "../../engine/LConfiguration.h"

using namespace std;

#define SET_SIZE 3

// Neighborhood structure given as :
// [ 0 -> [ ( c1, c3 ), ..., ( c5, c4 ) ] ,
//   1 -> [ ... ], ...]
typedef map<int, vector<engine::Pair<int,int>>> SwapNeighborhoods;

bool comparator( engine::Pair<int,double> p1, engine::Pair<int,double> p2 );

namespace neighborhood
{
	namespace swap
	{
        SwapNeighborhoods makeSwapNeighborhoods( engine::LConfiguration* pConfiguration );
	}

}
