

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

			m_pfType = finderType::SEARCH_A_STAR;
			m_parallelRequestsEnabled = false;
			m_optionsEnabled = true;

			m_uiScreen = new Screen();
			m_uiScreen->initialize( m_window, true );

			m_uiForm = new FormHelper( m_uiScreen );
			nanogui::ref<Window> nanoguiWindow = m_uiForm->addWindow( Eigen::Vector2i( 10, 10 ),
															 	      "GraphSearch app options" );
			m_uiForm->addGroup("Graph options");
			m_uiForm->addVariable( "Graph Size", m_graphSize, m_optionsEnabled )->setItems( { "100", "1000", "2000", "5000", "10000", "1000000" } );
		    m_uiForm->addButton( "Reload graph",
		    					 []() { 
		    					 		std::cout << "Button pressed. graph size options" << std::endl;
		    					 		reinterpret_cast<LAppGraph*>( LAppGraph::instance )->restartWorld();
		    					 	  } );			

		    m_uiForm->addGroup("Search options");
		    m_uiForm->addVariable( "Search type", m_pfType, m_optionsEnabled )->setItems( { "A*", "ALT" } );
		    m_uiForm->addVariable( "Parallel requests enabled", m_parallelRequestsEnabled );
		    m_uiForm->addButton( "Reload props",
		    					 []() { 
		    					 		std::cout << "Button pressed. graph search options" << std::endl;
		    					 		reinterpret_cast<LAppGraph*>( LAppGraph::instance )->onPropsModified();
		    					 	  } );

		    m_uiScreen->setVisible(true);
		    m_uiScreen->performLayout();
		    nanoguiWindow->center();

			cout << "done" << endl;
		#endif
		}

		#ifdef ENABLE_UI

		void LAppGraph::onPropsModified()
		{
			cout << "props modified" << endl;
			cout << "parallel? " << m_parallelRequestsEnabled << endl;
		    cout << "pfType? " << m_pfType << endl;

		    reinterpret_cast< LGraphWorldGeneral* >( m_world )->setParallelMode( m_parallelRequestsEnabled );
		    reinterpret_cast< LGraphWorldGeneral* >( m_world )->changePathFinder( m_pfType );
		}

		#endif

		void LAppGraph::createWorld()
		{
			cout << "creating graph-world" << endl;
		#ifdef ENABLE_UI
			m_graphSize = SIZE_100;
			m_graphSizeOptions[0] = 100;
			m_graphSizeOptions[1] = 1000;
			m_graphSizeOptions[2] = 2000;
			m_graphSizeOptions[3] = 5000;
			m_graphSizeOptions[4] = 10000;
			m_graphSizeOptions[5] = 1000000;

			m_world = new LGraphWorldGeneral( m_graphSizeOptions[m_graphSize], 
											  4000.0f, 2000.0f, 
                                       		  APP_WIDTH, APP_HEIGHT, 
                                       		  1.0f );
		#else
			m_world = new LGraphWorld( 4000.0f, 2000.0f, 
                                       APP_WIDTH, APP_HEIGHT, 
                                       1.0f );
		#endif
			m_stage->addChildScene( m_world->scene() );
		}

		#ifdef ENABLE_UI
		void LAppGraph::restartWorld()
		{
			if ( m_world != NULL )
			{
				cout << "removing previous world" << endl;
				m_stage->removeChildScene( m_world->scene() );
				reinterpret_cast< LGraphWorldGeneral* >( m_world )->clean();
				delete m_world;
				m_world = NULL;
			}

			cout << "graphSize: " << m_graphSizeOptions[m_graphSize] << endl;

			m_world = new LGraphWorldGeneral( m_graphSizeOptions[m_graphSize], 
											  4000.0f, 2000.0f, 
                                       		  APP_WIDTH, APP_HEIGHT, 
                                       		  1.0f );

			m_stage->addChildScene( m_world->scene() );
		}
		#endif

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