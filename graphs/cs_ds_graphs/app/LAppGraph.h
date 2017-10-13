
#pragma once

#include "../gl/LApp.h"
#include "LGraphWorld.h"


namespace app
{

	namespace graph
	{

		class LAppGraph : public engine::gl::LApp
		{

			public :

			LAppGraph() : engine::gl::LApp()
			{

			}

			static void create();

			void createWorld() override;

		};



	}

}


