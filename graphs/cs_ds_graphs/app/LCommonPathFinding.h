
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

        class LHeap
        {
            private :

            vector< DS::LNode< DS::LGraph< int, double > >* > m_arr;

            public :

            LHeap( const vector< DS::LNode< DS::LGraph< int, double > >* >& vNodes )
            {
                for ( int q = 0; q < vNodes.size(); q++ )
                {
                    m_arr.push_back( vNodes[q] );
                }

                for ( int q = ceil( m_arr.size() / 2 ); q >= 0; q-- )
                {
                    minHeapify( q );
                }
            }

            void minHeapify( int i )
            {
                int _l = left( i );
                int _r = right( i );
                int _smallest = i;

                if ( _l < m_arr.size() && m_arr[_l]->d <= m_arr[_smallest]->d )
                {
                    _smallest = _l;
                }

                if ( _r < m_arr.size() && m_arr[_r]->d < m_arr[_smallest]->d )
                {
                    _smallest = _r;
                }

                if ( _smallest != i )
                {
                    swap( i, _smallest );
                    minHeapify( _smallest );
                }
            }

            DS::LNode< DS::LGraph< int, double > >* extractMinimum()
            {
                if ( m_arr.size() < 1 )
                {
                    return NULL;
                }

                if ( m_arr.size() == 1 )
                {
                    DS::LNode< DS::LGraph< int, double > >* _res = m_arr[0];
                    m_arr.clear();
                    return _res;
                }

                DS::LNode< DS::LGraph< int, double > >*  _res = m_arr[0];
                cout << "_res: " << _res->d << endl;
                cout << "size: " << m_arr.size() << endl;
                m_arr[0] = m_arr[m_arr.size() - 1];
                m_arr.pop_back();
                cout << "nsize: " << m_arr.size() << endl;

                cout << "_res: " << _res->d << endl;

                minHeapify( 0 );

                for ( int q = 0; q < m_arr.size(); q++ )
                {
                    if ( m_arr[q]->d != INF )
                    {
                        cout << "?? " << m_arr[q]->d << " - q: " << q << endl;
                    }
                }

                cout << "m_arr[0]: " << m_arr[0]->d << endl;

                cin.get();

                return _res;
            }

            int left( int i )
            {
                return 2 * i + 1;
            }

            int right( int i )
            {
                return 2 * i + 2;
            }

            int parent( int i )
            {
                return floor( ( i - 1 ) / 2 );
            }

            void swap( int i, int j )
            {
                DS::LNode< DS::LGraph< int, double > >* _tmp = m_arr[i];
                m_arr[i] = m_arr[j];
                m_arr[j] = _tmp;
            }

            DS::LNode< DS::LGraph< int, double > >* getMin()
            {
                return m_arr[0];
            }

            bool isEmpty()
            {
                return m_arr.size() < 1;
            }
        };

        class LSpecialPriorityQueueDijsktra
        {

            private :

            LHeap* m_heap;

            public :

            LSpecialPriorityQueueDijsktra( const vector< DS::LNode< DS::LGraph< int, double > >* >& vNodes )
            {
                m_heap = new LHeap( vNodes );
            }

            DS::LNode< DS::LGraph< int, double > >* top()
            {
                return m_heap->getMin();
            }

            DS::LNode< DS::LGraph< int, double > >* pop()
            {
                return m_heap->extractMinimum();
            }            

            bool empty()
            {
                return m_heap->isEmpty();
            }

        };

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