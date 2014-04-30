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
#include "Algebra.h"

class interpolationKernelClass {
    
    //! Pointer to RBF function, these can be found in RBF.h
    double (*RBF) (std::valarray<double>& coord1,
                   std::valarray<double>& coord2,
                   double rho);
    
    //! Flag for creating G only on the first call
    bool firstCall;
    
    //! Rho to be used in the RBF
    double rho_;
    
    //! G storage
    std::valarray<double> G_;
    
    // ***********   METHODS   *******
    
    //!
    std::valarray<double> getCssInv
            (std::valarray< std::valarray<double> >& sCoor);
    
    std::valarray<double> getAasRow
                (std::valarray<std::valarray<double>>& sCoor,
                 std::valarray<std::valarray<double>>& aCoor,
                 int nRow);
    
    
public:
    //! Basic constructor
    interpolationKernelClass () {};
    
    //! Initiates the class, pointing to the right RBF function.
    void init (std::string RBF, double rho);
    
    //! Main function for interpolation
    void interpolate (std::vector<double>& hs,
                      std::valarray< std::valarray<double> >& sCoor,
                      std::vector<double>& ha,
                      std::valarray< std::valarray<double> >& aCoor);
};
