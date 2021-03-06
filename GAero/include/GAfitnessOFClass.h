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
#include <set>
#include <math.h>
#include <thread>
#include <mutex>

#include "GAfitnessClass.h"
#include "Configuration.h"
#include "OFtopology.h"
#include "constants.h"
#include "charUtilities.h"
#include "fileUtils.h"
#include "populationStorage.h"
#include "utilities.h"
#include "airfoil.h"
#include "NACA4digits.h"
#include "phiAirfoil.h"
#include "interpolationKernelClass.h"
#include "IOfile.h"


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
    
    std::string checkScript;
    
    std::string RBFsetting;
    
    //! Temporary mesh data structure
    //OFtopology mesh;
    
    //! Point coordinates
    /**  */
    std::valarray< std::valarray<double> > points;
    
    std::valarray< std::valarray<double> > sPoints;
    
    int maxIter;
    
    int maxThickness;
    
    int nThreads;
    
    //! Number of points
    int nPoints;
    
    //! Valarray of points in shape patch
    std::valarray<int> pointsInPatch;
    
    //! Number of points in patch
    int nPointsInPatch;
    
    //! Strings containing points file header
    std::vector<std::string> pointsHeader;
    
    //! Bounding box of mesh for rho calculation
    std::vector<double> xyzmin;
    std::vector<double> xyzmax;
    
    //! Polymorphic pointer to airfoil class for naca eqs.
    //! Memory dynamically allocated, check DELETE
    airfoil* profile;
    
    std::mutex lock_mutex;
    
    //! NACA class for eqs.
    //NACA4digits NACAeqs;
    
    interpolationKernelClass interpolationKernel;
    
    // **********************METHODS*********************
    
    //! Method for reading SET file.
    void readSETfile (OFtopology& mesh);
    
    //! Method for reading faces file.
    /** Special care: faces must be ordered!!! */
    void readFacesFile (OFtopology& mesh);
    //! Parses faces lines
    void parseFaceLine (std::string& buffer,
                        std::vector<int>& connectivities);
    
    //! Method for reading points.
    void readPointsFile (OFtopology& mesh);
    void parsePointsLine (std::string& buffer,
                          std::vector<double>& coord);

    
    //! Method for skipping header when reading OF files.
    void skipHeader (std::istream& file);
    
    //! Method for reading and storing points file header.
    void readPointsHeader (std::istream& file);
    
    //! Method for creating the topology
    void createTopology (OFtopology& mesh);
    
    //! Write points file in OF format
    void writePointsFile (int iDim, std::vector<double>& delta,
                          std::string caseDir);
    
    //! Temporary function for writing mesh points
    void writeTempPoints (OFtopology& mesh);
    
    //! Add individual to population
    void addIndividual (std::vector<double>& genome,
                        double fitness);
    
    //! gets ForceCoeffs from forceCoeffs.dat file
    void getForceCoeffs (std::vector<double>& forceCoeffs,
                         std::string caseDir);
    
    //! Calculates rho with bounding box
    double getRho ();
    
    //! Checks the OF mesh with checkMesh
    bool checkMesh (std::string caseDir);
    
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
    //! Creates temp case, runs and parses output.
    void getFitness (const std::vector<double>& genome,
                       const int nThread,
                       double& fitness);
    
    int getnThreads () {return this->nThreads;};

	std::string printCoordinates 
			(const std::vector<double>& genome)
			{
				return this->profile->genome2csv (genome);
			}
    
    GAfitnessOFClass () {};
    ~GAfitnessOFClass () {delete this->profile;};
};

#endif
