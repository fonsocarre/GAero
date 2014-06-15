#! /bin/sh

#**************************************************
# checkMesh wrapper.
#**************************************************
# Arguments -------------------
# $1 - absolute case dir
#------------------------------
# MACOSX
source ~/.profile > log
rm log

#linux
#source ~/.bashrc

of230 > log
rm log

cd $1
checkMesh > check.log
echo $(tail -4 check.log | head -1)
rm check.log