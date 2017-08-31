
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

#define RANDOM() ( rand() / ( float )RAND_MAX )

#define WX 100.0
#define WY 100.0
#define WV 2.0
#define N_PARTICLES 100
#define PI 3.1415926535

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


class CParticle
{

    public :

    CVector xy;
    CVector v;

    __host__ __device__ CParticle()
    {
        xy = CVector( 2 );
        v = CVector( 2 );
    }

    __host__ __device__ CParticle( float x, float y,
                                   float vx, float vy )
    {
        xy = CVector( 2 );
        xy[0] = x;
        xy[1] = y;

        v = CVector( 2 );
        v[0] = vx;
        v[1] = vy;
    }
    
    __host__ __device__ CParticle( const CParticle& pOther )
    {
        xy = pOther.xy;
        v  = pOther.v;
    }

    __host__ __device__ CParticle operator= ( const CParticle& pOther )
    {
        xy = pOther.xy;
        v  = pOther.v;

        return *this;
    }

    __host__ __device__ void step( float dt )
    {
        xy = xy + v * dt;
    }
    
};

__global__ void kernel_updateParticles( CParticle* cParticles, int cNumParticles, float dt )
{
    int tIndx = threadIdx.x;
    
    if ( tIndx >= cNumParticles )
    {
        return;
    }

    cParticles[tIndx].step( dt );
}


int main()
{

    CParticle* h_particles = new CParticle[N_PARTICLES];
    for ( int q = 0; q < N_PARTICLES; q++ )
    {
        h_particles[q].xy[0] = RANDOM() * WX;
        h_particles[q].xy[1] = RANDOM() * WY;

        float _ang = RANDOM() * 2 * PI;

        h_particles[q].v[0] = WV * cos( _ang );
        h_particles[q].v[1] = WV * sin( _ang );
    }

    CParticle* d_particles;
    cudaMalloc( ( void** ) &d_particles, sizeof( CParticle ) * N_PARTICLES );
    cudaMemcpy( d_particles, h_particles, sizeof( CParticle ) * N_PARTICLES, cudaMemcpyHostToDevice );


    // Just loop for now
    float T = 100.0;
    float dt = 0.01;
    int nIters = ( int ) ( T / dt );
    for ( int q = 0; q < nIters; q++ )
    {
        kernel_updateParticles<<<1,N_PARTICLES>>>( d_particles, N_PARTICLES, dt );
    }

    delete[] h_particles;
    cudaFree( d_particles );

    return 0;
}



