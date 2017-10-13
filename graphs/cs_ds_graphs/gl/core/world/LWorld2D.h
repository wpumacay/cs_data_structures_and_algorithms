

#pragma once

#include "../LScene.h"
#include "../camera/LBaseCamera2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"

#include <iostream>

using namespace std;

#define CAM_ZOOM_STEP 0.1f

namespace engine
{

    namespace gl
    {

        class LWorld2D
        {

            protected :

            LScene* m_scene;
            LBaseCamera2D* m_camera;

            float m_width;
            float m_height;

            float m_appWidth;
            float m_appHeight;

            float m_pix2world;

            public :

            LWorld2D( float wWidth, float wHeight,
                      float appWidth, float appHeight,
                      float pix2world )
            {
                m_width = wWidth;
                m_height = wHeight;

                m_appWidth = appWidth;
                m_appHeight = appHeight;

                m_pix2world = pix2world;

                m_scene = new LScene();
                m_camera = new LBaseCamera2D( appWidth, appHeight,
                                              pix2world,
                                              wWidth, wHeight );
            }

            ~LWorld2D()
            {
                delete m_scene;
                delete m_camera;
            }

            LBaseCamera2D* camera()
            {
                return m_camera;
            }

            LScene* scene()
            {
                return m_scene;
            }

            float width()
            {
                return m_width;
            }

            float height()
            {
                return m_height;
            }

            float pix2world()
            {
                return m_pix2world;
            }

            virtual void onKeyDown( int pKey )
            {
                // Override this
            }

            virtual void onKeyUp( int pKey )
            {
                // Override this
            }

            void screenToWorld( float sx, float sy, float &wx, float &wy )
            {
                // to clip space
                float cx = sx - 0.5 * m_appWidth;
                float cy = -sy + 0.5 * m_appHeight;

                cx = 2 * ( cx / m_appWidth );
                cy = 2 * ( cy / m_appHeight );


                glm::mat4 mat_view = m_camera->viewMatrix();
                glm::mat4 mat_proj = m_camera->projMatrix();

                glm::mat4 mat_pv = mat_proj * mat_view;

                glm::mat4 mat_inv_pv = glm::inverse( mat_pv );

                glm::vec4 clip_xy( cx, cy, 0.0f, 1.0f );
                glm::vec4 world_xy = mat_inv_pv * clip_xy;

                wx = world_xy.x;
                wy = world_xy.y;
                
                // cout << "cx: " << cx << endl;
                // cout << "cy: " << cy << endl;

                // cout << glm::to_string( clip_xy ) << endl;
                // cout << glm::to_string( world_xy ) << endl;
                // cout << glm::to_string( mat_pv ) << endl;
                // cout << glm::to_string( mat_inv_pv ) << endl;

                cout << "wx: " << wx << endl;
                cout << "wy: " << wy << endl;

                // from map
                // cout << "mx: " << ( wx + 0.5 * m_width ) << endl;
                // cout << "my: " << ( wy + 0.5 * m_height ) << endl;

            }
                

            void _onMouseDown( float sx, float sy )
            {
                float wx, wy;
                screenToWorld( sx, sy, wx, wy );

                onMouseDown( wx, wy );
            }

            void _onMouseUp( float sx, float sy )
            {
                float wx, wy;
                screenToWorld( sx, sy, wx, wy );

                onMouseUp( wx, wy );
            }

            void _onMouseScroll( float off )
            {
                m_camera->setZoom( m_camera->zoom() + off * CAM_ZOOM_STEP );
            }

            virtual void onMouseDown( float wx, float wy )
            {
                // Override this
            }

            virtual void onMouseUp( float wx, float wy )
            {
                // Override this
            }

            virtual void update( float dt )
            {
                // Override this
            }

        };

    }


}