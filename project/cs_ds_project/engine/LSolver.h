

#pragma once

#include "LCommon.h"
#include "LCircle.h"
#include "LConfiguration.h"

#include "../itsvnd/optimization/LBaseOptimizer.h"
#include "../itsvnd/optimization/LGradientDescentOptimizer.h"
#include "../itsvnd/intensification/LBaseIntensifier.h"
#include "../itsvnd/intensification/LVNDintensifier.h"

#define OPTIMIZATION_ITERATIONS 20

namespace engine
{


    namespace options
    {
        namespace optimizer
        {
            enum _optimizer
            {
                BS_GRADIENT_DESCENT,
                BS_LBFGS
            };
        }

        namespace intensifier
        {
            enum _intensifier
            {
                LOCAL_SEARCH,
                VND,
                TS_VND
            };
        }
    }


    class LSolver
    {

        LConfiguration* m_configuration;

        options::optimizer::_optimizer m_optimizerId;
        options::intensifier::_intensifier m_intensifierId;
        optimizers::LBaseOptimizer* m_optimizer;
        intensifiers::LBaseIntensifier* m_intensifier;

        LSolver( options::optimizer::_optimizer pOptimizer,
                 options::intensifier::_intensifier pIntensifier );
    public :

        static LSolver* instance;

        static LSolver* createSolver( options::optimizer::_optimizer pOptimizer,
                                      options::intensifier::_intensifier pIntensifier );
        void init( circleInstance::_circleInstance pCircleInstance, int pInstanceSize );
        ~LSolver();
        LConfiguration* configuration();
        void step();

        void test_swap( int indx1, int indx2 );
        void test_optimization();

    };




}
