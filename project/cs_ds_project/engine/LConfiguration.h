
#pragma once

#include <vector>
#include <iostream>
#include <armadillo>
#include <cmath>


#include "LCircle.h"

using namespace std; 

namespace engine
{

	namespace circleInstance
	{
		enum _circleInstance
		{
			INST_r_i,
			INST_r_sqrt_i,
			INST_r_neg_sqrt_i
		};
	}


    double potentialFunction( arma::mat pConf, arma::mat pRes );
	double testFunction( arma::mat pConf, arma::mat pRes );


	class LConfiguration
	{

	private :

		double m_feasibility;
		bool m_isFeasible;
		vector<LCircle> m_circles;
		vector<LCircle> m_bfCircles;
		LCircle m_container;

	public :

		int size;

		double feasibility()
		{
			return m_feasibility;
		}

        void updateFeasibility( double val )
        {
            m_feasibility = val;
            if ( m_feasibility <= 0.0 )
            {
                m_isFeasible = true;
            }
            else
            {
                m_isFeasible = false;
            }
        }

        void computeFeasibility()
        {
            arma::mat _x = arma::zeros<arma::mat>( 2 * this->size + 1, 1 );
            // A vector for the extra resources needed ( radius )
            arma::mat _r = arma::zeros<arma::mat>( this->size, 1 );

            _x( 0, 0 ) = ( this->getContainer() ).r;
            for ( int q = 0; q < this->size; q++ )
            {
                _x( 2 * q + 1, 0 ) = ( this->getCircleByIndx( q ) ).pos.x;
                _x( 2 * q + 2, 0 ) = ( this->getCircleByIndx( q ) ).pos.y;
                _r( q, 0 ) = ( this->getCircleByIndx( q ) ).r;
            }

            updateFeasibility( potentialFunction( _x, _r ) );
        }

		bool isFeasible()
		{
			return m_isFeasible;
		}

		LConfiguration()
		{
			m_feasibility = 0.0;
			m_isFeasible = true;
		}

		void addCircle( const LCircle& c )
		{
			m_circles.push_back( LCircle( c ) );
			( m_circles.back() ).id = m_circles.size();
			m_bfCircles.push_back( LCircle( c ) );
			( m_bfCircles.back() ).id = m_bfCircles.size();
		}

		LCircle& getCircleByIndx( int indx )
		{
			return m_circles[indx];
		}

		vector<LCircle> circles()
		{
			return m_circles;
		}

		void setContainer( LCircle c )
		{
			m_container = c;
		}

		LCircle& getContainer()
		{
			return m_container;
		}

		LConfiguration( const vector<LCircle> &circles )
		{
			m_circles = circles;
			this->size = circles.size();
		}

		void swapCircles( int pIndx1, int pIndx2 )
		{
			LCircle _tmpCircle = m_circles[pIndx1];
			m_circles[pIndx1] = m_circles[pIndx2];
			m_circles[pIndx2] = _tmpCircle;
		}


	};

}
