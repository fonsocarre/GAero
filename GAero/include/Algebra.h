//
//  Algebra.h
//  Needs Accelerate Framework
//
//  Created by Alfonso Carre on 17/02/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//
//! Preprocessor guard
#pragma once

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

//! Algebra namespace, for avoiding misunderstandings.
namespace algebra
{
    //! Inverse calculation by LAPACK wrapper.
    /** Wrapper for matrix inverse calculation
     based on LAPACK GETRF and GETRI functions. */
    template <typename T>
    T invertMatrix(const T& A)
    {
        int N = static_cast<int> (sqrt(A.size()));
        T mat = A;
        
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
    template <typename T>
    T matmul (const int Arows,
              T& A,
              const int Bcols,
              T& B)
    {
        const int Acols = static_cast<int> (A.size ())/Arows;
        const int Brows = static_cast<int> (B.size ())/Bcols;
        assert (Brows == Acols);
        
        T C (Arows * Bcols);
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
    template <typename T>
    T matvecmul (const int Arows,
                 const int Acols,
                 T& A,
                 T& X)
    {
        assert (Acols == X.size ());
        
        T result (X.size ());
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
    
    //! Basic general transposed matrix - vector multiplication.
    /** Wrapper for transposed matrix-vector product
     calculation based on GEMV (BLAS)*/
    template <typename T>
    T Tmatvecmul (const int Arows,
                 const int Acols,
                 T& A,
                 T& X)
    {
        assert (Acols == X.size ());
        
        T result (X.size ());
        cblas_dgemv (CblasRowMajor,
                     CblasTrans,
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
}