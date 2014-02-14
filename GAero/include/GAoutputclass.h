//
//  GAoutputclass.h
//  GAero
//
//  Created by Alfonso Carre on 14/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef GAero_GAoutputclass_h
#define GAero_GAoutputclass_h

#include <iostream>
#include <fstream>
#include <string>

class GAoutputclass {
    std::ofstream hist;
    
public:
    GAoutputclass (std::string histName);
    ~GAoutputclass ();
    
};


#endif
