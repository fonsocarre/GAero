//
//  GApopulation.h
//  GAlgorithm
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef __GApopulation__
#define __GApopulation__

#include <vector>
#include "GAfitnessFunction.h"

class GApopulation {
    std::vector<double> genome;
    int generation;
    int genomeLength;
    GAfitnessClass fitnessClass;
    
public:
    // Constructors ********************
    GApopulation();
    GApopulation(int genomeLength);
    ~GApopulation();
    //GApopulation(const GApopulation& obj); //copy constructor
    //**********************************
    
    // logical operators****************
    friend bool operator< (const GApopulation& obj1,
                           const GApopulation& obj2);
    friend bool operator> (const GApopulation& obj1,
                           const GApopulation& obj2);
    friend bool operator<=(const GApopulation& obj1,
                           const GApopulation& obj2);
    friend bool operator>=(const GApopulation& obj1,
                           const GApopulation& obj2);
    //**********************************
    
    double fitness;
    
    void evaluateFitness();
    friend class GAclass;
};


#endif /* defined(__GApopulation__) */
