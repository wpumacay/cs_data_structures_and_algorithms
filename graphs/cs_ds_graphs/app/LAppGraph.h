
#pragma once

#include "../gl/LApp.h"
#include "LGraphWorld.h"

#ifdef ENABLE_UI
	#include <nanogui/nanogui.h>
	#include "LGraphWorldGeneral.h"
	#include "LCommonPathFinding.h"
#endif

namespace app
{

	namespace graph
	{

		enum _graphSize
		{
			SIZE_100,
			SIZE_1000,
			SIZE_2000,
			SIZE_5000,
			SIZE_10000,
			SIZE_1000000
		};

		class LAppGraph : public engine::gl::LApp
		{

			private :
		#ifdef ENABLE_UI
			nanogui::Screen* m_uiScreen;
			nanogui::FormHelper* m_uiForm;

			// ui options
			finderType::_finderType m_pfType;
			bool m_parallelRequestsEnabled;
			bool m_optionsEnabled;

			_graphSize m_graphSize;
			int m_graphSizeOptions[7];
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

            void onPropsModified();

            void restartWorld();
        #endif
		};



	}

}


