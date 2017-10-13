

#pragma once

#include "../LCommonGL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#define FAR  100.0f
#define NEAR 1.0f

namespace engine
{


	namespace gl
	{


		class LBaseCamera2D
		{

			protected :

			float m_width;
			float m_height;

			float m_worldWidth;
			float m_worldHeight;

			float m_appWidth;
			float m_appHeight;

			float m_pix2world;

			float m_zoom;

			float m_x;
			float m_y;

			float m_vx;
			float m_vy;

			glm::mat4 m_matView;
			glm::mat4 m_matProj;

			public :

			LBaseCamera2D( float appWidth, float appHeight, 
						   float pix2world,
						   float wWidth, float wHeight )
			{
				m_zoom = 1.0f;

				m_x = 0.0f;
				m_y = 0.0f;

				m_vx = 0.0f;
				m_vy = 0.0f;

				m_appWidth = appWidth;
				m_appHeight = appHeight;
				m_pix2world = pix2world;

				m_worldWidth = wWidth;
				m_worldHeight = wHeight;

				m_matView = glm::mat4( 1.0f );
				m_matProj = glm::mat4( 1.0f );

				resize();
			}

			float width()
			{
				return m_width;
			}

			float height()
			{
				return m_height;
			}

			glm::mat4& matView()
			{
				return m_matView;
			}

			glm::mat4& matProj()
			{
				return m_matProj;
			}


			float x() { return m_x; }

			float y() { return m_y; }

			void setX( float x ) { m_x = x; }

			void setY( float y ) { m_y = y; }

			float vx() { return m_vx; }

			float vy() { return m_vy; }

			void setVx( float vx ) { m_vx = vx; }

			void setVy( float vy ) { m_vy = vy; }

			float zoom() { return m_zoom; }

			void setZoom( float pZoom )
			{
				m_zoom = pZoom;
				resize();
			}

			void resize()
			{
				m_width = ( 1. / m_zoom ) * m_appWidth * m_pix2world;
				m_height = ( 1. / m_zoom ) * m_appHeight * m_pix2world;

				float sx = 2. / m_width;
				float sy = 2. / m_height;
				float sz = -2. / ( FAR - NEAR );
				float tz = -( FAR + NEAR ) / ( FAR - NEAR );

				m_matProj = glm::mat4( 1.0f );
				m_matProj = glm::scale( m_matProj, glm::vec3( sx, sy, sz ) );
				m_matProj = glm::translate( m_matProj, glm::vec3( 0.0f, 0.0f, tz ) );
			}

			glm::mat4& viewMatrix()
			{
				return m_matView;
			}

			glm::mat4& projMatrix()
			{
				return m_matProj;
			}


			void update( float dt )
			{
				// setZoom( m_zoom - 0.1f * dt );
				// setZoom( 2.0f );
				// cout << "zoom: " << m_zoom << endl;

				m_x += m_vx * dt;
				m_y += m_vy * dt;

				m_matView = glm::mat4( 1.0f );
				m_matView = glm::translate( m_matView,
											glm::vec3( m_x, m_y, 0.0f ) );
			}
		};

	}





}