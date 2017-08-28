

#include <iostream>
#include "LSolver.h"
//#include "gl/LApp.h"


using namespace std;

engine::LSolver* engine::LSolver::instance = nullptr;

namespace engine
{

    LSolver::LSolver( options::optimizer::_optimizer pOptimizer,
                      options::intensifier::_intensifier pIntensifier )
    {
        m_optimizerId = pOptimizer;
        m_intensifierId = pIntensifier;

        m_configuration = nullptr;
        m_bestConfiguration = nullptr;

        switch ( m_optimizerId )
        {
            case options::optimizer::BS_GRADIENT_DESCENT :
                m_optimizer = new optimizers::LGradientDescentOptimizer();
            break;

            default :
                m_optimizer = new optimizers::LGradientDescentOptimizer();
            break;
        }

        switch ( m_intensifierId )
        {
            case options::intensifier::VND :
            cout << "Initialized VND intensifier" << endl;
                m_intensifier = new intensifiers::LVNDintensifier( m_optimizer );
            break;

            case options::intensifier::TS :
                cout << "Initialized TS intensifier" << endl;
                m_intensifier = new intensifiers::LTSintensifier( m_optimizer );
            break;

            case options::intensifier::TS_VND :
                cout << "Initialized TS intensifier" << endl;
                m_intensifier = new intensifiers::LTS_VNDintensifier( m_optimizer );
            break;

            default :
                m_intensifier = new intensifiers::LVNDintensifier( m_optimizer );
            break;
        }

        m_diversifier = new diversifiers::LRRdiversifier();
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
        m_bestConfiguration = LConfiguration::initializeFromInstance( pCircleInstance, pInstanceSize );

        if ( m_intensifierId == options::intensifier::TS )
        {
            reinterpret_cast<intensifiers::LTSintensifier*>( m_intensifier )->setTabuIterations( 10 * pInstanceSize );
        }
        else if ( m_intensifierId == options::intensifier::TS_VND )
        {
            reinterpret_cast<intensifiers::LTS_VNDintensifier*>( m_intensifier )->setTabuIterations( 10 * pInstanceSize );
        }
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
        m_diversifier->run( m_configuration );
        m_intensifier->run( m_configuration );
    }

    void LSolver::solve()
    {
        for ( int q = 0; q < SOLVER_ITERATIONS; q++ )
        {
            *m_configuration = *m_bestConfiguration;
            cout << "Iteration " << ( q + 1 ) << " -------" << endl;
            m_diversifier->run( m_configuration );
            m_intensifier->run( m_configuration );
            if ( m_configuration->getContainer().r < m_bestConfiguration->getContainer().r )
            {
                *m_bestConfiguration = *m_configuration;
            }
            cout << "LSolver> best so far: " << m_bestConfiguration->getContainer().r << endl;
            cout << "LSolver> feasibility: " << m_bestConfiguration->feasibility() << endl;

            cout << "Show-results? yes(1), no(-) : " << endl;
            int _option;
            cin >> _option;
            cout << endl;
            /*
            if ( _option == 1 )
            {
                engine::gl::LApp _app;
                _app.initialize();

                // Test the app, add some baseObjects


                _app.loop();
                _app.finalize();
            }
            */
        }
    }

    void LSolver::reset( options::optimizer::_optimizer pOptimizer,
                         options::intensifier::_intensifier pIntensifier )
    {
        m_optimizerId = pOptimizer;
        m_intensifierId = pIntensifier;

        if ( m_configuration != nullptr )
        {
            delete m_configuration;
        }
        if ( m_bestConfiguration != nullptr )
        {
            delete m_bestConfiguration;
        }
        if ( m_optimizer != nullptr )
        {
            delete m_optimizer;
        }
        if ( m_intensifier != nullptr )
        {
            delete m_intensifier;
        }


        m_configuration = nullptr;
        m_bestConfiguration = nullptr;

        switch ( m_optimizerId )
        {
            case options::optimizer::BS_GRADIENT_DESCENT :
                m_optimizer = new optimizers::LGradientDescentOptimizer();
            break;

            default :
                m_optimizer = new optimizers::LGradientDescentOptimizer();
            break;
        }

        switch ( m_intensifierId )
        {
            case options::intensifier::VND :
            cout << "Initialized VND intensifier" << endl;
                m_intensifier = new intensifiers::LVNDintensifier( m_optimizer );
            break;

            case options::intensifier::TS :
                cout << "Initialized TS intensifier" << endl;
                m_intensifier = new intensifiers::LTSintensifier( m_optimizer );
            break;

            case options::intensifier::TS_VND :
                cout << "Initialized TS_VND intensifier" << endl;
                m_intensifier = new intensifiers::LTS_VNDintensifier( m_optimizer );
            break;

            default :
                m_intensifier = new intensifiers::LVNDintensifier( m_optimizer );
            break;
        }

        m_diversifier = new diversifiers::LRRdiversifier();
    }

    void LSolver::test_swap( int indx1, int indx2 )
    {
        m_configuration->swapCircles( indx1, indx2 );
    }

    void LSolver::test_optimization()
    {
        m_optimizer->run( m_configuration );
    }

    void LSolver::test_random_reset()
    {
        m_diversifier->run( m_configuration );
    }

    void LSolver::test_insert()
    {
        p_insNeigh = neighborhood::insert::makeInsertNeighborhood( m_configuration,
                                                                   m_optimizer );
    }

}
