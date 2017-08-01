
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "../../engine/LConfiguration.h"
#include "../optimization/LBaseOptimizer.h"

using namespace std;

#define SET_SIZE 3

// Neighborhood structure given as :
// [ 0 -> [ ( c1, c3 ), ..., ( c5, c4 ) ] ,
//   1 -> [ ... ], ...]
typedef map<int, vector<engine::Pair<int,int>>> SwapNeighborhoods;

typedef vector<engine::Pair<engine::LPoint,double>> InsertNeighborhood;

bool comparator( engine::Pair<int,double> p1, engine::Pair<int,double> p2 );
bool insertComparator( engine::Pair<engine::LPoint,double> p1,
                       engine::Pair<engine::LPoint,double> p2 );

namespace neighborhood
{
	namespace swap
	{
        SwapNeighborhoods makeSwapNeighborhoods( engine::LConfiguration* pConfiguration );
	}

	namespace insert
	{
		InsertNeighborhood makeInsertNeighborhood( engine::LConfiguration* pConfiguration,
                                                   optimizers::LBaseOptimizer* pOptimizer );
	}

}
