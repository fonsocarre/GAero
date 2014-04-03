//
//  GAfitnessSample.cpp
//  GAero
//
//  Created by Alfonso Carre on 02/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GAfitnessSample.h"

double GAfitnessSample::getFitness (
                    std::vector<double> genome)
{
    double fitness;
    double x = genome[0];
    double y = genome[1];
    double r;
    
    r = sqrt(x*x + y*y);
    fitness = sin(15*r)*exp(-5*r*r*r);
    
    //fitness = exp (-12*(x*x + y*y));
    
    populationSto::individual temp;
    temp.genome = genome;
    temp.fitness = fitness;
    this->population.push_back (temp);
    ++(this->iPopulation);
    
    return fitness;
}
