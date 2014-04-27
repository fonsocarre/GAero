//
//  NACA4digits.h
//  GAero
//
//  Created by Alfonso Carre on 08/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#pragma once

#include <vector>
#include <cmath>
#include "airfoil.h"
#include "constants.h"

//! Specific class for generating 4 dig. NACA coordinates
/** It inherits from NACA class, in order to enhance
    modularity. */
class NACA4digits: public airfoil {
    
    std::vector<double> coeffVector;
    std::vector<double> eval (const std::vector<double>& genome,
                               double xCoor,
                               double chordLength = 1.);
    
    double NACAthick (double xCoor,
                      double genome,
                      double chordLength = 1.);
    
    double NACAcamber (double xCoor,
                       double m,
                       double p,
                       double chordLength = 1.);
    
    double NACAdiffY (double xCoor,
                      double m,
                      double p,
                      double chordLength = 1.);

public:
    //! Gives a value to the coefficients.
    NACA4digits ();
    
    //! Basic return of coordinates given the genome and the x coordinate.
    /** Genome must contain:
        - genome[0] = 2 first numbers of NACA. i.e. 00 for NACA 0012
        - genome[1] = 2 last numbers. i.e. 12 for NACA 0012.
        It assumes chord length = 1 unit */
    std::vector<double> operator()
                      (const std::vector<double>& genome,
                       const double xCoor);
};
