//
//  interpolationKernelClass.cpp
//  GAero
//
//  Created by Alfonso Carre on 28/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "interpolationKernelClass.h"

void interpolationKernelClass::init (std::string RBF, double rho)
{
    // Spline
    if (RBF == "Spline")
    {
        this->RBF = &dist;
    }
    // Wendland C2
    else if (RBF == "wC2")
    {
        this->RBF = &wendlandC2;
    }
    // Default case
    else
    {
        std::cerr << "RBF " << RBF << "NOT implemented *****"
                  << std::endl;
        std::cerr << "Using Spline as default   "
                  << std::endl;
        this->RBF = &dist;
    }
    std::cout << "Interpolation will be done with: " << RBF
              << std::endl;
    std::cout << "    with a rho = " << rho << std::endl;
    
    this->rho_ = rho;
    
    //this->firstCall = true;
}


void interpolationKernelClass::interpolate (std::vector<double>& hs,
                  std::valarray< std::valarray<double> >& sCoor,
                  std::vector<double>& ha,
                  std::valarray< std::valarray<double> >& aCoor)
{
    char RBFchar = 'S';
    int inputDIM = constant::DIM;
    int Ns = static_cast<int> (hs.size ());
    int Na = static_cast<int> (aCoor.size ());
    
    ha.resize (Na);
    
    double* sCoorT = nullptr;
    double* aCoorT = nullptr;
    
    this->valarray2f90(sCoor, sCoorT);
    this->valarray2f90(aCoor, aCoorT);
    
    interpolationorder0(Ns,
                        Na,
                        inputDIM,
                        &(sCoorT[0]),
                        &(aCoorT[0]),
                        &(hs[0]),
                        &(ha[0]),
                        RBFchar,
                        (this->rho_));
    
    delete [] sCoorT;
    delete [] aCoorT;
}


void interpolationKernelClass::valarray2f90
                   (std::valarray< std::valarray<double>>& val,
                    double*& vec)
{
    int nPoints = static_cast<int> (val.size ());
    int nCoor = static_cast<int> (val[0].size ());
    // allocation
    vec = new double[nCoor*nPoints];
    
    // Filling the array
    for (int iCoor=0; iCoor<nCoor; iCoor++)
    {
        for (int iPoint=0; iPoint<nPoints; iPoint++)
        {
            vec[iCoor*nPoints + iPoint] = val[iPoint][iCoor];
        }
    }
}
