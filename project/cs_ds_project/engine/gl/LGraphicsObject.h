
#pragma once

#include "LCommon.h"
#include <GL/glew.h>
#include "LCommonGL.h"


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

			public :


			LPoint xy;
			float rotation;
			LPoint scale;

			LGraphicsObject()
			{
				programResIndx = 0;
				this->xy.x = 0;
				this->xy.y = 0;
				this->rotation = 0.0f;
				this->scale.x = 1.0f;
				this->scale.y = 1.0f;
			}

			LGraphicsObject( double x, double y )
			{
				programResIndx = 0;
				this->xy.x = x;
				this->xy.y = y;
				this->rotation = 0.0f;
				this->scale.x = 1.0f;
				this->scale.y = 1.0f;
			}

			virtual void render( const LRenderInfo& rInfo ) = 0;
		};
	}



}