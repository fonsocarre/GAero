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
        this->deleteScript = cfg->lookupString
                                (scope, "deleteScript");
        this->shapePatch  = cfg->lookupString
                                (scope, "shapePatch");
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
    std::system (command.c_str());
    command = this->initScript + " " + this->mainCaseDir;
    std::system (command.c_str());
    
    // Mesh reading
    this->readSETfile();
    this->readFacesFile();
    this->readPointsFile();
    
}

// TEMPORARY
double GAfitnessOFClass::getFitness
                        (std::vector<double> genome)
{
    //todo ANADIR LISTA DE INDIVIDUALS.
    return genome[0]*genome[1];
}

void GAfitnessOFClass::readSETfile ()
{
    std::ifstream setFile;
    std::string buffer;
    setFile.open (this->mainCaseDir +
                  "/constant/polyMesh/sets/" +
                  this->shapePatch);
    if (setFile.is_open ())
    {
        std::cout << "SET file successfully open" << std::endl;
    }
    else
    {
        std::cerr << "Error opening set file" << std::endl;
    }

    this->skipHeader (setFile);
    
    // get number of faces
    while(std::getline (setFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (!charUtils::anyNumbers (buffer)) continue;
        this->mesh.nFacesInShape = std::stoi (buffer);
        break;
    }
    // allocates faces
    this->mesh.faces.resize (this->mesh.nFacesInShape);
    std::cout << "    " << this->mesh.nFacesInShape
              << " faces allocated"
              << std::endl;
    
    int counter = -1;
    while(std::getline (setFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (buffer.find ("(") != std::string::npos) continue;
        if (buffer.find (")") != std::string::npos) break;
        counter++;
        this->mesh.faces[counter].iFace = std::stoi (buffer);
    }

    setFile.close();
    
    std::sort (this->mesh.faces.begin(), this->mesh.faces.end());
}

void GAfitnessOFClass::skipHeader (std::istream& file)
{
    std::string buffer;
    for (int i=0; i<constant::NHEADERLINES; i++)
    {
        std::getline (file, buffer);
    }
    
}

void GAfitnessOFClass::readFacesFile()
{
    std::string buffer;
    std::ifstream facesFile;
    
    facesFile.open(this->mainCaseDir + "/constant/polyMesh/faces");
    
    this->skipHeader (facesFile);
    while(std::getline (facesFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (!charUtils::anyNumbers (buffer)) continue;
        this->mesh.nFaces = std::stoi (buffer);
        break;
    }
    
    std::vector<int> temp;
    int counter = -1;
    int iFace = 0;
    while (std::getline(facesFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (buffer[0] == '(') continue;
        
        counter++;
        if (counter == this->mesh.faces[iFace].iFace)
        {
            this->parseFaceLine(buffer, temp);
            iFace++;
            if (iFace >= this->mesh.nFacesInShape) break;
        }
    }
    
}


void GAfitnessOFClass::parseFaceLine(std::string& buffer,
                                     std::vector<int>& connectivities)
{
    int firstLimit;
    firstLimit = (int)(buffer.find("("));
    int nElems = std::atoi((buffer.substr(0,1)).c_str());
    connectivities.resize(nElems);
    
    buffer = charUtils::trim (buffer.substr (firstLimit+1, buffer.size()));
    
    int nextLimit;
    for (int i=0; i<nElems; i++)
    {
        nextLimit = (int)(buffer.find (" "));
        connectivities[i] = std::atoi (buffer.c_str ());
        buffer = charUtils::trim (buffer.substr (nextLimit+1, buffer.size()));
    }
}

void GAfitnessOFClass::readPointsFile()
{
    std::string buffer;
    std::ifstream pointsFile;
    
    pointsFile.open (this->mainCaseDir + "/constant/polyMesh/points");
    this->readPointsHeader (pointsFile);
    
    while(std::getline (pointsFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (!charUtils::anyNumbers (buffer)) continue;
        this->mesh.nPoints = std::stoi (buffer);
        break;
    }
    
    // allocates points
    this->mesh.points.resize (this->mesh.nPoints);
    std::cout << "    " << this->mesh.nPoints
              << " points allocated"
              << std::endl;

    std::cout << "Reading points file, may take a while..." << std::endl;
    int counter = -1;
    while (std::getline(pointsFile, buffer))
    {
        if (buffer.empty ()) continue;
        if (buffer == "(")   continue;

        counter++;
        if (counter > this->mesh.nPoints-1) break;
        this->parsePointsLine(buffer, this->mesh.points[counter].coords);
        this->mesh.points[counter].iPoint = counter;
    }
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
    limit = (int)(buffer.find ("("));
    buffer = charUtils::trim (buffer.substr (limit+1, buffer.size ()));
    
    for (int iCoor=0; iCoor < constant::DIM; iCoor++)
    {
        limit = (int)(buffer.find(" "));
        
        coord.push_back (std::stod (buffer.substr (0, limit)));
        buffer = charUtils::trim (buffer.substr (limit+1, buffer.size()));
    }
}















