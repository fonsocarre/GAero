//
//  randomGenerator.cpp
//  GAero
//
//  Created by Alfonso Carre on 11/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GArandomGenerator.h"

GArandom::GArandom():generator(0)
{
    this->seed = (int)time(0);
    
    static CRandomMersenne temp(seed);
    this->generator = temp;
    //std::cout<<"Init   "<<this->generator.Random()<<std::endl;
}

bool GArandom::flipCoin()
{
    double num = this->generator.Random();
    if (num < 0.5) return true;
    else return false;
}

bool GArandom::flipPounderedCoin(double trueProb)
{
    double num = this->generator.Random();
    if (num < trueProb) return true;
    else return false;
}

int GArandom::chooseInt(int low, int high)
{
    return this->generator.IRandom(low, high);
}

int GArandom::roulette(std::vector<double> weights)
{
    //normalize vector weights
    double sum = 0;
    for(int j=0; j<weights.size(); j++)
    {
        sum += weights[j];
    }
    for(int j=0; j<weights.size(); j++)
    {
        weights[j] = weights[j] / sum;
    }

    // random selection
    double num = this->generator.Random();
    std::vector<double> cummulative = weights;
    for (int j=1; j<weights.size(); j++)
    {
        cummulative[j] += cummulative[j-1];
    }
    
    for (int j=0; j<weights.size(); j++)
    {
        if (num <= cummulative[j] && num > cummulative[j-1]) return j;
    }
    
    return -1;
}


double GArandom::randDouble()
{
    return this->generator.Random();
}










