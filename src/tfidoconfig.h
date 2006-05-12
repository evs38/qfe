/***************************************************************************
 *   Copyright (C) 2005-2006 by Alexander Shiyan                           *
 *   shc@users.sourceforge.net                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _TFIDOCONFIG_H_
#define _TFIDOCONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FIDOCONF_VER1_4

#include <fidoconf/common.h>
#include <fidoconf/fidoconf.h>
#include <fidoconf/version.h>

#if defined(FC_VER_MAJOR)
#	if FC_VER_MAJOR == 0x01
#		if FC_VER_MINOR == 0x09
#			undef FIDOCONF_VER1_4
#			include <huskylib/huskylib.h>
#		endif
#	endif
#endif

#ifdef __cplusplus
}
#endif

#if defined (Q_OS_WIN)
#	undef read
#	undef write
#endif

#endif /* _TFIDOCONFIG_H_ */
