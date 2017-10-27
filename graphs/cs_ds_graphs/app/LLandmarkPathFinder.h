
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

            void loadLandmarks( int graphSize = -1 )
            {
                cout << "loading landmarks" << endl;

            #ifdef ENABLE_UI
                string _pathGraphLandmarks;
                _pathGraphLandmarks += "res/landmarks/";
                _pathGraphLandmarks += std::to_string( graphSize );
                _pathGraphLandmarks += "points_landmarks.data";

                cout << "_pathGraphLandmarks: " << _pathGraphLandmarks << endl;

                ifstream _fileHandle ( _pathGraphLandmarks.c_str() );
            #else
                ifstream _fileHandle ( USE_SAVED_GRAPH_LANDMARKS );
            #endif
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
                // cout << "nonconnected!!!" << endl;
                // for ( int q = 0; q < m_graphRef->nodes.size(); q++ )
                // {
                //     if ( m_graphRef->nodes[q]->edges.size() == 0 )
                //     {
                //         cout << m_graphRef->nodes[q]->id << endl;
                //     }
                // }
                // cout << "nonconnected!!!" << endl;

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
                cout << "precalulating for landmark " << pLandmarkIndx << " - " << pLandmarkNodeId << endl;
                float _total = m_graphRef->nodes.size();
                // Start Dijkstra
                DS::LNode< DS::LGraph<int, double> >* _start = m_graphRef->nodes[pLandmarkNodeId];
                _start->d = 0;

                for ( int q = 0; q < m_graphRef->nodes.size(); q++ )
                {
                    if ( m_graphRef->nodes[q]->id != _start->id )
                    {
                        m_graphRef->nodes[q]->d = INF;
                    }
                    m_graphRef->nodes[q]->inOpen = false;
                }

                //LNodePriorityQueueDijkstra _pq;
                LPairPriorityQueue _pq;
                //LNodeVectPriorityQueue _pq;
                //LSpecialPriorityQueueDijsktra _pq( m_graphRef->nodes );
                
                for ( int q = 0; q < m_graphRef->nodes.size(); q++ )
                {
                    _pq.push( LPair( m_graphRef->nodes[q], m_graphRef->nodes[q]->d ) );
                }

                int _count = 0;
                while ( !_pq.empty() )
                {
                    DS::LNode< DS::LGraph<int, double> >* _u = ( _pq.top() ).node;
                    _pq.pop();
                    

                    for ( int q = 0; q < _u->edges.size(); q++ )
                    {
                        DS::LNode< DS::LGraph<int, double> >* _v = _u->edges[q]->nodes[1];

                        // Relaxation
                        if ( _v->d > _u->d + _u->edges[q]->data )
                        {
                            _v->d = _u->d + _u->edges[q]->data;
                            _pq.push( LPair( _v, _v->d ) );
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

            void loadPreCalc( int graphSize = -1 )
            {
                cout << "loading precalc" << endl;

                for ( int q = 0; q < NUM_LANDMARKS; q++ )
                {
                    m_preCalc[q].clear();
                }

            #ifdef ENABLE_UI
                string _pathGraphPrecalc;
                _pathGraphPrecalc += "res/preprocessing/";
                _pathGraphPrecalc += std::to_string( graphSize );
                _pathGraphPrecalc += "points_preprocessing.data";

                ifstream _fileHandle ( _pathGraphPrecalc.c_str() );
            #else
                ifstream _fileHandle ( USE_SAVED_GRAPH_PRECALC );
            #endif
                if ( _fileHandle.is_open() )
                {
                    string _line;

                    for ( int q = 0; q < NUM_LANDMARKS; q++ )
                    {
                        getline( _fileHandle, _line ); // line containing the landmark id
                        vector<string> _strLandmark = split( _line );
                        int _lID = stoi( _strLandmark[1] );
                        if ( _lID != m_landmarkIDs[q] )
                        {
                            cout << "failure loading - landmarks mismatch" << endl;
                            return;
                        }
                        cout << "loading landmark " << q << endl;

                        for ( int n = 0; n < m_graphRef->nodes.size(); n++ )
                        {
                            getline( _fileHandle, _line );
                            vector<string> _strCalc = split( _line );
                            int _nId = stoi( _strCalc[0] );
                            float _nDist = stof( _strCalc[1] );

                            m_preCalc[q].push_back( _nDist );
                        }
                    }
                }


                cout << "done" << endl;
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

                //unordered_map<int,DS::LNode<DS::LGraph<int,double>>* > _explored;
                unordered_map<int,float> _costSoFar;

                LPairPriorityQueue _toExplore;

                // Calculate the first heuristic value
                float _dx = _wData->start->x - _wData->end->x;
                float _dy = _wData->start->y - _wData->end->y;
                float _h = sqrt( _dx * _dx + _dy * _dy );
                for ( int l = 0; l < NUM_LANDMARKS; l++ )
                {
                    if ( _wData->start->id == _wData->pLandmarkIDs[l] )
                    {
                        continue;
                    }
                    float dt = _wData->pPreCalc[l][_wData->end->id];
                    float dv = _wData->pPreCalc[l][_wData->start->id];

                    _h = max( abs( dt - dv ), _h );
                }

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
                            for ( int l = 0; l < NUM_LANDMARKS; l++ )
                            {
                                if ( _successor->id == _wData->pLandmarkIDs[l] )
                                {
                                    continue;
                                }
                                float dt = _wData->pPreCalc[l][_wData->end->id];
                                float dv = _wData->pPreCalc[l][_successor->id];

                                _h = max( abs( dt - dv ), _h );
                            }
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





