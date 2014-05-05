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
    
    this->firstCall = true;
}


void interpolationKernelClass::interpolate (std::vector<double>& hs,
                  std::valarray< std::valarray<double> >& sCoor,
                  std::vector<double>& ha,
                  std::valarray< std::valarray<double> >& aCoor)
{
    char RBFchar = 'W';
    int inputDIM = constant::DIM;
    int Ns = static_cast<int> (hs.size ());
    int Na = static_cast<int> (aCoor.size ());
    
    ha.resize (Na);
    
    double* sCoorT = nullptr;
    double* aCoorT = nullptr;
    
    this->valarray2f90(sCoor, sCoorT);
    this->valarray2f90(aCoor, aCoorT);
    
    //std::cout << "From C++:" << std::endl;
    //std::cout << sCoorT[0][Ns-1] << ", " << sCoorT[1][Ns-1] << ", "<< sCoorT[2][Ns-1] << std::endl;
    //std::cout << sCoor[Ns-1][0] << ", " << sCoor[Ns-1][1] << ", "<< sCoor[Ns-1][2] << std::endl;
    
//    std::ofstream temp;
//    temp.open("outputC++.txt");
//    
//    for (int i=0; i<Ns; i++)
//    {
//        temp << sCoorT[i] << " " << sCoorT[i + Ns] << " "<< sCoorT[i + 2*Ns];
//        temp << std::endl;
//    }
    
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

//void interpolationKernelClass::Tvalarray2f90
//            (std::valarray< std::valarray<double>>& val,
//             double*& vec)
//{
//    int nPoints = static_cast<int> (val.size ());
//    int nCoor = static_cast<int> (val[0].size ());
//    // allocation
//    vec = new double*[nPoints * nCoor];
//    
//    // Filling the array
//    for (int iCoor=0; iCoor<nCoor; iCoor++)
//    {
//        for (int iPoint=0; iPoint<nPoints; iPoint++)
//        {
//            vec[iCoor*nCoor + Point] = val[iPoint][iCoor];
//        }
//    }
//}



//void interpolationKernelClass::interpolate (std::vector<double>& hs,
//                  std::valarray< std::valarray<double> >& sCoor,
//                  std::vector<double>& ha,
//                  std::valarray< std::valarray<double> >& aCoor)
//{
//    int Na = static_cast<int>(aCoor.size ());
//    int Ns = static_cast<int>(sCoor.size ());
//    
//    if (this->firstCall)
//    {
//        // CssInv calculation
//        std::valarray<double> CssInv ((Ns + 1)*(Ns + 1));
//        CssInv = this->getCssInv(sCoor);
//        
//        // Aas*CssInv product
//        // Some care must be taken because
//        // vector*matrix = matrix^T*vector
//        // the second way is the one BLAS accepts
//        this->G_.resize ((Ns+1)*(Na));
//        for (int iRow=0; iRow<Na; iRow++)
//        {
//            auto temp = this->getAasRow (sCoor, aCoor, iRow);
//            this->G_[std::slice (iRow*(Ns+1), Ns+1, 1)] =
//                                            algebra::Tmatvecmul(Ns+1,
//                                                                Ns+1,
//                                                                CssInv,
//                                                                temp);
//        }
//        
//        this->firstCall = false;
//    }
//    hs.insert (hs.begin(), 0.0);
//    std::cout << *(std::end(this->G_)-1) << std::endl;
//    ha = algebra::matvecmul(Na, Ns+1, this->G_, hs);
//    hs.erase (hs.begin ());
//}
//
//std::valarray<double> interpolationKernelClass::getCssInv
//        (std::valarray< std::valarray<double> >& sCoor)
//{
//    int Ns = static_cast<int> (sCoor.size ());
//    //CssInv.resize((Ns + 1)*(Ns + 1));
//    
//    std::valarray<double> Css ((Ns + 1)*(Ns + 1));
//    
//    // first col & rows
//    for (int i=1; i<Ns+1; i++)
//    {
//        Css[i] = 1.0;
//        Css[i*(Ns + 1)] = 1.0;
//    }
//    // next rows & cols
//    for (int iRow=1; iRow <= Ns; iRow++)
//    {
//        for (int iCol=1; iCol <= iRow; iCol++)
//        {
//            Css[iRow*(Ns + 1) + iCol] = this->RBF (sCoor[iRow - 1],
//                                                   sCoor[iCol - 1],
//                                                   this->rho_);
//            Css[iCol*(Ns + 1) + iRow] = Css[iRow*(Ns + 1) + iCol];
//        }
//    }
//    
//    return algebra::invertMatrix(Css);
//}
//
//std::valarray<double> interpolationKernelClass::getAasRow
//            (std::valarray<std::valarray<double>>& sCoor,
//             std::valarray<std::valarray<double>>& aCoor,
//             int nRow)
//{
//    int Ns = static_cast<int>(sCoor.size ());
//    //int Na = static_cast<int>(aCoor.size ());
//    
//    std::valarray<double> AasRow (Ns+1);
//    
//    AasRow[0] = 1.0;
//    for (int i=1; i<Ns+1; i++)
//    {
//        AasRow[i] = this->RBF (aCoor[nRow],
//                               sCoor[i-1],
//                               this->rho_);
//        //std::cout << i << std::endl;
//        //std::cout << AasRow[i] << std::endl;
//    }
//    
//    return AasRow;
//}
// sdfasdgasd




