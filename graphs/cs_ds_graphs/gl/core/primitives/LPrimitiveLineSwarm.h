
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
			}

			void initGeometry() override
			{

				m_numVertices = 2 * m_nLines;
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

            bool isDrawable( const LRenderInfo& rInfo ) override
            {
                return true;
            }


			void drawGeometry( const LRenderInfo& rInfo, GLuint programId ) override
			{
				glDrawArrays( GL_LINES, 0, 2 * m_nLines );
			}

		};


	}


}