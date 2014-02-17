//
//  GAfitnessFunction.h
//  GAero
//
//  Created by Alfonso Carre on 13/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef GAero_fitnessFunction_h
#define GAero_fitnessFunction_h

#include <vector>
#include <cmath>

class GAfitnessClass {

public:
    double fitnessFunction(std::vector<double> genome)
    {
        //SUM OF THE GENES
//        double sum = 0.;
//        for (int i=0; i<genome.size(); i++)
//        {
//            sum += genome[i];
//        }
//        return sum;
        //STEEP BUT MONOTONE FUCNTION
//        double val;
//        val = -5. * (exp((genome[0]-0.5)*(genome[0]-0.5) +
//                         (genome[1]-0.5)*(genome[1]-0.5))) * sin(5*genome[0]*genome[1]);
//        return val;
        double r;
        double val;
        double x, y;
        x=genome[0];
        y=genome[1];
        
        r = sqrt(x*x + y*y);
        val = sin(15*r)*exp(-5*r*r*r);
        return val;
    }
};

#endif
