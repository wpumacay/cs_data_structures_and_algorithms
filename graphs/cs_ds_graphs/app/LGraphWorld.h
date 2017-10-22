
#pragma once

#include "../gl/core/world/LWorld2D.h"
#include "../gl/core/primitives/LPrimitivesRenderer2D.h"

#include "../graph/LGraph.h"
#include "../common.h"

#include "../lib/delaunay.h"

#include "LPathFinder.h"
#ifdef USE_PREPROCESSING
    #include "LLandmarkPathFinder.h"
#endif

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

        class LGraphWorld : public engine::gl::LWorld2D
        {

            private :

            DS::LGraph<int, double> m_graph;
            LPathInfo m_pInfo;

        #ifdef USE_PARALLEL_REQUESTS
            LPathFinderInterface* m_pathFinders[MAX_PARALLEL_REQUESTS];
            vector<int> m_startIndxs;
            vector<int> m_endIndxs;
        #elif defined( USE_PREPROCESSING )
            LPathFinderInterface* m_pathFinders[1];
        #else
            LPathFinderInterface* m_pathFinders[1];
        #endif

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

            LGraphWorld( float wWidth, float wHeight,
                         float appWidth, float appHeight,
                         float pix2world ) : engine::gl::LWorld2D( wWidth, wHeight,
                                                                   appWidth, appHeight,
                                                                   pix2world )
            {
            #ifdef USE_SAVED_GRAPH
                loadGraph();
            #else
                initRandomGraph();
            #endif

            #ifdef USE_PARALLEL_REQUESTS

                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    m_pathFinders[q] = new LPathFinder( &m_graph, q );
                }

            #elif defined( USE_PREPROCESSING )
                cout << "using preprocessing" << endl;
                m_pathFinders[0] = new LLandmarkPathFinder( &m_graph, 0 );
                #ifdef TEST_PRECALC
                    cout << "precalculating ..." << endl;

                    reinterpret_cast< LLandmarkPathFinder* >( m_pathFinders[0] )->loadLandmarks();
                    reinterpret_cast< LLandmarkPathFinder* >( m_pathFinders[0] )->preCalc();
                    reinterpret_cast< LLandmarkPathFinder* >( m_pathFinders[0] )->savePreCalc();

                    cout << "done" << endl;
                #else
                    cout << "starting" << endl;

                    reinterpret_cast< LLandmarkPathFinder* >( m_pathFinders[0] )->loadLandmarks();
                    reinterpret_cast< LLandmarkPathFinder* >( m_pathFinders[0] )->loadPreCalc();

                    cout << "done" << endl;
                #endif
            #else

                m_pathFinders[0] = new LPathFinder( &m_graph, 0 );

            #endif

            #ifdef USE_PARALLEL_REQUESTS

                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    m_pathFinders[q]->start_glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( 1000000.0f, 1000000.0f, 1.0f, 0.0f, 0.0f );
                    m_pathFinders[q]->end_glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( 1000000.0f, 1000000.0f, 1.0f, 0.0f, 0.0f );                    

                    // cout << "startglIndx: " << m_pathFinders[q]->start_glIndx << endl;
                }

            #else

                m_pInfo.start_glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( 1000000.0f, 1000000.0f, 1.0f, 0.0f, 0.0f );
                m_pInfo.end_glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( 1000000.0f, 1000000.0f, 1.0f, 0.0f, 0.0f );

                m_pathFinders[0]->start_glIndx = m_pInfo.start_glIndx;
                m_pathFinders[0]->end_glIndx = m_pInfo.end_glIndx;

            #endif
            }

            void initRandomGraph()
            {
                // Init Nodes **********************************************************************

                #ifdef USE_BATCH_RENDER

                float* _px = new float[NUM_GRAPH_NODES];
                float* _py = new float[NUM_GRAPH_NODES];

                vector<float> _p1x;
                vector<float> _p1y;
                vector<float> _p2x;
                vector<float> _p2y;

                int _nlines = 0;

                #endif

                for ( int q = 0; q < NUM_GRAPH_NODES; q++ )
                {
                    double _x = ( RANDOM() - 0.5 ) * m_width;
                    double _y = ( RANDOM() - 0.5 ) * m_height;
                    DS::LNode<DS::LGraph<int, double> >* _pNode = m_graph.insertNode( q, _x, _y, q );

                #ifdef USE_BATCH_RENDER
                    _px[q] = _x;
                    _py[q] = _y;
                #else
                    _pNode->glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( _x, _y );
                #endif
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
                        #ifdef USE_BATCH_RENDER

                            _nlines++;
                            _p1x.push_back( _pEdges[0]->nodes[0]->x );
                            _p1y.push_back( _pEdges[0]->nodes[0]->y );
                            _p2x.push_back( _pEdges[0]->nodes[1]->x );
                            _p2y.push_back( _pEdges[0]->nodes[1]->y );

                            _pEdges[0]->glIndx = _nlines - 1;
                            _pEdges[1]->glIndx = _nlines - 1;

                        #else
                            _pEdges[0]->glIndx = gl::LPrimitivesRenderer2D::instance->addLine( _pEdges[0]->nodes[0]->x, _pEdges[0]->nodes[0]->y, 
                                                                                               _pEdges[0]->nodes[1]->x, _pEdges[0]->nodes[1]->y );
                            _pEdges[1]->glIndx = _pEdges[0]->glIndx;
                        #endif
                        }
                            
                    }
                }

                delete _res;
                delete _tres;
                delete[] _points;

            #ifdef USE_BATCH_RENDER

                m_graph.nodes_glIndx = gl::LPrimitivesRenderer2D::instance->addPointSwarm( _px, _py, NUM_GRAPH_NODES );
                m_graph.edges_glIndx = gl::LPrimitivesRenderer2D::instance->addLineSwarm( _p1x.data(), _p1y.data(), _p2x.data(), _p2y.data(), _nlines );

            #endif

                // *********************************************************************************
            }

            void saveGraph()
            {
                cout << "saving graph" << endl;

                ofstream _fileHandle ( "graph_test.txt" );
                if ( _fileHandle.is_open() )
                {
                    _fileHandle << "POINTS " << m_graph.nodes.size() << endl;

                    for ( int q = 0; q < m_graph.nodes.size(); q++ )
                    {
                        DS::LNode<DS::LGraph<int,double>>* _node = m_graph.nodes[q];
                        double _x = _node->x;
                        double _y = _node->y;
                        int _id = _node->id;

                        _fileHandle << _x << " " << _y << endl;
                    }

                    _fileHandle << "EDGES " << "----" << endl;

                    for ( int q = 0; q < m_graph.nodes.size(); q++ )
                    {
                        DS::LNode<DS::LGraph<int,double>>* _node = m_graph.nodes[q];
                        for ( int p = 0; p < _node->edges.size(); p++ )
                        {
                            DS::LEdge<DS::LGraph<int,double>>* _edge = _node->edges[p];
                            _fileHandle << _edge->nodes[0]->id << " " << _edge->nodes[1]->id << endl;
                        }
                    }

                    _fileHandle.close();
                }

                cout << "done" << endl;
            }

            void loadGraph()
            {
                cout << "loading graph" << endl;

                ifstream _fileHandle ( USE_SAVED_GRAPH_PATH );
                if ( _fileHandle.is_open() )
                {
                    string _line;

                    getline( _fileHandle, _line );

                    vector<string> _params = split( _line );
                    int N = stoi( _params[1] );

                #ifdef USE_BATCH_RENDER

                    float* _px = new float[N];
                    float* _py = new float[N];

                    vector<float> _p1x;
                    vector<float> _p1y;
                    vector<float> _p2x;
                    vector<float> _p2y;

                    int _nlines = 0;

                #endif

                    for ( int q = 0; q < N; q++ )
                    {
                        getline( _fileHandle, _line );

                        vector<string> _nodeStr = split( _line );
                        int _id = q;
                        float _x = stof( _nodeStr[0] );
                        float _y = stof( _nodeStr[1] );

                        DS::LNode<DS::LGraph<int, double> >* _pNode = m_graph.insertNode( _id, _x, _y, _id );

                    #ifdef USE_BATCH_RENDER
                        _px[q] = _x;
                        _py[q] = _y;
                    #else
                        _pNode->glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( _x, _y );
                    #endif
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
                        #ifdef USE_BATCH_RENDER
                            _nlines++;
                            _p1x.push_back( _pNodeFrom->x );
                            _p1y.push_back( _pNodeFrom->y );
                            _p2x.push_back( _pNodeTo->x );
                            _p2y.push_back( _pNodeTo->y );

                            _pEdges[0]->glIndx = _nlines - 1;
                            _pEdges[1]->glIndx = _nlines - 1;
                        #else
                            _pEdges[0]->glIndx = gl::LPrimitivesRenderer2D::instance->addLine( _pNodeFrom->x, _pNodeFrom->y, 
                                                                                               _pNodeTo->x, _pNodeTo->y );
                            _pEdges[1]->glIndx = _pEdges[0]->glIndx;
                        #endif
                        }
                    }

                    _fileHandle.close();

                #ifdef USE_BATCH_RENDER

                    m_graph.nodes_glIndx = gl::LPrimitivesRenderer2D::instance->addPointSwarm( _px, _py, N );
                    m_graph.edges_glIndx = gl::LPrimitivesRenderer2D::instance->addLineSwarm( _p1x.data(), _p1y.data(), _p2x.data(), _p2y.data(), _nlines );

                #endif
                }

                cout << "done" << endl;
            }


        #ifdef USE_PARALLEL_REQUESTS

            void requestPathFinders()
            {
                for ( int q = 0; q < m_startIndxs.size(); q++ )
                {
                    m_pathFinders[q]->run( m_graph.nodes[m_startIndxs[q]], 
                                           m_graph.nodes[m_endIndxs[q]] );
                }
            }

        #else

            void calculatePath()
            {
                if ( m_pInfo.start == NULL ||
                     m_pInfo.end == NULL )
                {
                    return;
                }

                m_pathFinders[0]->run( m_pInfo.start, m_pInfo.end );
            }

        #endif

            void update( float dt )
            {
                m_camera->update( dt );

            #ifdef USE_PARALLEL_REQUESTS
                for ( int q = 0; q < MAX_PARALLEL_REQUESTS; q++ )
                {
                    m_pathFinders[q]->update( dt );
                }
            #else
                m_pathFinders[0]->update( dt );
            #endif
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

                if ( pKey == GLFW_KEY_SPACE )
                {
                    saveGraph();
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
                    #ifdef TEST_PICK_LANDMARKS
                            
                            cout << "id picked: " << _node->id << endl;
                            gl::LPrimitivesRenderer2D::instance->updatePoint( m_pathFinders[0]->start_glIndx,
                                                                              _node->x,
                                                                              _node->y );
                            return;
                    #endif
                        if ( m_pInfo.start != NULL )
                        {
                            if ( m_pInfo.start == _node )
                            {
                                continue;
                            }
                            cout << "picked end " << _node->id << endl;
                            m_pInfo.end = _node;

                        #ifdef USE_PARALLEL_REQUESTS

                            // Check for max MAX_PARALLEL_REQUESTS nodes in the range of the node picked
                            m_endIndxs.clear();
                            m_endIndxs.push_back( _node->id );

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
                                gl::LPrimitivesRenderer2D::instance->updatePoint( m_pathFinders[q]->end_glIndx,
                                                                                  m_graph.nodes[m_endIndxs[q]]->x,
                                                                                  m_graph.nodes[m_endIndxs[q]]->y );
                            }

                            requestPathFinders();
                        #else
                            gl::LPrimitivesRenderer2D::instance->updatePoint( m_pInfo.end_glIndx,
                                                                              m_pInfo.end->x,
                                                                              m_pInfo.end->y );
                            calculatePath();
                        #endif

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
                            #ifdef USE_BATCH_RENDER
                                for ( int e = 0; e < _node->edges.size(); e++ )
                                {
                                    gl::LPrimitivesRenderer2D::instance->updateSwarmLineColor( m_graph.edges_glIndx,
                                                                                               _node->edges[e]->glIndx, 
                                                                                               1.0f, 1.0f, 1.0f );
                                }
                            #else
                                for ( int e = 0; e < _node->edges.size(); e++ )
                                {
                                    gl::LPrimitivesRenderer2D::instance->updateLineColor( _node->edges[e]->glIndx, 1.0f, 1.0f, 1.0f );
                                }
                            #endif
                            }

                            m_pInfo.start = _node;

                        #ifdef USE_PARALLEL_REQUESTS
                            // Check for max MAX_PARALLEL_REQUESTS nodes in the range of the node picked
                            m_startIndxs.clear();
                            m_startIndxs.push_back( _node->id );

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
                                cout << "indx: " << m_startIndxs[q] << endl;
                                gl::LPrimitivesRenderer2D::instance->updatePoint( m_pathFinders[q]->start_glIndx,
                                                                                  m_graph.nodes[m_startIndxs[q]]->x,
                                                                                  m_graph.nodes[m_startIndxs[q]]->y );
                            }

                        #else
                            gl::LPrimitivesRenderer2D::instance->updatePoint( m_pInfo.start_glIndx,
                                                                              m_pInfo.start->x,
                                                                              m_pInfo.start->y );
                        #endif
                        }

                        break;
                    }
                }
            }

        };

    }

}