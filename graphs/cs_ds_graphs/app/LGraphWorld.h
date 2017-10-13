
#pragma once

#include "../gl/core/world/LWorld2D.h"
#include "../gl/core/primitives/LPrimitivesRenderer2D.h"

#include "../graph/LGraph.h"
#include "../common.h"

#include "../lib/delaunay.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <map>

using namespace std;
using namespace engine;

namespace app
{

    namespace graph
   {

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

        class LGraphWorld : public engine::gl::LWorld2D
        {

            private :

            DS::LGraph<int, double> m_graph;
            LPathInfo m_pInfo;

            public :

            LGraphWorld( float wWidth, float wHeight,
                         float appWidth, float appHeight,
                         float pix2world ) : engine::gl::LWorld2D( wWidth, wHeight,
                                                                   appWidth, appHeight,
                                                                   pix2world )
            {
                initRandomGraph();

                m_pInfo.start_glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( 1000000.0f, 1000000.0f, 1.0f, 0.0f, 0.0f );
                m_pInfo.end_glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( 1000000.0f, 1000000.0f, 1.0f, 0.0f, 0.0f );
            }

            void initRandomGraph()
            {
                // Init Nodes **********************************************************************

                for ( int q = 0; q < NUM_GRAPH_NODES; q++ )
                {
                    double _x = ( RANDOM() - 0.5 ) * m_width;
                    double _y = ( RANDOM() - 0.5 ) * m_height;
                    DS::LNode<DS::LGraph<int, double> >* _pNode = m_graph.insertNode( q, _x, _y, q );

                    _pNode->glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( _x, _y );
                }
                // *********************************************************************************

                // Init Edges **********************************************************************

                int _numPoints = m_graph.nodes.size();
                del_point2d_t* _points = new del_point2d_t[_numPoints];
                for ( int q = 0; q < _numPoints; q++ )
                {
                    DS::LNode<DS::LGraph<int, double> >* _node = m_graph.nodes[q];
                    _points[q].x = _node->x;
                    _points[q].y = _node->y;
                }

                delaunay2d_t* _res = delaunay2d_from( _points, _numPoints );
                tri_delaunay2d_t* _tres = tri_delaunay2d_from( _res );

                for ( int q = 0; q < _tres->num_triangles; q++ )
                {
                    int _indxs[3] = {0,0,0};
                    for ( int p = 0; p < 3; p++ )
                    {
                        _indxs[p] = _tres->tris[q * 3 + p];
                    }
                    float _dx, _dy, _dist;

                    for ( int p = 0; p < 3; p++ )
                    {
                        _dx = m_graph.nodes[_indxs[p]]->x - m_graph.nodes[_indxs[(p + 1) % 3]]->x;
                        _dy = m_graph.nodes[_indxs[p]]->y - m_graph.nodes[_indxs[(p + 1) % 3]]->y;
                        _dist = sqrt( _dx * _dx + _dy * _dy );
                        
                        vector<DS::LEdge<DS::LGraph<int, double> >*> _pEdges = m_graph.insertEdge( m_graph.nodes[_indxs[p]],
                                                                                                   m_graph.nodes[_indxs[(p + 1) % 3]],
                                                                                                   _dist );
                        if ( _pEdges.size() != 0 )
                        {
                            _pEdges[0]->glIndx = gl::LPrimitivesRenderer2D::instance->addLine( _pEdges[0]->nodes[0]->x, _pEdges[0]->nodes[0]->y, 
                                                                                               _pEdges[0]->nodes[1]->x, _pEdges[0]->nodes[1]->y );
                            _pEdges[1]->glIndx = _pEdges[0]->glIndx;
                        }
                            
                    }
                }

                delete _res;
                delete _tres;
                delete[] _points;

                // *********************************************************************************

            }

            void saveGraph()
            {

            }

            void loadGraph()
            {

            }

            void calculatePath()
            {
                if ( m_pInfo.start == NULL ||
                     m_pInfo.end == NULL )
                {
                    return;
                }

                // Clean up
                if ( m_pInfo.pathNode != NULL )
                {
                    cout << "clean previous path" << endl;

                    // Change the color of the edges of the path
                    DS::LNode<DS::LGraph<int,double>>* _node = m_pInfo.pathNode;
                    DS::LNode<DS::LGraph<int,double>>* _node_parent = _node->parentInfo.first;
                    DS::LEdge<DS::LGraph<int,double>>* _edge_parent = _node->parentInfo.second;
                    while( _node_parent != NULL )
                    {
                        if ( _edge_parent != NULL )
                        {
                            gl::LPrimitivesRenderer2D::instance->updateLineColor( _edge_parent->glIndx, 1.0f, 1.0f, 1.0f );
                        }
                        _node = _node_parent;
                        _node_parent = _node_parent->parentInfo.first;
                        if ( _node_parent != NULL )
                        {
                            _edge_parent = _node_parent->parentInfo.second;
                        }
                    }
                }


                map<int,DS::LNode<DS::LGraph<int,double>>* > _explored;
                map<int,DS::LNode<DS::LGraph<int,double>>* > _toExplore;
                _toExplore[m_pInfo.start->id] = m_pInfo.start;

                // Calculate the first heuristic value
                double _dx = m_pInfo.start->x - m_pInfo.end->x;
                double _dy = m_pInfo.start->y - m_pInfo.end->y;
                double _h = sqrt( _dx * _dx + _dy * _dy );
                m_pInfo.start->g = 0;
                m_pInfo.start->h = _h;
                m_pInfo.start->f = _h;
                //m_pInfo.start->parent = NULL;
                m_pInfo.start->parentInfo.first = NULL;
                m_pInfo.start->parentInfo.second = NULL;

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

                        gl::LPrimitivesRenderer2D::instance->updateLineColor( _edge->glIndx, 0.0f, 0.0f, 1.0f );

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
                        _successor->parentInfo.first = _nextToExplore;
                        _successor->parentInfo.second = _edge;

                        if ( _successor == m_pInfo.end )
                        {
                            found = true;
                            _pathNode = _successor;
                            break;
                        }

                        double dx = _successor->x - m_pInfo.end->x;
                        double dy = _successor->y - m_pInfo.end->y;
                        double dist = sqrt( dx * dx + dy * dy );
                        _successor->g = _nextToExplore->g + _edge->data;
                        _successor->h = dist;
                        _successor->f = _successor->g + _successor->h;

                        _toExplore[_successor->id] = _successor;

                        _opCount++;
                        // cout << "opCount: " << _opCount << endl;
                        // cout << "_toExplore: " << _toExplore.size() << endl;
                    }

                    if ( found )
                    {
                        break;
                    }
                }

                if ( _pathNode != NULL )
                {
                    cout << "found path" << endl;
                    m_pInfo.pathNode = _pathNode;

                    // Change the color of the edges of the path
                    DS::LNode<DS::LGraph<int,double>>* _node = m_pInfo.pathNode;
                    DS::LNode<DS::LGraph<int,double>>* _node_parent = _node->parentInfo.first;
                    DS::LEdge<DS::LGraph<int,double>>* _edge_parent = _node->parentInfo.second;
                    while( _node_parent != NULL )
                    {
                        if ( _edge_parent != NULL )
                        {
                            gl::LPrimitivesRenderer2D::instance->updateLineColor( _edge_parent->glIndx, 0.0f, 1.0f, 0.0f );
                        }
                        _node = _node_parent;
                        _node_parent = _node_parent->parentInfo.first;
                        if ( _node_parent != NULL )
                        {
                            _edge_parent = _node_parent->parentInfo.second;
                        }
                    }

                    cout << "opCount: " << _opCount << endl;
                }
                else
                {
                    cout << "not found" << endl;
                }


            }

            void update( float dt )
            {
                m_camera->update( dt );
            }


            void onKeyDown( int pKey ) override
            {
                if ( pKey == GLFW_KEY_W )
                {
                    m_camera->setVy( -CAM_SPEED_Y );
                }
                else if ( pKey == GLFW_KEY_S )
                {
                    m_camera->setVy( CAM_SPEED_Y );
                }
                else if ( pKey == GLFW_KEY_D )
                {
                    m_camera->setVx( -CAM_SPEED_X );
                }
                else if ( pKey == GLFW_KEY_A )
                {
                    m_camera->setVx( CAM_SPEED_X );
                }
            }

            void onKeyUp( int pKey ) override
            {
                if ( pKey == GLFW_KEY_W )
                {
                    m_camera->setVy( 0.0f );
                }
                else if ( pKey == GLFW_KEY_S )
                {
                    m_camera->setVy( 0.0f );
                }
                else if ( pKey == GLFW_KEY_D )
                {
                    m_camera->setVx( 0.0f );
                }
                else if ( pKey == GLFW_KEY_A )
                {
                    m_camera->setVx( 0.0f );
                }
            }

            void onMouseDown( float wx, float wy )
            {
                if ( m_pInfo.end != NULL &&
                     m_pInfo.start != NULL )
                {
                    m_pInfo.start = NULL;
                    m_pInfo.end = NULL;
                }
                for ( int q = 0; q < m_graph.nodes.size(); q++ )
                {
                    DS::LNode<DS::LGraph<int, double>>* _node = m_graph.nodes[q];

                    float dx = wx - _node->x;
                    float dy = wy - _node->y;
                    float dist = sqrt( dx * dx + dy * dy );
                    if ( dist < RANGE_PICK )
                    {
                        if ( m_pInfo.start != NULL )
                        {
                            if ( m_pInfo.start == _node )
                            {
                                continue;
                            }
                            cout << "picked end" << endl;
                            m_pInfo.end = _node;
                            gl::LPrimitivesRenderer2D::instance->updatePoint( m_pInfo.end_glIndx,
                                                                              m_pInfo.end->x,
                                                                              m_pInfo.end->y );
                            calculatePath();
                        }
                        else
                        {
                            cout << "picked start" << endl;
                            // Clean everything
                            for ( int p = 0; p < m_graph.nodes.size(); p++ )
                            {
                                DS::LNode<DS::LGraph<int, double>>* _node = m_graph.nodes[p];
                                _node->f = 0;
                                _node->g = 0;
                                _node->h = 0;
                                for ( int e = 0; e < _node->edges.size(); e++ )
                                {
                                    gl::LPrimitivesRenderer2D::instance->updateLineColor( _node->edges[e]->glIndx, 1.0f, 1.0f, 1.0f );
                                }
                            }

                            m_pInfo.start = _node;
                            gl::LPrimitivesRenderer2D::instance->updatePoint( m_pInfo.start_glIndx,
                                                                              m_pInfo.start->x,
                                                                              m_pInfo.start->y );
                        }

                        break;
                    }
                }
            }

        };

    }

}