//
//  utilities.h
//  GAero
//
//  Created by Alfonso Carre on 13/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//
//! Preprocessor guard
#ifndef GAero_utilities_h
#define GAero_utilities_h

namespace utilities
{
    inline void normalizeFitness(double& fit1, double&fit2)
    {
        double sum = fit1+fit2;
        fit1 /= sum;
        fit2 /= sum;
    }
}

#endif
