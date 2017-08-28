
#include "LApp.h"


namespace engine
{
    namespace gl
    {

        LApp::LApp()
        {
            m_window = NULL;
            m_initialized = false;

            m_renderer = new LSimpleRenderer( APP_WIDTH, APP_HEIGHT );
            m_stage = new LScene();

            ShaderManager::create();
        }

        LApp::~LApp()
        {
            m_window = NULL;
            if ( m_stage != NULL )
            {
                delete m_stage;
                m_stage = NULL;
            }
            if ( m_renderer != NULL )
            {
                delete m_renderer;
                m_renderer = NULL;
            }
        }

        void LApp::initialize()
        {
            glfwInit();
            glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
            glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
            glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
            glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

            m_window = glfwCreateWindow( APP_WIDTH, APP_HEIGHT,
                                         "ITS-VND application",
                                         NULL, NULL );

            if ( m_window == NULL )
            {
                glfwTerminate();
                return;
            }

            glfwMakeContextCurrent( m_window );

            // Initialize glew
            glewExperimental = GL_TRUE;
            if ( glewInit() != GLEW_OK )
            {
                glfwTerminate();
                return;
            }

            glfwSetKeyCallback( m_window, this->onKeyEvent );

            glfwGetFramebufferSize( m_window, &m_width, &m_height );
            glViewport( 0, 0, m_width, m_height );

            m_initialized = true;
        }


        void LApp::loop()
        {
            if ( !m_initialized )
            {
                return;
            }
            while ( !glfwWindowShouldClose( m_window ) )
            {
                glfwPollEvents();
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                
                m_renderer->render( m_stage );

                glfwSwapBuffers( m_window );
            }
        }

        void LApp::finalize()
        {
            glfwTerminate();
        }

        void LApp::onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
                               int pAction, int pMode )
        {
            if ( pKey == GLFW_KEY_ESCAPE && pAction == GLFW_PRESS )
            {
                glfwSetWindowShouldClose( pWindow, GL_TRUE );
            }
        }
    }
}

