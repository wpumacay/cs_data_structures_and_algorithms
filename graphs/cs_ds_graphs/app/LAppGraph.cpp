

#include "LAppGraph.h"

#include <iostream>
#include <string>

using namespace std;
#ifdef ENABLE_UI
using namespace nanogui;
#endif

namespace app
{

	namespace graph
	{

		void LAppGraph::create()
		{
			cout << "creating graph app" << endl;

			if ( LAppGraph::instance != NULL )
			{
				delete LAppGraph::instance;
			}

			LAppGraph::instance = new LAppGraph();
			LAppGraph::instance->initialize();
		}

		void LAppGraph::initialize()
		{
			LApp::initialize();
		#ifdef ENABLE_UI
			cout << "creating UI" << endl;

			m_uiScreen = new Screen();
			m_uiScreen->initialize( m_window, true );

			m_uiForm = new FormHelper( m_uiScreen );
			nanogui::ref<Window> nanoguiWindow = m_uiForm->addWindow( Eigen::Vector2i( 10, 10 ),
															 	      "Form helper example" );
		    m_uiForm->addGroup("Basic types");
		    m_uiForm->addButton("A button", []() { std::cout << "Button pressed." << std::endl; })->setTooltip("Testing a much longer tooltip, that will wrap around to new lines multiple times.");;

		    m_uiScreen->setVisible(true);
		    m_uiScreen->performLayout();
		    nanoguiWindow->center();

			cout << "done" << endl;
		#endif
		}

		void LAppGraph::createWorld()
		{
			cout << "creating graph-world" << endl;
		#ifdef ENABLE_UI
			m_world = new LGraphWorldGeneral( 4000.0f, 2000.0f, 
                                       		  APP_WIDTH, APP_HEIGHT, 
                                       		  1.0f );
		#else
			m_world = new LGraphWorld( 4000.0f, 2000.0f, 
                                       APP_WIDTH, APP_HEIGHT, 
                                       1.0f );
		#endif
			m_stage->addChildScene( m_world->scene() );
		}

		void LAppGraph::loop()
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
            #ifdef ENABLE_UI
                m_uiScreen->drawContents();
                m_uiScreen->drawWidgets();
            #endif
                glfwSwapBuffers( m_window );
            }

		}
	#ifdef ENABLE_UI
        void LAppGraph::onKeyCallback( int pKey, int pScancode, int pAction, int pMode )
        {
        	m_uiScreen->keyCallbackEvent( pKey, pScancode, pAction, pMode );
        }

        void LAppGraph::onMouseButtonCallback( int pButton, int pAction, int pMods )
        {
        	m_uiScreen->mouseButtonCallbackEvent( pButton, pAction, pMods );
        }

        void LAppGraph::onCursorCallback( double x, double y )
        {
        	m_uiScreen->cursorPosCallbackEvent( x, y );
        }
    #endif
	}

}