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
#include <fstream>
#include <cstdlib>
#include "GAfitnessClass.h"
#include "Configuration.h"
#include "OFtopology.h"
#include "constants.h"
#include "charUtilities.h"


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
    
    //! Route to the script in mainCaseDir that lauchs OF.
    std::string initScript;
    
    //! Route to the script that cleans cases.
    std::string cleanScript;
    
    //! Route to the script for duplicating cases.
    std::string duplicateScript;
    
    //! Route to the script for deleting complete cases.
    std::string deleteScript;
    
    //! Name of the optimised patch in the mesh.
    std::string shapePatch;
    
    //! Mesh data structure
    OFtopology mesh;
    
    //! Strings containing points file header
    std::vector<std::string> pointsHeader;
    
    // **********************METHODS*********************
    
    //! Method for reading SET file.
    void readSETfile ();
    
    //! Method for reading faces file.
    /** Special care: faces must be ordered!!! */
    void readFacesFile ();
    //! Parses faces lines
    void parseFaceLine (std::string& buffer,
                        std::vector<int>& connectivities);
    
    //! Method for reading points.
    void readPointsFile ();
    void parsePointsLine (std::string& buffer,
                          std::vector<double>& coord);

    
    //! Method for skipping header when reading OF files.
    void skipHeader (std::istream& file);
    
    //! Method for reading and storing points file header.
    void readPointsHeader (std::istream& file);

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
