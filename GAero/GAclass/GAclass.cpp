//
//  GAclass.cpp
//  GAlgorithm
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GAclass.h"

GAclass::GAclass()
{
    this->nPopulation=0;
    this->population.clear();
    this->nGenerations=0;
    this->rMutation=0.;
    this->rCrossover=0.;
}

GAclass::GAclass(int nPop)
{
    this->nPopulation=nPop;
    this->population.resize(nPop);
    this->nGenerations=0;
    this->rMutation=0.;
    this->rCrossover=0.;
}

GAclass::GAclass(const char* settingsFile)
{
    this->GAsettings = GAsettingsClass(settingsFile);
    this->nPopulation=this->GAsettings.populationSize;
    this->population.resize(this->GAsettings.populationSize);
    this->nGenerations=this->GAsettings.nGenerations;
    this->rMutation=0.;
    this->rCrossover=0.;
    
    for (int i=0; i<this->GAsettings.populationSize; i++)
    {
        this->population.at(i).genome.resize(this->GAsettings.genomeSize);
    }
}

GAclass::GAclass(int nPop, int genomeSize)
{
    this->nPopulation=nPop;
    this->population.resize(nPop);
    this->nGenerations=0;
    this->rMutation=0.;
    this->rCrossover=0.;
    
    for (int i=0; i<nPop; i++)
    {
        this->population.at(i).genome.resize(genomeSize);
    }
}

GAclass::~GAclass()
{
    for (int i=0; i<this->nPopulation; i++)
    {
        this->population[i].genome.clear();
    }
    this->population.clear();
    this->nGenerations=0;
    this->rMutation=0.;
    this->rCrossover=0.;
    this->nPopulation=0;
}