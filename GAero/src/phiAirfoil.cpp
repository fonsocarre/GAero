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
    
    this->xlim[0] = 0.2;
    this->xlim[1] = 0.6;
    
    this->zlim[0] = 0.03;
    this->zlim[1] = 0.15;
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
    
    result[1] = -(result[1] * deltaZ + this->zlim[0]);
    result[3] = result[3] * deltaZ + this->zlim[0];
    
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
    
    //fixed points [0,1,3,5,6]
    xPointsIn[0] = this->x2phi(1.);
    xPointsIn[1] = this->x2phi(1.-0.03);
    xPointsIn[3] = this->x2phi(0.);
    xPointsIn[5] = 2.*constant::PI - this->x2phi(1.-0.03);
    xPointsIn[6] = 2.*constant::PI - this->x2phi(1.);
    
    for (auto& z: zPointsIn)
    {
        z = 0.0;
    }
    
    xPointsIn[2] = this->x2phi(newGenome[0]);
    zPointsIn[2] = newGenome[1];
    
    xPointsIn[4] = 2.*constant::PI - this->x2phi(newGenome[2]);
    zPointsIn[4] = newGenome[3];
    
//    for (auto& x: xPointsIn)
//    {
//        x *= constant::PI;
//    }

    double phiOut;
    if (yPrevCoord < 0)
    {
        // phi -> [0,pi]
        phiOut = this->x2phi(xCoor);
    }
    else
    {
        // phi -> [pi, 2pi]
        phiOut = 2 * constant::PI - this->x2phi(xCoor);
    }
    
    std::vector<double> xPointsOut {phiOut};
    std::vector<double> zPointsOut {0.0};
    
    splineInterpolation(xPointsIn, zPointsIn, xPointsOut, zPointsOut);
    
    
    
    return zPointsOut[0];
}

double phiAirfoilClass::x2phi(double x)
{
    return (acos(2.*x - 1.));
}








