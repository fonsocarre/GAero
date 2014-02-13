//
//  GAsettingsClass.h
//  GAero
//
//  Created by Alfonso Carre on 09/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef __GAsettingsClass__
#define __GAsettingsClass__

#include <string>
#include <iostream>
#include <stdio.h>
#include "Configuration.h"

class GAsettingsClass {
    int nGenerations;
    int populationSize;
    int genomeSize;
    double mutationRate;
    int nElitist;
    int nCrossing;
    
    std::string logFile;
    std::string historyFile;
    bool plotHistory;

public:
    GAsettingsClass();
    GAsettingsClass(const char* settingsFile);
    //~GAsettingsClass();
    
    friend class GAclass;
};

#endif /* defined(__GAero__GAsettingsClass__) */
