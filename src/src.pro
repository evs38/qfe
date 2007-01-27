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

TEMPLATE = app

CONFIG -= qt warn_on warn_off release debug debug_and_release debug_and_release_target
CONFIG += qt warn_on debug

!include(../qfe.pri) {
	error(Can't find ../qfe.pri)
}

debug_off {
	CONFIG -= warn_on debug
	CONFIG += warn_off release
}

release:debug:error(Configuration Error!)

!unix:!win32:!mac:error(Unable to determine target operating system!)

TARGET = $$MAINTARGET

!win32:!exists(.tmp) {
	system($$QMAKE_MKDIR .tmp)
}

DEPENDPATH += .. ts ui

INCLUDEPATH += .. $$OPTIONAL_INCLUDES

QMAKE_CLEAN += *.bak ui/*~
win32:QMAKE_CLEAN += qferc.o object_ld_script objmoc_ld_script

release {
	QMAKE_CLEAN += ts/*.qm
	langsupport:QMAKE_POST_LINK = @$$QT_DIR_BIN/lrelease -verbose src.pri
}

gcc {
	QMAKE_CFLAGS_DEBUG += -O0
	QMAKE_CXXFLAGS_DEBUG += -O0
	QMAKE_CFLAGS_RELEASE += -fomit-frame-pointer -ffast-math -fPIC
	QMAKE_CXXFLAGS_RELEASE += -fomit-frame-pointer -ffast-math -fPIC
	QMAKE_LFLAGS_DEBUG += -O1
	QMAKE_LFLAGS_RELEASE += --gc-sections -O1

	gcc4 {
		unix:QMAKE_CFLAGS_DEBUG += -Wextra
		unix:QMAKE_CXXFLAGS_DEBUG += -Wextra
		QMAKE_CFLAGS_RELEASE += -g0
		QMAKE_CXXFLAGS_RELEASE += -g0 -fvisibility-inlines-hidden
		gcc402:QMAKE_CXXFLAGS_RELEASE += -fvisibility=hidden
	}
}

icc {
	icc9 {
		QMAKE_CFLAGS_RELEASE -= -O2 -fexceptions
		QMAKE_CXXFLAGS_RELEASE -= -O2
		QMAKE_CFLAGS += -O0
		QMAKE_CXXFLAGS += -O0
		QMAKE_CFLAGS_DEBUG += -Wdeprecated -Wunused-function -Wcheck -wd1125
		QMAKE_CXXFLAGS_DEBUG += -Wdeprecated -Wunused-function -Wcheck -wd1125
		QMAKE_LFLAGS_DEBUG += -g
	}
	unix {
		INCLUDEPATH += /usr/local/include
		LIBS += -L/usr/local/lib
	}
}

!exists($(QTDIR)/bin/qembed*) {
	!exists(stdconfig.h):error('qembed' not found! Can't continue.)
}

!msvc:QMAKE_LFLAGS_RELEASE += -s
icc9:QMAKE_LFLAGS += -O0

UI_DIR = .tmp
OBJECTS_DIR = .tmp
MOC_DIR = .tmp

staticlibs {
	!gcc:!icc {
		!msvc:message(Can't link statically, tryes to link dynamic)
		CONFIG -= staticlibs
	}
	staticlibs {
		LIBS +=  -Wl,-static -Wl,--start-group $$HUSKYLIBS -Wl,--end-group -Wl,-Bdynamic
	}
}
!staticlibs {
	LIBS += $$HUSKYLIBS
}

LIBS += $$OPTIONAL_LIBS

QMAKE_CFLAGS += $$OPTIONAL_CFLAGS
QMAKE_CXXFLAGS += $$OPTIONAL_CXXFLAGS
QMAKE_LFLAGS += $$OPTIONAL_LFLAGS

!include(src.pri) {
	error(Can't find src.pri)
}

debug:DEFINES += ENABLE_DEBUG

unix:!mac {
	DEFINES += ENABLE_BINRELOC HOME_PATH=\"$$HOME_PATH\"
}

aspell {
	DEFINES += ENABLE_ASPELL
	LIBS += -laspell
}

unix {
	exists($(QTDIR)/bin/qembed) {
		stdconfig.target = stdconfig.h
		stdconfig.commands = cd ../cfg && $(QTDIR)/bin/qembed `find *.tpl *.lst` > ../src/stdconfig.h
		QMAKE_EXTRA_UNIX_TARGETS += stdconfig

		PRE_TARGETDEPS += stdconfig.h
	}
}

!win32 {
	prefix {
		mac {
			target.path = $$HOME_PATH
		} else {
			target.path = $$HOME_PATH/bin
		}

		langsupport {
			mac {
				translations.path = $$HOME_PATH/$(QMAKE_TARGET).app/Contents/Resources/translations
			} else {
				translations.path = $$HOME_PATH/share/$$MAINTARGET/translations
			}
			translations.extra = $(COPY_FILE) ts/*.qm $(INSTALL_ROOT)$$translations.path
			translations.uninstall = -"$(DEL_FILE) $(INSTALL_ROOT)$$translations.path/*.qm"
			tarnslations.depends = target
		}

		mac {
			readme.path = $$HOME_PATH/$(QMAKE_TARGET).app/Contents/Resources
		} else {
			readme.path = $$HOME_PATH/share/$$MAINTARGET
		}
		readme.files = ../AUTHORS ../COPYING ../README ../TODO ../FAQ ../ChangeLog ../qfe.lsm ../misc/config.sample

		mac {
			examples.path = $$HOME_PATH/$(QMAKE_TARGET).app/Contents/Resources/examples
		} else {
			examples.path = $$HOME_PATH/share/$$MAINTARGET/examples
		}
		examples.files = ../examples/*.bat
		examples.files += ../examples/*.sh

		!mac {
			desklink.path = $$HOME_PATH/share/applications
			desklink.files = ../misc/qfe.desktop

			deskicon1.path = $$HOME_PATH/share/icons
			deskicon1.files = img/qfe.xpm

			deskicon2.path = $$HOME_PATH/share/pixmaps
			deskicon2.files = img/qfe.xpm

			mime1.path = $$HOME_PATH/share/icons
			mime1.files = ../misc/mime/fidonet.xpm

			mime2.path = $$HOME_PATH/share/mimelnk/message
			mime2.files = ../misc/mime/*.desktop
		}

		INSTALLS += target readme examples
		!mac:INSTALLS += desklink deskicon1 deskicon2 mime1 mime2
		langsupport:INSTALLS += translations
	}
}

win32 {
	RC_FILE += qferc.rc
	pack:QMAKE_POST_LINK = upx -8 $(TARGET)
}

mac {
	RC_FILE += qfe.icns
	LIBS += -framework CoreFoundation
	QMAKE_INFO_PLIST = qfe.plist
}
