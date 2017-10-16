
#pragma once

#include "LPrimitive.h"

#include <iostream>

using namespace std;

#define SWARM_POINT_SIZE 0.010f

#define SWARM_MIN_SIDES 3
#define SWARM_MAX_SIDES 20

namespace engine
{

	namespace gl
	{

		class LPrimitivePointSwarm : public LPrimitive
		{

			private :

			float* m_pxBuff;
			float* m_pyBuff;
			int m_nPoints;

			public :

			LPrimitivePointSwarm() : LPrimitive()
			{
				this->type = primitive::PRIMITIVE_POINT_SWARM;
			}

			LPrimitivePointSwarm( float *px, float *py, int nPoints ) : LPrimitive()
			{
				this->type = primitive::PRIMITIVE_POINT_SWARM;
				this->xy.x = 0.0f;
				this->xy.y = 0.0f;

				m_pxBuff = px;
				m_pyBuff = py;
				m_nPoints = nPoints;
			}

			void initGeometry() override
			{

				m_numVertices = m_nPoints;
				m_vertices = new GLfloat[ 3 * m_numVertices ];
				for ( int q = 0; q < m_numVertices; q++ )
				{
					m_vertices[3 * q + 0] = m_pxBuff[q];
					m_vertices[3 * q + 1] = m_pyBuff[q];
					m_vertices[3 * q + 2] = 0.0f;

					// cout << "px: " << m_pxBuff[q] << endl;
					// cout << "py: " << m_pyBuff[q] << endl;
				}

				programResIndx = LShaderManager::instance->loadedShaders[BASE_SHADER_CIRCLE];

				cout << "nvertices: " << m_numVertices << endl;
			}

            bool isDrawable( const LRenderInfo& rInfo ) override
            {
                return true;
            }


			void drawGeometry( const LRenderInfo& rInfo, GLuint programId ) override
			{
				GLuint u_cRadius = glGetUniformLocation( programId, "u_cRadius" );
				GLuint u_cSides = glGetUniformLocation( programId, "u_cSides" );

				glUniform1f( u_cRadius, SWARM_POINT_SIZE );
				glUniform1i( u_cSides, SWARM_MAX_SIDES );

				glDrawArrays( GL_POINTS, 0, m_numVertices );
			}

		};


	}


}