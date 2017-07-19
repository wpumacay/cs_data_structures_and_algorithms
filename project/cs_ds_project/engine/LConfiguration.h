
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
			m_bfCircles.push_back( LCircle( c ) );
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


	};

}
