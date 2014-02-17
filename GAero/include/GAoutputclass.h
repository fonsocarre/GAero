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
    std::string fileName;
    
public:
    std::ofstream hist;
    GAoutputclass();
    GAoutputclass(const GAoutputclass& other);
    GAoutputclass (std::string histName);
    ~GAoutputclass ();
    GAoutputclass& operator=(const GAoutputclass& other);
    
    GAoutputclass& operator<< (std::ostream& (*pfun)(std::ostream&));
    //template <class T>
    //GAoutputclass& operator<< (T* val);
};

template <class T>
inline GAoutputclass& operator<< (GAoutputclass& st, T val)
{
    st.hist << val;
    std::cout  << val;
    return st;
};


#endif
