//
//  IOfile.h
//  GAero
//
//  Created by Alfonso Carre on 20/05/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#pragma once

#include <iostream>
#include <fstream>
#include <string>

class GAoutput {
    std::string fileName;
    
public:
    GAoutput(std::string fileName, std::string comment)
    {
        this->fileName = fileName;
        file.open(this->fileName);
        this->file << comment << "\n";
    };
    std::ofstream file;
    GAoutput& operator<< (std::ostream& (*pfun)(std::ostream&));
};

template <class T>
inline GAoutput& operator<< (GAoutput& st, T val)
{
    st.file     << val;
    std::cout   << val;
    st.file.flush();
	std::cout.flush();
    return st;
};

static GAoutput output("GAeroOutput.txt", "# iGen, genome vars ");
static GAoutput genOutput("GenOutput.txt", "#  iGen, min, avg, max");
static GAoutput individualsOut("individuals.txt", "# iGen, fitness, genome vars");


