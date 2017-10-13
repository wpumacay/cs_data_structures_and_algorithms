
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../LCommonGL.h"

#define DEFAULT_PRIMITIVE_COLOR_R 0.0f
#define DEFAULT_PRIMITIVE_COLOR_G 1.0f
#define DEFAULT_PRIMITIVE_COLOR_B 0.0f
#define DEFAULT_PRIMITIVE_COLOR_A 1.0f

namespace engine
{

	namespace gl
	{

		class LGraphicsObject
		{

			protected :

			int m_numVertices;
			int m_numTriangles;
			GLfloat* m_vertices;
			GLuint* m_indices;

			GLuint programResIndx;
			GLuint vbo;
			GLuint vao;
			GLuint ebo;

			glm::vec4 m_color;

			public :


			LPoint2D xy;
			float rotation;
			LPoint2D scale;

			bool visible;

			LGraphicsObject()
			{
				programResIndx = 0;
				this->xy.x = 0;
				this->xy.y = 0;
				this->rotation = 0.0f;
				this->scale.x = 1.0f;
				this->scale.y = 1.0f;

				this->visible = true;

				m_color.x = DEFAULT_PRIMITIVE_COLOR_R;
				m_color.y = DEFAULT_PRIMITIVE_COLOR_G;
				m_color.z = DEFAULT_PRIMITIVE_COLOR_B;
				m_color.w = DEFAULT_PRIMITIVE_COLOR_A;
			}

			LGraphicsObject( double x, double y )
			{
				programResIndx = 0;
				this->xy.x = x;
				this->xy.y = y;
				this->rotation = 0.0f;
				this->scale.x = 1.0f;
				this->scale.y = 1.0f;

				m_color.x = DEFAULT_PRIMITIVE_COLOR_R;
				m_color.y = DEFAULT_PRIMITIVE_COLOR_G;
				m_color.z = DEFAULT_PRIMITIVE_COLOR_B;
				m_color.w = DEFAULT_PRIMITIVE_COLOR_A;
			}

			virtual void init() = 0;

	        void setColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
	        {
	        	m_color.x = r;
	        	m_color.y = g;
	        	m_color.z = b;
	        	m_color.w = a;
	        }

			virtual void render( const LRenderInfo& rInfo ) = 0;
		};
	}



}