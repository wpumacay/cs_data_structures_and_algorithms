
#include "cuda_vnd.h"

#include <stdio.h>

__global__ void kernel_vnd_test( int c1, int c2 )
{
    c1++;
    c2++;
    printf( "? %d \n", ( c1 + c2 ) );
}

__global__ void kernel_compute_best_vnd( float cRadius,
                                         CCircle* cCircles, int cNumCircles,
                                         CPair* cPairs, int cNumPairs,
                                         CCircle* cCirclesExt )
{
    int tIndx = threadIdx.x;
    if ( tIndx < 0 || tIndx >= cNumPairs )
    {
        return;
    }

    int c1 = cPairs[tIndx].c1;
    int c2 = cPairs[tIndx].c2;

    // Make the swap 
    int windxOff =  sizeof( CCircle ) * cNumCircles * tIndx;
    swap_circles( c1, c2, windxOff, cCirclesExt );

    // Intensify this new configuration

}

__device__ void swap_circles( int c1, int c2, int windxOff, CCircle* cCirclesExt )
{
    float _x = cCirclesExt[windxOff + c1].x;
    float _y = cCirclesExt[windxOff + c1].y;

    cCirclesExt[windxOff + c1].x = cCirclesExt[windxOff + c2].x;
    cCirclesExt[windxOff + c1].y = cCirclesExt[windxOff + c2].y;

    cCirclesExt[windxOff + c2].x = _x;
    cCirclesExt[windxOff + c2].y = _y;
}

__device__ void optimize( int windxOff, CCircle* cCirclesExt )
{
    
}

extern "C" 
{


    void computeVND( float cRadius, 
                     CCircle* circles, int numCircles, 
                     CPair* pairs, int numPairs,
                     int *h_bestIndx )
    {
        CCircle* d_circles;
        CPair* d_pairs;

        cudaMalloc( &d_circles, sizeof( CCircle ) * numCircles );
        cudaMemcpy( d_circles, circles, 
                    sizeof( CCircle ) * numCircles, 
                    cudaMemcpyHostToDevice );

        cudaMalloc( &d_pairs, sizeof( CPair ) * numPairs );
        cudaMemcpy( d_pairs, pairs, 
                    sizeof( CPair ) * numPairs, 
                    cudaMemcpyHostToDevice );

        printf( "starting kernel\n" );

        // Create the space neccessary for each thread to work in
        CCircle* d_circlesExt;
        cudaMalloc( &d_circlesExt, sizeof( CCircle ) * numCircles * numPairs );
        for ( int q = 0; q < numPairs; q++ )
        {
            cudaMemcpy( d_circlesExt + sizeof( CCircle ) * numCircles * q , circles, sizeof( CCircle ) * numCircles, cudaMemcpyHostToDevice );
        }

        //kernel_vnd_test<<<1, numPairs>>>( 1, 2 );
        kernel_compute_best_vnd<<<1, numPairs>>>( cRadius, d_circles, numCircles, d_pairs, numPairs, d_circlesExt );
    }
}