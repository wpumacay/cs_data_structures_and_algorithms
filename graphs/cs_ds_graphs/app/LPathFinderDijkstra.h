
#pragma once

#include "LPathFinderInterface.h"

using namespace std;
using namespace engine;

namespace app
{


	namespace graph
	{


		class LPathFinderDijkstra : public LPathFinderInterface
		{

			public :

			LPathFinderDijkstra( DS::LGraph<int, double>* pGraph, int pId )
			{
				m_graphRef = pGraph;
				id = pId;
				isRunning = false;

				m_start = NULL;
				m_end = NULL;
				m_pathNode = NULL;
				start_glIndx = -1;
				end_glIndx = -1;
			}

			void launch() override
			{
				pthread_create( &m_threadHandle, NULL, LPathFinderDijkstra::calculatePath, ( void* ) &m_wData );
			}

			static void* calculatePath( void* pWorkData )
			{
                LPathFinderWorkData* _wData = ( LPathFinderWorkData* ) pWorkData;

                LPairPriorityQueue _toExplore;

                for ( int q = 0; q < _wData->graphRef->nodes.size(); q++ )
                {
                    _wData->graphRef->nodes[q]->gg[_wData->id] = INF;
                }

                // Calculate the first heuristic value
                _wData->start->gg[_wData->id] = 0;

                for ( int q = 0; q < _wData->graphRef->nodes.size(); q++ )
                {
                    _toExplore.push( LPair( _wData->graphRef->nodes[q], _wData->graphRef->nodes[q]->gg[_wData->id] ) );
                }

                bool found = false;
                int _opCount = 0;

                DS::LNode<DS::LGraph<int, double>>* _pathNode = NULL;

                while ( !_toExplore.empty() )
                {

                    DS::LNode<DS::LGraph<int,double>>* _nextToExplore = ( _toExplore.top() ).node;
                    _toExplore.pop();

                    for ( int q = 0; q < _nextToExplore->edges.size(); q++ )
                    {
                        DS::LEdge<DS::LGraph<int,double>>* _edge = _nextToExplore->edges[q];
                        DS::LNode<DS::LGraph<int,double>>* _successor = _edge->nodes[1];

                    #ifdef USE_BATCH_RENDER
                        gl::LPrimitivesRenderer2D::instance->updateSwarmLineColor( _wData->graphRef->edges_glIndx,
                                                                                   _edge->glIndx, 
                                                                                   0.0f, 0.0f, 1.0f );
                    #else
                        gl::LPrimitivesRenderer2D::instance->updateLineColor( _edge->glIndx, 0.0f, 0.0f, 1.0f );
                    #endif

                        float _g = _nextToExplore->gg[_wData->id] + _edge->data;

                        if ( _successor->gg[_wData->id] > _g )
                        {
                            _successor->gg[_wData->id] = _g;

                            _successor->parentInfo[_wData->id].first = _nextToExplore;
                            _successor->parentInfo[_wData->id].second = _edge;

                            if ( _successor == _wData->end )
                            {
                               found = true;
                               _pathNode = _successor;
                            }
                        }

                        _opCount++;
                    }

                    if ( found )
                    {
                        break;
                    }
                }

                if ( _pathNode != NULL )
                {
                    cout << "found path" << endl;
                    _wData->pathNode = _pathNode;
                    cout << "opCount: " << _opCount << endl;

                   	_wData->working = 0;

                    return ( void* ) _pathNode;
                }
                
				cout << "not found" << endl;
				_wData->working = 0;

                return NULL;

			}


		};



	}






}
