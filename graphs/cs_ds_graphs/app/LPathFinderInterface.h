
#pragma once

#include "LCommonPathFinding.h"

using namespace std;
using namespace engine;


namespace app
{


	namespace graph
	{


		class LPathFinderInterface
		{

			protected :

			pthread_t m_threadHandle;
			LPathFinderWorkData m_wData;

			DS::LGraph<int, double>* m_graphRef;

			DS::LNode<DS::LGraph<int, double> >* m_start;
			DS::LNode<DS::LGraph<int, double> >* m_end;
			DS::LNode<DS::LGraph<int, double> >* m_pathNode;

			float m_dist;

			public :

			int start_glIndx;
			int end_glIndx;
			int id;
			bool isRunning;

			float time;

			LPathFinderInterface()
			{
				m_graphRef = NULL;
				id = -1;
			}

			LPathFinderInterface( DS::LGraph<int, double>* pGraph, int pId )
			{
				m_graphRef = pGraph;
				id = pId;
				isRunning = false;

				m_start = NULL;
				m_end = NULL;
				m_pathNode = NULL;
				start_glIndx = -1;
				end_glIndx = -1;

				time = 0;
			}

			void run( DS::LNode<DS::LGraph<int, double> >* pStart,
					  DS::LNode<DS::LGraph<int, double> >* pEnd )
			{
				m_start = pStart;
				m_end = pEnd;

				// Do some cleanup before calculating the path
                if ( m_pathNode != NULL )
                {
                    cout << "clean previous path from pathfinder " << this->id << endl;

                   	for ( int q = 0; q < m_graphRef->nodes.size(); q++ )
                   	{
                   		m_graphRef->nodes[q]->parentInfo[this->id].first = NULL;
                   		m_graphRef->nodes[q]->parentInfo[this->id].second = NULL;
                   	}

                    // // Change the color of the edges of the path
                    // DS::LNode<DS::LGraph<int,double>>* _node = m_pathNode;
                    // DS::LNode<DS::LGraph<int,double>>* _node_parent = _node->parentInfo[this->id].first;
                    // DS::LEdge<DS::LGraph<int,double>>* _edge_parent = _node->parentInfo[this->id].second;
                    // while( _node_parent != NULL )
                    // {
                    //     _node = _node_parent;
                    //     _node_parent = _node_parent->parentInfo[this->id].first;
                    //     if ( _node_parent != NULL )
                    //     {
                    //         _edge_parent = _node_parent->parentInfo[this->id].second;
                    //     }
                    //     if ( _node->id == m_start->id )
                    //     {
                    //     	break;
                    //     }
                    // }

                    m_pathNode = NULL;
                }

                for ( int q = 0; q < m_graphRef->nodes.size(); q++ )
                {
                	m_graphRef->nodes[q]->inOpen = false;
                }

				// prepare threads and launch them
				m_wData.graphRef = m_graphRef;
				m_wData.start = m_start;
				m_wData.end = m_end;
				m_wData.pathNode = m_pathNode;
				m_wData.id = this->id;
				m_wData.working = 1;

				this->isRunning = true;
				this->time = 0;
				m_dist = 0;

				launch();
			}

			virtual void launch() = 0;

			void update( float dt )
			{
				if ( isRunning )
				{
					time += dt;

					if ( m_wData.working == 0 )
					{
						// Join
						pthread_join( m_threadHandle, ( void** )&m_pathNode );

						reconstructPath();

						isRunning = false;

						cout << "time spent in pfinder " << this->id << " : " << time << endl;
					}
				}
			}

			void reconstructPath()
			{
                if ( m_pathNode != NULL )
                {
                    // Change the color of the edges of the path
                    DS::LNode<DS::LGraph<int,double>>* _node = m_pathNode;
                    DS::LEdge<DS::LGraph<int,double>>* _edge_parent = NULL;

                    while( _node != NULL )
                    {
                    	_edge_parent = _node->parentInfo[this->id].second;

                        if ( _edge_parent != NULL )
                        {
                        #ifdef USE_BATCH_RENDER
                            gl::LPrimitivesRenderer2D::instance->updateSwarmLineColor( m_graphRef->edges_glIndx,
                                                                                       _edge_parent->glIndx, 
                                                                                       0.0f, 1.0f, 0.0f );
                        #else
                            gl::LPrimitivesRenderer2D::instance->updateLineColor( _edge_parent->glIndx, 0.0f, 1.0f, 0.0f );
                        #endif
                           	m_dist += _edge_parent->data;
                        }

                        _node = _node->parentInfo[this->id].first;

                        if ( _node->id == m_start->id )
                        {
                        	break;
                        }
                    }

                    cout << "path length: " << m_dist << endl;

                }
			}

			DS::LNode<DS::LGraph<int, double> >* pathNode()
			{
				return m_pathNode;
			}

		};


	}

}