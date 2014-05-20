//
//  main.cpp
//  GAero
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GAclass.h"
#include "IOfile.h"

int main(int argc, const char * argv[])
{
    GAclass GA("/Users/fonso/C++/GAero/GAero/Settings/settings.cfg");
    
    GA.initPop();
    
    //std::cout << GA.nGenerations << std::endl;
    for (GA.iGeneration=0; GA.iGeneration<GA.nGenerations; GA.iGeneration++)
    {
        GA.evolve();
        if (GA.checkConvergence()) { break; }
    }
    
    return 0;
}

