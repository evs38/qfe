#!/bin/sh

#
# Unix & MacOS script to create source package
#

if [ "$1" = "" ]; then
	echo "Script may be run only from Makefile."
	exit 0
fi

PWDDIR=`pwd`

cd ..

rm -f $1
mkdir -p .tmp
rm -f .tmp/$1

PROGDIR=`pwd`
PROGDIR=`basename $PROGDIR`
OUTFILE=$PROGDIR/.tmp/$1

cd ..
tar --exclude=$PROGDIR/.distignore --exclude-from=$PROGDIR/.distignore -jcvf $OUTFILE $PROGDIR
mv -f $OUTFILE $PROGDIR
cd $PWDDIR

exit 0
