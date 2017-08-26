

#pragma once

extern "C"
{

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
    };

    void computeVND( float cRadius, 
                     CCircle* circles, int numCircles, 
                     CPair* pairs, int numPairs,
                     int *h_bestIndx );

}