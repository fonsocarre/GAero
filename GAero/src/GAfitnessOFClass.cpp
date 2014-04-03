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
    command = this->mainCaseDir+ "/" + this->initScript +
                " " + this->mainCaseDir;
    std::cout << "Calling OF for main case..." << std::endl;
    std::system(command.c_str());
}

// TEMPORARY
double GAfitnessOFClass::getFitness
                        (std::vector<double> genome)
{
    return genome[0]*genome[1];
}