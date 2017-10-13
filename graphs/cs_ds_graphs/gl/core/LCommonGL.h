
#pragma once

#include <glm/glm.hpp>

namespace engine
{

	namespace gl
	{

		struct LRenderInfo
		{
			int appWidth;
			int appHeight;
            int worldWidth;
            int worldHeight;

            float cameraWidth;
            float cameraHeight;
            float cameraX;
            float cameraY;
            float cameraZoom;

            float aspectRatio;
            float pix2world;// pixel to world equivalence ratio

            glm::mat4 mat_view;// camera matrix
            glm::mat4 mat_proj;// projection matrix
		};
	
		struct LPoint2D
		{
			double x;
			double y;

			LPoint2D()
			{
				x = 0;
				y = 0;
			}

			LPoint2D( double px, double py )
			{
				x = px;
				y = py;
			}
		};

	}

}