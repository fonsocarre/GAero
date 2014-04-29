//
//  interpolationKernelClass.cpp
//  GAero
//
//  Created by Alfonso Carre on 28/04/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#include "interpolationKernelClass.h"

//interpolationKernelClass::interpolationKernelClass
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
    
    this->firstCall = true;
}

void interpolationKernelClass::interpolate (std::vector<double>& hs,
                  std::valarray< std::valarray<double> >& sCoor,
                  std::vector<double>& ha,
                  std::valarray< std::valarray<double> >& aCoor)
{
    if (this->firstCall)
    {
        this->getCssInv(sCoor);
        this->firstCall = false;
    }
}

void interpolationKernelClass::getCssInv
        (std::valarray< std::valarray<double> >& sCoor)
{
    int Ns = static_cast<int> (sCoor.size ());
    this->CssInv.resize((Ns + 1)*(Ns + 1));
    
    std::valarray<double> Css ((Ns + 1)*(Ns + 1));
    
    // first col & rows
    for (int i=1; i<Ns+1; i++)
    {
        Css[i] = 1.0;
        Css[i*(Ns + 1)] = 1.0;
    }
    // next rows & cols
    for (int iRow=1; iRow<Ns+1; iRow++)
    {
        for (int iCol=0; iCol<Ns+1; iCol++)
        {
            Css[iRow*(Ns + 1) + iCol] = this->RBF (sCoor[iRow],
                                                   sCoor[iCol],
                                                   this->rho_);
            Css[iCol*(Ns + 1) + iRow] = Css[iRow*(Ns + 1) + iCol];
        }
    }
}









