

#include "LAppGraph.h"

#include <iostream>

using namespace std;

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

		void LAppGraph::initialize() override
		{
			LApp::initialize();

			m_uiScreen = new nanogui::Screen();
			m_uiScreen->initialize( m_window, true );

			m_uiForm = new nanogui::FormHelper( m_uiScreen );
		}

		void LAppGraph::createWorld()
		{
			cout << "creating graph-world" << endl;

			m_world = new LGraphWorld( 4000.0f, 2000.0f, 
                                       APP_WIDTH, APP_HEIGHT, 
                                       1.0f );
			m_stage->addChildScene( m_world->scene() );
		}

		void LAppGraph::loop() override
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

                m_uiScreen->drawContents();
                m_uiScreen->drawWidgets();

                glfwSwapBuffers( m_window );
            }

		}

	}

}