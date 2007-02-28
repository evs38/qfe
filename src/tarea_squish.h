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

#ifndef _TAREA_SQUISH_H_
#define _TAREA_SQUISH_H_

#include <qdir.h>
#include <qfile.h>

#include "tarea.h"
#include "ttypes.h"

#define SQUISH_MAGIC		0xafae4453

#define SQUISH_FRAME_NULL	0x0000

#define SQUISH_FRAME_NORMAL	0x0000
#define SQUISH_FRAME_FREE	0x0001
#define SQUISH_FRAME_RLE	0x0002
#define SQUISH_FRAME_LZW	0x0003

#define SQUISH_FLAG_PVT		0x00000001
#define SQUISH_FLAG_CRASH	0x00000002
#define SQUISH_FLAG_RECEIVED	0x00000004
#define SQUISH_FLAG_SENT	0x00000008
#define SQUISH_FLAG_ATTACH	0x00000010
#define SQUISH_FLAG_TRANSIT	0x00000020
#define SQUISH_FLAG_ORPHAN	0x00000040
#define SQUISH_FLAG_KILLSENT	0x00000080
#define SQUISH_FLAG_LOCAL	0x00000100
#define SQUISH_FLAG_HOLD	0x00000200
#define SQUISH_FLAG_RESERVED	0x00000400
#define SQUISH_FLAG_FREQ	0x00000800
#define SQUISH_FLAG_RETRECREQ	0x00001000
#define SQUISH_FLAG_RETREC	0x00002000
#define SQUISH_FLAG_AUDITREQ	0x00004000
#define SQUISH_FLAG_UPDREQ	0x00008000
#define SQUISH_FLAG_SCANNED	0x00010000
#define SQUISH_FLAG_UID		0x00020000
#define SQUISH_FLAG_PRINTED	0x00040000
#define SQUISH_FLAG_SEEN	0x00080000
#define SQUISH_FLAG_LOK		0x40000000

__PACKED_ATTR_PREF__ struct AreaItem_Squish_Frame
{
	bo32_t id;
	bo32_t next_frame;
	bo32_t prev_frame;
	bo32_t frame_length;
	bo32_t msg_length;
	bo32_t clen;
	bo16_t frame_type;
	bo16_t rsvd;
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct AreaItem_Squish_Header
{
	bo32_t attr;
	uint8_t from[36];
	uint8_t to[36];
	uint8_t subj[72];
	fidoaddr origaddr;
	fidoaddr destaddr;
	bo32_t date_written;
	bo32_t date_arrived;
	bo16_t utc_offset;
	bo32_t replyto;
	bo32_t replies[9];
	bo32_t umsgid;
	uint8_t fts_date[20];
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct AreaItem_Squish_Index
{
	bo32_t ofs;
	bo32_t umsgid;
	bo32_t hash;
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct AreaItem_Squish_Base
{
	bo16_t len;
	bo16_t rsvd1;
	bo32_t num_msg;
	bo32_t high_msg;
	bo32_t skip_msg;
	bo32_t high_water;
	bo32_t uid;
	uint8_t base[80];
	bo32_t begin_frame;
	bo32_t last_frame;
	bo32_t free_frame;
	bo32_t last_free_frame;
	bo32_t end_frame;
	bo32_t max_msg;
	bo16_t keep_days;
	bo16_t framesize;
	uint8_t rsvd2[124];
} __PACKED_ATTR_SUFF__;

class TArea_Squish_PvtObject
{
public:
	AreaItem_Squish_Base BaseInfo;

	FILE *SQD;
	QString SQDName;
	FILE *SQI;
	QString SQIName;

	QFile SQL;

	uint32_t MaxUID;
};

bool InitArea_Squish(TArea*);
bool OpenArea_Squish(TArea*);
bool RescanArea_Squish(TArea*);
bool ReadArea_Squish(TArea*, uint32_t);
bool WriteArea_Squish(TArea*, uint32_t);
TMessage *AppendArea_Squish(TArea*, bool);
bool DeleteArea_Squish(TArea*, uint32_t);
bool KillArea_Squish(TArea*);
void CloseArea_Squish(TArea*);
void DoneArea_Squish(TArea*);

uint32_t GetLastReadArea_Squish(TArea*, uint32_t);
void SetLastReadArea_Squish(TArea*, uint32_t, uint32_t);

void MarkAsReadArea_Squish(TArea*, uint32_t);

#endif /* _TAREA_SQUISH_H_ */
