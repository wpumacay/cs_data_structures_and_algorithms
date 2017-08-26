


#pragma once

#include "LBaseIntensifier.h"
#include <iostream>

#define USE_CUDA 1

namespace intensifiers
{


    class LVNDintensifier : public LBaseIntensifier
    {

    public :

        LVNDintensifier( optimizers::LBaseOptimizer* pOptimizer );

        ~LVNDintensifier();

        void run( engine::LConfiguration* pConfiguration );
    };



}
