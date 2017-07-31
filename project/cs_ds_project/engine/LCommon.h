
#pragma once

#define L_USE_QT

#define TEST_MAT_LIB

#ifdef TEST_MAT_LIB

#include "matrix/LMatrix.h"

#endif

namespace engine
{

	class LPoint
	{
		public :

        LPoint()
        {
            this->x = 0;
            this->y = 0;
        }

		LPoint( double x, double y )
		{
			this->x = x;
			this->y = y;
		}

		double x;
		double y;
	};

    template<class T1, class T2>
    struct Pair
    {
        T1 first;
        T2 second;
    };


}
