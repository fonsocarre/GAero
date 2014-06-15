//
//  GAclass.h
//  GAlgorithm
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//
#pragma once
//! Preprocessor guard
#ifndef __GAclass__
#define __GAclass__

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <cmath>

#include "GApopulation.h"
#include "GAsettingsClass.h"
#include "GArandomGenerator.h"
#include "GAoutputclass.h"
#include "utilities.h"
#include "IOfile.h"
#include "GAfitnessOFClass.h"

//! The main Genetic Algorithm class.
/*! This class implements all the necessary variables and functions
    needed for this GA. */
class GAclass {
    // class attributes
    //! Number of individuals per generation.
    int nPopulation;
    //! Temporary vector of previous generation population.
    std::vector<GApopulation> oldPopulation;
    std::vector<GApopulation> population;
    GAsettingsClass GAsettings;
    std::vector<double> avgFitness;
    std::vector<double> maxFitness;
    std::vector<double> minFitness;
    int usedPopulation;
    
    // private class members
    /** Outputs to file and screen the info about the
        population. Only works in fitness-ordered sets of pop.*/
    void getPopFitness();
    //! Calculates the fitness of all the NEW population.
    void calculatePopFitness();
    //! Calculates the fitness of the oldPopulation.
    /** Only for initPop() method. */
    void calculateOldPopFitness();
    /** Step of evolve() method. Copies the elitists individuals to the
        next generation. */
    void evolveElitists();
    /** Step of evolve(), crosses the individuals based on roulette
        selection. */
    void crossIndividuals();
    /** Last step of evolve(). Fills the empty population
        with new random inidividuals. */
    void createNewIndividuals();

	void mutate();

    std::vector<double> oldPopFitness2vec();
    
    double fitnessVariation();
    
     /*void fitnessWrapper (std::vector<double> genome,
                         const int iThread,
                         double& fitness); */

	void fitnessWrapper (std::vector<int> popVec,
						 bool isOldPop,
						 int iThread);

    double mutationRate();

	std::vector<std::vector<int>> threadDistribution(const int nPop, const int nThreads);

public:
    // Constructors
    //! Basic constructor.
    //! Initialises all the fields to 0 or empty vectors.
    //! Random generator is initialised with date-based seed.
    GAclass();
    //! Constructor for GAclass. Input nPop is used
    //! to initialise the population vector reserving memory.
    GAclass(int nPop);
    //! Principal constructor.
    //! Using the config4cpp for reading the settings file
    //! and initialise the vector allocating the necessary
    //! resources.
    GAclass(const char* settingsFile);
    //! Another basic constructor.
    GAclass(int nPop, int genomeSize);
    //! Destructor. Deallocates the vector variables.
    ~GAclass();
    
    //! Standard OpenFOAM fitness function.
    //GAfitnessSample fitnessSample;
    GAfitnessOFClass fitnessOF;
    
    // public attributes
    
    //! Random number generator. Uses the randomc library
    //! for improving the uniformity of random number.
    GArandom randomGen;
    //! Ostream for output to file (specified in the settings file)
    //! and screen.
    GAoutputclass GAout;
    //! Number of max generations in the algorithm.
    int nGenerations;
    //! Counter for generations.
    int iGeneration;
    
    // Member functions
    
    //! GA basic routine. Applies the GA steps for each generation
    //! when finished, a new generation is created and the old one
    //! is copied to GAclass::oldPopulation.
    void evolve ();
    //! Generates a random initial population.
    void initPop ();
    //! Checks convergence based on settings criteria.
    bool checkConvergence ();
    
    //double FitnessFunction(std::vector<double> genome);
    GAfitnessClass* fitness;
    
};


#endif /* defined(__GAclass__) */
