//
//  GAclass.cpp
//  GAlgorithm
//
//  Created by Alfonso Carre on 04/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GAclass.h"
#include "constants.h"

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
    this->GAout = GAoutputclass(this->GAsettings.historyFile);
    
    for (int i=0; i<this->GAsettings.populationSize; i++)
    {
        this->population.at(i).genome.resize(this->GAsettings.genomeSize);
        this->population.at(i).genomeLength = GAsettings.genomeSize;
        this->oldPopulation.at(i).genome.resize(this->GAsettings.genomeSize);
        this->oldPopulation.at(i).genomeLength = GAsettings.genomeSize;
    }
    this->fitness = &this->fitnessOF;
    this->fitness->getConfiguration
                (this->GAsettings.fitnessSettingsFile.c_str());
    this->fitness->initialise();
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
    this->GAout << "GAero successfully finished with ";
    this->GAout << this->fitness->iPopulation <<
            " function calls." << std::endl;
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
    this->GAout<<"EVOLVING......................"<<this->iGeneration<<std::endl;
    
    //------------------EVOLUTION-----------------------
    // selecting and procreating elite
    this->GAout<<"    Selecting elite...";
    this->evolveElitists();
    this->GAout<<"DONE"<<std::endl;
    
    // crossover
    this->GAout<<"    Crossing individuals...";
    this->crossIndividuals();
    this->GAout<<"DONE"<<std::endl;
    
    // new individuals
    this->GAout<<"    Creating new individuals...";
    this->createNewIndividuals();
    this->GAout<<"DONE"<<std::endl;
    
    //--------------------------------------------------
    
    // getting fitness for all pop
    this->calculatePopFitness();
    
    // sorting population
    std::sort (this->population.begin(), this->population.end());
    std::reverse (this->population.begin(), this->population.end());
    
    this->getPopFitness();
    this->oldPopulation.clear();
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
    this->GAout<<"    max Fitness = "<<maxFitness<<std::endl;
    this->GAout<<"    min Fitness = "<<minFitness<<std::endl;
    this->GAout<<"    avg Fitness = "<<avgFitness<<std::endl;
    this->GAout<<"    best indiv.: ";
    for (int i=0; i<this->GAsettings.genomeSize; i++)
    {
        this->GAout<<this->population[0].genome[i]<<" ";
    }
    this->GAout<<std::endl;
}

void GAclass::calculatePopFitness()
{
    for (int iPop=0; iPop<this->nPopulation; iPop++)
    {
        //this->population[iPop].evaluateFitness();
        this->population[iPop].fitness =
            this->fitness->getFitness (this->population[iPop].genome);
    }
}

void GAclass::calculateOldPopFitness()
{
    for (int iPop=0; iPop<this->nPopulation; iPop++)
    {
        this->oldPopulation[iPop].fitness =
            this->fitness->getFitness (this->oldPopulation[iPop].genome);
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
            index2 = this->randomGen.roulette (weights);
        }
        
        if ((index1==-1) || (index2==-1))
                {this->GAout<<"ROULETTE ERROR"<<std::endl;}
        
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
    int i;
    for (int iNewPop=0; iNewPop<nNew; iNewPop++)
    {
        i = this->usedPopulation + iNewPop;
        this->population[i] = GApopulation(this->GAsettings.genomeSize);
        this->population[i].generation = this->iGeneration;
        for (int iGen=0; iGen<this->GAsettings.genomeSize; iGen++)
        {
            this->population[i].genome[iGen] =
                            this->randomGen.randDouble();
        }
    }
    this->usedPopulation += nNew;
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

bool GAclass::checkConvergence()
{
    bool flag = false;
    
    if (this->iGeneration < this->GAsettings.minGenerations)
    {
        return flag;
    }
    
    if (this->iGeneration >= this->GAsettings.minGenerations
            &&
        this->fitnessVariation() < EPSILON)
    {
        flag = true;
        this->GAout << "Convergence reached in " << this->iGeneration
                    << " generations." << std::endl;
        return flag;
    }
    
    // flag == true if convergence reached
    return flag;
}

double GAclass::fitnessVariation()
{
    double fitness = 0;
    for (int i=(int)(this->maxFitness.size()-1);
                        i >= this->maxFitness.size()-4;
                        i--)
    {
        fitness += this->maxFitness[i];
    }
    fitness /= (double)(4.);
    return fabs((fitness - this->maxFitness[this->maxFitness.size()-1]))/
                (this->maxFitness[this->maxFitness.size()-1]);
}
