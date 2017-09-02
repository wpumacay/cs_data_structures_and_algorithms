

#pragma once

#include "LCommon.h"
#include "LCircle.h"
#include "LConfiguration.h"

#include "../itsvnd/optimization/LBaseOptimizer.h"
#include "../itsvnd/optimization/LGradientDescentOptimizer.h"
#include "../itsvnd/intensification/LBaseIntensifier.h"
#include "../itsvnd/intensification/LVNDintensifier.h"
#include "../itsvnd/intensification/LTSintensifier.h"
#include "../itsvnd/intensification/LTS_VNDintensifier.h"
#include "../itsvnd/diversification/LBaseDiversifier.h"
#include "../itsvnd/diversification/LRRdiversifier.h"
#include "../itsvnd/neighborhood/Neighborhoods.h"

#define OPTIMIZATION_ITERATIONS 20


#define USE_LOGGER 1

#ifdef USE_LOGGER

#include "../utils/LLogger.h"
#include <string>

#define SOLVER_ITERATIONS MAX_ITERATIONS

#else

#define SOLVER_ITERATIONS 100

#endif

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
                VND,
                TS,
                TS_VND
            };
        }
    }


    class LSolver
    {

        LConfiguration* m_configuration;
        LConfiguration* m_bestConfiguration;

        options::optimizer::_optimizer m_optimizerId;
        options::intensifier::_intensifier m_intensifierId;
        optimizers::LBaseOptimizer* m_optimizer;
        intensifiers::LBaseIntensifier* m_intensifier;

        diversifiers::LBaseDiversifier* m_diversifier;

        #ifdef USE_LOGGER

        LLogger m_logger;

        #endif

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
        void solve();
        void reset( options::optimizer::_optimizer pOptimizer,
                    options::intensifier::_intensifier pIntensifier );

        void test_swap( int indx1, int indx2 );
        void test_optimization();
        void test_random_reset();
        void test_insert();

        InsertNeighborhood p_insNeigh;
    };




}
