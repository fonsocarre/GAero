//
//  OFtopology.h
//  GAero
//
//  Created by Alfonso Carre on 03/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef GAero_OFtopology_h
#define GAero_OFtopology_h

#include <vector>
#include "OFdataStructures.h"

class OFtopology {
public:
    //! Vector of shape faces
    std::vector<faceClass> faces;
    //! Vector of mesh points
    std::vector<pointClass> points;
    //! Vector of shape points
    std::vector<int> shapePoints;
    //! Number of faces in shape
    int nFacesInShape;
    //! Number of faces in mesh
    int nFaces;
};

#endif
