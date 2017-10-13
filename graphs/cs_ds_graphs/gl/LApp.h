

#pragma once


#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define APP_WIDTH  1200
#define APP_HEIGHT 800

#include "core/LSimpleRenderer.h"
#include "core/LScene.h"
#include "core/shaders/LShaderManager.h"
#include "core/primitives/LPrimitivesRenderer2D.h"
#include "core/world/LWorld2D.h"

namespace engine
{


    namespace gl
    {

        class LApp
        {
            protected :

            GLFWwindow* m_window;
            bool m_initialized;

            int m_width;
            int m_height;

            float m_timeBef;
            float m_timeNow;
            float m_timeDelta;

            LSimpleRenderer* m_renderer;
            LScene* m_stage;

            LWorld2D* m_world;

            LApp();

            public :

            static LApp* instance;
            static void create();
            static void destroy();
            ~LApp();

            virtual void createWorld();

            void initialize();
            void loop();
            void finalize();

            int width()
            {
                return m_width;
            }

            int height()
            {
                return m_height;
            }

            LScene* stage()
            {
                return m_stage;
            }

            LWorld2D* world()
            {
                return m_world;
            }

            static void onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
                                    int pAction, int pMode );

            static void onMouseEvent( GLFWwindow* pWindow, int pButton, 
                                      int pAction, int pMods );

            static void onScrollEvent( GLFWwindow* pWindow, double xOff, double yOff );
        };

    }


}

