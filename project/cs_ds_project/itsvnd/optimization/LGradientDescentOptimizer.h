
#pragma once

#include "LBaseOptimizer.h"

using namespace std;

#ifndef TEST_MAT_LIB

using namespace arma;

#endif

namespace optimizers
{

    class LGradientDescentOptimizer : public LBaseOptimizer
    {

    public :

        /*
        * @override
        * @brief gradient descent computation step
        */

    #ifndef TEST_MAT_LIB

        void step( const PotFunction &potential, arma::mat &x, const arma::mat &res )
        {
            arma::mat _grad = computeGradient( potential, x, res );
            x = x - GRAD_STEP * _grad;
        }

    #else

        void step( const PotFunction &potential, engine::mat::LMatD &x, const engine::mat::LMatD &res )
        {
            engine::mat::LMatD _grad = computeGradient( potential, x, res );
            x = x - _grad * GRAD_STEP;
        }

    #endif

        /*
        * @override
        * @brief binary-search based gradient descent implementation
        */
        void run( engine::LConfiguration* pConfiguration )
        {
            for ( int s = 0; s < OPTIMIZER_ITERATIONS; s++ )
            {
                eval( pConfiguration );
            }

            pConfiguration->computeFeasibility();
            pConfiguration->computeContainerFeasibility();

            if ( !pConfiguration->isContainerFeasible() )
            {
                return;// Return with this configuration
            }

            pConfiguration->getContainer().isUpdatable = false;

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
                if ( pConfiguration->isContainerFeasible() )
                {
                    _upBound = k - 1;
                }
                else
                {
                    _loBound = k;
                }

                k = ( _loBound + _upBound ) / 2;
                pConfiguration->getContainer().r = k * R_V;
                
                for ( int s = 0; s < OPTIMIZER_ITERATIONS; s++ )
                {
                    eval( pConfiguration );
                }
                pConfiguration->computeFeasibility();
                pConfiguration->computeContainerFeasibility();

                if ( _upBound - _loBound <= 1 && k == _loBound )
                {
                    break;
                }

            }while( _upBound > _loBound );

            pConfiguration->getContainer().isUpdatable = true;
        }

        /*
        * @override
        * @brief binary-search based gradient descent implementation
        */
        void runLite( engine::LConfiguration* pConfiguration )
        {
            for ( int s = 0; s < 4; s++ )
            {
                eval( pConfiguration );
            }

            pConfiguration->computeFeasibility();
            pConfiguration->computeContainerFeasibility();

            if ( !pConfiguration->isContainerFeasible() )
            {
                return;// Return with this configuration
            }

            pConfiguration->getContainer().isUpdatable = false;

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
                if ( pConfiguration->isContainerFeasible() )
                {
                    _upBound = k - 1;
                }
                else
                {
                    _loBound = k;
                }

                k = ( _loBound + _upBound ) / 2;
                pConfiguration->getContainer().r = k * R_V;

                for ( int s = 0; s < 4; s++ )
                {
                    eval( pConfiguration );
                }
                pConfiguration->computeFeasibility();
                pConfiguration->computeContainerFeasibility();

                if ( _upBound - _loBound <= 1 && k == _loBound )
                {
                    break;
                }

            }while( _upBound > _loBound );

            pConfiguration->getContainer().isUpdatable = true;
        }
    };









}

