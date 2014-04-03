//
//  GAfitnessOFClass.h
//  GAero
//
//  Created by Alfonso Carre on 03/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef GAero_GAfitnessOFClass_h
#define GAero_GAfitnessOFClass_h

#include <string>
#include "GAfitnessClass.h"


//! OpenFOAM interface with GAero
/** This class inherits from GAfitnessClass
    and overwrites its function initialise() and
    the virtual method getFitness. */
class GAfitnessOFClass: public GAfitnessClass {
    
    //! Directory of basic case.
    std::string mainCaseDir;
    
    //! Used directory for new calculations.
    /** This folder is emptied at every evaluation. */
    std::string tempCaseDir;

public:
    void initialise();
    double getFitness (std::vector<double> genome);
};

#endif
