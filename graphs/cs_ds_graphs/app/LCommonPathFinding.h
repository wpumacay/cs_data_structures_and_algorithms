
#pragma once

#include "../graph/LGraph.h"
#include "../gl/core/primitives/LPrimitivesRenderer2D.h"
#include "../common.h"

#include <pthread.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <queue>

namespace app
{


	namespace graph
	{

        struct LPair
        {
            DS::LNode<DS::LGraph<int, double> >* node;
            float f;

            LPair( DS::LNode<DS::LGraph<int, double> >* pNode, float f )
            {
                this->node = pNode;
                this->f = f;
            }
        };

        struct LPathInfo
        {
            DS::LNode<DS::LGraph<int, double> >* start;
            int start_glIndx;
            DS::LNode<DS::LGraph<int, double> >* end;
            int end_glIndx;
            DS::LNode<DS::LGraph<int, double> >* pathNode;

            LPathInfo()
            {
                this->start = NULL;
                this->end = NULL;
                this->start_glIndx = -1;
                this->end_glIndx = -1;
                this->pathNode = NULL;
            }
        };

        struct LPathFinderWorkData
        {
        	DS::LGraph<int, double>* graphRef;
            DS::LNode<DS::LGraph<int, double> >* start;
            DS::LNode<DS::LGraph<int, double> >* end;
            DS::LNode<DS::LGraph<int, double> >* pathNode;
            int id;
            int working;
            vector<float>* pPreCalc;
            int* pLandmarkIDs;
        };

        struct LPairComparator
        {
            inline bool operator() ( const LPair& lhs,
                                     const LPair& rhs )
            {
                return lhs.f > rhs.f;
            }
        };

        struct LNodeComparator
        {

        	inline bool operator() ( const DS::LNode< DS::LGraph<int, double> >* lhs, 
        					         const DS::LNode< DS::LGraph<int, double> >* rhs )
        	{
        		return lhs->f > rhs->f;
        	}

        };

        struct LNodeComparatorDijkstra
        {
            inline bool operator() ( const DS::LNode< DS::LGraph<int, double> >* lhs, 
                                     const DS::LNode< DS::LGraph<int, double> >* rhs )
            {
                return lhs->d > rhs->d;
            }
        };

        typedef std::priority_queue< DS::LNode< DS::LGraph<int, double> >*,
        							 std::vector< DS::LNode< DS::LGraph<int, double> >* >,
        							 LNodeComparator > LNodePriorityQueue;

        typedef std::priority_queue< DS::LNode< DS::LGraph<int, double> >*,
                                     std::vector< DS::LNode< DS::LGraph<int, double> >* >,
                                     LNodeComparatorDijkstra > LNodePriorityQueueDijkstra;

        typedef std::priority_queue< LPair, 
                                     std::vector< LPair >, 
                                     LPairComparator > LPairPriorityQueue;

        namespace finderType
        {
            enum _finderType
            {
                SEARCH_A_STAR,
                SEARCH_A_STAR_LANDMARKS,
                SEARCH_DIJKSTRA
            };
        }

        class LNodeVectPriorityQueue
        {
            private :

            std::vector< DS::LNode< DS::LGraph<int, double> >* > m_buff;

            public :

            void push( DS::LNode< DS::LGraph<int, double> >* pNode )
            {
                m_buff.push_back( pNode );
            }

            DS::LNode< DS::LGraph<int, double> >* top()
            {
                int _bestIndx = 0;
                for ( int q = 1; q < m_buff.size(); q++ )
                {
                    if ( m_buff[q]->d < m_buff[_bestIndx]->d )
                    {
                        _bestIndx = q;
                    }
                }

                return m_buff[_bestIndx];
            }

            void pop()
            {
                int _bestIndx = 0;
                for ( int q = 1; q < m_buff.size(); q++ )
                {
                    if ( m_buff[q]->d < m_buff[_bestIndx]->d )
                    {
                        _bestIndx = q;
                    }
                }

                m_buff.erase( m_buff.begin() + _bestIndx );
            }

            bool empty()
            {
                return m_buff.size() < 1;
            }
        };

	}


}