//
//  GAoutputclass.cpp
//  GAero
//
//  Created by Alfonso Carre on 14/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "GAoutputclass.h"

GAoutputclass::GAoutputclass()
{
    this->fileName = "undefined";
    this->hist.close();
}

GAoutputclass::GAoutputclass(const GAoutputclass& other)
{
    this->fileName = other.fileName;
    this->hist.open(this->fileName);
}

GAoutputclass::GAoutputclass(std::string histName)
{
    this->fileName = histName;
    this->hist.open(histName);
}

GAoutputclass::~GAoutputclass()
{
    this->hist.close();
}

GAoutputclass& GAoutputclass::operator=(const GAoutputclass& other)
{
    this->fileName = other.fileName;
    this->hist.open(this->fileName);
    return *this;
}



GAoutputclass& GAoutputclass::operator<< (std::ostream& (*pfun)(std::ostream&))
{
    pfun(hist);
    pfun(std::cout);
    return *this;
}
