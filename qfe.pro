#***************************************************************************
#*   Copyright (C) 2005-2007 by Alexander Shiyan                           *
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

TEMPLATE = subdirs 

CONFIG -= qt warn_on warn_off release debug debug_and_release debug_and_release_target
CONFIG += qt warn_on debug

!include(qfe.pri) {
	error(Can't find qfe.pri)
}

debug_off {
	CONFIG -= warn_on debug
	CONFIG += warn_off release
}

release:debug:error(Configuration Error!)

SUBDIRS += src

DEPENDPATH += doc src

unix:!mac:exists(.distignore) {
	DISTARCH = "$$MAINTARGET"-"$$VERSION".tar.bz2
	DIRNAME = $$system(basename `pwd`)

	dist.target = dist
	dist.commands = @cd misc && sh -c "./make_dist.sh $$DISTARCH"

	update.target = update
	update.commands = @svn update

	commit.target = commit
	commit.commands = @svn commit

	QMAKE_EXTRA_UNIX_TARGETS += dist update commit
}

!quiet {
	!exists(Makefile) {
		win32:message(Target: Windows)
		unix:!mac:message(Target: Unix)
		mac:message(Target: MacOS X)

		message(Options used: $$CONFIG)

		aspell {
			message(Aspell support enabled.)
		} else {
			message(Aspell support disabled.)
		}

		prefix {
			win32 {
				message(Direct installation not supported yet.)
			} else {
				message(Prepared to install in $$HOME_PATH)
			}
		} else {
			message(Installation disabled.)
		}

		win32:pack:message(Using UPX compression after binary has been built.)

		!qt4 {
			msvc {
				message(Now type `nmake' to compile qfe.)
			} else {
				win32:gcc {
					message(Now type `mingw32-make' to compile qfe.)
				} else {
					message(Now type `make' to compile qfe.)
				}
			}
		}
		qt4:message(Project can`t be compiled with Qt4!)
	}
}
