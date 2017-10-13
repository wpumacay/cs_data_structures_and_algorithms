
#include "LPrimitivesRenderer2D.h"
#include <iostream>

using namespace std;

namespace engine
{

	namespace gl
	{

		LPrimitivesRenderer2D* LPrimitivesRenderer2D::instance = NULL;

		LPrimitivesRenderer2D::LPrimitivesRenderer2D()
		{
			//m_pointsPoolUseIndx = -1;
		}

		LPrimitivesRenderer2D::~LPrimitivesRenderer2D()
		{
			for ( int q = 0; q < 5; q++ )
			{
				for ( int s = 0; s < m_primitivesPools[q].size(); s++ )
				{
					delete m_primitivesPools[q][s];
					m_primitivesPools[q][s] = NULL;
				}
			}
		}

		void LPrimitivesRenderer2D::create()
		{
			if ( engine::gl::LPrimitivesRenderer2D::instance != NULL )
			{
				delete engine::gl::LPrimitivesRenderer2D::instance;
			}

			engine::gl::LPrimitivesRenderer2D::instance = new engine::gl::LPrimitivesRenderer2D;
		}


		void LPrimitivesRenderer2D::render( const LRenderInfo& rInfo )
		{
			//m_numDrawable = 0;
			//m_numNonDrawable = 0;
			for ( int q = 4; q >= 0; q-- )
			{
				for ( int s = 0; s < m_primitivesPools[q].size(); s++ )
				{
	                if ( !m_primitivesPools[q][s]->isDrawable( rInfo ) )
	                {
	                	//m_numNonDrawable++;
	                    continue;
	                }
	                //m_numDrawable++;
					m_primitivesPools[q][s]->render( rInfo );
				}
			}

			//cout << "nDrawable: " << m_numDrawable << endl;
			//cout << "nNonDrawable: " << m_numNonDrawable << endl;
			//cout << "percentDraw: " << ( ( float ) m_numDrawable ) / ( m_numDrawable + m_numNonDrawable ) << endl;
		}

		int LPrimitivesRenderer2D::addPoint( float px, float py,
		                                     float r, float g, float b )
		{
			//m_pointsPoolUseIndx++;
			//m_pointsPool[m_pointsPoolUseIndx].init();
			//m_pointsPool[m_pointsPoolUseIndx].setColor( r, g, b, 1.0f );
			//m_pointsPool[m_pointsPoolUseIndx].xy.x = px;
			//m_pointsPool[m_pointsPoolUseIndx].xy.y = px;

			LPrimitivePoint* _point = new LPrimitivePoint();
			_point->init();
			_point->setColor( r, g, b, 1.0f );
			_point->xy.x = px;
			_point->xy.y = py;

			m_primitivesPools[primitive::PRIMITIVE_POINT].push_back( _point );
			return m_primitivesPools[primitive::PRIMITIVE_POINT].size() - 1;
		}

		int LPrimitivesRenderer2D::addLine( float p1x, float p1y, float p2x, float p2y,
								            float r, float g, float b )
		{
			LPrimitiveLine* _line = new LPrimitiveLine( p1x, p1y, p2x, p2y );
			_line->init();
			_line->setColor( r, g, b, 1.0f );

			m_primitivesPools[primitive::PRIMITIVE_LINE].push_back( _line );
			return m_primitivesPools[primitive::PRIMITIVE_LINE].size() - 1;
		}

		void LPrimitivesRenderer2D::updatePoint( int indx, float px, float py )
		{
			m_primitivesPools[primitive::PRIMITIVE_POINT][indx]->xy.x = px;
			m_primitivesPools[primitive::PRIMITIVE_POINT][indx]->xy.y = py;
		}

		void LPrimitivesRenderer2D::updateLine( int indx, float p1x, float p1y, float p2x, float p2y )
		{
			reinterpret_cast<LPrimitiveLine*>
						( m_primitivesPools[primitive::PRIMITIVE_LINE][indx] )->updatePoints( p1x, p1y, p2x, p2y );
		}

		void LPrimitivesRenderer2D::updateLineColor( int indx, float r, float g, float b )
		{
			reinterpret_cast<LPrimitiveLine*>
						( m_primitivesPools[primitive::PRIMITIVE_LINE][indx] )->setColor( r, g, b, 1.0f );
		}

		void LPrimitivesRenderer2D::addTriangle( float p1x, float p1y,
								                 float p2x, float p2y,
								                 float p3x, float p3y,
								                 float r, float g, float b )
		{

		}

		void LPrimitivesRenderer2D::addQuad( float p1x, float p1y,
								             float p2x, float p2y,
								             float p3x, float p3y,
								             float p4x, float p4y,
								             float r, float g, float b )
		{

		}

		int LPrimitivesRenderer2D::addRect( float px, float py,
                          					 float w, float h, float t,
								             float r, float g, float b )
		{
			LPrimitiveRect* _rect = new LPrimitiveRect( px, py, w, h, t );
			_rect->init();
			_rect->setColor( r, g, b, 1.0f );

			m_primitivesPools[primitive::PRIMITIVE_RECT].push_back( _rect );
			return m_primitivesPools[primitive::PRIMITIVE_RECT].size() - 1;
		}

		void LPrimitivesRenderer2D::addCircle( float cx, float cy, float radius,
								               float r, float g, float b )
		{

		}

	}


}

