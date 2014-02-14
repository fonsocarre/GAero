//
//  randomGenerator.h
//  GAero
//
//  Created by Alfonso Carre on 11/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef __GAero__randomGenerator__
#define __GAero__randomGenerator__

#include <time.h>
#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>
#include "randomc.h"


class GArandom {
    int seed;
    CRandomMersenne generator;

public:
    GArandom();
    bool flipCoin();
    bool flipPounderedCoin(double trueProb);
    int chooseInt(int low, int high);
    int roulette(std::vector<double> weights);
    double randDouble();
};




#endif /* defined(__GAero__randomGenerator__) */
