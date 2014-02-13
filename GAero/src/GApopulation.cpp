//
//  GApopulation.cpp
//  GAlgorithm
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GApopulation.h"

//**********************CONSTRUCTORS*********************
GApopulation::GApopulation()
{
    this->genome.clear();
    this->fitness = 0.;
    this->generation = 0;
    this->genomeLength = 0;
}

GApopulation::GApopulation(int length)
{
    this->genomeLength = length;
    this->genome.resize(this->genomeLength);
    this->fitness = 0.;
    this->generation = 0;
}

GApopulation::~GApopulation()
{
    this->genome.clear();
    this->fitness = 0.;
    this->generation = 0;
    this->genomeLength = 0;
}

GApopulation::GApopulation(const GApopulation& obj)
{
    GApopulation* out;
    out = new GApopulation;
    
    out->genome = obj.genome;
    out->generation = obj.generation;
    out->genomeLength = obj.genomeLength;
    
    out->fitness = obj.fitness;
}
//********************************************************

//******************LOGICAL OPERATORS*********************
bool operator< (const GApopulation& obj1,
                const GApopulation& obj2)
{
    return obj1.fitness<obj2.fitness;
}

bool operator> (const GApopulation& obj1,
                const GApopulation& obj2)
{
    return obj1.fitness>obj2.fitness;
}

bool operator<= (const GApopulation& obj1,
                 const GApopulation& obj2)
{
    return obj1.fitness<=obj2.fitness;
}

bool operator>= (const GApopulation& obj1,
                 const GApopulation& obj2)
{
    return obj1.fitness>=obj2.fitness;
}
//*******************************************************


//sample FUNCTION TODO
void GApopulation::evaluateFitness()
{
    this->fitness = 0.;
}