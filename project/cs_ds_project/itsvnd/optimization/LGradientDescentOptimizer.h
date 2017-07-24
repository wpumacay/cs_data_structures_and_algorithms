
#pragma once

#include "LBaseOptimizer.h"

using namespace std;
using namespace arma;

namespace optimizers
{

    class LGradientDescentOptimizer : public LBaseOptimizer
    {

    public :

        /*
        * @override
        * @brief gradient descent computation step
        */
        void step( const PotFunction &potential, arma::mat &x, const arma::mat &res )
        {
            arma::mat _grad = computeGradient( potential, x, res );
            x = x - GRAD_STEP * _grad;
        }

        /*
        * @override
        * @brief binary-search based gradient descent implementation
        */
        void run( engine::LConfiguration* pConfiguration )
        {
            eval( pConfiguration );

            pConfiguration->computeFeasibility();

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
                this->eval( pConfiguration );
                pConfiguration->computeFeasibility();

                if ( _upBound - _loBound <= 1 && k == _loBound )
                {
                    break;
                }

            }while( _upBound > _loBound );
        }
    };









}

