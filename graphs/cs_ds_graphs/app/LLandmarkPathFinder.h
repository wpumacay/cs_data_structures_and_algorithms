
#pragma once

#include "LPathFinderInterface.h"

#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace engine;

namespace app
{


    namespace graph
    {


        class LLandmarkPathFinder : public LPathFinderInterface
        {

            private :

            int m_landmarkIDs[NUM_LANDMARKS];

            vector<float> m_preCalc[NUM_LANDMARKS];

            public :

            LLandmarkPathFinder( DS::LGraph<int, double>* pGraph, int pId )
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

            void loadLandmarks()
            {
                cout << "loading landmarks" << endl;

                ifstream _fileHandle ( USE_SAVED_GRAPH_LANDMARKS );
                if ( _fileHandle.is_open() )
                {
                    string _line;

                    for ( int q = 0; q < NUM_LANDMARKS; q++ )
                    {
                        getline( _fileHandle, _line );

                        m_landmarkIDs[q] = stoi( _line );
                    }
                }

                // Just for fun, show the nonconnected nodes
                cout << "nonconnected!!!" << endl;
                for ( int q = 0; q < m_graphRef->nodes.size(); q++ )
                {
                    if ( m_graphRef->nodes[q]->edges.size() == 0 )
                    {
                        cout << m_graphRef->nodes[q]->id << endl;
                    }
                }
                cout << "nonconnected!!!" << endl;

                for ( int q = 0; q < NUM_LANDMARKS; q++ )
                {
                    gl::LPrimitivesRenderer2D::instance->addPoint( m_graphRef->nodes[m_landmarkIDs[q]]->x,
                                                                   m_graphRef->nodes[m_landmarkIDs[q]]->y,
                                                                   0.0f, 1.0f, 0.0f );
                }
            }

            void preCalc()
            {
                cout << "preCalc ..." << endl;

                for ( int q = 0; q < NUM_LANDMARKS; q++ )
                {
                    preCalc_( q, m_landmarkIDs[q] );
                }

                cout << "done" << endl;
            }

            void preCalc_( int pLandmarkIndx, int pLandmarkNodeId )
            {
                cout << "precalulating for landmark " << pLandmarkIndx << endl;
                float _total = m_graphRef->nodes.size();
                // Start Dijkstra
                DS::LNode< DS::LGraph<int, double> >* _start = m_graphRef->nodes[pLandmarkNodeId];
                _start->d = 0;

                LNodePriorityQueueDijkstra _pq;
                //LNodeVectPriorityQueue _pq;

                for ( int q = 0; q < m_graphRef->nodes.size(); q++ )
                {
                    if ( m_graphRef->nodes[q]->id != _start->id )
                    {
                        m_graphRef->nodes[q]->d = INF;
                    }
                    _pq.push( m_graphRef->nodes[q] );
                }

                int _count = 0;
                while ( !_pq.empty() )
                {
                    DS::LNode< DS::LGraph<int, double> >* _u = _pq.top();
                    _pq.pop();
                    // cout << "dddd: " << _u->d << endl;

                    for ( int q = 0; q < _u->edges.size(); q++ )
                    {
                        DS::LNode< DS::LGraph<int, double> >* _v = _u->edges[q]->nodes[1];

                        // Relaxation
                        if ( _v->d > _u->d + _u->edges[q]->data )
                        {
                            _v->d = _u->d + _u->edges[q]->data;
                            _pq.push( _v );
                        }
                    }

                    
                    _count++;
                    //cout << "percent: " << _count / _total << endl;
                }

                // Save the precalculation into the corresponding map
                for ( int q = 0; q < m_graphRef->nodes.size(); q++ )
                {
                    m_preCalc[pLandmarkIndx].push_back( m_graphRef->nodes[q]->d );

                    // print just for fun
                    // cout << "d: " << m_graphRef->nodes[q]->d << endl;
                }

            }

            void savePreCalc()
            {
                cout << "saving precalc" << endl;

                ofstream _fileHandle ( USE_SAVED_GRAPH_PRECALC );

                if ( _fileHandle.is_open() )
                {
                    for ( int q = 0; q < NUM_LANDMARKS; q++ )
                    {
                        _fileHandle << "LANDMARK_ID " << m_landmarkIDs[q] << endl;
                        for ( int p = 0; p < m_preCalc[q].size(); p++ )
                        {
                            _fileHandle << p << " " << m_preCalc[q][p] << endl;
                        }
                    }

                    _fileHandle.close();
                }
                cout << "done" << endl;
            }

            void loadPreCalc()
            {

            }

            void launch() override
            {
                m_wData.pPreCalc = m_preCalc;
                m_wData.pLandmarkIDs = m_landmarkIDs;

                pthread_create( &m_threadHandle, NULL, LLandmarkPathFinder::calculatePath, ( void* ) &m_wData );
            }

            static void* calculatePath( void* pWorkData )
            {
                LPathFinderWorkData* _wData = ( LPathFinderWorkData* ) pWorkData;

                unordered_map<int,DS::LNode<DS::LGraph<int,double>>* > _explored;

                LNodePriorityQueue _toExplore;
                _toExplore.push( _wData->start );

                // Calculate the first heuristic value
                float _dx = _wData->start->x - _wData->end->x;
                float _dy = _wData->start->y - _wData->end->y;
                float _h = sqrt( _dx * _dx + _dy * _dy );
                _wData->start->g = 0;
                for ( int l = 0; l < NUM_LANDMARKS; l++ )
                {
                    if ( _wData->start->id == _wData->pLandmarkIDs[l] )
                    {
                        continue;
                    }
                    float dt = _wData->pPreCalc[l][_wData->end->id];
                    float dv = _wData->pPreCalc[l][_wData->start->id];

                    //cout << "abs( dt - dv ) : " << abs( dt - dv ) << endl;
                    _h = max( abs( dt - dv ), _h );
                }
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

                        if ( _explored.find( _successor->id ) != _explored.end() )
                        {
                            // Already explored, don't count it
                            continue;
                        }

                        if ( _successor->inOpen )
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

                        float dx = _successor->x - _wData->end->x;
                        float dy = _successor->y - _wData->end->y;
                        float dist = sqrt( dx * dx + dy * dy );
                        _successor->g = _nextToExplore->g + _edge->data;
                        //cout << "dist: " << dist << endl;
                        for ( int l = 0; l < NUM_LANDMARKS; l++ )
                        {
                            if ( _successor->id == _wData->pLandmarkIDs[l] )
                            {
                                continue;
                            }
                            float dt = _wData->pPreCalc[l][_wData->end->id];
                            float dv = _wData->pPreCalc[l][_successor->id];

                            //cout << "abs( dt - dv ) : " << abs( dt - dv ) << endl;
                            dist = max( abs( dt - dv ), dist );
                        }
                        _successor->h = dist;
                        //cout << "fdist: " << dist << endl;
                        _successor->f = _successor->g + _successor->h;
                        _successor->inOpen = true;
                        _toExplore.push( _successor );

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
