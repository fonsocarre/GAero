//
//  Algebra.h
//  Needs Accelerate Framework
//
//  Created by Alfonso Carre on 17/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//
//! Preprocessor guard
#ifndef _Algebra_h
#define _Algebra_h

#include <cmath>
#include <vector>
#include <cassert>
#include "cblas.h"

extern "C" {
    //! LU decomoposition of a general matrix (LAPACK)
    void dgetrf_(int* M, int *N, double* A, int* lda, int* IPIV, int* INFO);
    
    //! Generates inverse of a matrix given its LU decomposition (LAPACK)
    void dgetri_(int* N, double* A, int* lda, int* IPIV, double* WORK, int* lwork, int* INFO);

}

//! Inverse calculation by LAPACK wrapper.
/** Wrapper for matrix inverse calculation
 based on LAPACK GETRF and GETRI functions. */
std::vector<double> invertMatrix(std::vector<double>& A)
{
    int N = sqrt(A.size());
    std::vector<double> mat = A;
    
    int *IPIV = new int[N+1];
    int LWORK = N*N;
    double *WORK = new double[LWORK];
    int INFO;
    
    dgetrf_(&N, &N, &(mat[0]), &N, IPIV, &INFO);
    if (INFO != 0)
    {
        std::cerr << "LAPACK ERROR. INFO = " << INFO << std::endl;
    }
    
    dgetri_(&N,&(mat[0]),&N,IPIV,WORK,&LWORK,&INFO);
    
    if (INFO != 0)
    {
        std::cerr << "LAPACK ERROR. INFO = " << INFO << std::endl;
    }
    
    delete[] IPIV;
    delete[] WORK;
    return mat;
}

//! Basic general matrix matmul.
/** Wrapper for matrix-matrix product
 calculation based on DGEMM BLAS
 function */
std::vector<double> matmul (const int Arows,
                            std::vector<double>& A,
                            const int Bcols,
                            std::vector<double>& B)
{
    const int Acols = static_cast<int> (A.size ())/Arows;
    const int Brows = static_cast<int> (B.size ())/Bcols;
    assert (Brows == Acols);
    
    std::vector<double> C (Arows * Bcols);
    cblas_dgemm (CblasRowMajor,   //TODO: check if RowMajor is correct
                 CblasNoTrans,
                 CblasNoTrans,
                 Arows,
                 Bcols,
                 Acols,
                 1.,
                 &(A[0]),
                 Arows,
                 &(B[0]),
                 Brows,
                 0.0,
                 &(C[0]),
                 Arows);
    
    return C;
}

//! Basic general matrix - vector multiplication.
/** Wrapper for matrix-vector product
    calculation based on GEMV (BLAS)*/
std::vector<double> matvecmul (const int Arows,
                               const int Acols,
                               std::vector<double>& A,
                               std::vector<double>& X)
{
    assert (Acols == X.size ());
    
    std::vector<double> result (X.size ());
    cblas_dgemv (CblasRowMajor,
                 CblasNoTrans,
                 Arows,
                 Acols,
                 1.0,
                 &(A[0]),
                 Arows,
                 &(X[0]),
                 1,
                 0.0,
                 &(result[0]),
                 1);
    return result;
}















#endif