#!/bin/sh

PWDDIR=`pwd`

cd ../src

lupdate -verbose -noobsolete src.pri

cd $PWDDIR

exit 0
