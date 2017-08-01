
#pragma once


#include "LCircle.h"

#include <vector>
#include <iostream>

#ifndef TEST_MAT_LIB
#include <armadillo>
#endif

#include <cmath>

using namespace std; 

#define RANGE_U_FEASIBLE 1

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

#ifndef TEST_MAT_LIB

    double potentialFunction( arma::mat pConf, arma::mat pRes );
    double containerPotentialFunction( arma::mat pConf, arma::mat pRes );
	double testFunction( arma::mat pConf, arma::mat pRes );

#else

    double potentialFunction( engine::mat::LMatD pConf, engine::mat::LMatD pRes );
    double containerPotentialFunction( engine::mat::LMatD pConf, engine::mat::LMatD pRes );
    double testFunction( engine::mat::LMatD pConf, engine::mat::LMatD pRes );

#endif


	class LConfiguration
	{

	private :

		double m_feasibility;
        bool m_isFeasible;
        double m_containerFeasibility;
        bool m_isContainerFeasible;
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

        void updateFeasibility( double val )
        {
            m_feasibility = val;
            if ( m_feasibility <= RANGE_U_FEASIBLE )
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
        #ifndef TEST_MAT_LIB
            arma::mat _x = arma::zeros<arma::mat>( 2 * this->size + 1, 1 );
            // A vector for the extra resources needed ( radius )
            arma::mat _r = arma::zeros<arma::mat>( this->size, 1 );
        #else
            engine::mat::LMatD _x = engine::mat::LMat<double>::simple( 2 * this->size + 1, 1,
                                                               engine::mat::fill::ZEROS );
            engine::mat::LMatD _r = engine::mat::LMat<double>::simple( this->size, 1,
                                                               engine::mat::fill::ZEROS );
        #endif
            _x( 0, 0 ) = ( this->getContainer() ).r;
            for ( int q = 0; q < this->size; q++ )
            {
                _x( 2 * q + 1, 0 ) = ( this->getCircleByIndx( q ) ).pos.x;
                _x( 2 * q + 2, 0 ) = ( this->getCircleByIndx( q ) ).pos.y;
                _r( q, 0 ) = ( this->getCircleByIndx( q ) ).r;
            }

            updateFeasibility( potentialFunction( _x, _r ) );
        }

        bool isContainerFeasible()
        {
            return m_isContainerFeasible;
        }

        void updateContainerFeasibility( double val )
        {
            m_containerFeasibility = val;
            if ( m_containerFeasibility <= RANGE_U_FEASIBLE )
            {
                m_isContainerFeasible = true;
            }
            else
            {
                m_isContainerFeasible = false;
            }
        }

        void computeContainerFeasibility()
        {
        #ifndef TEST_MAT_LIB
            arma::mat _x = arma::zeros<arma::mat>( 2 * this->size + 1, 1 );
            // A vector for the extra resources needed ( radius )
            arma::mat _r = arma::zeros<arma::mat>( this->size, 1 );
        #else
            engine::mat::LMatD _x = engine::mat::LMat<double>::simple( 2 * this->size + 1, 1,
                                                               engine::mat::fill::ZEROS );
            engine::mat::LMatD _r = engine::mat::LMat<double>::simple( this->size, 1,
                                                               engine::mat::fill::ZEROS );
        #endif
            _x( 0, 0 ) = ( this->getContainer() ).r;
            for ( int q = 0; q < this->size; q++ )
            {
                _x( 2 * q + 1, 0 ) = ( this->getCircleByIndx( q ) ).pos.x;
                _x( 2 * q + 2, 0 ) = ( this->getCircleByIndx( q ) ).pos.y;
                _r( q, 0 ) = ( this->getCircleByIndx( q ) ).r;
            }

            updateContainerFeasibility( containerPotentialFunction( _x, _r ) );
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

        LConfiguration( const LConfiguration& other )
        {
            this->m_feasibility = other.m_feasibility;
            this->m_isFeasible  = other.m_isFeasible;
            this->m_circles     = other.m_circles;
            this->m_bfCircles   = other.m_bfCircles;
            this->m_container   = other.m_container;
            this->size          = other.size;
        }

        LConfiguration& operator=( const LConfiguration& other )
        {
            this->m_feasibility = other.m_feasibility;
            this->m_isFeasible  = other.m_isFeasible;
            this->m_circles     = other.m_circles;
            this->m_bfCircles   = other.m_bfCircles;
            this->m_container   = other.m_container;
            this->size          = other.size;

            return *this;
        }

		void swapCircles( int pIndx1, int pIndx2 )
		{
            LPoint _tmpPos = m_circles[pIndx1].pos;
            m_circles[pIndx1].pos = m_circles[pIndx2].pos;
            m_circles[pIndx2].pos = _tmpPos;
		}


        LConfiguration* clone()
        {
            LConfiguration* _clone = new LConfiguration( *this );

            return _clone;
        }

        bool isBetter( LConfiguration* other )
        {
            // if ( other->isFeasible() && this->isFeasible() )
            // {
                return this->m_container.r < other->m_container.r;
            // }
            // else if ( ( other->isFeasible() && this->feasibility() < RANGE_U_FEASIBLE ) ||
            //           ( this->isFeasible() && other->feasibility() < RANGE_U_FEASIBLE ) )
            // {
            //     return this->m_container.r < other->m_container.r;
            // }
            // else if ( other->isFeasible() )
            // {
            //     return false;
            // }
            // else if ( this->isFeasible() )
            // {
            //     return true;
            // }
            
            // return this->feasibility() < other->feasibility();
        }

        static LConfiguration* initializeFromInstance( circleInstance::_circleInstance pCircleInstance, int pSize = 7 )
        {
            LConfiguration* _conf = new LConfiguration();
            _conf->size = pSize;

            double _containerRadius = 0.0;

            for ( int q = 0; q < pSize; q++ )
            {
                LCircle _circle( 0, 0, 1 );

                switch ( pCircleInstance )
                {

                    case circleInstance::INST_r_i :
                        _circle.r = ( q + 1 );
                    break;

                    case circleInstance::INST_r_sqrt_i :
                        _circle.r = sqrt( q + 1 );
                    break;

                    case circleInstance::INST_r_neg_sqrt_i :
                        _circle.r = pSize / sqrt( q + 1 );
                    break;
                }

                _containerRadius +=_circle.r * _circle.r;
                _circle.id = ( q + 1 );
                _conf->addCircle( _circle );
            }

            _containerRadius = 2 * sqrt( _containerRadius );
            LCircle _cContainer( 0, 0, _containerRadius );
            _cContainer.inInitialization = true;
            _cContainer.isDynamic = false;
            _cContainer.vx = 0;
            _cContainer.vy = 0;
            _conf->setContainer( _cContainer );

            for ( int q = 0; q < pSize; q++ )
            {
                LCircle& _circle = _conf->getCircleByIndx( q );

                _circle.inInitialization = true;
                _circle.isDynamic = true;
                _circle.vx = RANDOM() * 0.1;
                _circle.vy = RANDOM() * 0.1;
                double _r = RANDOM() * _containerRadius - _circle.r;
                double _t = RANDOM() * 2 * M_PI;
                _circle.pos.x = _r * cos( _t );
                _circle.pos.y = _r * sin( _t );
            }

            return _conf;
        }

	};

}
