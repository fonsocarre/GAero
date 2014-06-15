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

double NACA4digits::eval
                (const std::vector<double>& genome,
                 double& xCoor,
                 double& yPrevCoord,
                 double chordLength = 1.)
{
    std::vector<double> yCoorVec;
    
    std::vector<double> tempGenome {genome};
    
    tempGenome[2] *= static_cast<double> (this->maxThickness)/100.;
    
    double A = static_cast<int>(tempGenome[0]*10)/100.; // [0,1]
    double B = static_cast<int>(tempGenome[1]*10)/10.; // [0,1]
    double thickness = static_cast<int>(tempGenome[2]*100)/100. ; // [0,1]
    
    double yThick = this->NACAthick(xCoor, thickness);
    
    double yCamber = this->NACAcamber(xCoor, A, B);
    
    double theta = atan (this->NACAdiffY(xCoor, A, B));
    
    // Positive solution
    yCoorVec.push_back (yCamber + yThick*cos (theta));
    // Negative Solution
    yCoorVec.push_back (yCamber - yThick*cos (theta));
    
    if (yPrevCoord < 0.0)
    {
        return yCoorVec[1];
    } else if (yPrevCoord > 0.0)
    {
        return yCoorVec[0];
    }
    
    return 0.0;
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
    
    return yCamber*chordLength;

}


double NACA4digits::NACAthick (double xCoor,
                               double genome,
                               double chordLength)
{
    double yThick;
    
    yThick = 0.0;
    
    yThick += this->coeffVector[0]*sqrt (xCoor/chordLength);
    yThick += this->coeffVector[1]*(xCoor/chordLength);
    yThick += this->coeffVector[2]*(xCoor/chordLength)*(xCoor/chordLength);
    yThick += this->coeffVector[3]*pow(xCoor/chordLength, 3);
    yThick += this->coeffVector[4]*pow(xCoor/chordLength, 4);
    
    yThick *= genome/0.2 * chordLength;
    
    return yThick;
}


std::string NACA4digits::genome2string
                (const std::vector<double>& genome)
{
    std::string result = "NACA";
    std::stringstream ss;
    double genome2;

    ss.precision (0);

    ss <<static_cast<int> (genome[0]*10)<<static_cast<int> (genome[1]*10);

    result += ss.str ();

    //result += std::to_string(static_cast<long double> (static_cast<int> (genome[0]*10)));
    //result += std::to_string(static_cast<long double> (static_cast<int> (genome[1]*10)));
    genome2 = genome[2] * static_cast<double> (this->maxThickness)/100.;
    if (genome2 < 0.1) result += "0";
    //result += std::to_string(static_cast<long double> (static_cast<int> (genome[2]*100)));
    ss.str(std::string());
    ss <<static_cast<int> (genome2*100);

    result += ss.str ();   

    return result;
}
