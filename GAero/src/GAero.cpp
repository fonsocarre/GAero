//
//  main.cpp
//  GAlgorithm
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include "GAclass.h"


int main(int argc, const char * argv[])
{

    GAclass GA("/Users/fonso/C++/GAero/GAero/Settings/settings.cfg");
    
    GA.initPop();
    
    for (GA.iGeneration=0; GA.iGeneration<GA.nGenerations; GA.iGeneration++)
    {
        GA.evolve();
    }
    
    return 0;
}

