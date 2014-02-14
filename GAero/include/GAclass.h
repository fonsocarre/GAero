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
#include <algorithm>

#include "GApopulation.h"
#include "GAsettingsClass.h"
#include "GArandomGenerator.h"
#include "GAoutputclass.h"
#include "utilities.h"


class GAclass {
    // class attributes
    int nPopulation;
    std::vector<GApopulation> oldPopulation;
    std::vector<GApopulation> population;
    GAsettingsClass GAsettings;
    std::vector<double> avgFitness;
    std::vector<double> maxFitness;
    std::vector<double> minFitness;
    int usedPopulation;
    
    // private class members
    void getPopFitness();
    void calculatePopFitness();
    void calculateOldPopFitness();
    void evolveElitists();
    void crossIndividuals();
    void createNewIndividuals();
    std::vector<double> oldPopFitness2vec();
    
    

public:
    // Constructors
    GAclass();
    GAclass(int nPop);
    GAclass(const char* settingsFile);
    GAclass(int nPop, int genomeSize);
    ~GAclass();
    
    // public attributes
    GArandom randomGen;
    int nGenerations;
    int iGeneration;
    
    // Member functions
    void evolve();
    void initPop();
    
    double FitnessFunction(std::vector<double> genome);
    
};


#endif /* defined(__GAclass__) */