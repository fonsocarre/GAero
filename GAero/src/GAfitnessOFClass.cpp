//
//  GAfitnessOFClass.cpp
//  GAero
//
//  Created by Alfonso Carre on 03/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//
#include "GAfitnessOFClass.h"

void GAfitnessOFClass::getConfiguration
                    (const char* settingsFile)
{
    using namespace config4cpp;
    const char* scope = "GAeroFitness";
    
    Configuration* cfg = Configuration::create();
    std::cout << std::endl << std::endl;
    std::cout << "Parsing GAero fitness function"
              << "settings -----------------------"
              << std::endl;
    
    try
    {
        cfg->parse(settingsFile);
        
        this->mainCaseDir = cfg->lookupString
                                (scope, "mainCaseDir");
        this->tempCaseDir = cfg->lookupString
                                (scope, "tempCaseDir");
        this->initScript  = cfg->lookupString
                                (scope, "initScript");
        this->cleanScript = cfg->lookupString
                                (scope, "cleanScript");
        this->duplicateScript = cfg->lookupString
                                (scope, "duplicateScript");
        this->deleteScript= cfg->lookupString
                                (scope, "deleteScript");
        this->shapePatch  = cfg->lookupString
                                (scope, "shapePatch");
        this->RBFsetting  = cfg->lookupString
                                (scope, "RBF");
        this->maxIter     = cfg->lookupInt
                                (scope, "maxIter");
        this->maxThickness= cfg->lookupInt
                                (scope, "maxThickness");
        this->checkScript = cfg->lookupString
                                (scope, "checkScript");
        this->nThreads    = cfg->lookupInt
                                (scope, "nThreads");
        cfg->destroy();
    }
    catch (const ConfigurationException& ex)
    {
        std::cout << ex.c_str() << std::endl;
    }
}

void GAfitnessOFClass::initialise()
{
    std::string command;
    
    // OF main case run
    
//    std::cout << "Calling OF for main case..." << std::endl;
//    command = this->cleanScript + " " + this->mainCaseDir;
//    std::system (command.c_str ());
//    command = this->initScript + " " + this->mainCaseDir;
//    std::system (command.c_str ());
    
    
    // !!! changing profile parametrisation
    //this->profile = new NACA4digits;
    this->profile = new phiAirfoilClass;
    this->profile->maxThickness = this->maxThickness;
    
    {
        // new temporary mesh topology
        OFtopology* mesh = new OFtopology;
        
        // Mesh reading
        // checked
        this->readSETfile    (*mesh);
        this->readFacesFile  (*mesh);
        this->readPointsFile (*mesh);
        
        // Create topology
        this->createTopology (*mesh);
        
        // deallocate mesh topology
        delete mesh;
    }
    
    this->interpolationKernel.init(this->RBFsetting, 2.5*this->getRho());
    
    // s points to std::vector
    this->sPoints.resize (this->nPointsInPatch);
    for (int iPoint=0; iPoint<this->nPointsInPatch; iPoint++)
    {
        this->sPoints[iPoint].resize (constant::DIM);
        for (int iCoor=0; iCoor<constant::DIM; iCoor++)
        {
            this->sPoints[iPoint][iCoor] =
            this->points[this->pointsInPatch[iPoint]][iCoor];
        }
    }
}


void GAfitnessOFClass::getFitness
                        (const std::vector<double>& genome,
                         const int nThread,
                         double& fitness)
{
    //double fitness;
    std::string command;
    
    std::string caseDir;
    caseDir = this->tempCaseDir + std::to_string(nThread);
    
    std::vector<double> newZ (this->nPointsInPatch);
    
    std::vector<double> tempGenome {genome};
    
    //tempGenome[2] = genome[2] * static_cast<double> (this->maxThickness)/100.;
//    genome[0] = 0.8;
//    genome[1] = 0.2;
//    genome[2] = 0.01;
    //std::cout << genome[2] << std::endl;
    
    // Generates new OF case duplicating the main one
    command = this->duplicateScript
            + " " + this->mainCaseDir
            + " " + caseDir;
    std::system (command.c_str ());
    
    double xTemp;
    double zTemp;
    std::vector<double> deltaZ (this->nPointsInPatch);
    
    //***************************
//    std::ofstream oFile;
//    oFile.open ("hz.dat");
//    oFile << "#\"iPoint\"" << "  \t" << "\"hz\"" << std::endl;
//    oFile << "# iPoint empieza en 1" << std::endl;
//    oFile << "#" << this->NACA->genome2string(genome) << std::endl;
    // GET PROFILE POINTS
    for (int i = 0; i < this->nPointsInPatch; i++)
    {
        int iPoint = this->pointsInPatch[i];
        xTemp = this->points[iPoint][0];
        zTemp = this->points[iPoint][2];
        
        newZ[i] = this->profile->eval (tempGenome, xTemp, zTemp, 1.);
        deltaZ[i] = newZ[i] - zTemp;
        //oFile << iPoint+1 << "  \t" << deltaZ[i] << std::endl;
    }
    
    // MORPH MESH
    char RBF = '0';
    if (this->RBFsetting == "Spline") RBF = 'S';
    else if (this->RBFsetting == "wC2") RBF = 'W';
    
    std::vector<double> deltaZa;
    this->lock_mutex.lock();
    this->interpolationKernel.interpolate(deltaZ,
                                          this->sPoints,
                                          deltaZa,
                                          this->points,
                                          RBF);
    this->lock_mutex.unlock();
    
    
    // WRITE MESH
    this->writePointsFile(2, deltaZa, caseDir);
    
    // RUN CASE
    // OF temp case run
	this->lock_mutex.lock();
    //std::cout << "Calling checkMesh from " + std::to_string(nThread) << std::endl;
    if (!this->checkMesh(caseDir))
    {
        std::cout << this->profile->genome2string(tempGenome) << std::endl;
        std::cout << "    Mesh not good." << std::endl;
        fitness = 0.0;
        return;
    }
	this->lock_mutex.unlock();

    //std::cout << "Calling OF for temp case... t=" << nThread;
    //std::cout << this->profile->genome2string(tempGenome)
    //          << std::endl;
    command = this->cleanScript + " " + caseDir;
    //std::cout << "Calling clean from " + std::to_string(nThread) << std::endl;
    //std::system (command.c_str ());
    command = this->initScript + " " + caseDir;
    //std::cout << "Calling init from " + std::to_string(nThread) << std::endl;
    //std::system (command.c_str ());
    utilities::paralellExec(command.c_str ());
    std::vector<double> forceCoeffs;
    //std::cout << "Calling fCoeffs from " + std::to_string(nThread) << std::endl;
    this->getForceCoeffs(forceCoeffs, caseDir);
    
    // EXTRACT FITNESS
    fitness = forceCoeffs[3]/forceCoeffs[2]; // Cl / Cd
    //std::cout << "    fitness = " << fitness << std::endl;
    
    // Deletes temp case
    command = this->deleteScript
            + " " + caseDir;
    //std::cout << "Calling delete from " + std::to_string(nThread) << std::endl;
    std::system (command.c_str ());
    
    if (forceCoeffs[0] < maxIter)
    {
		//std::cout << "adding individual from " + std::to_string(nThread) << std::endl;
        this->addIndividual(tempGenome, fitness);
		//std::cout << "finished adding individual" << std::endl;
    }
    
    newZ.clear ();
    
    //std::cout << "---------------------------------------" << std::endl;
    
    return;
}


void GAfitnessOFClass::getForceCoeffs (std::vector<double>& forceCoeffs,
                                       std::string caseDir)
{
    std::string coeffsString;
    std::string command;
    
    command = caseDir
            + "/postProcessing/forceCoeffs/397/forceCoeffs.dat";
    command = "tail -1 " + command + " | head -1";
    
    coeffsString = utilities::exec (command.c_str ());
    
    forceCoeffs = fileUtils::parseForceCoeffsLine (coeffsString);
}

void GAfitnessOFClass::readSETfile (OFtopology& mesh)
{
    std::ifstream setFile;
    std::string buffer;
    
    fileUtils::openFile(setFile, this->mainCaseDir +
                                 "/constant/polyMesh/sets/" +
                        this->shapePatch);

    this->skipHeader (setFile);
    
    // get number of faces
    while (std::getline (setFile, buffer))
    {
        if (!charUtils::anyNumbers (buffer)) continue;
        mesh.nFacesInShape = std::stoi (buffer);
        break;
    }
    // allocates faces
    mesh.faces.resize (mesh.nFacesInShape);
    std::cout << "    " << mesh.nFacesInShape
              << " faces allocated"
              << std::endl;
    
    int counter = -1;
    while(std::getline (setFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (buffer.find ("(") != std::string::npos) continue;
        if (buffer == ")") break;
        counter++;
        mesh.faces[counter].iFace = std::stoi (buffer);
    }

    setFile.close ();
    
    std::sort (mesh.faces.begin (), mesh.faces.end ());
}

void GAfitnessOFClass::skipHeader (std::istream& file)
{
    std::string buffer;
    for (int i=0; i<constant::NHEADERLINES; i++)
    {
        std::getline (file, buffer);
    }
    
}

void GAfitnessOFClass::readFacesFile(OFtopology& mesh)
{
    std::string buffer;
    std::ifstream facesFile;
    
    fileUtils::openFile(facesFile, this->mainCaseDir +
                                   "/constant/polyMesh/faces");
    
    //facesFile.open(this->mainCaseDir + "/constant/polyMesh/faces");
    
    this->skipHeader (facesFile);
    while (std::getline (facesFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (!charUtils::anyNumbers (buffer)) continue;
        mesh.nFaces = std::stoi (buffer);
        break;
    }
    
    int counter = -1;
    int iFace = 0;
    while (std::getline(facesFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (buffer == "(") continue;
        
        counter++;
        if (counter == mesh.faces[iFace].iFace)
        {
            this->parseFaceLine (buffer, mesh.faces[iFace].points);
            iFace++;
            if (iFace == mesh.nFacesInShape) break;
        }
    }
    facesFile.close ();
}


void GAfitnessOFClass::parseFaceLine (std::string& buffer,
                                      std::vector<int>& connectivities)
{
    int firstLimit;
    firstLimit = static_cast<int>(buffer.find ("("));
    int nElems = std::atoi((buffer.substr (0,1)).c_str ());
    connectivities.resize (nElems);
    
    buffer = charUtils::trim (buffer.substr (firstLimit+1, buffer.size ()));
    
    int nextLimit;
    for (int i=0; i<nElems; i++)
    {
        nextLimit = static_cast<int>(buffer.find (" "));
        connectivities[i] = std::atoi (buffer.c_str ());
        buffer = charUtils::trim (buffer.substr (nextLimit+1, buffer.size ()));
    }
}

void GAfitnessOFClass::readPointsFile(OFtopology& mesh)
{
    std::string buffer;
    std::ifstream pointsFile;
    
    fileUtils::openFile(pointsFile, this->mainCaseDir +
                                    "/constant/polyMesh/points");
    this->readPointsHeader (pointsFile);
    
    while(std::getline (pointsFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (!charUtils::anyNumbers (buffer)) continue;
        mesh.nPoints = std::stoi (buffer);
        break;
    }
    
    // allocates points
    mesh.points.resize (mesh.nPoints);
    std::cout << "    " << mesh.nPoints
              << " points allocated"
              << std::endl;

    std::cout << "Reading points file, may take a while..." << std::endl;
    int counter = -1;
    while (std::getline(pointsFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (buffer == "(")   continue;

        counter++;
        if (counter > mesh.nPoints-1) break;
        parsePointsLine(buffer, mesh.points[counter].coords);
        mesh.points[counter].iPoint = counter;
    }
    pointsFile.close ();
}

void GAfitnessOFClass::readPointsHeader (std::istream& file)
{
    this->pointsHeader.resize (constant::NHEADERLINES);
    std::string buffer;
    for (int iLine=0; iLine < constant::NHEADERLINES; iLine++)
    {
        std::getline (file, buffer);
        this->pointsHeader[iLine] = buffer;
    }
}

void GAfitnessOFClass::parsePointsLine (std::string& buffer,
                                        std::vector<double>& coord)
{
    int limit;
    limit = static_cast<int>(buffer.find ("("));
    buffer = charUtils::trim (buffer.substr (limit+1, buffer.size ()));
    
    for (int iCoor=0; iCoor < constant::DIM; iCoor++)
    {
        limit = static_cast<int>(buffer.find (" "));
        
        coord.push_back (std::stod (buffer.substr (0, limit)));
        buffer = charUtils::trim (buffer.substr (limit+1, buffer.size()));
    }
}


void GAfitnessOFClass::createTopology (OFtopology& mesh)
{
    std::set<int> pointsInPatch;
    
    std::cout << "Generating topology..." << std::endl;
    
    for (int iFace = 0; iFace < mesh.nFacesInShape; iFace++)
    {
        for (int iPoint=0; iPoint<mesh.faces[iFace].points.size (); iPoint++)
        {
            pointsInPatch.insert (mesh.faces[iFace].points[iPoint]);
        }
    }
    std::cout << "    Found " << pointsInPatch.size ()
              << " points in patch "
              << this->shapePatch
              << std::endl;
    
    this->nPointsInPatch = static_cast<int> (pointsInPatch.size ());
    
    std::set<int>::iterator iter = pointsInPatch.begin();
    
    // Point of shape to valarray pointsInPatch
    this->pointsInPatch.resize (pointsInPatch.size ());
    for (int iPoint = 0; iPoint < pointsInPatch.size (); iPoint++, std::advance(iter, 1))
    {
        this->pointsInPatch[iPoint] = *iter;
    }
    
    // deallocate set of points
    pointsInPatch.clear ();
    
    this->xyzmin.resize (constant::DIM);
    this->xyzmax.resize (constant::DIM);
    
    for (int iCoor = 0; iCoor<constant::DIM; iCoor++)
    {
        this->xyzmin[iCoor] =  1e10;
        this->xyzmax[iCoor] = -1e10;
    }
    
    // Points of mesh to valarray points.
    this->nPoints = mesh.nPoints;
    this->points.resize (mesh.nPoints);
    for (int iPoint = 0; iPoint < this->nPoints; iPoint++)
    {
        this->points[iPoint].resize (constant::DIM);
        for (int iCoor = 0; iCoor < constant::DIM; iCoor++)
        {
            this->points[iPoint][iCoor] = mesh.points[iPoint].coords[iCoor];
            if (this->points[iPoint][iCoor] < this->xyzmin[iCoor])
            {
                this->xyzmin[iCoor] = this->points[iPoint][iCoor];
            }
            else if (this->points[iPoint][iCoor] > this->xyzmax[iCoor])
            {
                this->xyzmax[iCoor] = this->points[iPoint][iCoor];
            }
        }
    }
    std::cout << "    Bounding box:" << std::endl;
    std::cout << "       [";
    for (int iCoor = 0; iCoor<constant::DIM; iCoor++)
    {
        std::cout << this->xyzmin[iCoor];
        if (iCoor < constant::DIM-1) {std::cout << ", ";}
    }
    std::cout << "]" << std::endl;
    std::cout << "       [";
    for (int iCoor = 0; iCoor<constant::DIM; iCoor++)
    {
        std::cout << this->xyzmax[iCoor];
        if (iCoor < constant::DIM-1) {std::cout << ", ";}
    }
    std::cout << "]" << std::endl;
    
    std::cout << "  ... DONE" << std::endl;
}

void GAfitnessOFClass::writeTempPoints (OFtopology& mesh)
{
    std::ofstream file;
    file.open(this->mainCaseDir + "/../../Output/temp.dat");
    
    for (int iFace=0; iFace<mesh.nFacesInShape; iFace++)
    {
        for (int iPoint=0; iPoint<mesh.faces[iFace].points.size (); iPoint++)
        {
            for (int iCoor=0; iCoor<constant::DIM; iCoor++)
            {
                file << mesh.points[mesh.faces[iFace].points[iPoint]].coords[iCoor] << "    ";
            }
            file << std::endl;
        }
    }
}

void GAfitnessOFClass::writePointsFile
                           (int iDim,
                            std::vector<double>& delta,
                            std::string caseDir)
{
    std::ofstream pointsFile;
    
    std::mutex lock;
    std::lock_guard<std::mutex> l(lock);
    
    fileUtils::openFile(pointsFile, caseDir +
                                    "/constant" +
                                    "/polyMesh/points");

    // Write header
    for (int iLine=0; iLine<constant::NHEADERLINES; iLine++)
    {
        pointsFile << this->pointsHeader[iLine] << std::endl;
    }
    
    pointsFile << std::endl;
    pointsFile << std::endl;
    
    pointsFile << this->nPoints << std::endl;
    pointsFile << "(" << std::endl;
    
    pointsFile.precision(constant::PRECISION);
    
    for (int iPoint=0; iPoint<this->nPoints; iPoint++)
    {
        pointsFile << "(";
        for (int iCoor=0; iCoor<constant::DIM; iCoor++)
        {
            if (iCoor == iDim)
            {
                pointsFile << this->points[iPoint][iCoor] + delta[iPoint]
                           << " ";
            }
            else
            {
                pointsFile << this->points[iPoint][iCoor]
                           << " ";
            }
        }
        pointsFile << ")" << std::endl;
    }
    pointsFile << ")" << std::endl;
    
    pointsFile.close ();
}

void GAfitnessOFClass::addIndividual (std::vector<double>& genome,
                    double fitness)
{
    
    std::lock_guard<std::mutex> lock(this->lock_mutex);
    populationSto::individual temp;
    temp.genome = genome;
    temp.fitness = fitness;
    
    individualsOut << fitness << ", ";
    individualsOut << this->printCoordinates(genome);
    individualsOut << "\n";
    
    this->population.push_back (temp);
    ++(this->nPopulation);
}

double GAfitnessOFClass::getRho ()
{
    double rho = 0.0;
    
    for (int iCoor=0; iCoor<constant::DIM; iCoor++)
    {
        rho += (this->xyzmax[iCoor] - this->xyzmin[iCoor])*
               (this->xyzmax[iCoor] - this->xyzmin[iCoor]);
    }
    rho = sqrt(rho);
    return rho;
}


bool GAfitnessOFClass::checkMesh (std::string caseDir)
{
    std::string result;
    std::string command;
    
    command = this->checkScript + " " + caseDir;
    result = utilities::execSys(command.c_str ());
	//std::cout << "checkMesh from: " << caseDir << std::endl;
    if (result.find("Mesh OK") != std::string::npos) return true;
    
    return false;
}





