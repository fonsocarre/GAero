#!/bin/sh
# argument $1 = case to be duplicated
# argument $2 = target folder

# cleans target folder
rm -rf $2
# copies the main case to the target folder
cp -r $1 $2
