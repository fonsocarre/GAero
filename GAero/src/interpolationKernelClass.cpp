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
    int Na = static_cast<int>(aCoor.size ());
    int Ns = static_cast<int>(sCoor.size ());
    
    if (this->firstCall)
    {
        // CssInv calculation
        std::valarray<double> CssInv ((Ns + 1)*(Ns + 1));
        CssInv = this->getCssInv(sCoor);
        
        // Aas*CssInv product
        // Some care must be taken because
        // vector*matrix = matrix^T*vector
        // the second way is the one BLAS accepts
        
        this->firstCall = false;
    }
    
    
    
    //!!!!
    std::valarray<double> temp = this->getAasRow(sCoor, aCoor, 0);
}

std::valarray<double> interpolationKernelClass::getCssInv
        (std::valarray< std::valarray<double> >& sCoor)
{
    int Ns = static_cast<int> (sCoor.size ());
    //CssInv.resize((Ns + 1)*(Ns + 1));
    
    std::valarray<double> Css ((Ns + 1)*(Ns + 1));
    
    // first col & rows
    for (int i=1; i<Ns+1; i++)
    {
        Css[i] = 1.0;
        Css[i*(Ns + 1)] = 1.0;
    }
    // next rows & cols
    for (int iRow=1; iRow <= Ns; iRow++)
    {
        for (int iCol=1; iCol <= iRow; iCol++)
        {
            Css[iRow*(Ns + 1) + iCol] = this->RBF (sCoor[iRow - 1],
                                                   sCoor[iCol - 1],
                                                   this->rho_);
            Css[iCol*(Ns + 1) + iRow] = Css[iRow*(Ns + 1) + iCol];
        }
    }
    
    return algebra::invertMatrix(Css);
}

std::valarray<double> interpolationKernelClass::getAasRow
            (std::valarray<std::valarray<double>>& sCoor,
             std::valarray<std::valarray<double>>& aCoor,
             int nRow)
{
    int Ns = static_cast<int>(sCoor.size ());
    //int Na = static_cast<int>(aCoor.size ());
    
    std::valarray<double> AasRow (Ns+1);
    
    AasRow[0] = 1.0;
    for (int i=1; i<Ns+1; i++)
    {
        AasRow[i] = this->RBF (aCoor[nRow],
                               sCoor[i-1],
                               this->rho_);
        //std::cout << i << std::endl;
        //std::cout << AasRow[i] << std::endl;
    }
    
    return AasRow;
}









