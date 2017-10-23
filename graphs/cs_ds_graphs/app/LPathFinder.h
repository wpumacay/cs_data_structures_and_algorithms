
#pragma once

#include "LPathFinderInterface.h"

using namespace std;
using namespace engine;

namespace app
{


	namespace graph
	{


		class LPathFinder : public LPathFinderInterface
		{

			public :

			LPathFinder( DS::LGraph<int, double>* pGraph, int pId )
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
				pthread_create( &m_threadHandle, NULL, LPathFinder::calculatePath, ( void* ) &m_wData );
			}

			static void* calculatePath( void* pWorkData )
			{
				LPathFinderWorkData* _wData = ( LPathFinderWorkData* ) pWorkData;

                //unordered_map<int,DS::LNode<DS::LGraph<int,double>>* > _explored;
                unordered_map<int,float> _costSoFar;

                LPairPriorityQueue _toExplore;

                // Calculate the first heuristic value
                float _dx = _wData->start->x - _wData->end->x;
                float _dy = _wData->start->y - _wData->end->y;
                float _h = sqrt( _dx * _dx + _dy * _dy );
                _wData->start->gg[_wData->id] = 0;
                _wData->start->hh[_wData->id] = _h;
                _wData->start->ff[_wData->id] = _h;
                _costSoFar[_wData->start->id] = _wData->start->gg[_wData->id];

                _toExplore.push( LPair( _wData->start, _wData->start->ff[_wData->id] ) );

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

                        if ( _successor == _wData->end )
                        {
                            _successor->parentInfo[_wData->id].first = _nextToExplore;
                            _successor->parentInfo[_wData->id].second = _edge;
                            found = true;
                            _pathNode = _successor;
                            break;
                        }

                        float _g = _nextToExplore->gg[_wData->id] + _edge->data;

                        if ( _costSoFar.find( _successor->id ) == _costSoFar.end() ||
                             _g < _costSoFar[_successor->id] )
                        {
                            float dx = _successor->x - _wData->end->x;
                            float dy = _successor->y - _wData->end->y;
                            float _h = sqrt( dx * dx + dy * dy );
                            float _f = _g + _h;

                            _successor->gg[_wData->id] = _g;
                            _successor->hh[_wData->id] = _h;
                            _successor->ff[_wData->id] = _f;

                            _toExplore.push( LPair( _successor, _f ) );
                            _costSoFar[_successor->id] = _g;

                            _successor->parentInfo[_wData->id].first = _nextToExplore;
                            _successor->parentInfo[_wData->id].second = _edge;
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
