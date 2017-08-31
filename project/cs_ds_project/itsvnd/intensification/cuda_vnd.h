
#pragma once

#include <cuda.h>
#include <cuda_runtime.h>

struct CPair
{
    int c1;
    int c2;
};

struct CCircle
{
    float x;
    float y;
    float r;
    bool isUpdatable;

    CCircle()
    {
        x = 0;
        y = 0;
        isUpdatable = true;
    }
};

bool computeVND( float& cRadius, 
                 CCircle* circles, int numCircles, 
                 CPair* pairs, int numPairs );


class CVector
{

    private :

    float* m_buff;
    int m_dim;

    public :

    __host__ __device__ CVector()
    {
        m_dim = 0;
        m_buff = NULL;
    }

    __host__ __device__ CVector( int dim )
    {
        m_dim = dim;
        m_buff = ( float* ) malloc( sizeof( float ) * m_dim );
        for ( int q = 0; q < m_dim; q++ )
        {
            m_buff[q] = 0.0f;
        }
    }

    __host__ __device__ CVector( const CVector& vOther )
    {
        m_dim = vOther.m_dim;

        m_buff = ( float* ) malloc( sizeof( float ) * m_dim );
        for ( int q = 0; q < m_dim; q++ )
        {
            m_buff[q] = vOther.m_buff[q];
        }
    }

    __host__ __device__ ~CVector()
    {
        if ( m_buff != NULL )
        {
            free( m_buff );
        }
        m_buff = NULL;
    }

    __host__ __device__ CVector operator= ( const CVector& vOther )
    {
        if ( m_buff )
        {
            free( m_buff );
            m_buff = NULL;
        }
        
        m_dim = vOther.m_dim;

        m_buff = ( float* ) malloc( sizeof( float ) * m_dim );
        for ( int q = 0; q < m_dim; q++ )
        {
            m_buff[q] = vOther.m_buff[q];
        }
       
        return *this;
    }

    __host__ __device__ void set( int indx, float value )
    {
        m_buff[indx] = value;
    }

    __host__ __device__ int dim()
    {
        return m_dim;
    }

    __host__ __device__ float& operator[]( int indx )
    {
        return m_buff[indx];
    }

    __host__ __device__ CVector operator+ ( const CVector& other )
    {
        CVector _res( m_dim );
        
        if ( other.m_dim != m_dim )
        {
            return _res;
        }

        for ( int q = 0; q < m_dim; q++ )
        {
            _res[q] = m_buff[q] + other.m_buff[q];
        }

        return _res;
    }

    __host__ __device__ CVector operator- ( const CVector& other )
    {
        CVector _res( m_dim );
        
        if ( other.m_dim != m_dim )
        {
            return _res;
        }

        for ( int q = 0; q < m_dim; q++ )
        {
            _res[q] = m_buff[q] - other.m_buff[q];
        }

        return _res;
    }

    __host__ __device__ CVector operator* ( float scale )
    {
        for ( int q = 0; q < m_dim; q++ )
        {
            m_buff[q] *= scale;
        }

        return *this;
    }
};


