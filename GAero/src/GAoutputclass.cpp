//
//  GAoutputclass.cpp
//  GAero
//
//  Created by Alfonso Carre on 14/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GAoutputclass.h"

GAoutputclass::GAoutputclass(std::string histName)
{
    this->hist.open(histName);
}

GAoutputclass::~GAoutputclass()
{
    this->hist.close();
}

