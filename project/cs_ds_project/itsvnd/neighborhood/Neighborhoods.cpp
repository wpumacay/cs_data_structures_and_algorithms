

#include "Neighborhoods.h"
#include <cmath>

using namespace std;

bool comparator( engine::Pair<int,double> p1, engine::Pair<int,double> p2 )
{
    return p1.second < p2.second;
}

bool insertComparator( engine::Pair<engine::LPoint,double> p1,
                       engine::Pair<engine::LPoint,double> p2 )
{
    return p1.second < p2.second;
}

namespace neighborhood
{

    namespace swap
    {

        SwapNeighborhoods makeSwapNeighborhoods( engine::LConfiguration* pConfiguration )
        {
            SwapNeighborhoods _res;
            // vector of pairs <circleIndx, circleRadius>
            vector<engine::Pair<int,double>> _confInfo;
            for( int q = 0; q < pConfiguration->size; q++ )
            {
                engine::Pair<int,double> _cPair;
                _cPair.first = ( pConfiguration->getCircleByIndx( q ) ).id - 1;
                _cPair.second = ( pConfiguration->getCircleByIndx( q ) ).r;

                _confInfo.push_back( _cPair );
            }

            sort( _confInfo.begin(), _confInfo.end(), comparator );

            map<int, vector<int>> _nSets;

            int k = 0, q = 0, c = 0;

            for ( q = 0; q < _confInfo.size(); q++ )
            {
                if ( q - c < SET_SIZE )
                {
                    if ( _nSets.find( k ) == _nSets.end() )
                    {
                        _nSets[k] = vector<int>();
                    }

                    _nSets[k].push_back( _confInfo[q].first );
                }
                else
                {
                    c = q;
                    k++;
                    _nSets[k] = vector<int>();
                    _nSets[k].push_back( _confInfo[q].first );
                }
            }

            int _maxKeyId = k;

            map<int, vector<int>>::iterator it;

            for ( it = _nSets.begin(); it != _nSets.end(); ++it )
            {
                int _nId = it->first;

                if ( _nId == _maxKeyId )
                {
                    continue;
                }

                vector<int> _currSet = _nSets[_nId];

                if ( _res.find( _nId ) == _res.end() )
                {
                    _res[_nId] = vector<engine::Pair<int,int>>();
                }

                vector<int> _nextSet = _nSets[ _nId + 1 ];

                for ( int p = 0; p < _currSet.size(); p++ )
                {
                    for ( int q = 0; q < _nextSet.size(); q++ )
                    {
                        engine::Pair<int, int> _pPair;
                        _pPair.first = _currSet[p];
                        _pPair.second = _nextSet[q];

                        _res[_nId].push_back( _pPair );
                    }
                }
            }

            return _res;
        }

    }

    namespace insert
    {
        InsertNeighborhood makeInsertNeighborhood( engine::LConfiguration* pConfiguration,
                                                   optimizers::LBaseOptimizer* pOptimizer )
        {
            InsertNeighborhood _res;

            // Divide into a lattice
            int _dimLattice = 2 * floor( sqrt( pConfiguration->size ) );
            int _nLattice = _dimLattice * _dimLattice;

            double R = pConfiguration->getContainer().r;

            double r = R / _dimLattice;

            engine::LConfiguration* _optConfiguration = pConfiguration->clone();

            _optConfiguration->getContainer().isUpdatable = false;

            for ( int q = 0; q < _optConfiguration->size; q++ )
            {
                _optConfiguration->getCircleByIndx( q ).isUpdatable = false;
            }

            _optConfiguration->addCircle( engine::LCircle() );
            _optConfiguration->size++;

            for ( int q = 0; q < _dimLattice; q++ )
            {
                for ( int p = 0; p < _dimLattice; p++ )
                {

                    double x = -R + r * ( 2 * q + 1 );
                    double y = R - r * ( 2 * p + 1 );

                    engine::LCircle& _probationCircle = _optConfiguration->getCircleByIndx( _optConfiguration->size - 1 );
                    _probationCircle.r = r;
                    _probationCircle.pos.x = x;
                    _probationCircle.pos.y = y;

                    pOptimizer->runLite( _optConfiguration );

                    engine::LPoint _pInsert;
                    _pInsert.x = _probationCircle.pos.x;
                    _pInsert.y = _probationCircle.pos.y;

                    engine::Pair<engine::LPoint,double> _pEntry;
                    _pEntry.first = _pInsert;
                    _pEntry.second = _optConfiguration->feasibility();

                    _res.push_back( _pEntry );

                }
            }

            sort( _res.begin(), _res.end(), insertComparator );

            InsertNeighborhood _resTotal;

            int N = pConfiguration->size / 3;

            for ( int q = 0; q < N; q++ )
            {
                double _dist = sqrt( _res[q].first.x * _res[q].first.x +
                                     _res[q].first.y * _res[q].first.y );
                if ( _dist < R )
                {
                    _resTotal.push_back( _res[q] );
                }
            }

            return _resTotal;
        }
    }

}
