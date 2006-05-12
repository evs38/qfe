#!/bin/sh

#
# Unix & MacOS script for toss inbound mail
#

# Toss tics.
htick toss

# Toss bad tics.
htick toss -b

# Announce new files.
htick announce

# Toss mail.
hpt toss

# Toss bad mail.
hpt toss -b

# Process AreaFix.
hpt afix

# Process FileFix.
htick ffix

# Update Queue file.
hpt qupd

exit 0
