//
//  constants.h
//  GAero
//
//  Created by Alfonso Carre on 02/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef GAero_constants_h
#define GAero_constants_h

namespace constant {
    //! Epsilon definition for convergence check.
    const double EPSILON = 1e-4;
    
    //! Epsilon definition for profile coordinates
    const double EPSILON1 = 1e-5;
    
    //! Number of lines in OpenFOAM header
    const int NHEADERLINES = 16;
    
    //! Dimension of the problem.
    const int DIM = 3;
    
    //! Output default precision
    const int PRECISION = 8;
}



#endif