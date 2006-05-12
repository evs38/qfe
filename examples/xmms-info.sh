#!/bin/sh

# Unix script to insert playing song name into message
#
# Use with @command@ macro in message templates,
# such as: @command /home/user/bin/xmms-info.sh@

XMMSPIPE=/tmp/xmms-info

if ( test -e $XMMSPIPE )
then
    head -n 13 $XMMSPIPE | grep Title | awk -F :\  '{print "xmms: "$2;}';
else
    echo "xmms: Silence";
fi;

exit 0
