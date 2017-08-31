

#include "LVNDintensifier.h"

using namespace std;

#ifdef USE_CUDA

#include "cuda_vnd.h"

extern bool computeVND( float& cRadius, 
                        CCircle* circles, int numCircles, 
                        CPair* pairs, int numPairs );

#endif



namespace intensifiers
{

        LVNDintensifier::LVNDintensifier( optimizers::LBaseOptimizer* pOptimizer )
        {
            m_optimizer = pOptimizer;
        }

        LVNDintensifier::~LVNDintensifier()
        {
            m_optimizer = nullptr;
        }

        void LVNDintensifier::run( engine::LConfiguration* pConfiguration )
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
                cout << "_iterCount: " << _iterCount << " / " << _maxIter << endl;

            #ifdef USE_CUDA

                SwapNeighborhoods::iterator _it;
                vector<engine::Pair<int,int>> _pairs;

                for ( _it = _sNeighborhoods.begin(); _it != _sNeighborhoods.end(); _it++ )
                {
                    vector<engine::Pair<int,int>> _moves = _it->second;
                    for ( int q = 0; q < _moves.size(); q++ )
                    {
                        engine::Pair<int,int> _pair;

                        _pair.first = _moves[q].first;
                        _pair.second = _moves[q].second;

                        _pairs.push_back( _pair );
                    }
                }

                // Prepare the data of the current configuration and the swaps to make

                int c_numPairs = _pairs.size();
                CPair* c_pairs = new CPair[c_numPairs];
                for ( int q = 0; q < _pairs.size(); q++ )
                {
                    c_pairs[q].c1 = _pairs[q].first;
                    c_pairs[q].c2 = _pairs[q].second;
                }

                float c_cRadius = pConfiguration->getContainer().r;
                vector<engine::LCircle> _circles = pConfiguration->circles();
                int c_numCircles = _circles.size();
                CCircle* c_circles = new CCircle[c_numCircles];
                for ( int q = 0; q < _circles.size(); q++ )
                {
                    c_circles[q].r = _circles[q].r;
                    c_circles[q].x = _circles[q].pos.x;
                    c_circles[q].y = _circles[q].pos.y;
                }

                // Launch the kernels to compute the best by calling the helper function
                // Also, extract the data and get the best one, and compare it with the current one

                cout << "started parallel intensifier *****" << endl;
                _foundBetter = computeVND( c_cRadius, 
                                           c_circles, c_numCircles,
                                           c_pairs, c_numPairs );
                cout << "finished parallel intensifier ****" << endl;

                // Retrieve the results back into the configuration

                if ( _foundBetter )
                {
                    pConfiguration->getContainer().r = c_cRadius;

                    for ( int q = 0; q < c_numCircles; q++ )
                    {
                        pConfiguration->getCircleByIndx( q ).r     = c_circles[q].r;
                        pConfiguration->getCircleByIndx( q ).pos.x = c_circles[q].x;
                        pConfiguration->getCircleByIndx( q ).pos.y = c_circles[q].y;
                    }
                }

                // clean up
                delete[] c_pairs;
                delete[] c_circles;

            #else

                for ( it = _sNeighborhoods.begin(); it != _sNeighborhoods.end(); it++ )
                {
                    if ( _foundBetter )
                    {
                        break;
                    }
                    vector<engine::Pair<int,int>> _moves = it->second;
                    // search in this neighborhood

                    // cout << "moves.size() > " << _moves.size() << endl;

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
                
            #endif

                if ( !_foundBetter )
                {
                    break;
                }

                _iterCount++;
            
            }


            // Continuously optimize the new solution
            m_optimizer->run( pConfiguration );


            InsertNeighborhood _iNeighborhood = neighborhood::insert::makeInsertNeighborhood( pConfiguration, m_optimizer );

            engine::LConfiguration* _sol = pConfiguration->clone();                                                                                          

            for ( int q = 0; q < _iNeighborhood.size(); q++ )
            {
                engine::LPoint& _pt = _iNeighborhood[q].first;

                _sol->getCircleByIndx( q ).pos.x = _pt.x;
                _sol->getCircleByIndx( q ).pos.y = _pt.y;
            }

            m_optimizer->run( _sol );

            if ( _sol->isBetter( pConfiguration ) )
            {
                *pConfiguration = *_sol;
                _best_r = _sol->getContainer().r;
                cout << "Intensifier> Better solution found by insert: " << _best_r << endl;
            }


            cout << "Intensifier> end" << endl;
        }

}
