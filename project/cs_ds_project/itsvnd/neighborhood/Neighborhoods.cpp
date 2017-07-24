

#include "Neighborhoods.h"


bool comparator( engine::Pair<int,double> p1, engine::Pair<int,double> p2 )
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

}
