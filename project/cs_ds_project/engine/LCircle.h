
#pragma once

#include "LCommon.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

#define RANDOM() ( rand() / ( float )RAND_MAX )


namespace engine
{

	class LCircle
	{

		public :

		double r;
		LPoint pos;
		bool hit;
		int id;

        LCircle()
        {
            this->r = 1;
            this->pos.x = 0;
            this->pos.y = 0;

            isDynamic = true;
            hit = false;
        }

		LCircle( double x, double y, double r )
		{
			this->r = r;
			this->pos.x = x;
			this->pos.y = y;

			this->isDynamic = true;
			this->inInitialization = false;
			this->vx = 0;
			this->vy = 0;
			hit = false;
		}

		LCircle( const LCircle& other )
		{
			this->r = other.r;
			this->pos.x = other.pos.x;
			this->pos.y = other.pos.y;

            this->isDynamic = other.isDynamic;
            hit = other.hit;
            this->id = other.id;
		}

		LCircle& operator=( const LCircle& other )
		{
			if ( &other == this )
			{
				return *this;
			}

			this->r = other.r;
			this->pos.x = other.pos.x;
			this->pos.y = other.pos.y;

			this->isDynamic = other.isDynamic;
			this->hit = other.hit;
			this->id = other.id;

			return *this;
		}

		// For the physics based initialization ********************************
		double vx;
		double vy;
		bool isDynamic;
		bool inInitialization;

		void onCollision( LCircle& other )
		{
			double dx = other.pos.x - this->pos.x;
			double dy = other.pos.y - this->pos.y;
			double d = sqrt( dx * dx + dy * dy );

			double _ud_x = dx / d;
			double _ud_y = dy / d;

			if ( other.isDynamic )
			{
				double _delta = 0.5 * ( this->r + other.r - d );
				// update positions for just contact
				this->pos.x -= _delta * _ud_x;
				this->pos.y -= _delta * _ud_y;

				other.pos.x += _delta * _ud_x;
				other.pos.y += _delta * _ud_y;

				// v onto n,t
				// v dot ud
				double _vn_1 = other.vx * _ud_x + other.vy * _ud_y;
				double _vt_1 = other.vx * ( _ud_y ) + other.vy * ( -_ud_x );
				double _vn_2 = this->vx * _ud_x + this->vy * _ud_y;
				double _vt_2 = this->vx * ( _ud_y ) + this->vy * ( -_ud_x );
				// Apply change
				_vn_1 = -_vn_1;
				_vn_2 = -_vn_2;
				// v = vn * ud + vt * udp
				other.vx = _vn_1 * ( _ud_x ) + _vt_1 * ( _ud_y );
				other.vy = _vn_1 * ( _ud_y ) + _vt_1 * ( -_ud_x );

				this->vx = _vn_2 * ( _ud_x ) + _vt_2 * ( _ud_y );
				this->vy = _vn_2 * ( _ud_y ) + _vt_2 * ( -_ud_x );
			}
			else
			{
				double _delta = d - ( other.r - this->r );

				this->pos.x -= _delta * _ud_x;
				this->pos.y -= _delta * _ud_y;

				double _vn = this->vx * _ud_x + this->vy * _ud_y;
				double _vt = this->vx * ( _ud_y ) + this->vy * ( -_ud_x );

				_vn = -_vn;

				this->vx = _vn * ( _ud_x ) + _vt * ( _ud_y );
				this->vy = _vn * ( _ud_y ) + _vt * ( -_ud_x );

                if ( d > this->r + other.r )
                {
                    this->vx = RANDOM() * 0.1;
                    this->vy = RANDOM() * 0.1;
                    double _r = RANDOM() * other.r - this->r;
                    double _t = RANDOM() * 2 * M_PI;
                    this->pos.x = _r * cos( _t );
                    this->pos.y = _r * sin( _t );
                }
			}
		}

		bool hitTest( const LCircle& other )
		{
			double dx = other.pos.x - this->pos.x;
			double dy = other.pos.y - this->pos.y;
			double d = sqrt( dx * dx + dy * dy );

			if ( other.isDynamic )
			{
				return d < other.r + this->r;
			}

			return d > other.r - this->r ;
		}

		void onTickPhysics( double dt )
		{
            if ( isDynamic )
            {
                this->pos.x += this->vx * dt;
                this->pos.y += this->vy * dt;
            }
            else
            {
                this->pos.x = 0;
                this->pos.y = 0;
            }
		}
		//************************************************************************

	};


}









