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
    std::cout << "Calling OF for main case..." << std::endl;
    command = this->cleanScript + " " + this->mainCaseDir;
    std::system (command.c_str ());
    command = this->initScript + " " + this->mainCaseDir;
    std::system (command.c_str ());
    
    this->NACA = new NACA4digits;
    
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
}


double GAfitnessOFClass::getFitness
                        (std::vector<double> genome)
{
    double fitness;
    std::string command;
    
    std::vector<double> newZ (this->nPointsInPatch);
    
    // Generates new OF case duplicating the main one
    command = this->duplicateScript
            + " " + this->mainCaseDir
            + " " + this->tempCaseDir;
    std::system (command.c_str ());
    
    double xTemp;
    double zTemp;
    std::vector<double> deltaZ (this->nPointsInPatch);
    genome[0] = 0.24;
    genome[1] = 0.18;
    // GET PROFILE POINTS
    //std::ofstream tempFile;
    //tempFile.open("/Users/fonso/C++/GAero/GAero/tempFIle.txt");
    for (int i = 0; i < this->nPointsInPatch; i++)
    {
        int iPoint = this->pointsInPatch[i];
        xTemp = this->points[0][iPoint];
        zTemp = this->points[2][iPoint];
        
        newZ[i] = this->NACA->eval (genome, xTemp, zTemp, 1.);
        deltaZ[i] = newZ[i] - zTemp;
        //tempFile << xTemp << " " << newZ[i] << std::endl;;
    }
    
    // MORPH MESH
    
    // WRITE MESH
    
    // RUN CASE
    std::vector<double> temp1;
    this->getForceCoeffs(temp1);
    
    // EXTRACT FITNESS
    
    // Deletes temp case
    command = this->duplicateScript
            + " " + this->tempCaseDir;
    std::system (command.c_str ());
    
    fitness = genome[0] + genome[1];
    
    this->addIndividual(genome, fitness);
    
    newZ.clear ();
    //tempFile.close ();
    
    return fitness;
}


void GAfitnessOFClass::getForceCoeffs (std::vector<double>& forceCoeffs)
{
    std::string coeffsString;
    std::string command;
    
    command = this->tempCaseDir
            + "/postProcessing/forceCoeffs/0/forceCoeffs.dat";
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
    
    // Points of mesh to valarray points.
    this->nPoints = mesh.nPoints;
    this->points.resize (constant::DIM);
    for (int iCoor = 0; iCoor < constant::DIM; iCoor++)
    {
        this->points[iCoor].resize (mesh.nPoints);
        for (int iPoint = 0; iPoint < mesh.nPoints; iPoint++)
        {
             this->points[iCoor][iPoint] = mesh.points[iPoint].coords[iCoor];
        }
    }
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

void GAfitnessOFClass::writePointsFile ()
{
    std::ofstream pointsFile;
    
    fileUtils::openFile(pointsFile, this->tempCaseDir +
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
            pointsFile << this->points[iCoor][iPoint]
                       << " ";
        }
        pointsFile << ")" << std::endl;
    }
    pointsFile << ")" << std::endl;
    
    pointsFile.close ();
}

void GAfitnessOFClass::addIndividual (std::vector<double>& genome,
                    double fitness)
{
    populationSto::individual temp;
    temp.genome = genome;
    temp.fitness = fitness;
    
    this->population.push_back (temp);
    ++(this->nPopulation);
}








