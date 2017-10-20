
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

                unordered_map<int,DS::LNode<DS::LGraph<int,double>>* > _explored;

                LNodePriorityQueue _toExplore;
                _toExplore.push( _wData->start );

                // Calculate the first heuristic value
                double _dx = _wData->start->x - _wData->end->x;
                double _dy = _wData->start->y - _wData->end->y;
                double _h = sqrt( _dx * _dx + _dy * _dy );
                _wData->start->g = 0;
                _wData->start->h = _h;
                _wData->start->f = _h;

                _wData->start->parentInfo[_wData->id].first = NULL;
                _wData->start->parentInfo[_wData->id].second = NULL;

                bool found = false;
                DS::LNode<DS::LGraph<int, double>>* _pathNode = NULL;

                int _opCount = 0;

                while ( !_toExplore.empty() )
                {

                    DS::LNode<DS::LGraph<int,double>>* _nextToExplore = _toExplore.top();
                    _toExplore.pop();

                    // Expand this node
                    _explored[_nextToExplore->id] = _nextToExplore;

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
                            found = true;
                            _pathNode = _successor;
                            break;
                        }

                        float dx = _successor->x - _wData->end->x;
                        float dy = _successor->y - _wData->end->y;
                        float dist = sqrt( dx * dx + dy * dy );
                        float _g = _nextToExplore->g + _edge->data;

                        if ( _explored.find( _successor->id ) == _explored.end() ||
                             ( _g < _successor->g && _successor->inOpen == false ) )
                        {
                            float _h = dist;
                            float _f = _g + _h;
                            _successor->f = _f;

                            _successor->inOpen = true;
                            _toExplore.push( _successor );

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
