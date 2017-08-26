

#pragma once


#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define APP_WIDTH 800
#define APP_HEIGHT 800


namespace engine
{


    namespace gp
    {

        class LApp
        {
            private :

            GLFWwindow* m_window;
            bool m_initialized;

            int m_width;
            int m_height;

            public :

            LApp();
            ~LApp();


            void initialize();
            void loop();
            void finalize();

            static void onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
                                    int pAction, int pMode );
        };

    }


}


