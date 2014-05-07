//
//  airfoil.h
//  GAero
//
//  Created by Alfonso Carre on 08/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#pragma once
#include <vector>

class airfoil {

public:
    //virtual std::vector<double>
    //    operator() (const std::vector<double>& genome,
    //                double xCoord) = 0;
    
    virtual double
            eval (const std::vector<double>& genome,
                  double& xCoor,
                  double& yPrevCoord,
                  double chordLength) = 0;
    
    virtual std::string genome2string
            (const std::vector<double>& genome) = 0;
};