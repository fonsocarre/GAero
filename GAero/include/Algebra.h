//
//  Algebra.h
//  Needs Accelerate Framework
//
//  Created by Alfonso Carre on 17/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#ifndef _Algebra_h
#define _Algebra_h

#include <cmath>
#include <vector>

extern "C" {
    // LU decomoposition of a general matrix
    void dgetrf_(int* M, int *N, double* A, int* lda, int* IPIV, int* INFO);
    
    // generate inverse of a matrix given its LU decomposition
    void dgetri_(int* N, double* A, int* lda, int* IPIV, double* WORK, int* lwork, int* INFO);
}

std::vector<double> invertMatrix(std::vector<double>& A)
{
    int N = sqrt(A.size());
    std::vector<double> mat(N*N);
    
    int *IPIV = new int[N+1];
    int LWORK = N*N;
    double *WORK = new double[LWORK];
    int INFO;
    
        dgetrf_(&N, &N, &(mat[0]), &N, IPIV, &INFO);
        dgetri_(&N,&(mat[0]),&N,IPIV,WORK,&LWORK,&INFO);
    
    delete[] IPIV;
    delete[] WORK;
    return mat;
}

#endif
