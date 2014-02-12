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
#include <iostream>

#include "GApopulation.h"
#include "GAsettingsClass.h"
#include "GArandomGenerator.h"

class GAclass {
    // class attributes
    int nPopulation;
    std::vector<GApopulation> population;
    double rMutation;
    double rCrossover;
    GAsettingsClass GAsettings;
    std::vector<double> avgFitness;
    std::vector<double> maxFitness;

public:
    // Constructors
    GAclass();
    GAclass(int nPop);
    GAclass(const char* settingsFile);
    GAclass(int nPop, int genomeSize);
    ~GAclass();
    GArandom randomGen;
    int nGenerations;
    int iGeneration;
    
    // Member functions
    void evolve();
    void initPop();
    
    void (*pFitness)(std::vector<double> genome);
    
};

#endif /* defined(__GAclass__) */