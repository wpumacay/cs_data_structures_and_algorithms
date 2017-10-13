
#pragma once

#include "LPrimitive.h"

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

namespace engine
{

	namespace gl
	{

		class LPrimitiveRect : public LPrimitive
		{

			float w;
			float h;
			float t;

			public :

			LPrimitiveRect( float px, float py, 
							float w, float h, float t ) : LPrimitive()
			{

				this->type = primitive::PRIMITIVE_RECT;
				this->xy.x = px;
				this->xy.y = py;

				this->w = w;
				this->h = h;
				this->t = t;
			}

			void initGeometry() override
			{

				m_numVertices = 1;
				m_vertices = new GLfloat[ 3 * m_numVertices ];
				m_vertices[0] = 0.0f;
				m_vertices[1] = 0.0f;
				m_vertices[2] = 0.0f;

				programResIndx = LShaderManager::instance->createProgramAdv( "gl/core/shaders/primitives/gl_primitive_rect_vertex_shader.glsl",
																		  	 "gl/core/shaders/primitives/gl_primitive_rect_fragment_shader.glsl",
																		  	 "gl/core/shaders/primitives/gl_primitive_rect_geometry_shader.glsl" );

			}

			void drawGeometry( const LRenderInfo& rInfo, GLuint programId ) override
			{
				GLuint u_w = glGetUniformLocation( programId, "u_w" );
				GLuint u_h = glGetUniformLocation( programId, "u_h" );
				GLuint u_t = glGetUniformLocation( programId, "u_t" );

				float w_clip = ( 2. * rInfo.cameraZoom ) * ( this->w / rInfo.pix2world ) / rInfo.appWidth;
				float h_clip = ( 2. * rInfo.cameraZoom ) * ( this->h / rInfo.pix2world ) / rInfo.appHeight;

				glUniform1f( u_w, w_clip );
				glUniform1f( u_h, h_clip );
				glUniform1f( u_t, this->t );

				glDrawArrays( GL_POINTS, 0, m_numVertices );
			}

		};


	}


}