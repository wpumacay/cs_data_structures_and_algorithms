
#pragma once

#include "LCommon.h"
#include <GL/glew.h>

namespace engine
{

	namespace gp
	{

		class LGraphicsObject
		{

			protected :

			GLfloat m_vertices[] = {  0.1f,  0.1f, 0.0f,
									  0.1f, -0.1f, 0.0f,
									 -0.1f, -0.1f, 0.0f,
									 -0.1f,  0.1f, 0.0f };

			GLuint programId;
			GLuint vbo;
			GLuint vao;
			GLuint ebo;
			

			public :


			LPoint xy;

			LGraphicsObject()
			{
				this->xy.x = 0;
				this->xy.y = 0;
			}

			LGraphicsObject( double x, double y )
			{
				this->xy.x = x;
				this->xy.y = y;
			}

			

			void render()
			{
				// Render the object
				glUseProgram( this->programId );

				glBindVertexArray( this->vao );

				glBindVertexArray( 0 );

				glUseProgram( 0 );
			}
		};
	}



}