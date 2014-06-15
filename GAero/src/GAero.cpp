//
//  main.cpp
//  GAero
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include <ctime>

#include "GAclass.h"
#include "IOfile.h"

int main(int argc, const char * argv[])
{
    std::clock_t begin = clock();
    std::chrono::time_point<std::chrono::system_clock> cstart, cend;
    cstart = std::chrono::system_clock::now();
    
    GAclass GA("/Users/fonso/C++/GAero/GAero/Settings/settings.cfg");
    
    GA.initPop();
    
    for (GA.iGeneration=0; GA.iGeneration<GA.nGenerations; GA.iGeneration++)
    {
        GA.evolve();
        if (GA.checkConvergence()) { break; }
    }
    
    std::clock_t end = clock();
    cend = std::chrono::system_clock::now();
    std::cout << "CPU time elapsed: " << float(end-begin)/CLOCKS_PER_SEC;
    std::cout << std::endl;
    
    std::chrono::duration<double> elapsed_seconds = cend-cstart;
    std::cout << "Real time elapsed: " << elapsed_seconds.count()
              << "\n";
    
    
    return 0;
}

