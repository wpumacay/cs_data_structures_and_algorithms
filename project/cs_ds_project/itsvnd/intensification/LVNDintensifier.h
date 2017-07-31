


#pragma once

#include "LBaseIntensifier.h"
#include <iostream>


using namespace std;

namespace intensifiers
{


    class LVNDintensifier : public LBaseIntensifier
    {

    public :

        LVNDintensifier( optimizers::LBaseOptimizer* pOptimizer )
        {
            m_optimizer = pOptimizer;
        }

        ~LVNDintensifier()
        {
            m_optimizer = nullptr;
        }

        void run( engine::LConfiguration* pConfiguration )
        {
            cout << "Intensifier> start" << endl;
            pConfiguration->computeFeasibility();
            double _best_r = pConfiguration->getContainer().r;
            int _maxIter = 20;
            int _iterCount = 0;
            while ( _iterCount < _maxIter )
            {
                SwapNeighborhoods _sNeighborhoods = neighborhood::swap::makeSwapNeighborhoods( pConfiguration );

                SwapNeighborhoods::iterator it;

                bool _foundBetter = false;

                for ( it = _sNeighborhoods.begin(); it != _sNeighborhoods.end(); it++ )
                {
                    if ( _foundBetter )
                    {
                        break;
                    }
                    vector<engine::Pair<int,int>> _moves = it->second;
                    // search in this neighborhood

                    for ( int q = 0; q < _moves.size(); q++ )
                    {
                        int _circle1 = _moves[q].first;
                        int _circle2 = _moves[q].second;

                        engine::LConfiguration* _sol = pConfiguration->clone();
                        _sol->swapCircles( _circle1, _circle2 );

                        // Continuously optimize the new solution
                        m_optimizer->run( _sol );

                        if ( _sol->isBetter( pConfiguration ) )
                        {
                            if ( std::abs( _sol->getContainer().r - _best_r ) > 0.5 )
                            {
                                _foundBetter = true;
                                *pConfiguration = *_sol;
                                _best_r = _sol->getContainer().r;
                                cout << "Intensifier> Better solution found: " << _best_r << endl;
                                break;
                            }

                        }
                        // TODO: Is there a leak here?
                        // delete _sol; ??
                    }

                }
                
                if ( !_foundBetter )
                {
                    break;
                }
                _iterCount++;
            }
            // Continuously optimize the new solution
            m_optimizer->run( pConfiguration );
            cout << "Intensifier> end" << endl;
        }


    };



}
