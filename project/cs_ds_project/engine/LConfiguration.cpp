

#include "LConfiguration.h"
#include <algorithm>



namespace engine
{


#ifndef TEST_MAT_LIB
    double potentialFunction( arma::mat pConf, arma::mat pRes )
#else
    double potentialFunction( engine::mat::LMatD pConf, engine::mat::LMatD pRes )
#endif
	{
		double _res = 0.0;
#ifndef TEST_MAT_LIB
        int _nCircles = ( pConf.n_rows - 1 ) / 2;
#else
        int _nCircles = ( pConf.rows() - 1 ) / 2;
#endif
		double _rContainer = pConf( 0, 0 );
		// Calculate overlap with container
		for ( int q = 0; q < _nCircles; q++ )
		{
			double _xi = pConf( 2 * q + 1, 0 );
			double _yi = pConf( 2 * q + 2, 0 );
			double _dist = sqrt( _xi * _xi + _yi * _yi );
			double _r = pRes( q, 0 );
            double _cost_q = std::max( _dist + _r - _rContainer, 0.0 );
            _res += _cost_q * _cost_q;
		}

		// Calculate the overlap between circles
		for ( int q = 0; q < _nCircles; q++ )
		{
			double _xi = pConf( 2 * q + 1, 0 );
			double _yi = pConf( 2 * q + 2, 0 );
			double _ri = pRes( q, 0 );

			for ( int p = q + 1; p < _nCircles; p++ )
			{
				double _xj = pConf( 2 * p + 1, 0 );
				double _yj = pConf( 2 * p + 2, 0 );
                double _rj = pRes( p, 0 );

				double _dist = sqrt( ( _xi - _xj ) * ( _xi - _xj ) + 
									 ( _yi - _yj ) * ( _yi - _yj ) );
                double _cost_pq = std::max( _ri + _rj - _dist, 0.0 );

                _res += _cost_pq * _cost_pq;// + _dist * _dist;
			}
		}

		return _res;
	}

#ifndef TEST_MAT_LIB
    double containerPotentialFunction( arma::mat pConf, arma::mat pRes );
#else
    double containerPotentialFunction( engine::mat::LMatD pConf, engine::mat::LMatD pRes )
#endif
    {
        double _res = 0.0;
#ifndef TEST_MAT_LIB
        int _nCircles = ( pConf.n_rows - 1 ) / 2;
#else
        int _nCircles = ( pConf.rows() - 1 ) / 2;
#endif
        double _rContainer = pConf( 0, 0 );
        // Calculate overlap with container
        for ( int q = 0; q < _nCircles; q++ )
        {
            double _xi = pConf( 2 * q + 1, 0 );
            double _yi = pConf( 2 * q + 2, 0 );
            double _dist = sqrt( _xi * _xi + _yi * _yi );
            double _r = pRes( q, 0 );
            double _cost_q = std::max( _dist + _r - _rContainer, 0.0 );
            _res += _cost_q * _cost_q;
        }

        return _res;
    }

#ifndef TEST_MAT_LIB
	double testFunction( arma::mat pConf, arma::mat pRes )
#else
    double testFunction( engine::mat::LMatD pConf, engine::mat::LMatD pRes )
#endif
	{
		double _res = 0.0;
#ifndef TEST_MAT_LIB
		for ( int q = 0; q < pConf.n_rows; q++ )
#else
        for ( int q = 0; q < pConf.rows(); q++ )
#endif
		{
            _res += ( pConf( q, 0 ) - 1 ) * ( pConf( q, 0 ) - 1 );
		}
		return _res;
	}

	

}
