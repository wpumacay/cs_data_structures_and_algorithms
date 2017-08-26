
#pragma once

#include "LCommon.h"
#include <GL/glew.h>

namespace engine
{

	namespace gp
	{

		class LGraphicsObject2D
		{

			protected :

			int m_numVertices;
			int m_numTriangles;
			GLfloat* m_vertices;
			GLuint* m_indices;

			GLuint programId;
			GLuint vbo;
			GLuint vao;
			GLuint ebo;

			public :


			LPoint xy;
			float rotation;
			LPoint scale;

			LGraphicsObject2D()
			{
				programId = 0;
				this->xy.x = 0;
				this->xy.y = 0;
				this->rotation = 0.0f;
				this->scale.x = 1.0f;
				this->scale.y = 1.0f;
			}

			LGraphicsObject2D( double x, double y )
			{
				programId = 0;
				this->xy.x = x;
				this->xy.y = y;
				this->rotation = 0.0f;
				this->scale.x = 1.0f;
				this->scale.y = 1.0f;
			}

			virtual void render() = 0;
		};
	}



}