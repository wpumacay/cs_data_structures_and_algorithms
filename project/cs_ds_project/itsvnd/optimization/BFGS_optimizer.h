
#pragma once

#include "../../engine/LConfiguration.h"
#include <armadillo>

using namespace arma;

typedef double ( *PotFunction )( arma::mat, arma::mat );

#define ITERS_TEST 5000
#define GRAD_STEP 0.1

namespace itsvnd
{


	namespace optimizers
	{

		arma::mat computeGradient( const PotFunction &pFunction, const arma::mat &pConf, const arma::mat &pRes )
		{
			arma::mat _grad = arma::zeros<arma::mat>( pConf.n_rows, pConf.n_cols );
			double _step = 0.001;

            double _f = pFunction( pConf, pRes );

			for ( int q = 0; q < pConf.n_rows; q++ )
			{
				arma::mat _conf = pConf;
				_conf[q] += _step;

                _grad[q] = ( pFunction( _conf, pRes ) - _f ) / _step;
			}

			return _grad;
		}

		void gradientDescentStep( const PotFunction &potential, arma::mat &x, const arma::mat &res )
		{
			arma::mat _grad = computeGradient( potential, x, res );
			x = x - GRAD_STEP * _grad;
		}

        // To test the optimization of some functions
        void testGradientDescent( arma::mat &x, const arma::mat &res )
        {
            for ( int q = 0; q < ITERS_TEST; q++ )
            {
                gradientDescentStep( engine::testFunction, x, res );
            }
        }

        void bfgsStep( const PotFunction &potential, arma::mat &x, const arma::mat &res )
        {

        }



        void evalGradientDescent( engine::LConfiguration* pConfiguration )
        {
            // For a vector of the params to optimize
            arma::mat _x = arma::zeros<arma::mat>( 2 * pConfiguration->size + 1, 1 );
            // A vector for the extra resources needed ( radius )
            arma::mat _r = arma::zeros<arma::mat>( pConfiguration->size, 1 );

            _x( 0, 0 ) = ( pConfiguration->getContainer() ).r;
            for ( int q = 0; q < pConfiguration->size; q++ )
            {
                _x( 2 * q + 1, 0 ) = ( pConfiguration->getCircleByIndx( q ) ).pos.x;
                _x( 2 * q + 2, 0 ) = ( pConfiguration->getCircleByIndx( q ) ).pos.y;
                _r( q, 0 ) = ( pConfiguration->getCircleByIndx(q) ).r;
            }

            gradientDescentStep( engine::potentialFunction, _x, _r );

            ( pConfiguration->getContainer() ).r = _x( 0, 0 );
            for ( int q = 0; q < pConfiguration->size; q++ )
            {
                ( pConfiguration->getCircleByIndx( q ) ).pos.x = _x( 2 * q + 1, 0 );
                ( pConfiguration->getCircleByIndx( q ) ).pos.y = _x( 2 * q + 2, 0 );
            }
        }


	}





}
