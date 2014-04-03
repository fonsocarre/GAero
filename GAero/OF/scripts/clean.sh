#!/bin/sh
# argument $1 = case to be cleaned


echo "Cleaning OF case"
cd $1
rm -rf processor*
rm -rf post*
rm -rf foam.log*
rm -rf 0
rm -rf *00
rm -rf {1,2,3,4,5,6,7,8,9}*
rm -f *.log
