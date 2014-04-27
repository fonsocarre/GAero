//
//  fileUtils.h
//  GAero
//
//  Created by Alfonso Carre on 08/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "charUtilities.h"

namespace fileUtils {
    //! Template function for handling IO files.
    template <class T>
    void openFile (T& file, const std::string& route)
    {
        file.open (route.c_str());
        if (!file.is_open ())
        {
            std::cerr << "*** ERROR ***" << std::endl;
            std::cerr << "Could not open "
                      << route
                      << std::endl;
        }
    }
    
    inline std::vector<double> parseForceCoeffsLine (std::string& buffer)
    {
        std::vector<double> result;
        
        int limit;
        while (true)
        {
            if (buffer.find ("\t") == std::string::npos) break;
            limit = static_cast<int> (buffer.find("\t"));
            result.push_back (std::stod ( buffer.substr (0, limit)));
            buffer = charUtils::trim (buffer.substr (limit+1, buffer.size ()));
        }
        return result;
    }
}