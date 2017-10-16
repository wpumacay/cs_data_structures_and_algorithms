
#pragma once

#include "LPrimitive.h"

#include <iostream>

using namespace std;

namespace engine
{

	namespace gl
	{

		class LPrimitiveLineSwarm : public LPrimitive
		{

			private :

			float* m_px1Buff;
			float* m_py1Buff;
			float* m_px2Buff;
			float* m_py2Buff;
			int m_nLines;

			GLuint vbo_colors;
			GLfloat* m_colors;

			public :

			LPrimitiveLineSwarm() : LPrimitive()
			{
				this->type = primitive::PRIMITIVE_LINE_SWARM;
			}

			LPrimitiveLineSwarm( float *p1x, float *p1y, float *p2x, float *p2y, int nLines ) : LPrimitive()
			{
				this->type = primitive::PRIMITIVE_LINE_SWARM;
				this->xy.x = 0.0f;
				this->xy.y = 0.0f;

				m_px1Buff = p1x;
				m_py1Buff = p1y;
				m_px2Buff = p2x;
				m_py2Buff = p2y;
				m_nLines = nLines;
				m_numVertices = 2 * m_nLines;

				m_colors = new GLfloat[ 3 * m_numVertices ];

				for ( int q = 0; q < m_nLines; q++ )
				{
					m_colors[ 6 * q + 0 ] = 1.0f;
					m_colors[ 6 * q + 1 ] = 1.0f;
					m_colors[ 6 * q + 2 ] = 1.0f;
					m_colors[ 6 * q + 3 ] = 1.0f;
					m_colors[ 6 * q + 4 ] = 1.0f;
					m_colors[ 6 * q + 5 ] = 1.0f;
				}
			}

			void init() override
			{
				cout << "init special xD" << endl;

                initGeometry();
                glGenBuffers( 1, &vbo );
                glGenBuffers( 1, &vbo_colors );

                glGenVertexArrays( 1, &vao );
                glBindVertexArray( vao );

                glBindBuffer( GL_ARRAY_BUFFER, vbo );
                glBufferData( GL_ARRAY_BUFFER, 
                              sizeof( GLfloat ) * 3 * m_numVertices,
                              m_vertices, GL_STATIC_DRAW );
                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0 );
                glEnableVertexAttribArray( 0 );

               	glBindBuffer( GL_ARRAY_BUFFER, vbo_colors );
               	glBufferData( GL_ARRAY_BUFFER,
               				  sizeof( GLfloat ) * 3 * m_numVertices,
               				  m_colors, GL_DYNAMIC_DRAW );
               	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0 );
               	glEnableVertexAttribArray( 1 );

                glBindVertexArray( 0 );
			}

			void initGeometry() override
			{

				m_vertices = new GLfloat[ 3 * m_numVertices ];
				for ( int q = 0; q < m_nLines; q++ )
				{
					m_vertices[6 * q + 0] = m_px1Buff[q];
					m_vertices[6 * q + 1] = m_py1Buff[q];
					m_vertices[6 * q + 2] = 0.0f;

					m_vertices[6 * q + 3] = m_px2Buff[q];
					m_vertices[6 * q + 4] = m_py2Buff[q];
					m_vertices[6 * q + 5] = 0.0f;
				}

				programResIndx = LShaderManager::instance->loadedShaders[BASE_SHADER_SWARM_LINE];

				cout << "nvertices: " << m_numVertices << endl;
			}

			void updateLineColor( int indx, float r, float g, float b )
			{
				m_colors[6 * indx + 0] = r;
				m_colors[6 * indx + 1] = g;
				m_colors[6 * indx + 2] = b;
				m_colors[6 * indx + 3] = r;
				m_colors[6 * indx + 4] = g;
				m_colors[6 * indx + 5] = b;
			}

            bool isDrawable( const LRenderInfo& rInfo ) override
            {
                return true;
            }


			void drawGeometry( const LRenderInfo& rInfo, GLuint programId ) override
			{
               	glBindBuffer( GL_ARRAY_BUFFER, vbo_colors );
               	glBufferData( GL_ARRAY_BUFFER,
               				  sizeof( GLfloat ) * 3 * m_numVertices,
               				  m_colors, GL_DYNAMIC_DRAW );
				glDrawArrays( GL_LINES, 0, 2 * m_nLines );
			}

		};


	}


}