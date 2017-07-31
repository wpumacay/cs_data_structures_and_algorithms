


#pragma once

#include "LBaseDiversifier.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>


using namespace std;

namespace diversifiers
{


    class LRRdiversifier : public LBaseDiversifier
    {

    private :
        int m_perturbationStrength;

    public :

        LRRdiversifier()
        {
            m_perturbationStrength = 1;
        }

        ~LRRdiversifier()
        {
        }

        void run( engine::LConfiguration* pConfiguration )
        {
            m_perturbationStrength = rand() % ( pConfiguration->size / 6 ) + 1;

            vector<int> _opts;
            for ( int q = 0; q < pConfiguration->size; q++ )
            {
                _opts.push_back( q );
            }
            std::random_shuffle( _opts.begin(), _opts.end() );

            vector<int> _choices;

            for ( int q = 0; q < m_perturbationStrength; q++ )
            {
                _choices.push_back( _opts[q] );
            }

            // Realocate randomly all circles

            for( int q = 0; q < _choices.size(); q++ )
            {
                int _indx = _choices[q];

                engine::LCircle& _circle = pConfiguration->getCircleByIndx( _indx );
                double _containerRadius = pConfiguration->getContainer().r;
                double _r = RANDOM() * _containerRadius - _circle.r;
                double _t = RANDOM() * 2 * M_PI;

                _circle.pos.x = _r * cos( _t );
                _circle.pos.y = _r * sin( _t );
            }
        }


    };



}
