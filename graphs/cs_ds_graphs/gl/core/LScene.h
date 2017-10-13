
#pragma once

#include <vector>

using namespace std;

#include "base/LGraphicsObject.h"


namespace engine
{

	namespace gl
	{

		class LScene
		{

			private :

			vector<LGraphicsObject*> m_objs;
			vector<LScene*> m_childScenes;

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

				for ( int q = 0; q < m_childScenes.size(); q++ )
				{
					delete m_childScenes[q];
					m_childScenes[q] = NULL;
				}
			}

			void addObject2D( LGraphicsObject* pObj )
			{
				m_objs.push_back( pObj );
			}

			void addChildScene( LScene* pScene )
			{
				m_childScenes.push_back( pScene );
			}

			void render( const LRenderInfo& rInfo )
			{
				for ( int q = 0; q < m_childScenes.size(); q++ )
				{
					m_childScenes[q]->render( rInfo );
				}

				for ( int q = 0; q < m_objs.size(); q++ )
				{
					m_objs[q]->render( rInfo );
				}
			}

		};




	}




}