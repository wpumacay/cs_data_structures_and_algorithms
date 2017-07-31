

#pragma once

#include <iostream>
#include <cmath>

#include "../../engine/LConfiguration.h"

#ifdef TEST_MAT_LIB

#include "../../engine/matrix/LMatrix.h"

#else

#include <armadillo>

#endif


using namespace std;

#ifndef TEST_MAT_LIB

using namespace arma;
typedef double ( *PotFunction )( arma::mat, arma::mat );

#else

typedef double ( *PotFunction )( engine::mat::LMatD, engine::mat::LMatD );

#endif

#define ITERS_TEST 5000
#define GRAD_STEP 0.1f
#define R_V 0.25
#define OPTIMIZER_ITERATIONS 100

namespace optimizers
{


    class LBaseOptimizer
    {

    protected :

        double m_mem_u;

        /**
        * @param {PotFunction} pFunction objective function to optimize
        * @param {arma::mat} x current configuration
        * @param {arma::mat} res optional parameter used to evaluate the objective function
        */
    #ifndef TEST_MAT_LIB
        arma::mat computeGradient( const PotFunction &pFunction, 
                                   const arma::mat &x, 
                                   const arma::mat &res )
        {
            arma::mat _grad = arma::zeros<arma::mat>( x.n_rows, x.n_cols );
            double _step = 0.01;

            double _f = pFunction( x, res );

            for ( int q = 0; q < x.n_rows; q++ )
            {
                arma::mat _conf = x;
                _conf( q, 0 ) += _step;
                double _f_next = pFunction( _conf, res );
                _grad( q, 0 ) = ( _f_next - _f ) / _step;
            }

            return _grad;
        }
    #else

        engine::mat::LMatD computeGradient( const PotFunction &pFunction, 
                                            const engine::mat::LMatD &x, 
                                            const engine::mat::LMatD &res )
        {
            engine::mat::LMatD _grad = engine::mat::LMat<double>::simple( x.rows(), x.cols(),
                                                                  engine::mat::fill::ZEROS );
            double _step = 0.01;

            double _f = pFunction( x, res );

            for ( int q = 0; q < x.rows(); q++ )
            {
                engine::mat::LMatD _conf = x;
                _conf( q, 0 ) += _step;
                double _f_next = pFunction( _conf, res );
                _grad( q, 0 ) = ( _f_next - _f ) / _step;
            }

            return _grad;
        }

    #endif


    public :

        LBaseOptimizer()
        {
            m_mem_u = 0.0;
        }

        /*
        * @param {PotFunction} potential objective function to optimize
        * @param {arma::mat} x current configuration as a vector
        * @param {arma::mat} res optional parameter used to evaluate the objective function
        */

    #ifndef TEST_MAT_LIB

        virtual void step( const PotFunction &potential, arma::mat &x, const arma::mat &res ) = 0;

    #else

        virtual void step( const PotFunction &potential, engine::mat::LMatD &x, const engine::mat::LMatD &res ) = 0;

    #endif

        void eval( engine::LConfiguration* pConfiguration )
        {

    #ifndef TEST_MAT_LIB

            // For a vector of the params to optimize
            arma::mat _x = arma::zeros<arma::mat>( 2 * pConfiguration->size + 1, 1 );
            // A vector for the extra resources needed ( radius )
            arma::mat _r = arma::zeros<arma::mat>( pConfiguration->size, 1 );

    #else
            engine::mat::LMatD _x = engine::mat::LMat<double>::simple( 2 * pConfiguration->size + 1, 1,
                                                               engine::mat::fill::ZEROS );
            engine::mat::LMatD _r = engine::mat::LMat<double>::simple( pConfiguration->size, 1,
                                                               engine::mat::fill::ZEROS );
    #endif

            _x( 0, 0 ) = ( pConfiguration->getContainer() ).r;
            for ( int q = 0; q < pConfiguration->size; q++ )
            {
                _x( 2 * q + 1, 0 ) = ( pConfiguration->getCircleByIndx( q ) ).pos.x;
                _x( 2 * q + 2, 0 ) = ( pConfiguration->getCircleByIndx( q ) ).pos.y;
                _r( q, 0 ) = ( pConfiguration->getCircleByIndx( q ) ).r;
            }

            this->step( engine::potentialFunction, _x, _r );

            ( pConfiguration->getContainer() ).r = _x( 0, 0 );
            for ( int q = 0; q < pConfiguration->size; q++ )
            {
                ( pConfiguration->getCircleByIndx( q ) ).pos.x = _x( 2 * q + 1, 0 );
                ( pConfiguration->getCircleByIndx( q ) ).pos.y = _x( 2 * q + 2, 0 );
            }
        }

        /*
        * @param {engine::LConfiguration} pConfiguration current circles configuration
        */
        virtual void run( engine::LConfiguration* pConfiguration ) = 0;


    };







}
