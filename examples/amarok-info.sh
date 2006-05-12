#!/bin/sh

# Unix script to insert playing song name into message
#
# Use with @command@ macro in message templates,
# such as: @command /home/user/bin/amarok-info.sh@

TEMPFILE=/tmp/amarok

dcop amarok player nowPlaying &> $TEMPFILE

if [ "`cat $TEMPFILE`" = "call failed" ]; then
	echo "amaroK sleep..."
else
	if [ "`cat $TEMPFILE`" = "" ]; then
		echo "amaroK sleep..."
	else
		echo "amaroK play: `cat $TEMPFILE`"
	fi
fi

rm -f $TEMPFILE

exit 0
