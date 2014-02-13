//
//  GAsettingsClass.cpp
//  GAero
//
//  Created by Alfonso Carre on 09/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GAsettingsClass.h"


GAsettingsClass::GAsettingsClass()
{
    
}

GAsettingsClass::GAsettingsClass(const char* settingsFile)
{
    using namespace config4cpp;
    
    const char* scope="GAero";
    config4cpp::Configuration * cfg = config4cpp::Configuration::create();
    
    std::cout<<"Parsing GAero settings-----------------------------"<<std::endl;
    
    try
    {
        cfg->parse(settingsFile);
        
        this->logFile           = cfg->lookupString(scope, "output.logFile");
        this->historyFile       = cfg->lookupString(scope, "output.historyFile");
        this->plotHistory       = cfg->lookupBoolean(scope, "output.plotHistory");
        
        this->nGenerations      = cfg->lookupInt(scope, "nGenerations");
        this->populationSize    = cfg->lookupInt(scope, "populationSize");
        this->genomeSize        = cfg->lookupInt(scope, "genomeSize");
        this->mutationRate      = cfg->lookupFloat(scope, "mutationRate");
        this->nElitist          = cfg->lookupInt(scope, "nElitist");
        this->crossingRatio     = cfg->lookupFloat(scope, "crossingRatio");
        cfg->destroy();
    }
    catch(const config4cpp::ConfigurationException & ex)
    {
        std::cout << ex.c_str() << std::endl;
    }
    
    std::cout<<"--GA Settings"          <<std::endl;
    std::cout<<"     nGenerations = "   <<this->nGenerations<<std::endl;
    std::cout<<"     populationSize = " <<this->populationSize<<std::endl;
    std::cout<<"     genomeSize = "     <<this->genomeSize<<std::endl;
    std::cout<<"     mutationRate = "   <<this->mutationRate<<std::endl;
    std::cout<<"     nElitist = "       <<this->nElitist<<std::endl;
    std::cout<<"     crossingRatio = "  <<this->crossingRatio<<std::endl;
    std::cout<<" "                      <<std::endl;
    
    std::cout<<"--Output Settings"<<std::endl;
    std::cout<<"     logFile = "<<this->logFile<<std::endl;
    if (this->plotHistory)
    {
        std::cout<<"     plotHistory = true"<<std::endl;
        std::cout<<"     historyFile = "<<this->historyFile<<std::endl;
    }
    std::cout<<" "<<std::endl;

    std::cout<<"Finished parsing settings"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;
}











