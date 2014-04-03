//
//  GAfitnessSample.h
//  GAero
//
//  Created by Alfonso Carre on 02/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//
#pragma once
//! Preprocessor guard
#ifndef GAero_GAfitnessSample_h
#define GAero_GAfitnessSample_h

#include "GAfitnessClass.h"
#include "populationStorage.h"
#include <cmath>

//! Inherited class from GAfitnessClass.
/** Implements a sample example fitness
    function.*/
class GAfitnessSample: public GAfitnessClass {
public:
    //! New non-virtual function for fitness calculation. Specific for 2D genomes.
    double getFitness (std::vector<double> genome);
    // Empty function, no need for configuration.
    void getConfiguration (const char* settingsFile)
                                        {/* EMPTY */};
    void initialise() {/* EMPTY */};
};

#endif
