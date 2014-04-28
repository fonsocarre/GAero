//
//  GAfitnessClass.h
//  GAero
//
//  Created by Alfonso Carre on 02/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//
#pragma once
//! Preprocessor guard
#ifndef GAero_GAfitnessClass_h
#define GAero_GAfitnessClass_h

#include <vector>
#include "populationStorage.h"

//! Abstract Class for fitness function.
/** getFitness is virtual so every child of this 
    function will have to code it */
class GAfitnessClass {
protected:
    //! Population storage
    std::vector<populationSto::individual> population;
    
public:
    //! Virtual method for reading settings from file.
    virtual void getConfiguration (const char* settingsFile) = 0;
    //! Counter of population stored
    int nPopulation = 0;
    //! Virtual initiate function.
    virtual void initialise() = 0;
    //! Virtual method for getFitness.
    virtual double getFitness (std::vector<double> genome) = 0;
    //! Empty constructor
    GAfitnessClass () { };
};

#endif