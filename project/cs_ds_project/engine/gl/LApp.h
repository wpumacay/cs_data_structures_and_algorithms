

#pragma once


#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define APP_WIDTH 800
#define APP_HEIGHT 800

#include "LSimpleRenderer.h"
#include "LScene.h"
#include "LShaderManager.h"

namespace engine
{


    namespace gl
    {

        class LApp
        {
            private :

            GLFWwindow* m_window;
            bool m_initialized;

            int m_width;
            int m_height;

            LSimpleRenderer* m_renderer;
            LScene* m_stage;

            public :

            LApp();
            ~LApp();


            void initialize();
            void loop();
            void finalize();

            LScene* stage()
            {
                return m_stage;
            }

            static void onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
                                    int pAction, int pMode );
        };

    }


}

