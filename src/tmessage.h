/***************************************************************************
 *   Copyright (C) 2005-2008 by Alexander Shiyan                           *
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

#ifndef _TMESSAGE_H_
#define _TMESSAGE_H_

#include <qstring.h>

#include "ttypes.h"

class TMessage;

#include "tarea.h"
#include "tmessageitem.h"

#define MAX_FROM_NAME_LEN	36
#define MAX_TO_NAME_LEN		36
#define MAX_SUBJ_LEN		72
#define MAX_DATE_LEN		20

#define FLAG_PVT		0x00000001
#define FLAG_CRASH		0x00000002
#define FLAG_RECV		0x00000004
#define FLAG_SENT		0x00000008
#define FLAG_FILE		0x00000010
#define FLAG_FWD		0x00000020
#define FLAG_ORP		0x00000040
#define FLAG_KILL		0x00000080
#define FLAG_LOC		0x00000100
#define FLAG_HLD		0x00000200
#define FLAG_DIR		0x00000400
#define FLAG_FRQ		0x00000800
#define FLAG_RRQ		0x00001000
#define FLAG_CPT		0x00002000
#define FLAG_ARQ		0x00004000
#define FLAG_URQ		0x00008000
#define FLAG_IMM		0x00010000
#define FLAG_LOK		0x00020000
#define FLAG_ENC		0x00040000
#define FLAG_CMP		0x00080000
#define FLAG_ESC		0x00100000
#define FLAG_READ		0x00200000

class TMessage
{
public:
	TMessage(TArea*);

	uint8_t from[MAX_FROM_NAME_LEN + 1];
	uint8_t to[MAX_TO_NAME_LEN + 1];
	uint8_t subj[MAX_SUBJ_LEN + 1];
	fidoaddr origaddr;
	fidoaddr destaddr;
	time_t dt;
	uint32_t attr;
	uint32_t size;
	uint32_t uid;

//	TMessage *Parent;

	TArea *Area;
	uint32_t AreaIndex;
	TMessageItem *MessageItem;

	void *MessagePvtData;

	bool Read();

	void MarkAsRead();

	void CopyAttributesTo(TMessage*);
};

#define Flags2Global(a,b,c,d)	a = (a | (((b & (c)) == 0) ? 0 : (d)))
#define Global2Flags(a,b,c,d)	a = (a | (((b & (c)) == 0) ? 0 : (d)))

#define isNetMail(x)		(x->Area->AreaType == AREATYPE_NETMAIL)
#define isEchoMail(x)		(x->Area->AreaType == AREATYPE_ECHOMAIL)

#define isReaded(x)		((x->attr & (FLAG_READ | FLAG_LOC)) != 0)

#endif /* _TMESSAGE_H_ */
