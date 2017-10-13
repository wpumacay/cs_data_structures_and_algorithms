
#pragma once

#include <GL/glew.h>
#include "LScene.h"
#include "LCommonGL.h"
#include "world/LWorld2D.h"
#include "primitives/LPrimitivesRenderer2D.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#define DX 200.0
#define DY 200.0

#define BG_R 0.0f
#define BG_G 0.0f
#define BG_B 0.0f

namespace engine
{

	namespace gl
	{

		class LSimpleRenderer
		{

			private :

			int m_appWidth;
			int m_appHeight;

			LRenderInfo m_renderInfo;

			public :

			LSimpleRenderer( int appWidth, int appHeight )
			{
				m_appWidth = appWidth;
				m_appHeight = appHeight;
			}


			LPoint2D fromXYtoGL( const LPoint2D& xy )
			{
				return LPoint2D( xy.x / DX, xy.y / DY );
			}

			void prepareRender( LWorld2D* pWorld )
			{
				m_renderInfo.appWidth 		= m_appWidth;
				m_renderInfo.appHeight 		= m_appHeight;
				m_renderInfo.worldWidth 	= pWorld->width();
				m_renderInfo.worldHeight 	= pWorld->height();

				m_renderInfo.cameraWidth  	= pWorld->camera()->width();
				m_renderInfo.cameraHeight 	= pWorld->camera()->height();
				m_renderInfo.cameraX 	  	= pWorld->camera()->x();
				m_renderInfo.cameraY 		= pWorld->camera()->y();
				m_renderInfo.cameraZoom 	= pWorld->camera()->zoom();

				m_renderInfo.aspectRatio = m_renderInfo.appWidth / m_renderInfo.appHeight;
				m_renderInfo.pix2world = pWorld->pix2world();

				m_renderInfo.mat_view = pWorld->camera()->matView();
				m_renderInfo.mat_proj = pWorld->camera()->matProj();
			}

			void render( LScene* scene )
			{
				glClearColor( BG_R, BG_G, BG_B, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT );

				scene->render( m_renderInfo );

				// Draw debug primitives
				LPrimitivesRenderer2D::instance->render( m_renderInfo );
			}

		};
		
	}

}