//
//  NACA4digits.cpp
//  GAero
//
//  Created by Alfonso Carre on 08/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "NACA4digits.h"

NACA4digits::NACA4digits ()
{
    this->coeffVector.resize (5);
    this->coeffVector[0] =  0.2969;
    this->coeffVector[1] = -0.1260;
    this->coeffVector[2] = -0.3516;
    this->coeffVector[3] =  0.2843;
    this->coeffVector[4] = -0.1036;
}

std::vector<double> NACA4digits::eval
                (const std::vector<double>& genome,
                 double xCoor,
                 double chordLength)
{
    std::vector<double> yCoorVec;

    double yThick = this->NACAthick(xCoor, genome[1]);
    
    double digit1;
    double digit2;
    
    digit2 = round (10.*modf(10.*genome[0], &digit1));
    
    double yCamber = this->NACAcamber(xCoor, digit1/100., digit2/10.);
    
    
    double theta = atan (this->NACAdiffY(xCoor, digit1/100., digit2/10.));
    
    // Positive solution
    yCoorVec.push_back (yCamber + yThick*cos (theta));
    // Negative Solution
    yCoorVec.push_back (yCamber - yThick*cos (theta));
    
    return yCoorVec;
}

double NACA4digits::NACAdiffY (double xCoor,
                               double m,
                               double p,
                               double chordLength)
{
    double result;
    
    xCoor /= chordLength;
    if (xCoor < p)
    {
        result = 2.*m/p * (p-xCoor);
    } else
    {
        result = (2.*m)/((1 - p)*(1 - p)) * (p - xCoor);
    }
    return result;
}

double NACA4digits::NACAcamber (double xCoor,
                                double m,
                                double p,
                                double chordLength)
{

    double yCamber;
    yCamber = 0.;
    
    xCoor /= chordLength;
    
    if (xCoor < p)
    {
        yCamber = m*xCoor/(p*p) *
                (2.*p - xCoor);
    } else
    {
        yCamber = m*(1. - xCoor)/((1. - p)*(1. - p)) *
                (1. + xCoor - 2.*p);
    }
    
    return yCamber;

}


double NACA4digits::NACAthick (double xCoor,
                               double genome,
                               double chordLength)
{
    double yThick;
    
    if (fabs(xCoor) < constant::EPSILON1
        ||
        fabs(xCoor - 1.) < constant::EPSILON1)
    {
        return 0.0;
    }
    
    yThick = 0.0;
    
    yThick += this->coeffVector[0]*sqrt (xCoor/chordLength);
    yThick += this->coeffVector[1]*(xCoor/chordLength);
    yThick += this->coeffVector[2]*(xCoor/chordLength)*(xCoor/chordLength);
    yThick += this->coeffVector[3]*pow(xCoor/chordLength, 3);
    yThick += this->coeffVector[4]*pow(xCoor/chordLength, 4);
    
    yThick *= genome/0.2 * chordLength;
    
    return yThick;
}


std::vector<double> NACA4digits::operator()
                        (const std::vector<double>& genome,
                         const double xCoor)
{
    return (this->eval(genome, xCoor));
}