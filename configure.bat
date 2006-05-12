@echo off

rem ##########################################################################
rem # Configure script for Windows target                                    #
rem ##########################################################################

set QMAKE_OPTIONS=debug_off

if not exist Makefile goto clean1
del Makefile > NUL
:clean1
if not exist src\Makefile goto clean2
del src\Makefile > NUL
:clean2

set QFEMEAKESPEC=
if x%QMAKESPEC% == x goto qmakephaze
set QFEMEAKESPEC=-spec %QMAKESPEC%

:qmakephaze
qmake %QFEMEAKESPEC% "CONFIG+=%QMAKE_OPTIONS% %1 %2 %3 %4 %5" qfe.pro

cd src
qmake %QFEMEAKESPEC% "CONFIG+=%QMAKE_OPTIONS% %1 %2 %3 %4 %5" src.pro

cd ..

exit 0
