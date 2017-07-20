
#pragma once

#include "../../engine/LConfiguration.h"
#include <armadillo>
#include <iostream>
#include <cmath>

using namespace arma;
using namespace std;

typedef double ( *PotFunction )( arma::mat, arma::mat );

#define ITERS_TEST 5000
#define GRAD_STEP 0.1
#define R_V 0.1

namespace itsvnd
{


	namespace optimizers
	{

		arma::mat computeGradient( const PotFunction &pFunction, const arma::mat &pConf, const arma::mat &pRes )
		{
			arma::mat _grad = arma::zeros<arma::mat>( pConf.n_rows, pConf.n_cols );
            double _step = 0.01;

            double _f = pFunction( pConf, pRes );

			for ( int q = 0; q < pConf.n_rows; q++ )
			{
				arma::mat _conf = pConf;
                _conf( q, 0 ) += _step;
                double _f_next = pFunction( _conf, pRes );
                _grad( q, 0 ) = ( _f_next - _f ) / _step;
			}

			return _grad;
		}

        double gradientDescentStep( const PotFunction &potential, arma::mat &x, const arma::mat &res )
		{
			arma::mat _grad = computeGradient( potential, x, res );
			x = x - GRAD_STEP * _grad;
            return potential( x, res );
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



        double evalGradientDescent( engine::LConfiguration* pConfiguration )
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
                _r( q, 0 ) = ( pConfiguration->getCircleByIndx( q ) ).r;
            }

            double _pot = gradientDescentStep( engine::potentialFunction, _x, _r );

            ( pConfiguration->getContainer() ).r = _x( 0, 0 );
            for ( int q = 0; q < pConfiguration->size; q++ )
            {
                ( pConfiguration->getCircleByIndx( q ) ).pos.x = _x( 2 * q + 1, 0 );
                ( pConfiguration->getCircleByIndx( q ) ).pos.y = _x( 2 * q + 2, 0 );
            }

            return _pot;
        }


        void BS_GradientDescent( engine::LConfiguration* pConfiguration )
        {
            double _pot = evalGradientDescent( pConfiguration );

            pConfiguration->updateFeasibility( _pot );

            if ( !pConfiguration->isFeasible() )
            {
                return;// Return with this configuration
            }

            // Do binary search to look for a better one
            int k = pConfiguration->getContainer().r / R_V;
            int _upBound = k - 1;
            int _loBound = 0;
            double _sum;
            for ( int q = 0; q < pConfiguration->size; q++ )
            {
                double _frac = pConfiguration->getCircleByIndx( q ).r / R_V;
                _sum += _frac * _frac;
            }
            _loBound = floor( sqrt( _sum ) );

            do
            {
                if ( pConfiguration->isFeasible() )
                {
                    _upBound = k - 1;
                }
                else
                {
                    _loBound = k;
                }

                k = ( _loBound + _upBound ) / 2;
                pConfiguration->getContainer().r = k * R_V;
                evalGradientDescent( pConfiguration );
                pConfiguration->computeFeasibility();

                if ( _upBound - _loBound <= 1 && k == _loBound )
                {
                    break;
                }

            }while( _upBound > _loBound );

        }

	}





}
