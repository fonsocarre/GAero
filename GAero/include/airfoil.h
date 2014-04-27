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
    virtual std::vector<double>
        operator() (const std::vector<double>& genome,
                    double xCoord) = 0;
};