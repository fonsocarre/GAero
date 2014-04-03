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
#include <iostream>
#include <cstdlib>
#include "GAfitnessClass.h"
#include "Configuration.h"


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
    
    //! Name of the script in mainCaseDir that lauchs OF.
    std::string initScript;
    
    //! Name of the script that cleans cases.
    std::string cleanScript;
    
    //! Name of the optimised patch in the mesh.
    std::string shapePatch;

public:
    //! Similar to GAsettingsClass constructor.
    /** Specific for this class.
      * Settings are:
      *     - mainCaseDir
      *     - tempCaseDir
      *     - initScript
      *     - cleanScript
      *     - shapePatch */
    void getConfiguration (const char* settingsFile);
    //! Calls shell script for running the basic OF case.
    /** Also must read OF mesh and create topology */
    void initialise ();
    //! Creates temp case, runs and parse output.
    double getFitness (std::vector<double> genome);
};

#endif
