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

#ifndef _TAREA_JAM_H_
#define _TAREA_JAM_H_

#include <qdir.h>
#include <qfile.h>

#include "tarea.h"
#include "ttypes.h"
#include "funchider.h"
#include "utils.h"

#define JAM_MAGIC		"JAM\0"
#define JAM_REVISION		1
#define JAM_MAX_DATA_LEN	100

#define JAM_FLAG_LOCAL		0x00000001
#define JAM_FLAG_INTRANSIT	0x00000002
#define JAM_FLAG_PRIVATE	0x00000004
#define JAM_FLAG_RCVD		0x00000008
#define JAM_FLAG_SENT		0x00000010
#define JAM_FLAG_KILLSENT	0x00000020
#define JAM_FLAG_ARCHIVESENT	0x00000040
#define JAM_FLAG_HOLD		0x00000080
#define JAM_FLAG_CRASH		0x00000100
#define JAM_FLAG_IMMEDIATE	0x00000200
#define JAM_FLAG_DIRECT		0x00000400
#define JAM_FLAG_GATE		0x00000800
#define JAM_FLAG_FILEREQUEST	0x00001000
#define JAM_FLAG_FILEATTACH	0x00002000
#define JAM_FLAG_TRUNCFILE	0x00004000
#define JAM_FLAG_KILLFILE	0x00008000
#define JAM_FLAG_RECEIPTREQ	0x00010000
#define JAM_FLAG_CONFIRMREQ	0x00020000
#define JAM_FLAG_ORPHAN		0x00040000
#define JAM_FLAG_ENCRYPT	0x00080000
#define JAM_FLAG_COMPRESS	0x00100000
#define JAM_FLAG_ESCAPED	0x00200000
#define JAM_FLAG_FPU		0x00400000
#define JAM_FLAG_TYPELOCAL	0x00800000
#define JAM_FLAG_TYPEECHO	0x01000000
#define JAM_FLAG_TYPENET	0x02000000
#define JAM_FLAG_READ		0x10000000
#define JAM_FLAG_NODISP		0x20000000
#define JAM_FLAG_LOCKED		0x40000000
#define JAM_FLAG_DELETED	0x80000000

__PACKED_ATTR_PREF__ struct AreaItem_Jam_HeaderInfo
{
	uint8_t signature[4];
	bo32_t datecreated;
	bo32_t modcounter;
	bo32_t activemsgs;
	bo32_t passwordcrc;
	bo32_t basemsgnum;
	bo32_t highwater;
	uint8_t reserved[996];
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct AreaItem_Jam_Header
{
	uint8_t signature[4];
	bo16_t revision;
	bo16_t reservedword;
	bo32_t subfieldlen;
	bo32_t timesread;
	bo32_t msgidcrc;
	bo32_t replycrc;
	bo32_t replyto;
	bo32_t reply1st;
	bo32_t replynext;
	bo32_t datewritten;
	bo32_t datereceived;
	bo32_t dateprocessed;
	bo32_t messagenumber;
	bo32_t attribute;
	bo32_t attribute2;
	bo32_t offset;
	bo32_t txtlen;
	bo32_t passwordcrc;
	bo32_t cost;
} __PACKED_ATTR_SUFF__;

#define JAMSFLD_OADDRESS	0
#define JAMSFLD_DADDRESS	1
#define JAMSFLD_SENDERNAME	2
#define JAMSFLD_RECVRNAME	3
#define JAMSFLD_MSGID		4
#define JAMSFLD_REPLYID		5
#define JAMSFLD_SUBJECT		6
#define JAMSFLD_PID		7
#define JAMSFLD_TRACE		8
#define JAMSFLD_ENCLFILE	9
#define JAMSFLD_ENCLFWALIAS	10
#define JAMSFLD_ENCLFREQ	11
#define JAMSFLD_ENCLFILEWC	12
#define JAMSFLD_ENCLINDFILE	13
#define JAMSFLD_EMBINDAT	1000
#define JAMSFLD_FTSKLUDGE	2000
#define JAMSFLD_SEENBY2D	2001
#define JAMSFLD_PATH2D		2002
#define JAMSFLD_FLAGS		2003
#define JAMSFLD_TZUTCINFO	2004
#define JAMSFLD_UNKNOWN		0xffff

__PACKED_ATTR_PREF__ struct AreaItem_Jam_SubfieldHeader
{
	bo16_t loid;
	bo16_t hiid;
	bo32_t datlen;
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct AreaItem_Jam_Subfield
{
	AreaItem_Jam_SubfieldHeader Hdr;
	uint8_t data[JAM_MAX_DATA_LEN];
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct AreaItem_Jam_Index
{
	bo32_t usercrc;
	bo32_t hdroffset;
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct AreaItem_Jam_Lastread
{
	bo32_t usercrc;
	bo32_t userid;
	bo32_t lastread;
	bo32_t highread;
} __PACKED_ATTR_SUFF__;

class TArea_Jam_PvtObject
{
public:
	AreaItem_Jam_HeaderInfo HdrInfo;

	FILE *JDT;
	QString JDTName;
	FILE *JDX;
	QString JDXName;
	FILE *JHR;
	QString JHRName;

	QFile JLR;

	uint32_t MaxUID;
};

bool InitArea_Jam(TArea*);
bool OpenArea_Jam(TArea*);
bool RescanArea_Jam(TArea*);
bool ReadArea_Jam(TArea*, uint32_t);
bool WriteArea_Jam(TArea*, uint32_t);
TMessage *AppendArea_Jam(TArea*, bool);
bool DeleteArea_Jam(TArea*, uint32_t);
bool KillArea_Jam(TArea*);
void CloseArea_Jam(TArea*);
void DoneArea_Jam(TArea*);

uint32_t GetLastReadArea_Jam(TArea*, uint32_t);
void SetLastReadArea_Jam(TArea*, uint32_t, uint32_t);

void MarkAsReadArea_Jam(TArea*, uint32_t);

#endif /* _TAREA_JAM_H_ */
