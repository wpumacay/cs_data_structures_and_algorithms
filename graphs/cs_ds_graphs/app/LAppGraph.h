
#pragma once

#include "../gl/LApp.h"
#include "LGraphWorld.h"

#include <nanogui/nanogui.h>


namespace app
{

	namespace graph
	{

		class LAppGraph : public engine::gl::LApp
		{

			private :

			nanogui::Screen* m_uiScreen;
			nanogui::FormHelper* m_uiForm;

			public :

			LAppGraph() : engine::gl::LApp()
			{

			}

			static void create();

			void createWorld() override;

		};



	}

}


