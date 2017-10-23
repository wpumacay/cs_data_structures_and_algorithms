
#pragma once

#include "../gl/core/world/LWorld2D.h"
#include "../gl/core/primitives/LPrimitivesRenderer2D.h"

#include "../graph/LGraph.h"
#include "../common.h"

#include "../lib/delaunay.h"

#include "LPathFinder.h"
#include "LPathFinderDijkstra.h"
#include "LLandmarkPathFinder.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <map>

using namespace std;
using namespace engine;

namespace app
{

    namespace graph
   {

        class LGraphWorldGeneral : public engine::gl::LWorld2D
        {

            private :

            DS::LGraph<int, double> m_graph;

            finderType::_finderType m_pfType;

            LPathFinderInterface* m_currentPathFinders[MAX_PARALLEL_REQUESTS];
            bool m_useParallelRequests;

            // PathFinders
            // Dijkstra
            LPathFinderDijkstra* m_pf_dijkstra[MAX_PARALLEL_REQUESTS];
            // A* normal
            LPathFinder* m_pf_astar[MAX_PARALLEL_REQUESTS];
            // A* with landmarks
            LLandmarkPathFinder* m_pf_astar_landmarks[MAX_PARALLEL_REQUESTS];

            // parallel requests stuff
            vector<int> m_startIndxs;
            vector<int> m_endIndxs;

            vector<string> split( const string &txt )
            {
                vector<string> _res;
                
                int pos = txt.find( ' ' );
                if ( pos == std::string::npos )
                {
                    _res.push_back( txt );
                    return _res;
                }

                int initpos = 0;

                while ( pos != std::string::npos )
                {
                    _res.push_back( txt.substr( initpos, pos - initpos + 1 ) );
                    initpos = pos + 1;

                    pos = txt.find( ',', initpos );
                }

                _res.push_back( txt.substr( initpos, std::min( pos, (int) txt.size() ) - initpos + 1 ) );
                
                return _res;
            }

            public :

            LGraphWorldGeneral( int graphSize,
                                float wWidth, float wHeight,
                                float appWidth, float appHeight,
                                float pix2world ) : engine::gl::LWorld2D( wWidth, wHeight,
                                                                   appWidth, appHeight,
                                                                   pix2world )
            {

                loadGraph( graphSize );

                m_pfType = finderType::SEARCH_A_STAR;

                // initialize the pathfinders
                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    m_pf_dijkstra[q] = new LPathFinderDijkstra( &m_graph, q );
                    m_pf_astar[q] = new LPathFinder( &m_graph, q );
                    m_pf_astar_landmarks[q] = new LLandmarkPathFinder( &m_graph, q );
                    m_pf_astar_landmarks[q]->loadLandmarks( graphSize );
                    m_pf_astar_landmarks[q]->loadPreCalc( graphSize );
                }

                m_useParallelRequests = false;
                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    m_currentPathFinders[q] = m_pf_astar[q];
                }

                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    int _start_glIndx   = gl::LPrimitivesRenderer2D::instance->addPoint( 1000000.0f, 1000000.0f, 1.0f, 0.0f, 0.0f );
                    int _end_glIndx     = gl::LPrimitivesRenderer2D::instance->addPoint( 1000000.0f, 1000000.0f, 1.0f, 0.0f, 0.0f );

                    m_pf_dijkstra[q]->start_glIndx          = _start_glIndx;
                    m_pf_astar[q]->start_glIndx             = _start_glIndx;
                    m_pf_astar_landmarks[q]->start_glIndx   = _start_glIndx;

                    m_pf_dijkstra[q]->end_glIndx          = _end_glIndx;
                    m_pf_astar[q]->end_glIndx             = _end_glIndx;
                    m_pf_astar_landmarks[q]->end_glIndx   = _end_glIndx;
                }
            }

            void clean()
            {
                gl::LPrimitivesRenderer2D::instance->clean();

                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    if ( m_pf_dijkstra[q] != NULL )
                    {
                        delete m_pf_dijkstra[q];
                        m_pf_dijkstra[q] = NULL;
                    }
                    if ( m_pf_astar[q] != NULL )
                    {
                        delete m_pf_astar[q];
                        m_pf_astar[q] = NULL;
                    }
                    if ( m_pf_astar_landmarks[q] != NULL )
                    {
                        delete m_pf_astar_landmarks[q];
                        m_pf_astar_landmarks[q] = NULL;
                    }
                }
            }

            void setParallelMode( bool enable )
            {
                m_useParallelRequests = enable;
            }

            void changePathFinder( finderType::_finderType pFinderType )
            {
                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    if ( m_currentPathFinders[q]->isRunning )
                    {
                        cout << "cant change pfinders until they finish working" << endl;
                        return;
                    }
                }

                m_pfType = pFinderType;
                if ( m_pfType == finderType::SEARCH_DIJKSTRA )
                {
                    for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                    {
                        m_currentPathFinders[q] = m_pf_dijkstra[q];
                    }
                }
                else if ( m_pfType == finderType::SEARCH_A_STAR )
                {
                    for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                    {
                        m_currentPathFinders[q] = m_pf_astar[q];
                    }
                }
                else if ( m_pfType == finderType::SEARCH_A_STAR_LANDMARKS )
                {
                    for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                    {
                        m_currentPathFinders[q] = m_pf_astar_landmarks[q];
                    }
                }
                else
                {
                    cout << "error: wrong option for search method" << endl;
                }
            }

            void loadGraph( int graphSize )
            {
                cout << "loading graph" << endl;

                string _pathGraph;
                _pathGraph += "res/";
                _pathGraph += std::to_string( graphSize );
                _pathGraph += "points.data";

                ifstream _fileHandle ( _pathGraph.c_str() );
                if ( _fileHandle.is_open() )
                {
                    string _line;

                    getline( _fileHandle, _line );

                    vector<string> _params = split( _line );
                    int N = stoi( _params[1] );

                    float* _px = new float[N];
                    float* _py = new float[N];

                    vector<float> _p1x;
                    vector<float> _p1y;
                    vector<float> _p2x;
                    vector<float> _p2y;

                    int _nlines = 0;

                    for ( int q = 0; q < N; q++ )
                    {
                        getline( _fileHandle, _line );

                        vector<string> _nodeStr = split( _line );
                        int _id = q;
                        float _x = stof( _nodeStr[0] );
                        float _y = stof( _nodeStr[1] );

                        DS::LNode<DS::LGraph<int, double> >* _pNode = m_graph.insertNode( _id, _x, _y, _id );

                        _px[q] = _x;
                        _py[q] = _y;
                    }

                    getline( _fileHandle, _line );// dummy line
                    _params = split( _line );

                    for( string _edgeLine; getline( _fileHandle, _edgeLine ); )
                    {
                        vector<string> _edgeStr = split( _edgeLine );
                        int _id_from = stoi( _edgeStr[0] );
                        int _id_to   = stoi( _edgeStr[1] );

                        DS::LNode<DS::LGraph<int, double> >* _pNodeFrom = m_graph.nodes[_id_from];
                        DS::LNode<DS::LGraph<int, double> >* _pNodeTo   = m_graph.nodes[_id_to];

                        float _dx   = _pNodeFrom->x - _pNodeTo->x;
                        float _dy   = _pNodeFrom->y - _pNodeTo->y;
                        float _dist = sqrt( _dx * _dx + _dy * _dy );

                        vector<DS::LEdge<DS::LGraph<int, double> >*> _pEdges = m_graph.insertEdge( _pNodeFrom,
                                                                                                   _pNodeTo,
                                                                                                   _dist );
                        if ( _pEdges.size() != 0 )
                        {
                            _nlines++;
                            _p1x.push_back( _pNodeFrom->x );
                            _p1y.push_back( _pNodeFrom->y );
                            _p2x.push_back( _pNodeTo->x );
                            _p2y.push_back( _pNodeTo->y );

                            _pEdges[0]->glIndx = _nlines - 1;
                            _pEdges[1]->glIndx = _nlines - 1;
                        }
                    }

                    _fileHandle.close();

                    m_graph.nodes_glIndx = gl::LPrimitivesRenderer2D::instance->addPointSwarm( _px, _py, N );
                    m_graph.edges_glIndx = gl::LPrimitivesRenderer2D::instance->addLineSwarm( _p1x.data(), _p1y.data(), _p2x.data(), _p2y.data(), _nlines );
                }

                cout << "done" << endl;
            }

            void requestPathFinders()
            {
                cout << "launching parallel requests" << endl;
                for ( int q = 0; q < m_startIndxs.size(); q++ )
                {
                    m_currentPathFinders[q]->run( m_graph.nodes[m_startIndxs[q]], 
                                                  m_graph.nodes[m_endIndxs[q]] );
                }
            }

            void calculatePath()
            {
                cout << "launching single request" << endl;
                if ( m_startIndxs.size() < 1  ||
                     m_endIndxs.size() < 1 )
                {
                    return;
                }

                m_currentPathFinders[0]->run( m_graph.nodes[m_startIndxs[0]], 
                                              m_graph.nodes[m_endIndxs[0]] );
            }

            void update( float dt )
            {
                m_camera->update( dt );

                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    m_currentPathFinders[q]->update( dt );
                }
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
                if ( m_startIndxs.size() > 0 &&
                     m_endIndxs.size() > 0 )
                {
                    m_startIndxs.clear();
                    m_endIndxs.clear();
                }

                for ( int q = 0; q < m_graph.nodes.size(); q++ )
                {
                    DS::LNode<DS::LGraph<int, double>>* _node = m_graph.nodes[q];

                    float dx = wx - _node->x;
                    float dy = wy - _node->y;
                    float dist = sqrt( dx * dx + dy * dy );
                    if ( dist < RANGE_PICK )
                    {
                        if ( m_startIndxs.size() > 0 )
                        {
                            if ( m_startIndxs[0] == _node->id )
                            {
                                continue;
                            }
                            cout << "picked end " << _node->id << endl;

                            m_endIndxs.clear();
                            m_endIndxs.push_back( _node->id );

                            if ( m_useParallelRequests )
                            {
                                // Check for max MAX_PARALLEL_REQUESTS nodes in the range of the node picked
                                for ( int q = 0; q < m_startIndxs.size() - 1; q++ )
                                {
                                    for ( int n = 0; n < m_graph.nodes.size(); n++ )
                                    {
                                        bool _already = false;
                                        for ( int s = 0; s < m_endIndxs.size(); s++ )
                                        {
                                            if ( m_endIndxs[s] == m_graph.nodes[n]->id )
                                            {
                                                _already = true;
                                                break;
                                            }
                                        }

                                        if ( _already )
                                        {
                                            continue;
                                        }

                                        float _ddx = _node->x - m_graph.nodes[n]->x;
                                        float _ddy = _node->y - m_graph.nodes[n]->y;
                                        float _ddist = sqrt( _ddx * _ddx + _ddy * _ddy );

                                        if ( _ddist < MAX_RANGE_SEARCH )
                                        {
                                            m_endIndxs.push_back( m_graph.nodes[n]->id );
                                            break;
                                        }
                                    }
                                }

                                if ( m_endIndxs.size() != m_startIndxs.size() )
                                {
                                    cout << "warning: start and end requests doesnt not match" << endl;
                                    return;
                                }

                                for ( int q = 0; q < m_endIndxs.size(); q++ )
                                {
                                    gl::LPrimitivesRenderer2D::instance->updatePoint( m_currentPathFinders[q]->end_glIndx,
                                                                                      m_graph.nodes[m_endIndxs[q]]->x,
                                                                                      m_graph.nodes[m_endIndxs[q]]->y );
                                }

                                requestPathFinders();
                            }
                            else
                            {
                                gl::LPrimitivesRenderer2D::instance->updatePoint( m_currentPathFinders[0]->end_glIndx,
                                                                                  m_graph.nodes[m_endIndxs[0]]->x,
                                                                                  m_graph.nodes[m_endIndxs[0]]->y );
                                calculatePath();
                            }
                                    
                        }
                        else
                        {
                            cout << "picked start " << _node->id << endl;
                            // Clean everything
                            for ( int p = 0; p < m_graph.nodes.size(); p++ )
                            {
                                DS::LNode<DS::LGraph<int, double>>* _node = m_graph.nodes[p];
                                _node->f = 0;
                                _node->g = 0;
                                _node->h = 0;
                            
                                for ( int e = 0; e < _node->edges.size(); e++ )
                                {
                                    gl::LPrimitivesRenderer2D::instance->updateSwarmLineColor( m_graph.edges_glIndx,
                                                                                               _node->edges[e]->glIndx, 
                                                                                               1.0f, 1.0f, 1.0f );
                                }
                            }

                            m_startIndxs.clear();
                            m_startIndxs.push_back( _node->id );

                            if ( m_useParallelRequests )
                            {

                                for ( int q = 0; q < MAX_PARALLEL_REQUESTS - 1; q++ )
                                {
                                    for ( int n = 0; n < m_graph.nodes.size(); n++ )
                                    {
                                        bool _already = false;
                                        for ( int s = 0; s < m_startIndxs.size(); s++ )
                                        {
                                            if ( m_startIndxs[s] == m_graph.nodes[n]->id )
                                            {
                                                _already = true;
                                                break;
                                            }
                                        }

                                        if ( _already )
                                        {
                                            continue;
                                        }

                                        float _ddx = _node->x - m_graph.nodes[n]->x;
                                        float _ddy = _node->y - m_graph.nodes[n]->y;
                                        float _ddist = sqrt( _ddx * _ddx + _ddy * _ddy );

                                        if ( _ddist < MAX_RANGE_SEARCH )
                                        {
                                            m_startIndxs.push_back( m_graph.nodes[n]->id );
                                            break;
                                        }
                                    }
                                }

                                if ( m_startIndxs.size() < MAX_PARALLEL_REQUESTS )
                                {
                                    cout << "warning: couldnt find the requested number of start nodes" << endl;
                                }

                                for ( int q = 0; q < m_startIndxs.size(); q++ )
                                {
                                    gl::LPrimitivesRenderer2D::instance->updatePoint( m_currentPathFinders[q]->start_glIndx,
                                                                                      m_graph.nodes[m_startIndxs[q]]->x,
                                                                                      m_graph.nodes[m_startIndxs[q]]->y );
                                }                                
                            }
                            else
                            {
                                gl::LPrimitivesRenderer2D::instance->updatePoint( m_currentPathFinders[0]->start_glIndx,
                                                                                  m_graph.nodes[m_startIndxs[0]]->x,
                                                                                  m_graph.nodes[m_startIndxs[0]]->y );
                            }

                        }

                        break;
                    }
                }
            }

        };

    }

}