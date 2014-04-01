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


//! Class for individuals belonging to the population.
/*! This class implements all the necessary variables and functions
 needed for accurately representing the individual. */
class GApopulation {
    std::vector<double> genome;
    int generation;
    int genomeLength;
    GAfitnessClass fitnessClass;
    
public:
    // Constructors ********************
    //! Population default constructor
    GApopulation();
    //! Population constructor with genome length
    //! It allocates the genome vector.
    GApopulation(int genomeLength);
    //! Default destructor.
    ~GApopulation();
    //GApopulation(const GApopulation& obj); //copy constructor
    //**********************************
    
    // logical operators****************
    //! Overloaded < based on fitness value
    friend bool operator< (const GApopulation& obj1,
                           const GApopulation& obj2);
    //! Overloaded > based on fitness value
    friend bool operator> (const GApopulation& obj1,
                           const GApopulation& obj2);
    //! Overloaded <= based on fitness value
    friend bool operator<=(const GApopulation& obj1,
                           const GApopulation& obj2);
    //! Overloaded >= based on fitness value
    friend bool operator>=(const GApopulation& obj1,
                           const GApopulation& obj2);
    //**********************************
    
    double fitness;
    
    void evaluateFitness();
    friend class GAclass;
};


#endif /* defined(__GApopulation__) */
