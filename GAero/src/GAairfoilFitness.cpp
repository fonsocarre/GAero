//
//  GAairfoilFitness.cpp
//  GAero
//
//  Created by Alfonso Carre on 21/03/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GAairfoilFitness.h"

void GAfitness::readSettings(const char* settingsFile)
{
    using namespace config4cpp;
    
    const char* scope="GAfitness";
    config4cpp::Configuration * cfg = config4cpp::Configuration::create();
    
    std::cout<<"Parsing GAfitness settings-----------------------------"<<std::endl;
    
    try
    {
        cfg->parse(settingsFile);
        
        this->mainCaseRoute     = cfg->lookupString(scope, "mainCaseRoute");
        this->wallPatchName     = cfg->lookupString(scope, "wallPatchName");

        cfg->destroy();
    }
    catch(const config4cpp::ConfigurationException & ex)
    {
        std::cout << ex.c_str() << std::endl;
    }
    
    std::cout<<"--GA Fitness Settings"  <<std::endl;
    std::cout<<"     OF case route = "  <<this->mainCaseRoute<<std::endl;
    std::cout<<"     Wall Patch name = "<<this->wallPatchName<<std::endl;
    std::cout<<" "                      <<std::endl;
    std::cout<<"Finished parsing settings"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;
}

GAfitness::GAfitness()
{
    this->settingsFile  = "unknown";
    this->mainCaseRoute = "unknown";
    this->wallPatchName = "unknown";
}