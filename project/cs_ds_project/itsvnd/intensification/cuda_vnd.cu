
extern "C" 
{
    
    #include "cuda_vnd.h"

}


__global__ void kernel_vnd_test( int c1, int c2 )
{
    c1++;
    c2++;
    printf( "? %d \n", ( c1 + c2 ) );
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
        cudaMalloc( &d_pairs, sizeof( CPair ) * numPairs );
        cudaMemcpy( d_circles, circles, 
                    sizeof( CCircle ) * numCircles, 
                    cudaMemcpyHostToDevice );
        cudaMemcpy( d_pairs, pairs, 
                    sizeof( CPair ) * numPairs, 
                    cudaMemcpyHostToDevice );

        kernel_vnd_test<<<1, numPairs>>>( 1, 2 );
    }
}