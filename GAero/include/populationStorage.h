//
//  populationStorage.h
//  GAero
//
//  Created by Alfonso Carre on 02/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef GAero_populationStorage_h
#define GAero_populationStorage_h

#include <vector>

//! Namespace containing population data structures for GAero
namespace populationSto {
    //! Structure for storing population data gathered while running GAero
    struct individual {
        //! Vector for genome (parameters) storage
        std::vector<double> genome;
        //! Fitness value of the individual
        double fitness;
    };
}



#endif
