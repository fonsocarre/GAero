//
//  charUtilities.h
//  mshReader
//
//  Created by Alfonso Carre on 27/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef mshReader_charUtilities_h
#define mshReader_charUtilities_h

#include <string>


namespace charUtils
{
    inline std::string trim(const std::string& str,
                     const std::string& whitespace = " \t")
    {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == std::string::npos)
            return ""; // no content
        
        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin + 1;
        
        return str.substr(strBegin, strRange);
    }
    
    inline bool anyNumbers(std::string buffer)
    {
        if (buffer.empty()) return false;
        for (int iChar=0; iChar<buffer.size(); iChar++)
        {
            if (isdigit(buffer[iChar])) return true;
        }
        return false;
    }
    
    
}

#endif
