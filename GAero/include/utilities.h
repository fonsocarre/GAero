//
//  utilities.h
//  GAero
//
//  Created by Alfonso Carre on 13/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//
#pragma once
//! Preprocessor guard
#ifndef GAero_utilities_h
#define GAero_utilities_h

#include <string>
#include <iostream>
#include <stdio.h>

namespace utilities
{
    inline void normalizeFitness(double& fit1, double&fit2)
    {
        double sum = fit1+fit2;
        fit1 /= sum;
        fit2 /= sum;
    }
    
    inline std::string exec(const char* cmd)
    {
        FILE* pipe = popen(cmd, "r");
        if (!pipe) return "ERROR";
        char buffer[128];
        std::string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        return result;
    }
    
//    //! Deallocates 2D array of the shape: array[N1][N2]
//    template <typename T>
//    inline void deallocateArray (T*& array, int N1)
//    {
//        for (int i1=0; i1<N1; i1++)
//        {
//            delete [] array[i1];
//            array[i1] = nullptr;
//        }
//        delete [] array;
//        array = nullptr;
//    }
}

#endif
