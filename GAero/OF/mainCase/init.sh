#!/bin/sh

# Initialises an OpenFOAM case.
# Arguments -------------------
# $1 - absolute case dir
#------------------------------

source ~/.bashrc
of230
cd $1
cd 0.org/include
m4 initialConditions.m4 > initialConditions
cd ../..
rm -r 0
cp -r 0.org 0
potentialFoam -noFunctionObjects
decomposePar
mpirun -np 2 simpleFoam -parallel > foam.log
reconstructPar
rm -r processor*