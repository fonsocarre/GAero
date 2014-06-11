//
//  phiAirfoil.h
//  GAero
//
//  Created by Alfonso Carre on 20/05/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#pragma once
#include <vector>
#include <string>
#include <sstream>

#include "airfoil.h"
#include "spline.h"
#include "constants.h"


class phiAirfoilClass: public airfoil
{
    std::vector<double> xlim;
    std::vector<double> zlim;
    
    std::vector<double> genome2coord
                (const std::vector<double>& genome);
    double x2phi (double x);
    
public:
    //! Sets the max and min values of genome. They will be mapped.
    phiAirfoilClass();
    
    double eval (const std::vector<double>& genome,
                 double& xCoor,
                 double& yPrevCoord,
                 double chordLength);
    
    std::string genome2string
                (const std::vector<double>& genome);

	std::string genome2csv
				(const std::vector<double>& genome);
    
};
