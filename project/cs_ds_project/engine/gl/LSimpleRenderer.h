
#pragma once

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include "LCommon.h"
#include "LScene.h"
#include "LCommonGL.h"

#define DX 200.0
#define DY 200.0

#define BG_R 0.2f
#define BG_G 0.3f
#define BG_B 0.3f

namespace engine
{

	namespace gl
	{

		class LSimpleRenderer
		{

			private :

			int appWidth;
			int appHeight;

			public :

			LSimpleRenderer( int appWidth, int appHeight )
			{
				this->appWidth = appWidth;
				this->appHeight = appHeight;
			}


			LPoint fromXYtoGL( const LPoint& xy )
			{
				return LPoint( xy.x / DX, xy.y / DY );
			}

			void render( LScene* scene )
			{
				glClearColor( BG_R, BG_G, BG_B, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT );

				LRenderInfo _rInfo;
				_rInfo.appWidth = this->appWidth;
				_rInfo.appHeight = this->appHeight;
				_rInfo.worldWidth = DX;
				_rInfo.worldHeight = DY;

				scene->render( _rInfo );
			}

		};


		
	}





}