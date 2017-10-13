
#pragma once

#include <GL/glew.h>
#include <vector>
#include "../LCommonGL.h"
#include "LPrimitive.h"
#include "LPrimitivePoint.h"
#include "LPrimitiveLine.h"
#include "LPrimitiveRect.h"

#define DEFAULT_COLOR_R 1.0f
#define DEFAULT_COLOR_G 1.0f
#define DEFAULT_COLOR_B 1.0f

using namespace std;

#define POOL_SIZE_POINTS 10000

namespace engine
{

    namespace gl
    {

        class LPrimitivesRenderer2D
        {
            private :

            //LPrimitivePoint m_pointsPool[POOL_SIZE_POINTS];
            //int m_pointsPoolUseIndx;

            vector<LPrimitive*> m_primitivesPools[6];

            LPrimitivesRenderer2D();

            public :

            int m_numDrawable;
            int m_numNonDrawable;

            static LPrimitivesRenderer2D* instance;
            static void create();
            ~LPrimitivesRenderer2D();

            void render( const LRenderInfo& rInfo );

            int addPoint( float px, float py,
                           float r = DEFAULT_COLOR_R, 
                           float g = DEFAULT_COLOR_G, 
                           float b = DEFAULT_COLOR_B );

            int addLine( float p1x, float p1y, float p2x, float p2y,
                          float r = DEFAULT_COLOR_R, 
                          float g = DEFAULT_COLOR_G, 
                          float b = DEFAULT_COLOR_B );

            void addTriangle( float p1x, float p1y,
                              float p2x, float p2y,
                              float p3x, float p3y,
                              float r = DEFAULT_COLOR_R, 
                              float g = DEFAULT_COLOR_G, 
                              float b = DEFAULT_COLOR_B );

            void addQuad( float p1x, float p1y,
                          float p2x, float p2y,
                          float p3x, float p3y,
                          float p4x, float p4y,
                          float r = DEFAULT_COLOR_R, 
                          float g = DEFAULT_COLOR_G, 
                          float b = DEFAULT_COLOR_B );

            int addRect( float px, float py,
                          float w, float h, float t,
                          float r = DEFAULT_COLOR_R, 
                          float g = DEFAULT_COLOR_G, 
                          float b = DEFAULT_COLOR_B );            

            void addCircle( float cx, float cy, float radius,
                            float r = DEFAULT_COLOR_R, 
                            float g = DEFAULT_COLOR_G, 
                            float b = DEFAULT_COLOR_B );

            void updatePoint( int indx, 
                              float px, float py );

            void updateLine( int indx, 
                             float p1x, float p1y, float p2x, float p2y );

            void updateLineColor( int indx,
                                  float r, float g, float b );

            /*
            void addPointSwarm( float* px, float* py,
                                 float r = DEFAULT_COLOR_R, 
                                 float g = DEFAULT_COLOR_G, 
                                 float b = DEFAULT_COLOR_B );

            void addCircleSwarm( float* cx, float* cy, float* radius,
                                  int nCircles,
                                  float r = DEFAULT_COLOR_R, 
                                  float g = DEFAULT_COLOR_G, 
                                  float b = DEFAULT_COLOR_B );

            void addLineSwarm( float* p1x, float* p1y,
                                float* p2x, float* p2y,
                                int nLines,
                                float r = DEFAULT_COLOR_R, 
                                float g = DEFAULT_COLOR_G, 
                                float b = DEFAULT_COLOR_B );
            */
        };

    }


}
