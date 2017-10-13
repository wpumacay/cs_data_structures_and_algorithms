
#include "LApp.h"

#include <iostream>

using namespace std;

namespace engine
{
    namespace gl
    {

        LApp* LApp::instance = NULL;

        LApp::LApp()
        {
            m_window = NULL;
            m_initialized = false;

            LShaderManager::create();
            LPrimitivesRenderer2D::create();

            m_renderer = new LSimpleRenderer( APP_WIDTH, APP_HEIGHT );
            m_stage = new LScene();

        }

        void LApp::create()
        {
            if ( LApp::instance != NULL )
            {
                delete LApp::instance;
            }

            LApp::instance = new LApp();
            LApp::instance->initialize();
        }

        void LApp::destroy()
        {
            LApp::instance->finalize();
            delete LApp::instance;
            LApp::instance = NULL;
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
            if ( m_world != NULL )
            {
                delete m_world;
                m_world = NULL;
            }

            LApp::instance = NULL;
        }

        
        void LApp::createWorld()
        {
            cout << "creating base world" << endl;

            m_world = new LWorld2D( 4000.0f, 2000.0f, 
                                    APP_WIDTH, APP_HEIGHT, 
                                    1.0f );
            m_stage->addChildScene( m_world->scene() );
        }
        

        void LApp::initialize()
        {
            glfwInit();
            glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
            glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
            glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
            glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

            m_window = glfwCreateWindow( APP_WIDTH, APP_HEIGHT,
                                         "Base App",
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

            glfwSetKeyCallback( m_window, LApp::onKeyEvent );
            glfwSetMouseButtonCallback( m_window, LApp::onMouseEvent );
            glfwSetScrollCallback( m_window, LApp::onScrollEvent );

            glfwGetFramebufferSize( m_window, &m_width, &m_height );
            glViewport( 0, 0, m_width, m_height );

            m_initialized = true;

            LShaderManager::instance->initialize();
            createWorld();
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
                        
                m_timeNow = glfwGetTime();
                m_timeDelta = m_timeNow - m_timeBef;
                m_timeBef = m_timeNow;

                m_world->update( m_timeDelta );

                m_renderer->prepareRender( m_world );

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
            else
            {
                if ( pAction == GLFW_PRESS )
                {
                    LApp::instance->world()->onKeyDown( pKey );
                }
                else if ( pAction == GLFW_RELEASE )
                {
                    LApp::instance->world()->onKeyUp( pKey );
                }
            }
        }

        void LApp::onMouseEvent( GLFWwindow* pWindow, int pButton, 
                                 int pAction, int pMods )
        {
            double evx, evy;

            glfwGetCursorPos( pWindow, &evx, &evy );

            if ( pAction == GLFW_PRESS )
            {
                LApp::instance->world()->_onMouseDown( (float)evx, (float)evy );
            }
            else if ( pAction == GLFW_RELEASE )
            {
                LApp::instance->world()->_onMouseUp( (float)evx, (float)evy );
            }
        }

        void LApp::onScrollEvent( GLFWwindow* pWindow, double xOff, double yOff )
        {
            LApp::instance->world()->_onMouseScroll( (float) yOff );
        }
    }
}

