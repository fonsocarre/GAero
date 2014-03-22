//
//  GAairfoilFitness.h
//  GAero
//
//  Created by Alfonso Carre on 21/03/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef GAero_GAairfoilFitness_h
#define GAero_GAairfoilFitness_h

#include "Configuration.h"
#include <string>
#include <iostream>

class GAfitness
{
    std::string settingsFile;
    std::string mainCaseRoute;
    std::string wallPatchName;
  

public:
    GAfitness();
    void readSettings(const char* settingsFile);
};



#endif
