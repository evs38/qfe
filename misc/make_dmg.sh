#!/bin/sh

# MacOS script to create binary dmg-package
#
# Much of this code based on work provided at:
# http://www.kernelthread.com/mac/apme/archive/mkdmg.txt

MAC_VER=`uname -s | grep "Darwin" -`
MAC_ARH=`uname -i`

VERSION=`cat ../config.h | awk '/QFE_VERSION/ {print $3}' | sed s/\"//g`
BUNDLE_SOURCE=".tmp/qfe.app"
BUNDLE_NAME="QFE ${VERSION}.app"
DISK_IMAGE="../qfe-${VERSION}-${MAC_ARH}.dmg"
MOUNTED_DISK_NAME="QFE ${VERSION}"

# Output function
croak()
{
	echo "$1"
}
  
# Return error message
chkerror()
{
	if [ $? -ne 0 ]; then
		echo "Error!"
		exit 1
	fi
}

# Check MacOS
if [ "$MAC_VER" = "" ]; then
	croak "This script designed for running on MacOS only."
	exit 1;
fi

# Make sure all resources are placed into bundle
PWDDIR=`pwd`
cd ../src

if [ ! -f Makefile ]; then
	qmake -macx "CONFIG+=debug_off"
fi

make
lrelease src.pri
cd $PWDDIR
chkerror

if [ ! -d ../src/qfe.app ]; then
	croak "../src/qfe.app not exists."
	exit 1
fi
if [ ! -d ../src/qfe.app/Contents ]; then
	croak "../src/qfe.app/Contents not exists."
	exit 1
fi
if [ ! -d ../src/qfe.app/Contents/MacOS ]; then
	croak "../src/qfe.app/Contents/MacOS not exists."
	exit 1
fi
if [ ! -d ../src/qfe.app/Contents/Resources ]; then
	croak "../src/qfe.app/Contents/Resources not exists."
	exit 1
fi
if [ ! -f ../src/qfe.app/Contents/MacOS/qfe ]; then
	croak "../src/qfe.app/Contents/MacOS/qfe not exists."
	exit 1
fi

mkdir -p .tmp
rm -fR .tmp/*

cp -fR ../src/qfe.app .tmp

mkdir -p .tmp/qfe.app/Contents/Resources/translations
cp -f ../src/ts/*.qm .tmp/qfe.app/Contents/Resources/translations

mkdir -p .tmp/qfe.app/Contents/Resources/examples
cp -f ../examples/*.sh ../examples/*.bat .tmp/qfe.app/Contents/Resources/examples

mkdir -p .tmp/qfe.app/Contents/Resources/manual

cp -f ../AUTHORS ../COPYING ../README ../TODO ../FAQ ../ChangeLog ../qfe.lsm config.sample .tmp/qfe.app/Contents/Resources

# Clear stale image
rm -f $DISK_IMAGE

# Calculate size
SIZE=`du -s -k $BUNDLE_SOURCE | awk '{print $1}'`
chkerror
SIZE=`expr 1 + $SIZE / 1000`
chkerror
croak "Using image size $SIZE MB"

# Create sparse image
hdiutil create "${DISK_IMAGE}.sparseimage" -volname "${MOUNTED_DISK_NAME}" -megabytes $SIZE -type SPARSE -fs HFS+ 2>/dev/null >/dev/null
chkerror
croak "Image ${DISK_IMAGE}.sparseimage created."

# Mount sparse image
hdiutil mount ./${DISK_IMAGE}.sparseimage 2>/dev/null >/dev/null
chkerror
croak "Image ${DISK_IMAGE}.sparseimage mounted."

#find out allocated device
DEV=`mount | grep "Volumes/${MOUNTED_DISK_NAME}" | awk '{print $1}'`
croak "Device in use is $DEV"

# Use ditto to copy everything to the image, preserving resource forks
ditto -rsrcFork $BUNDLE_SOURCE "/Volumes/${MOUNTED_DISK_NAME}/${BUNDLE_NAME}" 2>/dev/null >/dev/null
chkerror
croak "Bundle copied successfully."

# Detach the disk image
hdiutil detach $DEV 2>/dev/null >/dev/null
chkerror
croak "Device $DEV detached."

# Compress the image
hdiutil convert "${DISK_IMAGE}.sparseimage" -format UDZO -o "${DISK_IMAGE}" -imagekey zlib-devel=9 2>/dev/null >/dev/null
chkerror
croak "Disk image compressed successfully."

# Remove sparseimage folder
croak "Removing temporaty files."
rm -rf ${DISK_IMAGE}.sparseimage
rm -rf $BUNDLE_SOURCE
rm -fR .tmp/*
rmdir -p .tmp

# Make disk image interent enabled
hdiutil internet-enable -yes ${DISK_IMAGE} 2>/dev/null >/dev/null
chkerror
croak "${DISK_IMAGE} created!"

exit 0
