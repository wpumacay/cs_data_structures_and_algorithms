

#include "LConfiguration.h"
#include <algorithm>



namespace engine
{

    double potentialFunction( arma::mat pConf, arma::mat pRes )
	{
		double _res = 0.0;

		int _nCircles = ( pConf.n_rows - 1 ) / 2;
		double _rContainer = pConf( 0, 0 );
		// Calculate overlap with container
		for ( int q = 0; q < _nCircles; q++ )
		{
			double _xi = pConf( 2 * q + 1, 0 );
			double _yi = pConf( 2 * q + 2, 0 );
			double _dist = sqrt( _xi * _xi + _yi * _yi );
			double _r = pRes( q, 0 );
            _res += std::max( _dist + _r - _rContainer, 0.0 );
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
				double _rj = pConf( p, 0 );

				double _dist = sqrt( ( _xi - _xj ) * ( _xi - _xj ) + 
									 ( _yi - _yj ) * ( _yi - _yj ) );

                _res += std::max( _ri + _rj - _dist, 0.0 );
			}
		}

		return _res;
	}

	double testFunction( arma::mat pConf, arma::mat pRes )
	{
		double _res = 0.0;
		for ( int q = 0; q < pConf.n_rows; q++ )
		{
            _res += ( pConf( q, 0 ) - 1 ) * ( pConf( q, 0 ) - 1 );
		}
		return _res;
	}

	

}
