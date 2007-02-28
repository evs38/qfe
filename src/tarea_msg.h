/***************************************************************************
 *   Copyright (C) 2005-2007 by Alexander Shiyan                           *
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

#ifndef _TAREA_MSG_H_
#define _TAREA_MSG_H_

#include <qdir.h>
#include <qfile.h>

#include "tarea.h"
#include "ttypes.h"
#include "funchider.h"
#include "utils.h"

#define MSG_FLAG_PVT		0x0001
#define MSG_FLAG_CRASH		0x0002
#define MSG_FLAG_RECEIVED	0x0004
#define MSG_FLAG_SENT		0x0008
#define MSG_FLAG_ATTACH		0x0010
#define MSG_FLAG_TRANSIT	0x0020
#define MSG_FLAG_ORPHAN		0x0040
#define MSG_FLAG_KILLSENT	0x0080
#define MSG_FLAG_LOCAL		0x0100
#define MSG_FLAG_HOLD		0x0200
#define MSG_FLAG_RESERVED	0x0400
#define MSG_FLAG_FREQ		0x0800
#define MSG_FLAG_RETRECREQ	0x1000
#define MSG_FLAG_RETREC		0x2000
#define MSG_FLAG_AUDITREQ	0x4000
#define MSG_FLAG_UPDREQ		0x8000

__PACKED_ATTR_PREF__ struct AreaItem_Msg_Header
{
	uint8_t from[36];
	uint8_t to[36];
	uint8_t subj[72];
	uint8_t datetime[20];
	bo16_t timesread;
	bo16_t destnode;
	bo16_t orignode;
	bo16_t cost;
	bo16_t orignet;
	bo16_t destnet;
	union
	{
		struct
		{
			bo16_t destzone;
			bo16_t origzone;
			bo16_t destpoint;
			bo16_t origpoint;
		} ftsc;
		struct
		{
			bo32_t date_written;
			bo32_t date_arrived;
		} opus;
	};
	bo16_t replyto;
	bo16_t attr;
	bo16_t reply1st;
} __PACKED_ATTR_SUFF__;

class TArea_Msg_PvtObject
{
public:
	QDir MsgPath;
	QFile LastRead;
	uint32_t MaxUID;
};

class TAreaItem_Msg_PvtObject
{
public:
	QString FileName;
	bool AddrFlagsOK;
};

bool InitArea_Msg(TArea*);
bool OpenArea_Msg(TArea*);
bool RescanArea_Msg(TArea*);
bool ReadArea_Msg(TArea*, uint32_t);
bool WriteArea_Msg(TArea*, uint32_t);
TMessage *AppendArea_Msg(TArea*, bool);
bool DeleteArea_Msg(TArea*, uint32_t);
bool KillArea_Msg(TArea*);
void CloseArea_Msg(TArea*);
void DoneArea_Msg(TArea*);

uint32_t GetLastReadArea_Msg(TArea*, uint32_t);
void SetLastReadArea_Msg(TArea*, uint32_t, uint32_t);

void MarkAsReadArea_Msg(TArea*, uint32_t);

#endif /* _TAREA_MSG_H_ */
