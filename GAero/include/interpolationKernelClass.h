//
//  interpolationKernelClass.h
//  GAero
//
//  Created by Alfonso Carre on 28/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <valarray>
#include "RBF.h"

class interpolationKernelClass {
    
    double (*RBF) (std::vector<double>& coord1,
                   std::vector<double>& coord2,
                   double rho);
public:
    //! Basic constructor
    interpolationKernelClass () {};
    
    //! Initiates the class, pointing to the right RBF function.
    void init (std::string RBF);
    
    //! Main function for interpolation
    void interpolate (std::vector<double>& hs,
                      std::valarray< std::valarray<double> >& sCoor,
                      std::vector<double>& ha,
                      std::valarray< std::valarray<double> >& aCoor);
};
