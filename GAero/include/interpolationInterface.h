//
//  interpolationInterface.h
//  GAero
//
//  Created by Alfonso Carre on 03/05/14.
//  Copyright (c) 2014 Alfonso Carre. All rights reserved.
//

#pragma once

extern "C" {
    void interpolationorder0(int Ns,
                               int Na,
                               int DIM,
                               double** coorS,
                               double** coorA,
                               double* hS,
                               double* hA,
                               char RBF,
                               double rho);
    void test (int Ns, double* a);
}