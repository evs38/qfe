@ECHO OFF
REM
REM Windows batch-file for toss inbound mail
REM

REM Toss tics.
htick toss

REM Toss bad tics.
htick toss -b

REM Announce new files.
htick announce

REM Toss mail.
hpt toss

REM Toss bad mail.
hpt toss -b

REM Process AreaFix.
hpt afix

REM Process FileFix.
htick ffix

REM Update Queue file.
hpt qupd

exit 0
