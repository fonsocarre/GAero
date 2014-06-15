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
#include <sstream>
//#include <s
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    
    inline std::string execSys(const char* cmd)
    {
        std::string route;
        std::string command;
        std::string str;
        
        route = (std::string(cmd)).substr (0, std::string(cmd).find(" "));
        route = route.substr (0, route.find_last_of("/")+1);
        
        command =(std::string(cmd)).substr (route.size (), std::string(cmd).size ());
        
        // Pipe command to a temporary file
        system((route + command + ">" + route +"temp.txt").c_str ());
        // Open file
        std::ifstream infile(route + "temp.txt");
        // Read the file data into a string object
        std::stringstream strStream;
        strStream << infile.rdbuf();//read the file
        
        str = strStream.str();//str holds the content of the file
        
        //std::string data((std::istreambuf_iterator<char>(infile)),
                    //std::istreambuf_iterator<char>());
        // Delete the temporary file
        system(("rm -f " + route + "temp.txt").c_str ());
        
        return str;
    }
    
    inline void paralellExec (const char *strCommand)
    {
        //pid_t process;
        //process = vfork();
        std::string command;
        
        command = "sh " + std::string(strCommand);
        
        exec(command.c_str());
    }
}

#endif
