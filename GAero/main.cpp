//
//  main.cpp
//  GAlgorithm
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include <iostream>
#include "GAclass.h"
#include <stdlib.h>
#include <fstream>
#include <time.h>                      // define time()
#include "randomc.h"                   // define classes for random number generators
#include <stdio.h>

int main(int argc, const char * argv[])
{

    GAclass GA("/Users/fonso/C++/GAero/GAero/Settings/settings.cfg");
    
    // generate initial population
    int seed = (int)time(0);            // random seed
    CRandomMersenne RanGen(seed);       // make instance of random number generator
    
    std::ofstream fOut;
    fOut.open("/Users/fonso/C++/GAero/GAero/Output/unifDist.txt");
    
    double number;
    
    for (int i=0; i<1000; i++)
    {
        number = RanGen.Random();
        fOut<< number << std::endl;
    }
    
    fOut.close();

    
    return 0;
}

