//
//  GAclass.h
//  GAlgorithm
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef __GAclass__
#define __GAclass__

#include <string>
#include <vector>

#include "GApopulation.h"
#include "GAsettingsClass.h"

class GAclass {
    // class attributes
    int nPopulation;
    std::vector<GApopulation> population;
    int nGenerations;
    double rMutation;
    double rCrossover;
    GAsettingsClass GAsettings;

public:
    // Constructors
    GAclass();
    GAclass(int nPop);
    GAclass(const char* settingsFile);
    GAclass(int nPop, int genomeSize);
    ~GAclass();
    
    // Member functions
    void evolve();
    
    void (*pFitness)(std::vector<double> genome);
};

#endif /* defined(__GAclass__) */