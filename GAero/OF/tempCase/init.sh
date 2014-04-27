#!/bin/sh

source ~/.bashrc
of230
#run
#cd NACABCNsa
cd 0.org/include
m4 initialConditions.m4 > initialConditions
cd ../..
rm -r 0
cp -r 0.org 0
potentialFoam -noFunctionObjects
decomposePar
mpirun -np 2 simpleFoam -parallel > foam.log &
pyFoamPlotWatcher.py foam.log
