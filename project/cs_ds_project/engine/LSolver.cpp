

#include "LSolver.h"

engine::LSolver* engine::LSolver::instance = nullptr;

namespace engine
{

    LSolver::LSolver( options::optimizer::_optimizer pOptimizer,
                      options::intensifier::_intensifier pIntensifier )
    {
        m_optimizerId = pOptimizer;
        m_intensifierId = pIntensifier;

        m_configuration = nullptr;

        switch ( m_optimizerId )
        {
            case options::optimizer::BS_GRADIENT_DESCENT :
                m_optimizer = new optimizers::LGradientDescentOptimizer();
                // m_optimizer = new optimizers::LBS_BFGSoptimizer();
            break;

            default :
                m_optimizer = new optimizers::LGradientDescentOptimizer();
                // m_optimizer = new optimizers::LBS_BFGSoptimizer();
            break;
        }

        switch ( m_intensifierId )
        {
            case options::intensifier::VND :
                m_intensifier = new intensifiers::LVNDintensifier( m_optimizer );
            break;

            default :
                m_intensifier = new intensifiers::LVNDintensifier( m_optimizer );
            break;
        }
    }

    LSolver* LSolver::createSolver( options::optimizer::_optimizer pOptimizer,
                                    options::intensifier::_intensifier pIntensifier )
    {
        if ( LSolver::instance != nullptr )
        {
            delete LSolver::instance;
        }

        LSolver::instance = new LSolver( pOptimizer, pIntensifier );

        return LSolver::instance;
    }


    /*
    * Called to initialize the configuration
    */
    void LSolver::init( circleInstance::_circleInstance pCircleInstance, int pInstanceSize )
    {
        m_configuration = LConfiguration::initializeFromInstance( pCircleInstance, pInstanceSize );
    }

    LSolver::~LSolver()
    {

        if ( m_configuration != nullptr )
        {
            delete m_configuration;
        }

        if ( m_optimizer != nullptr )
        {
            delete m_optimizer;
        }

        if ( m_intensifier != nullptr )
        {
            delete m_intensifier;
        }
    }

    LConfiguration* LSolver::configuration()
    {
        return m_configuration;
    }

    void LSolver::step()
    {
        // Test
        // for ( int q = 0; q < OPTIMIZATION_ITERATIONS; q++ )
        // {
        //     m_optimizer->run( m_configuration );
        // }

        m_intensifier->run( m_configuration );
    }

    void LSolver::test_swap( int indx1, int indx2 )
    {
        m_configuration->swapCircles( indx1, indx2 );
    }

    void LSolver::test_optimization()
    {
        // Test
        //for ( int q = 0; q < OPTIMIZATION_ITERATIONS; q++ )
        //{
            m_optimizer->run( m_configuration );
        //}
    }

}
