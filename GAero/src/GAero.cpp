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
    
    for (GA.iGeneration=0; GA.iGeneration<GA.nGenerations; GA.iGeneration++) {
        GA.evolve();
    }
    
    
    
//    std::ofstream fOut;
//    fOut.open("/Users/fonso/C++/GAero/GAero/Output/unifDist.txt");
//    
//    //double number;
//    std::vector<double> weights(5);
//    weights[0] = 0.1;
//    weights[1] = 0.2;
//    weights[2] = 0.1;
//    weights[3] = 0.2;
//    weights[4] = 0.4;
    

//    
//    for (int i=0; i<5000; i++)
//    {
//        fOut<<GA.randomGen.flipPounderedCoin(0.3)<<std::endl;
//    }
//    
//    fOut.close();

    
    return 0;
}

