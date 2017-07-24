


#pragma once



#include "../../engine/LConfiguration.h"
#include "../optimization/LBaseOptimizer.h"
#include "../neighborhood/Neighborhoods.h"
#include <iostream>
#include <algorithm>

using namespace std;

#define OPTIMIZER_ITERATIONS 20

namespace intensifiers
{


    class LBaseIntensifier
    {

    protected :
    
        optimizers::LBaseOptimizer* m_optimizer;

    public :

        LBaseIntensifier()
        {
            m_optimizer = nullptr;
        }

        LBaseIntensifier( optimizers::LBaseOptimizer* pOptimizer )
        {
            m_optimizer = pOptimizer;
        }

        ~LBaseIntensifier()
        {
            m_optimizer = nullptr;
        }

        virtual void run( engine::LConfiguration* pConfiguration ) = 0;


    };



}
