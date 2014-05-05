#!/bin/sh

# Initialises an OpenFOAM case.
# Arguments -------------------
# $1 - absolute case dir
#------------------------------

# MACOSX
source ~/.profile

#linux
#source ~/.bashrc
of230
cd $1
cd 0.org/include
echo "	... m4 initialConditions.m4 "
m4 initialConditions.m4 > initialConditions
cd ../..
rm -rf 0
cp -r 0.org 0
echo "	... potentialFoam -noFunctionObjects"
potentialFoam -noFunctionObjects > potentialFoam.log
echo "	... decomposePar"
decomposePar > decomposePar.log
echo "	... simpleFoam -parallel "
mpirun -np 2 simpleFoam -parallel > foam.log
echo "	... reconstructPar "
reconstructPar > reconstructPar.log
rm -rf processor*