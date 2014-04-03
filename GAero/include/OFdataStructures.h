//
//  OFdataStructures.h
//  GAero
//
//  Created by Alfonso Carre on 03/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef GAero_OFdataStructures_h
#define GAero_OFdataStructures_h

#include <vector>
#include <algorithm>

//! OpenFOAM face data structure
class faceClass {
public:
    //! Overloaded < for sorting faces based on iFace.
    bool operator< (const faceClass& obj) const
    {
        return (this->iFace < obj.iFace);
    }

    int iFace;
    std::vector<int> points;
};

//! OpenFOAM points data structure
class pointClass {
public:
    int iPoint;
    std::vector<int> coords;
};

//bool faceClass::operator< (const faceClass& obj)


#endif
