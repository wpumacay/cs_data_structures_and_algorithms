
#pragma once

#include "LGraphicsObject.h"




namespace engine
{

	namespace gl
	{

		class LScene
		{

			private :

			std::vector<LGraphicsObject*> m_objs;



			public :


			LScene()
			{

			}

			~LScene()
			{
				for ( int q = 0; q < m_objs.size(); q++ )
				{
					delete m_objs[q];
					m_objs[q] = NULL;
				}
			}

			void addObject2D( LGraphicsObject* pObj )
			{
				m_objs.push_back( pObj );
			}

			void render( const LRenderInfo& rInfo )
			{
				for ( int q = 0; q < m_objs.size(); q++ )
				{
					m_objs[q]->render( rInfo );
				}
			}

		};




	}




}