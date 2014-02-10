//
//  FitnessFunction.h
//  GAlgorithm
//
//  Created by Alfonso Carre on 05/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef _FitnessFunction_h
#define _FitnessFunction_h

#define <vector>
#define <math>

/**********************************************************
 Trial fitness function // 2 VARIABLES
**********************************************************/
void FitnessFunction(std::vector<double> genome)
{
    //this->fitness = exp(genome(0)*genome(0) + genome(1)*genome(1));
    
    this->fitness = exp((genome*genome).sum());
}

#endif
