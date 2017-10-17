
#pragma once

#include "LCommonPathFinding.h"

using namespace std;
using namespace engine;

namespace app
{


    namespace graph
    {


        class LPathFinder
        {

            private :

            DS::LGraph<int, double>* m_graphRef;
            pthread_t m_threadHandle;

            DS::LNode<DS::LGraph<int, double> >* m_start;
            DS::LNode<DS::LGraph<int, double> >* m_end;
            DS::LNode<DS::LGraph<int, double> >* m_pathNode;

            LPathFinderWorkData m_wData;

            public :

            int start_glIndx;
            int end_glIndx;
            int id;
            bool isRunning;

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

            virtual void launch( DS::LNode<DS::LGraph<int, double> >* pStart,
                                 DS::LNode<DS::LGraph<int, double> >* pEnd )
            {
                m_start = pStart;
                m_end = pEnd;
            }

            void join()
            {
                pthread_join( m_threadHandle, ( void** )&m_pathNode );
            }

            void reconstructPath()
            {
                if ( m_pathNode != NULL )
                {
                    // Change the color of the edges of the path
                    DS::LNode<DS::LGraph<int,double>>* _node = m_pathNode;
                    DS::LNode<DS::LGraph<int,double>>* _node_parent = _node->parentInfo[this->id].first;
                    DS::LEdge<DS::LGraph<int,double>>* _edge_parent = _node->parentInfo[this->id].second;
                    while( _node_parent != NULL )
                    {
                        if ( _edge_parent != NULL )
                        {
                        #ifdef USE_BATCH_RENDER
                            gl::LPrimitivesRenderer2D::instance->updateSwarmLineColor( m_graphRef->edges_glIndx,
                                                                                       _edge_parent->glIndx, 
                                                                                       0.0f, 1.0f, 0.0f );
                        #else
                            gl::LPrimitivesRenderer2D::instance->updateLineColor( _edge_parent->glIndx, 0.0f, 1.0f, 0.0f );
                        #endif
                        }
                        _node = _node_parent;
                        _node_parent = _node_parent->parentInfo[this->id].first;
                        if ( _node_parent != NULL )
                        {
                            _edge_parent = _node_parent->parentInfo[this->id].second;
                        }
                    }

                }
            }

            static void* calculatePath( void* pWorkData )
            {
                LPathFinderWorkData* _wData = ( LPathFinderWorkData* ) pWorkData;

                map<int,DS::LNode<DS::LGraph<int,double>>* > _explored;
                map<int,DS::LNode<DS::LGraph<int,double>>* > _toExplore;
                _toExplore[_wData->start->id] = _wData->start;

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
                    DS::LNode<DS::LGraph<int,double>>* _bestCandidate = NULL;

                    map<int,DS::LNode<DS::LGraph<int,double>>* >::iterator _it;

                    for ( _it = _toExplore.begin(); _it != _toExplore.end(); ++_it )
                    {
                        DS::LNode<DS::LGraph<int,double>>* _toExplore_candidate = _it->second;
                        if ( _bestCandidate == NULL )
                        {
                            _bestCandidate = _toExplore_candidate;
                        }
                        else if ( _toExplore_candidate->f < _bestCandidate->f )
                        {
                            _bestCandidate = _toExplore_candidate;
                        }
                    }
                    DS::LNode<DS::LGraph<int,double>>* _nextToExplore = _bestCandidate;
                    _toExplore.erase( _bestCandidate->id );
                    // Expand this node
                    _explored[_nextToExplore->id] = _nextToExplore;

                    for ( int q = 0; q < _nextToExplore->edges.size(); q++ )
                    {
                        DS::LEdge<DS::LGraph<int,double>>* _edge = _nextToExplore->edges[q];
                        DS::LNode<DS::LGraph<int,double>>* _successor = _edge->nodes[1];

                        if ( _explored.find( _successor->id ) != _explored.end() )
                        {
                            // Already explored, don't count it
                            continue;
                        }

                        if ( _toExplore.find( _successor->id ) != _toExplore.end() )
                        {
                            continue;
                        }

                        //_successor->parent = _nextToExplore;
                        _successor->parentInfo[_wData->id].first = _nextToExplore;
                        _successor->parentInfo[_wData->id].second = _edge;

                        if ( _successor == _wData->end )
                        {
                            found = true;
                            _pathNode = _successor;
                            break;
                        }

                        double dx = _successor->x - _wData->end->x;
                        double dy = _successor->y - _wData->end->y;
                        double dist = sqrt( dx * dx + dy * dy );
                        _successor->g = _nextToExplore->g + _edge->data;
                        _successor->h = dist;
                        _successor->f = _successor->g + _successor->h;

                        _toExplore[_successor->id] = _successor;

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

                    return ( void* ) _pathNode;
                }
                
                cout << "not found" << endl;
                return NULL;

            }

            DS::LNode<DS::LGraph<int, double> >* pathNode()
            {
                return m_pathNode;
            }


        };



    }






}
