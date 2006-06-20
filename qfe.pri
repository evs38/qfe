#***************************************************************************
#*   Copyright (C) 2005-2006 by Alexander Shiyan                           *
#*   shc@users.sourceforge.net                                             *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; either version 2 of the License, or     *
#*   (at your option) any later version.                                   *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU General Public License for more details.                          *
#*                                                                         *
#*   You should have received a copy of the GNU General Public License     *
#*   along with this program; if not, write to the                         *
#*   Free Software Foundation, Inc.,                                       *
#*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************

win32 {
	NULDEV=NUL
	GREPCMD=fgrep -G
	QT_DIR_BASE=$$system(echo %QTDIR%)
	contains(QMAKE_CXX, cl) {
		CONFIG += msvc
	}
}

!win32 {
	NULDEV=/dev/null
	GREPCMD=grep -E
	QT_DIR_BASE=$$system(echo -n $QTDIR)
}

macx:CONFIG += mac

# Qt 4.1.x stuff for create universal binary on MacOS X
#mac {
#	QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
#	CONFIG+=x86 ppc
#}

isEmpty(QT_DIR_BASE) {
	error('QTDIR' variable is not set.)
}

QT_DIR_BIN=$$QT_DIR_BASE/bin
QT_DIR_LIB=$$QT_DIR_BASE/lib
QT_DIR_INC=$$QT_DIR_BASE/include
QGLOBAL=$$QT_DIR_INC/qglobal.h

exists($$QT_DIR_LIB/libqt*-mt*) {
	CONFIG += thread
}

exists($$QT_DIR_BIN/lrelease*) {
	CONFIG += langsupport
}

!exists($$QGLOBAL) {
	QGLOBAL=$$QT_DIR_INC/Qt/qglobal.h
	exists($$QGLOBAL) {
		CONFIG += qt4
	} else {
		error(Unable to determine Qt version.)
	}
}

!qt4 {
	win32 {
		TEST_QTVER = $$system(grep "QT_VERSION_STR" $$QGLOBAL | tr -d "\042" | tr -s [:space:])
	} else {
		TEST_QTVER = $$system(awk '/QT_VERSION_STR/ {print $3}' <$$QGLOBAL | sed s/\"//g)
	}
	TEST_QT4 = $$system(echo $$TEST_QTVER | $$GREPCMD "^4\.")
	!isEmpty(TEST_QT4) {
		CONFIG += qt4
	}
}

!qt4 {
	win32 {
		TEST_QT3 = $$system(echo "$$TEST_QTVER" | $$GREPCMD " 3\.")
	} else {
		TEST_QT3 = $$system(echo "$$TEST_QTVER" | $$GREPCMD "^3\.")
	}
	isEmpty(TEST_QT3) {
		error(Compile allow only with Qt 3.2.X and above.)
	}
}

qt4 {
#	QT += xml qt3support
	message(Compile with Qt4 is not supported yet.)
#	debug {
#		DEFINES += QT3_SUPPORT_WARNINGS
#	}
}

MAINTARGET = qfe

win32 {
	OPTIONAL_INCLUDES=../../../include
	OPTIONAL_LIBS=
	OPTIONAL_CFLAGS=
	OPTIONAL_CXXFLAGS=
	msvc {
		OPTIONAL_LFLAGS=/LIBPATH:"../../../lib"
	} else {
		OPTIONAL_LFLAGS=-L../../../lib
	}
}

!win32 {
	OPTIONAL_INCLUDES=
	OPTIONAL_LIBS=
	OPTIONAL_CFLAGS=
	OPTIONAL_CXXFLAGS=
	OPTIONAL_LFLAGS=
}

QMAKE_UIC = $$QMAKE_UIC -tr QObject::tr

unix {
	VERSION = $$system(awk '/QFE_VERSION/ {print $3}' <$$PWD/config.h | sed s/\"//g)
}

!win32 {
	system(env | $$GREPCMD "QFE_PREFIX" > $$NULDEV):CONFIG += prefix

	!isEmpty(PREFIX) {
		HOME_PATH = $$PREFIX
		CONFIG += prefix
	} else {
		prefix {
			HOME_PATH = $(QFE_PREFIX)
		} else {
			CONFIG += prefix
			mac {
				HOME_PATH = "/Applications/Fido"
			} else {
				HOME_PATH = "/usr/local"
			}
		}
	}
}

win32:QMAKE_COPY = xcopy /Y /K /E /Q

!msvc {
	!isEmpty(QMAKE_CXX) {
		GCCTEST = $$system($$QMAKE_CXX --version | $$GREPCMD "GCC")
		!isEmpty(GCCTEST) {
			CONFIG += gcc
			TEST_GCC2 = $$system($$QMAKE_CXX -dumpversion | $$GREPCMD "^2\.")
			!isEmpty(TEST_GCC2) {
				error(GCC installation is too old. Use GCC 3.X.X or newer!)
			}
			TEST_GCC4 = $$system($$QMAKE_CXX -dumpversion | $$GREPCMD "^4\.")
			!isEmpty(TEST_GCC4) {
				CONFIG += gcc4
				TEST_GCC402 = $$system($$QMAKE_CXX -dumpversion | $$GREPCMD "^4\.([1-9]|0\.[2-9])")
				!isEmpty(TEST_GCC402) {
					CONFIG += gcc402
				}
			}
		}
		!gcc {
			ICCTEST = $$system($$QMAKE_CXX --version | $$GREPCMD "(ICC)")
			!isEmpty(ICCTEST) {
				CONFIG += icc
				TEST_ICC9 = $$system($$QMAKE_CXX -dumpversion | $$GREPCMD "^9\.")
				!isEmpty(TEST_ICC9) {
					CONFIG += icc9
				} else {
					message(Warning! Unknown compiler.)
				}
			}
		}
	}
}

unix {
	OPTIONAL_INCLUDES += $$ASPELLINC
	OPTIONAL_LFLAGS += $$ASPELLLIB
	!mac {
		gcc {
			system(which apgcc > /dev/null && { exit 0; }) {
				QMAKE_CC = apgcc
			}
			system(which apg++ > /dev/null && { exit 0; }) {
				QMAKE_CXX = apg++
				QMAKE_LINK = apg++
			}
		}
	}
}

mac {
	!link_prl:CONFIG += link_prl
}

msvc {
	HUSKYLIBS = -lhuskymvc -lfconfmvc
} else {
	HUSKYLIBS = -lhusky -lfidoconfig
}
