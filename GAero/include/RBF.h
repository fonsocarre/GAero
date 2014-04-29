//
//  RBF.h
//  GAero
//
//  Created by Alfonso Carre on 28/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#pragma once
#include <cmath>
#include <vector>


/* Standard euclidean distance */
inline double dist (std::valarray<double>& coord1,
                    std::valarray<double>& coord2,
                    double rho)
{
    // Vector size check omitted for speed
    //std::cout << coord1.size () << std::endl;
    //std::cout << coord2.size () << std::endl;
    double dist = 0.0;
    for (int i=0; i<coord1.size (); i++)
    {
        dist += (coord2[i] - coord1[i])*(coord2[i] - coord1[i]);
    }
    dist = sqrt(dist);
    return dist;
}

/* Wendland C2 */
static double wendlandC2 (std::valarray<double>& coord1,
                   std::valarray<double>& coord2,
                   double rho)
{
    double result = 0.0;
    double distance = dist(coord1, coord2, 1.);
    result = pow ((1. - distance/rho), 4);
    result *= 4.*distance + 1.;
    return result;
}