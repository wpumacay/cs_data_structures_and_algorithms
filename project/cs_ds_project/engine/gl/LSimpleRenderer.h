
#pragma once

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include "LCommon.h"
#include "LGraphicsObject.h"

#define DX 100.0
#define DY 100.0

#define BG_R 0.2f
#define BG_G 0.3f
#define BG_B 0.3f

namespace engine
{

	namespace gp
	{

		class LSimpleRenderer
		{

			private :

			std::vector<LGraphicsObject*> m_objs;

			public :


			LPoint fromXYtoGL( const LPoint& xy )
			{
				return LPoint( xy.x / DX, xy.y / DY );
			}

			void render()
			{
				glClearColor( BG_R, BG_G, BG_B, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT );

				for ( int q = 0; q < m_objs.size(); q++ )
				{
					m_objs[q].render();
				}
			}

		};


		
	}





}