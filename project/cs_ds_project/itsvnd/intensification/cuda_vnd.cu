
#include "cuda_vnd.h"

#include <stdio.h>

#define K_OPTIMIZER_ITERATIONS 100
#define K_GRAD_STEP 0.1f
#define K_DELTA_STEP 0.01f
#define K_RANGE_U_FEASIBLE 1
#define K_R_V 0.25

__device__ void k_swap_circles( int c1, int c2, int wIndxOff, CCircle* cCirclesExt )
{
    float _x = cCirclesExt[wIndxOff + c1].x;
    float _y = cCirclesExt[wIndxOff + c1].y;

    cCirclesExt[wIndxOff + c1].x = cCirclesExt[wIndxOff + c2].x;
    cCirclesExt[wIndxOff + c1].y = cCirclesExt[wIndxOff + c2].y;

    cCirclesExt[wIndxOff + c2].x = _x;
    cCirclesExt[wIndxOff + c2].y = _y;
}

__device__ float k_potential( CVector &xConf,
                              CVector &rConf )
{
    float _res = 0.0f;

    float _rContainer = xConf[0];
    // Overlap with the container
    for ( int q = 0; q < rConf.dim(); q++ )
    {
        float _xi = xConf[ 2 * q + 1 ];
        float _yi = xConf[ 2 * q + 2 ];
        float _ri = rConf[q];
        float _dist = sqrtf( _xi * _xi + _yi * _yi );
        float _cost_q = fmaxf( _dist + _ri - _rContainer, 0.0f );

        _res += _cost_q * _cost_q;
    }
    // Overlap between circles
    for ( int q = 0; q < rConf.dim(); q++ )
    {
        float _xi = xConf[ 2 * q + 1 ];
        float _yi = xConf[ 2 * q + 2 ];
        float _ri = rConf[q];

        for ( int p = q + 1; p < rConf.dim(); p++ )
        {
            float _xj = xConf[ 2 * p + 1 ];
            float _yj = xConf[ 2 * p + 2 ];
            float _rj = rConf[p];

            float _dist = sqrtf( ( _xi - _xj ) * ( _xi - _xj ) + 
                                 ( _yi - _yj ) * ( _yi - _yj ) );

            float _cost_pq = fmaxf( _ri + _rj - _dist, 0.0f );

            _res += _cost_pq * _cost_pq;
        }
    }

    return _res;
}

__device__ CVector k_computeGradient( CVector &xConf,
                                      CVector &rConf )
{
    CVector _grad( xConf.dim() );
    float _step = K_DELTA_STEP;

    float _f = k_potential( xConf, rConf );

    for ( int q = 0; q < xConf.dim(); q++ )
    {
        CVector _xc = xConf;
        _xc[q] += _step;
        float _f_next = k_potential( _xc, rConf );
        _grad[q] = ( _f_next - _f ) / _step;
    }

    return _grad;
}

__device__ float k_container_potential( CVector &xConf,
                                        CVector &rConf )
{
    float _res = 0.0f;

    float _rContainer = xConf[0];
    // Overlap with the container
    for ( int q = 0; q < rConf.dim(); q++ )
    {
        float _xi = xConf[ 2 * q + 1 ];
        float _yi = xConf[ 2 * q + 2 ];
        float _ri = rConf[q];
        float _dist = sqrtf( _xi * _xi + _yi * _yi );
        float _cost_q = fmaxf( _dist + _ri - _rContainer, 0.0f );

        _res += _cost_q * _cost_q;
    }

    return _res;
}

__device__ float k_computeContainerFeasibility( int wIndxOff, CCircle* cCirclesExt, float cRadius, int cNumCircles )
{
    CVector _xConf( 1 + 2 * cNumCircles );
    CVector _rConf( cNumCircles );

    _xConf[0] = cRadius;
    for ( int q = 0; q < cNumCircles; q++ )
    {
        _xConf[ 1 + 2 * q ] = cCirclesExt[wIndxOff + q].x;
        _xConf[ 2 + 2 * q ] = cCirclesExt[wIndxOff + q].y;
        _rConf[q] = cCirclesExt[wIndxOff + q].r;
    }

    return k_container_potential( _xConf, _rConf );
}

__device__ void k_optimize( int wIndxOff, CCircle* cCirclesExt, float& cRadius, int cNumCircles )
{

    for ( int s = 0; s < K_OPTIMIZER_ITERATIONS; s++ )
    {
        CVector _xConf( 1 + 2 * cNumCircles );
        CVector _rConf( cNumCircles );

        _xConf[0] = cRadius;
        for ( int q = 0; q < cNumCircles; q++ )
        {
            _xConf[ 1 + 2 * q ] = cCirclesExt[wIndxOff + q].x;
            _xConf[ 2 + 2 * q ] = cCirclesExt[wIndxOff + q].y;
            _rConf[q] = cCirclesExt[wIndxOff + q].r;
        }
        // Perform a step
        CVector _grad = k_computeGradient( _xConf, _rConf );
        _xConf = _xConf - _grad * K_GRAD_STEP;

        cRadius = _xConf[0];

        for ( int q = 0; q < cNumCircles; q++ )
        {
            cCirclesExt[wIndxOff + q].x = _xConf[ 1 + 2 * q ];
            cCirclesExt[wIndxOff + q].y = _xConf[ 2 + 2 * q ];
        }
    }
}

__global__ void kernel_compute_best_vnd( float cRadius,
                                         CCircle* cCircles, int cNumCircles,
                                         CPair* cPairs, int cNumPairs,
                                         CCircle* cCirclesExt,
                                         int* d_bests, float* d_bestsRadius )
{
    int tIndx = threadIdx.x;
    if ( tIndx < 0 || tIndx >= cNumPairs )
    {
        return;
    }

    int c1 = cPairs[tIndx].c1;
    int c2 = cPairs[tIndx].c2;

    // Make the swap 
    int wIndxOff =  cNumCircles * tIndx;
    k_swap_circles( c1, c2, wIndxOff, cCirclesExt );

    float containerRadius = cRadius;

    // Intensify this new configuration *************************************************
    k_optimize( wIndxOff, cCirclesExt, containerRadius, cNumCircles );

    // Check if touches container too much
    float _cFeasibility = k_computeContainerFeasibility( wIndxOff, cCirclesExt, cRadius, cNumCircles );

    // **********************************************************************************

    if ( _cFeasibility < K_RANGE_U_FEASIBLE )
    {
        // binary search
        
        int k = containerRadius / K_R_V;
        int _upBound = k - 1;
        int _loBound = 0;
        float _sum;
        for ( int q = 0; q < cNumCircles; q++ )
        {
            float _frac = cCirclesExt[wIndxOff + q].r / K_R_V;
            _sum += _frac * _frac;
        }
        _loBound = floorf( sqrtf( _sum ) );

        do
        {
            float _contFeasib = k_computeContainerFeasibility( wIndxOff, cCirclesExt, cRadius, cNumCircles );
            if ( _contFeasib < K_RANGE_U_FEASIBLE )
            {
                _upBound = k - 1;
            }
            else
            {
                _loBound = k;
            }
            k = ( _loBound + _upBound ) / 2;
            containerRadius = k * K_R_V;
            float _copyContRadius = containerRadius;
            k_optimize( wIndxOff, cCirclesExt, _copyContRadius, cNumCircles );

            if ( _upBound - _loBound <= 1 && k == _loBound )
            {
                break;
            }

        }while( _upBound > _loBound );
    }

    // Check if is better than the current solution

    float _best_r = cRadius;

    if ( containerRadius < _best_r )
    {
        if ( fabsf( containerRadius - _best_r ) > 0.5 )
        {
            d_bests[tIndx] = tIndx;
            d_bestsRadius[tIndx] = containerRadius;
        }
    }
}


bool computeVND( float& cRadius, 
                 CCircle* circles, int numCircles, 
                 CPair* pairs, int numPairs )
{

    // Reserve space for the current configuration and  ****
    // possible swap moves
    CCircle* d_circles;
    CPair* d_pairs;

    cudaMalloc( ( void** ) &d_circles, sizeof( CCircle ) * numCircles );
    cudaMemcpy( d_circles, circles, 
                sizeof( CCircle ) * numCircles, 
                cudaMemcpyHostToDevice );

    cudaMalloc( ( void** ) &d_pairs, sizeof( CPair ) * numPairs );
    cudaMemcpy( d_pairs, pairs, 
                sizeof( CPair ) * numPairs, 
                cudaMemcpyHostToDevice );
    // *****************************************************

    // Create arrays to store the results ***********

    int* h_bests = new int[numPairs];
    for ( int q = 0; q < numPairs; q++ )
    {
        h_bests[q] = -1;
    }

    int* d_bests;
    cudaMalloc( ( void** ) &d_bests, sizeof( int ) * numPairs );
    cudaMemcpy( d_bests, h_bests, sizeof( int ) * numPairs, cudaMemcpyHostToDevice );

    float* h_bestsRadius = new float[numPairs];
    for ( int q = 0; q < numPairs; q++ )
    {
        h_bestsRadius[q] = 0.0f;
    }

    float* d_bestsRadius;
    cudaMalloc( ( void** ) &d_bestsRadius, sizeof( float ) * numPairs );
    cudaMemcpy( d_bestsRadius, h_bestsRadius, sizeof( float ) * numPairs, cudaMemcpyHostToDevice );

    // **********************************************

    // Create the space neccessary for each thread to work in ****************

    CCircle* h_circlesExt = new CCircle[numCircles * numPairs];
    for ( int q = 0; q < numPairs; q++ )
    {
        for ( int p = 0; p < numCircles; p++ )
        {
            h_circlesExt[q * numCircles + p].r = circles[p].r;
            h_circlesExt[q * numCircles + p].x = circles[p].x;
            h_circlesExt[q * numCircles + p].y = circles[p].y;
        }
    }

    CCircle* d_circlesExt;
    cudaMalloc( ( void** ) &d_circlesExt, sizeof( CCircle ) * numCircles * numPairs );
    cudaMemcpy( d_circlesExt, h_circlesExt, sizeof( CCircle ) * numCircles * numPairs, cudaMemcpyHostToDevice );

    // ***********************************************************************
    
    kernel_compute_best_vnd<<<1, numPairs>>>( cRadius, 
                                              d_circles, numCircles, 
                                              d_pairs, numPairs, 
                                              d_circlesExt, d_bests, d_bestsRadius );
    
    // Retrieve the search results ********************

    cudaMemcpy( h_bests, d_bests, sizeof( int ) * numPairs, cudaMemcpyDeviceToHost );
    cudaMemcpy( h_bestsRadius, d_bestsRadius, sizeof( float ) * numPairs, cudaMemcpyDeviceToHost );
    cudaMemcpy( h_circlesExt, d_circlesExt, sizeof( CCircle ) * numCircles * numPairs, cudaMemcpyDeviceToHost );

    // ************************************************

    // for all search results that gave a found-better solution ...
    // get the best among them 

    int _bestIndx = -1;
    float _bestRadius = 1000000.0f;


    for ( int q = 0; q < numPairs; q++ )
    {
        if ( h_bests[q] == -1 )
        {
            continue;
        }

        if ( h_bestsRadius[q] < _bestRadius )
        {
            _bestIndx = q;
            _bestRadius = h_bestsRadius[q];
        }
    }

    bool foundBetter = false;

    if ( _bestIndx != -1 )
    {

        printf( "found better \n" );
        foundBetter = true;
        // If a better solution was found, use this to ...
        // update the circle configuration

        int wIndxOff = numCircles * _bestIndx;

        cRadius = _bestRadius;

        for ( int q = 0; q < numCircles; q++ )
        {
            
            circles[q].x = h_circlesExt[wIndxOff + q].x;
            circles[q].y = h_circlesExt[wIndxOff + q].y;
            circles[q].r = h_circlesExt[wIndxOff + q].r;
        }
    }

    cudaFree( d_circles );
    cudaFree( d_pairs );
    cudaFree( d_bests );
    cudaFree( d_bestsRadius );
    cudaFree( d_circlesExt );

    return foundBetter;
}
