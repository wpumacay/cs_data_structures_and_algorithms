
#pragma once

#include "../gl/LApp.h"
#include "LGraphWorld.h"

#ifdef ENABLE_UI
	#include <nanogui/nanogui.h>
	#include "LGraphWorldGeneral.h"
#endif

namespace app
{

	namespace graph
	{

		class LAppGraph : public engine::gl::LApp
		{

			private :
		#ifdef ENABLE_UI
			nanogui::Screen* m_uiScreen;
			nanogui::FormHelper* m_uiForm;
		#endif
			public :

			LAppGraph() : engine::gl::LApp()
			{

			}

			static void create();

			void createWorld() override;
			void initialize() override;
			void loop() override;

		#ifdef ENABLE_UI
            void onKeyCallback( int pKey, int pScancode, int pAction, int pMode ) override;

            void onMouseButtonCallback( int pButton, int pAction, int pMods ) override;

            void onCursorCallback( double x, double y ) override;
        #endif
		};



	}

}


