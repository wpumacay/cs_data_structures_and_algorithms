
#pragma once

#include "LBaseOptimizer.h"

using namespace std;
using namespace arma;

#define STEP_CALCULATION_ITERS 10

namespace optimizers
{

    class LBS_BFGSoptimizer : public LBaseOptimizer
    {

    private :

        arma::mat B_k;
        arma::mat B_inv_k;

    public :

        /*
        * @override
        * @brief bfgs computation step
        */
        void step( const PotFunction &potential, arma::mat &x, const arma::mat &res )
        {
            // Compute the gradient
            arma::mat _grad_k = computeGradient( potential, x, res );
            // COmpute direction p_{k} by B_{k}p_{k} = -grad(x_{k})
            arma::mat p_k =  -B_inv_k * _grad_k;
            // Calculate stepsize alpha_{k} = argmin( f(x_k + alpha * p_k) )
            double alpha_k = 0.001;
            //double f_xk = potential( x, res );

            /*
            for( int q = 0; q < STEP_CALCULATION_ITERS; q++ )
            {
                double f_sub = potential( x + alpha_k, res );
                if ( f_sub < f_xk )
                {
                    alpha_k *= 0.5;
                }
                else
                {
                    alpha_k *= 2.0;
                }
            }
            */

            // Update x_{k+1}
            arma::mat s_k = alpha_k * p_k;

            x = x + s_k;
            // Calculate delta gradient
            arma::mat y_k = computeGradient( potential, x, res ) -
                            _grad_k;
            // Update hessian
            arma::mat _ykt_sk = ( y_k.t() * s_k );
            double ykt_sk = _ykt_sk( 0, 0 );
            if ( ykt_sk > 100 )
            {
                cout << "??" << endl;
                cout << _ykt_sk << endl;
                cout << y_k << endl;
                cout << s_k << endl;
                cout << p_k << endl;
                cout << alpha_k << endl;
                cout << B_inv_k << endl;
                cout << "??" << endl;
            }
            arma::mat _skt_Bk_sk = ( s_k.t() * this->B_k * s_k );
            double skt_Bk_sk = _skt_Bk_sk( 0, 0 );
            this->B_k = this->B_k + ( 1 / ykt_sk ) * ( y_k * y_k.t() ) -
                        ( 1 / skt_Bk_sk ) * this->B_k * s_k * s_k.t() * this->B_k;
            this->B_inv_k = ( arma::eye<arma::mat>( x.n_rows, x.n_rows ) - ( 1 / ykt_sk ) * ( s_k * y_k.t() ) ) *
                            this->B_inv_k * 
                            ( arma::eye<arma::mat>( x.n_rows, x.n_rows ) - ( 1 / ykt_sk ) * ( y_k * s_k.t() ) ) +
                            ( 1 / ykt_sk ) * s_k * s_k.t();
        }

        /*
        * @override
        * @brief binary-search based bfgs implementation
        */
        void run( engine::LConfiguration* pConfiguration )
        {
            int state_size = 2 * pConfiguration->size + 1;
            this->B_k = arma::eye<arma::mat>( state_size, state_size );
            this->B_inv_k = arma::eye<arma::mat>( state_size, state_size );

            cout << this->B_inv_k << endl;
            for ( int q = 0; q < OPTIMIZE_ITERS; q++ )
            {
                eval( pConfiguration );
            }
            cout << this->B_inv_k << endl;

            pConfiguration->computeFeasibility();

            if ( !pConfiguration->isFeasible() )
            {
                return;// Return with this configuration
            }
            return;
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
                this->B_k = arma::eye<arma::mat>( state_size, state_size );
                this->B_inv_k = arma::eye<arma::mat>( state_size, state_size );
                for ( int q = 0; q < OPTIMIZE_ITERS; q++ )
                {
                    eval( pConfiguration );
                }
                pConfiguration->computeFeasibility();

                if ( _upBound - _loBound <= 1 && k == _loBound )
                {
                    break;
                }

            }while( _upBound > _loBound );
        }
    };









}

