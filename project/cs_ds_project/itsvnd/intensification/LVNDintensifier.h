


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
            pConfiguration->computeFeasibility();
            while ( true )
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
                        for ( int s = 0; s < OPTIMIZER_ITERATIONS; s++ )
                        {
                            m_optimizer->run( _sol );
                        }

                        if ( _sol->isBetter( pConfiguration ) )
                        {
                            _foundBetter = true;
                            *pConfiguration = *_sol;
                            break;
                        }
                        // TODO: Is there a leak here?
                        // delete _sol; ??
                    }

                }
                
                if ( !_foundBetter )
                {
                    break;
                }
            }
            // Continuously optimize the new solution
            for ( int s = 0; s < OPTIMIZER_ITERATIONS; s++ )
            {
                m_optimizer->run( pConfiguration );
            }
            std::cout << "finished step" << std::endl;
        }


    };



}
