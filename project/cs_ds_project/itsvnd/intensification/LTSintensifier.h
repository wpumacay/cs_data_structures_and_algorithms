


#pragma once

#include "LBaseIntensifier.h"
#include <iostream>


using namespace std;

namespace intensifiers
{

    struct LTSMove
    {
        engine::Pair<int,int> pair;
        int tabuCount;
    };

    class LTSintensifier : public LBaseIntensifier
    {

    private :

        vector<LTSMove> m_tabuList;
        int m_tabuIterations;
        int m_tabuIterationsCounter;

    public :

        LTSintensifier( optimizers::LBaseOptimizer* pOptimizer )
        {
            m_optimizer = pOptimizer;
        }

        ~LTSintensifier()
        { 
            m_optimizer = nullptr;
        }

        void setTabuIterations( int pTabuIterations )
        {
            m_tabuIterations = pTabuIterations;
        }

        void addToTabuList( engine::Pair<int,int> pPair )
        {
            LTSMove _move;
            _move.pair = pPair;
            _move.tabuCount = 100;

            m_tabuList.push_back( _move );
        }

        bool isInTabuList( const engine::Pair<int,int> &pPair )
        {
            for ( int q = 0; q < m_tabuList.size(); q++ )
            {
                if ( m_tabuList[q].pair.first == pPair.first &&
                     m_tabuList[q].pair.second == pPair.second )
                {
                    return true;
                }
            }
            return false;
        }

        void startTabuIterations()
        {
            m_tabuIterationsCounter = m_tabuIterations;
        }

        void updateTabuIteration()
        {
            for ( int q = 0; q < m_tabuList.size(); q++ )
            {
                m_tabuList[q].tabuCount--;
                if ( m_tabuList[q].tabuCount <= 0 )
                {
                    m_tabuList.erase( m_tabuList.begin() + q );
                    q--;
                }
            }

            m_tabuIterationsCounter--;
        }

        bool tabuStopCondition()
        {
            return m_tabuIterationsCounter < 0;
        }

        void run( engine::LConfiguration* pConfiguration )
        {
            pConfiguration->computeFeasibility();

            SwapNeighborhoods _sNeighborhoods = neighborhood::swap::makeSwapNeighborhoods( pConfiguration );

            SwapNeighborhoods::iterator it;

            bool _foundBetter = false;

            engine::LConfiguration* _sCandidate = nullptr;
            engine::LConfiguration* _bestCandidate = nullptr;

            for ( it = _sNeighborhoods.begin(); it != _sNeighborhoods.end(); it++ )
            {
                
                vector<engine::Pair<int,int>> _moves = it->second;
                // search in this neighborhood 

                for ( int q = 0; q < _moves.size(); q++ )
                {
                    if ( isInTabuList( _moves[q] ) )
                    {
                        continue;
                    }

                    _sCandidate = pConfiguration->clone();
                    _sCandidate->swapCircles( _moves[q].first, _moves[q].second );
                    // Continuously optimize the new solution
                    m_optimizer->run( _sol );

                    if ( _bestCandidate == nullptr || _sCandidate->isBetter( _bestCandidate ) )
                    {
                        _bestCandidate = _sCandidate;
                    }

                    addToTabuList( _moves[q] );
                    updateTabuIteration();
                }
                if ( _bestCandidate != nullptr && pConfiguration->isBetter( _bestCandidate ) )
                {
                    *pConfiguration = *_bestCandidate;
                    break;
                }

                if ( tabuStopCondition() )
                {
                    break;
                }

                // TODO: Is there a leak here?
                // delete _sCandidate,_bestCandidate; ??
            }
        }


    };



}
