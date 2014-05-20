//
//  phiAirfoil.cpp
//  GAero
//
//  Created by Alfonso Carre on 20/05/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "phiAirfoil.h"

phiAirfoilClass::phiAirfoilClass()
{
    this->xlim.resize (2);
    this->zlim.resize (2);
    
    this->xlim[0] = 0.1;
    this->xlim[1] = 0.9;
    
    this->zlim[0] = 0.05;
    this->zlim[1] = 0.20;
}

std::vector<double> phiAirfoilClass::genome2coord
                (const std::vector<double>& genome)
{
    std::vector<double> result {genome};
    
    double deltaX;
    double deltaZ;
    
    deltaX = this->xlim[1] - this->xlim[0];
    deltaZ = this->zlim[1] - this->zlim[0];
    
    result[0] = result[0] * deltaX + this->xlim[0];
    result[2] = result[2] * deltaX + this->xlim[0];
    
    result[1] = result[1] * deltaZ + this->zlim[0];
    result[3] = -(result[3] * deltaZ + this->zlim[0]);
    
    return result;
}

std::string phiAirfoilClass::genome2string
                (const std::vector<double>& genome)
{
    std::string str {"phiAirfoil "};
    std::stringstream ss;
    std::vector<double> newGenome = this->genome2coord(genome);
    
    ss.precision(2);
    
    ss << "[" << newGenome[0] << ", " << newGenome[1] << "]";
    ss << std::endl;
    
    ss << "[" << newGenome[2] << ", " << newGenome[3] << "]";
    ss << std::endl;
    
    str += ss.str();
    
    return str;
}

double phiAirfoilClass::eval(const std::vector<double>& genome,
                             double& xCoor,
                             double& yPrevCoord,
                             double chordLength)
{
    //double newz = 0.;
    std::vector<double> newGenome = this->genome2coord(genome);
    
    std::vector<double> xPointsIn;
    std::vector<double> zPointsIn;
    
    int nPoints = static_cast<int>(genome.size())/2;
    
    int N = nPoints + 5;
    
    xPointsIn.resize(N);
    zPointsIn.resize(N);
    
    for (int i=0; i<N; ++i)
    {
        xPointsIn[i] = 2. * (i*1.)/(N-1.);
        zPointsIn[i] = 0.0;
    }
    
    xPointsIn[2] = newGenome[0];
    zPointsIn[2] = newGenome[1];
    
    xPointsIn[4] = 1 + newGenome[2];
    zPointsIn[4] = -newGenome[3];
    
    for (auto& x: xPointsIn)
    {
        x += constant::PI;
    }
    
    double xOutRad;
    if (yPrevCoord < 0)
    {
        // phi -> [0,pi]
        xOutRad = xCoor*constant::PI;
    }
    else
    {
        // phi -> [pi, 2pi]
        xOutRad = (xCoor + 1)*constant::PI;
    }
    
    std::vector<double> xPointsOut {xOutRad};
    std::vector<double> zPointsOut {0.0};
    
    splineInterpolation(xPointsIn, zPointsIn, xPointsOut, zPointsOut);
    
    
    
    return zPointsOut[0];
}









