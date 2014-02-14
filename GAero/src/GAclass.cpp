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
    this->randomGen = GArandom();
}

GAclass::GAclass(int nPop)
{
    this->nPopulation=nPop;
    this->population.resize(nPop);
    this->nGenerations=0;
}

GAclass::GAclass(const char* settingsFile)
{
    this->GAsettings = GAsettingsClass(settingsFile);
    this->nPopulation=this->GAsettings.populationSize;
    this->population.resize(this->GAsettings.populationSize);
    this->oldPopulation.resize(this->GAsettings.populationSize);
    this->nGenerations=this->GAsettings.nGenerations;
    this->iGeneration = 0;
    this->usedPopulation = 0;
    
    for (int i=0; i<this->GAsettings.populationSize; i++)
    {
        this->population.at(i).genome.resize(this->GAsettings.genomeSize);
        this->population.at(i).genomeLength = GAsettings.genomeSize;
        this->oldPopulation.at(i).genome.resize(this->GAsettings.genomeSize);
        this->oldPopulation.at(i).genomeLength = GAsettings.genomeSize;
    }
    //this->oldPopulation = this->population;
    this->avgFitness.resize(this->nGenerations);
    this->maxFitness.resize(this->nGenerations);
}

GAclass::GAclass(int nPop, int genomeSize)
{
    this->nPopulation=nPop;
    this->population.resize(nPop);
    this->nGenerations=0;
    
    for (int i=0; i<nPop; i++)
    {
        this->population.at(i).genome.resize(genomeSize);
        this->population.at(i).genomeLength = genomeSize;
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
    this->nPopulation=0;
}

void GAclass::initPop()
{
    for (int i=0; i<this->nPopulation; i++)
    {
        for (int j=0; j<this->GAsettings.genomeSize; j++)
        {
            this->oldPopulation.at(i).genome[j] = this->randomGen.randDouble();
        }
        this->oldPopulation[i].generation = 0;
        this->oldPopulation[i].fitness = 0.;
    }
    
    this->calculateOldPopFitness();
    
    // sorting old population
    std::sort (this->oldPopulation.begin(), this->oldPopulation.end());
    std::reverse (this->oldPopulation.begin(), this->oldPopulation.end());
}



void GAclass::evolve()
{
    std::cout<<"EVOLVING......................"<<this->iGeneration<<std::endl;
    
    //------------------EVOLUTION-----------------------
    // selecting and procreating elite
    std::cout<<"    Selecting elite...";
    this->evolveElitists();
    std::cout<<"DONE"<<std::endl;
    
    // crossover
    std::cout<<"    Crossing individuals...";
    this->crossIndividuals();
    std::cout<<"DONE"<<std::endl;
    
    // new individuals
    std::cout<<"    Creating new individuals...";
    this->createNewIndividuals();
    std::cout<<"DONE"<<std::endl;
    
    //--------------------------------------------------
    
    // getting fitness for all pop
    this->calculatePopFitness();
    
    // sorting population
    std::sort (this->population.begin(), this->population.end());
    std::reverse (this->population.begin(), this->population.end());
    
    this->getPopFitness();
    this->oldPopulation = this->population;
    this->usedPopulation = 0;
}

// ONLY for ordered sets of population!!!
void GAclass::getPopFitness()
{
    double avgFitness = 0.;
    double maxFitness = 0.;
    double minFitness = 0.;
    
    for (int iPop=0; iPop<this->nPopulation; iPop++)
    {
        avgFitness += this->population[iPop].fitness;
    }
    avgFitness /= this->nPopulation;
    maxFitness =  this->population[0].fitness;
    minFitness =  this->population[this->nPopulation-1].fitness;
    
    this->maxFitness.push_back(maxFitness);
    this->avgFitness.push_back(avgFitness);
    this->minFitness.push_back(minFitness);
    std::cout<<"    max Fitness = "<<maxFitness<<std::endl;
    std::cout<<"    min Fitness = "<<minFitness<<std::endl;
    std::cout<<"    avg Fitness = "<<avgFitness<<std::endl;
}

void GAclass::calculatePopFitness()
{
    for (int iPop=0; iPop<this->nPopulation; iPop++)
    {
        this->population[iPop].evaluateFitness();
    }
}

void GAclass::calculateOldPopFitness()
{
    for (int iPop=0; iPop<this->nPopulation; iPop++)
    {
        this->oldPopulation[iPop].evaluateFitness();
    }
}

void GAclass::evolveElitists()
{
    for (int iPop=0; iPop<this->GAsettings.nElitist; iPop++)
    {
        this->population[iPop] = this->oldPopulation[iPop];
        this->population[iPop].generation = this->iGeneration;
        this->usedPopulation += 1;
    }
}

void GAclass::crossIndividuals()
{
    std::vector<double> weights;
    weights = oldPopFitness2vec();
    
    for (int i=0; i<this->GAsettings.nCrossing; i++)
    {
        // newPop position of the created individual
        int iNewPop = i + this->usedPopulation;
        // roulette selection of the two parents
        int index1 = this->randomGen.roulette (weights);
        int index2 = this->randomGen.roulette (weights);
        while (index1 == index2)
        {
            //std::cout<<"en el bucle"<<std::endl;
            index2 = this->randomGen.roulette (weights);
        }
        
        if (index1==-1 || index2==-1) {std::cout<<"ROULETTE ERROR"<<std::endl;}
        
        double fitness1 = this->oldPopulation[index1].fitness;
        double fitness2 = this->oldPopulation[index2].fitness;
        utilities::normalizeFitness(fitness1, fitness2);
        
        // genome filling
        for (int j=0; j<this->GAsettings.genomeSize; j++)
        {
            this->population[iNewPop].genome[j] =
                        fitness1*this->oldPopulation[index1].genome[j] +
                        fitness2*this->oldPopulation[index2].genome[j];
        }
        // other attributes
        this->population[iNewPop].generation = this->iGeneration;
        this->population[iNewPop].genomeLength = this->GAsettings.genomeSize;
    }
    this->usedPopulation += this->GAsettings.nCrossing;
}

void GAclass::createNewIndividuals()
{
    int nNew = this->nPopulation - this->usedPopulation;
    for (int iNewPop=0; iNewPop<nNew; iNewPop++)
    {
        this->population[iNewPop] = GApopulation(this->GAsettings.genomeSize);
        this->population[iNewPop].generation = this->iGeneration;
        for (int iGen=0; iGen<this->GAsettings.genomeSize; iGen++)
        {
            this->population[iNewPop].genome[iGen] =
                            this->randomGen.randDouble();
        }
    }
}

std::vector<double> GAclass::oldPopFitness2vec()
{
    std::vector<double> vec;
    
    vec.resize (this->nPopulation);
    for (int j=0; j<this->nPopulation; j++)
    {
        vec[j] = this->oldPopulation[j].fitness;
    }
    
    return vec;
}





















