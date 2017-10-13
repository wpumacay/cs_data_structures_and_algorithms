
#pragma once

#include "../gl/core/world/LWorld2D.h"
#include "../gl/core/primitives/LPrimitivesRenderer2D.h"

#include "../graph/LGraph.h"
#include "../common.h"

#include <iostream>

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

            public :

            LGraphWorld( float wWidth, float wHeight,
                         float appWidth, float appHeight,
                         float pix2world ) : engine::gl::LWorld2D( wWidth, wHeight,
                                                                   appWidth, appHeight,
                                                                   pix2world )
            {
                initRandomGraph();

                //gl::LPrimitivesRenderer2D::instance->addLine( -200.0f, 200.0f, 200.0f, 200.0f );

            }

            void initRandomGraph()
            {
                for ( int q = 0; q < NUM_GRAPH_NODES; q++ )
                {
                    double _x = ( RANDOM() - 0.5 ) * m_width;
                    double _y = ( RANDOM() - 0.5 ) * m_height;
                    DS::LNode<DS::LGraph<int, double> >* _pNode = m_graph.insertNode( q, _x, _y, q );

                    _pNode->glIndx = gl::LPrimitivesRenderer2D::instance->addPoint( _x, _y );
                }
            }

            void update( float dt )
            {
                m_camera->update( dt );
            }

        };

    }

}