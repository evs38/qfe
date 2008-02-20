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

#include "tarea_squish.h"

bool CreateSQD(TArea *Base)
{
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	debugmessage("Create new SQD-file for area \"" + Base->Name + "\".");
	memset((char*)&b_obj->BaseInfo, '\0', sizeof(AreaItem_Squish_Base));
	b_obj->BaseInfo.len = sizeof(AreaItem_Squish_Base);
	b_obj->BaseInfo.uid = 1;
	qstrncpy((char*)&b_obj->BaseInfo.base, Base->Path.ascii(), sizeofrec(AreaItem_Squish_Base, base) + 1);
	b_obj->BaseInfo.begin_frame = SQUISH_FRAME_NULL;
	b_obj->BaseInfo.last_frame = SQUISH_FRAME_NULL;
	b_obj->BaseInfo.free_frame = SQUISH_FRAME_NULL;
	b_obj->BaseInfo.last_free_frame = SQUISH_FRAME_NULL;
	b_obj->BaseInfo.end_frame = sizeof(AreaItem_Squish_Base);
	b_obj->BaseInfo.framesize = sizeof(AreaItem_Squish_Frame);

	b_obj->SQD = freopen(b_obj->SQDName.ascii(), "w+b", b_obj->SQD);
	if (fwrite((char*)&b_obj->BaseInfo, sizeof(AreaItem_Squish_Base), 1, b_obj->SQD) != 1)
		return false;

	return true;
}

bool RegenerateSQI(TArea *Base)
{
	AreaItem_Squish_Index BaseIndex;
	AreaItem_Squish_Frame Frame;
	AreaItem_Squish_Header Header;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;
	uint32_t cnt = 0, ofs = b_obj->BaseInfo.begin_frame;

	debugmessage("Regenerate SQI-file for area \"" + Base->Name + "\".");

	fseek(b_obj->SQI, 0, SEEK_END);
	int32_t IdxSize = ftell(b_obj->SQI);
	char *IdxBuff = new char[IdxSize];
	rewind(b_obj->SQI);
	IdxSize = fread(IdxBuff, 1, IdxSize, b_obj->SQI);

	b_obj->SQI = freopen(b_obj->SQIName.ascii(), "w+b", b_obj->SQI);
	while (ofs > 0)
	{
		if (fseek(b_obj->SQD, ofs, SEEK_SET) != 0)
			break;
		if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
			break;
		if ((Frame.id == SQUISH_MAGIC) && (Frame.frame_type == SQUISH_FRAME_NORMAL))
			if (fread((char*)&Header, sizeof(AreaItem_Squish_Header), 1, b_obj->SQD) == 1)
			{
				int32_t i = 0;
				bool readed = false;
				while (i < IdxSize)
				{
					AreaItem_Squish_Index *TmpIdx = (AreaItem_Squish_Index*)(IdxBuff + i);
					if ((TmpIdx->ofs == ofs) && (TmpIdx->umsgid == Header.umsgid))
					{
						readed = (TmpIdx->hash & 0x80000000) != 0;
						break;
					}
					i += sizeof(AreaItem_Squish_Index);
				}
				BaseIndex.ofs = ofs;
				BaseIndex.umsgid = Header.umsgid;
				BaseIndex.hash = Calculate_Hash((const uint8_t*)&Header.to) | (readed ? 0x80000000 : 0);
				if (fwrite((char*)&BaseIndex, sizeof(AreaItem_Squish_Index), 1, b_obj->SQI) != 1)
					break;
				cnt++;
			}
		ofs = Frame.next_frame;
	}

	delete IdxBuff;

	if (cnt != b_obj->BaseInfo.num_msg)
	{
		debugmessage(QString("BaseInfo.num_msg(%1) & index records(%2) not equal in area \"%3\"!").arg(b_obj->BaseInfo.num_msg).arg(cnt).arg(Base->Name));

		/* Uncomment to repair BaseInfo.num_msg field */
		/*
		b_obj->BaseInfo.num_msg = cnt;
		b_obj->BaseInfo.high_msg = cnt;
		rewind(b_obj->SQD);
		fwrite((char*)&b_obj->BaseInfo, sizeof(AreaItem_Squish_Base), 1, b_obj->SQD);
		*/

		return false;
	}

	if (b_obj->BaseInfo.high_msg != b_obj->BaseInfo.num_msg)
	{
		b_obj->BaseInfo.high_msg = b_obj->BaseInfo.num_msg;
		rewind(b_obj->SQD);
		fwrite((char*)&b_obj->BaseInfo, sizeof(AreaItem_Squish_Base), 1, b_obj->SQD);
	}

	return true;
}

uint32_t FindFreeFrame(TArea *Base, uint32_t NeedSize)
{
	AreaItem_Squish_Frame Frame;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;
	uint32_t ret = b_obj->BaseInfo.end_frame, BestSize = 0, ofs = b_obj->BaseInfo.free_frame;

	while (ofs > 0)
	{
		if (fseek(b_obj->SQD, ofs, SEEK_SET) != 0)
			break;
		if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
			break;
		if ((Frame.id == SQUISH_MAGIC) && (Frame.frame_type == SQUISH_FRAME_FREE))
		{
			if ((Frame.frame_length >= NeedSize) && ((BestSize == 0) || ((BestSize > 0) && (Frame.frame_length < BestSize))))
			{
				ret = ofs;
				BestSize = Frame.frame_length;
				break;
			}
		} else
			break;
		ofs = Frame.next_frame;
	}

	debugmessage(QString((ret != b_obj->BaseInfo.end_frame) ? "Free frame found at %1" : "No free frames. Frame position set at %1").arg(ret));

	return ret;
}

bool RemoveFrameFromChain(TArea *Base, uint32_t Ofs, bool isFree)
{
	bool ret = false;
	AreaItem_Squish_Frame Frame;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	debugmessage(QString("Remove frame at %1 from %2 frames chain.").arg(Ofs).arg(isFree ? "free" : "normal"));

	for (;;)
	{
		if (fseek(b_obj->SQD, Ofs, SEEK_SET) != 0)
			break;
		if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
			break;

		uint32_t PrevOfs = Frame.prev_frame, NextOfs = Frame.next_frame;

		/* Update previous frame info */
		if (PrevOfs > 0)
		{
			if (fseek(b_obj->SQD, PrevOfs, SEEK_SET) != 0)
				break;
			if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
				break;
			Frame.next_frame = (NextOfs > 0) ? NextOfs : SQUISH_FRAME_NULL;
			if (fseek(b_obj->SQD, PrevOfs, SEEK_SET) != 0)
				break;
			if (fwrite((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
				break;
		}


		/* Update next frame info */
		if (NextOfs > 0)
		{
			if (fseek(b_obj->SQD, NextOfs, SEEK_SET) != 0)
				break;
			if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
				break;
			Frame.prev_frame = (PrevOfs > 0) ? PrevOfs : SQUISH_FRAME_NULL;
			if (fseek(b_obj->SQD, NextOfs, SEEK_SET) != 0)
				break;
			if (fwrite((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
				break;
		}

		/* Temporaty mark frame as free */
		if (!isFree)
		{
			if (fseek(b_obj->SQD, Ofs, SEEK_SET) != 0)
				break;
			if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
				break;
			Frame.frame_type = SQUISH_FRAME_FREE;
			fwrite((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD);
		}

		/* Update base info */
		if (isFree)
		{
			if (Ofs == b_obj->BaseInfo.free_frame)
				b_obj->BaseInfo.free_frame = (NextOfs > 0) ? NextOfs : SQUISH_FRAME_NULL;
			if (Ofs == b_obj->BaseInfo.last_free_frame)
				b_obj->BaseInfo.last_free_frame = (PrevOfs > 0) ? PrevOfs : SQUISH_FRAME_NULL;
		} else {
			if (Ofs == b_obj->BaseInfo.begin_frame)
				b_obj->BaseInfo.begin_frame = (NextOfs > 0) ? NextOfs : SQUISH_FRAME_NULL;
			if (Ofs == b_obj->BaseInfo.last_frame)
				b_obj->BaseInfo.last_frame = (PrevOfs > 0) ? PrevOfs : SQUISH_FRAME_NULL;
		}
		rewind(b_obj->SQD);
		if (fwrite((char*)&b_obj->BaseInfo, sizeof(AreaItem_Squish_Base), 1, b_obj->SQD) == 1)
			ret = true;

		break;
	}

	return ret;
}

bool AddFrameToChain(TArea *Base, uint32_t Ofs, bool isFree)
{
	bool ret = false;
	AreaItem_Squish_Frame Frame;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	debugmessage(QString("Add frame at %1 to %2 frames chain.").arg(Ofs).arg(isFree ? "free" : "normal"));

	for (;;)
	{
		uint32_t PrevOfs = isFree ? b_obj->BaseInfo.last_free_frame : b_obj->BaseInfo.last_frame;

		/* Update last frame */
		if (PrevOfs > 0)
		{
			if (fseek(b_obj->SQD, PrevOfs, SEEK_SET) != 0)
				break;
			if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
				break;
			Frame.next_frame = Ofs;
			if (fseek(b_obj->SQD, PrevOfs, SEEK_SET) != 0)
				break;
			if (fwrite((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
				break;
		}

		/* Update current frame with previous info & type */
		if (fseek(b_obj->SQD, Ofs, SEEK_SET) != 0)
			break;
		if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
			break;
		Frame.prev_frame = PrevOfs;
		Frame.next_frame = 0;
		Frame.frame_type = isFree ? SQUISH_FRAME_FREE : SQUISH_FRAME_NORMAL;
		if (fseek(b_obj->SQD, Ofs, SEEK_SET) != 0)
			break;
		if (fwrite((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
			break;

		/* Update base info */
		if (isFree)
		{
			if (b_obj->BaseInfo.free_frame == 0)
				b_obj->BaseInfo.free_frame = Ofs;
			b_obj->BaseInfo.last_free_frame = Ofs;
		} else {
			if (b_obj->BaseInfo.begin_frame == 0)
				b_obj->BaseInfo.begin_frame = Ofs;
			b_obj->BaseInfo.last_frame = Ofs;
		}
		rewind(b_obj->SQD);
		if (fwrite((char*)&b_obj->BaseInfo, sizeof(AreaItem_Squish_Base), 1, b_obj->SQD) == 1)
			ret = true;

		break;
	}

	return ret;
}

bool InitArea_Squish(TArea *Base)
{
	TArea_Squish_PvtObject *b_obj = new TArea_Squish_PvtObject();

	b_obj->SQDName = Base->Path + ".sqd";
	b_obj->SQIName = Base->Path + ".sqi";

	b_obj->SQD = NULL;
	b_obj->SQI = NULL;

	b_obj->SQL.setName(Base->Path + ".sql");

	memset((char*)&b_obj->BaseInfo, '\0', sizeof(AreaItem_Squish_Base));

	b_obj->MaxUID = 0;

	Base->AreaPvtData = b_obj;

	QFileInfo fi(b_obj->SQDName);

	return (fi.dir(true).exists() && fi.dir(true).isReadable());
}

bool OpenArea_Squish(TArea *Base)
{
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	b_obj->SQD = fopen(b_obj->SQDName.ascii(), QFile::exists(b_obj->SQDName) ? "r+b" : "w+b");
	b_obj->SQI = fopen(b_obj->SQIName.ascii(), QFile::exists(b_obj->SQIName) ? "r+b" : "w+b");

	return (b_obj->SQD != NULL) && (b_obj->SQI != NULL);
}

bool RescanArea_Squish(TArea *Base)
{
	bool DoCreateSQD = true;
	AreaItem_Squish_Index BaseIndex;
	AreaItem_Squish_Frame Frame;
	AreaItem_Squish_Header Header;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	Base->BaseSize = 0;
	if (fseek(b_obj->SQD, 0, SEEK_END) == 0)
		Base->BaseSize += ftell(b_obj->SQD);
	if (fseek(b_obj->SQI, 0, SEEK_END) == 0)
		Base->BaseSize += ftell(b_obj->SQI);

	rewind(b_obj->SQD);
	if (fread((char*)&b_obj->BaseInfo, sizeof(AreaItem_Squish_Base), 1, b_obj->SQD) == 1)
		if ((b_obj->BaseInfo.len == sizeof(AreaItem_Squish_Base)) && (b_obj->BaseInfo.framesize == sizeof(AreaItem_Squish_Frame)))
		{
			fseek(b_obj->SQD, 0, SEEK_END);
			if (ftell(b_obj->SQD) != (int32_t)b_obj->BaseInfo.end_frame)
				debugmessage("BaseInfo.end_frame & filesize not equal in area \"" + Base->Name + "\"!");
			DoCreateSQD = false;
		}

	if (DoCreateSQD)
	{
		if (!CreateSQD(Base))
			return false;
		if (!RegenerateSQI(Base))
			return false;
	}

	fseek(b_obj->SQI, 0, SEEK_END);
	if ((ftell(b_obj->SQI) / sizeof(AreaItem_Squish_Index)) != b_obj->BaseInfo.num_msg)
		if (!RegenerateSQI(Base))
			return false;

	rewind(b_obj->SQI);
	for (uint32_t i = 0; i < b_obj->BaseInfo.num_msg; i++)
	{
		if (fread((char*)&BaseIndex, sizeof(AreaItem_Squish_Index), 1, b_obj->SQI) != 1)
			break;
		if (fseek(b_obj->SQD, BaseIndex.ofs, SEEK_SET) != 0)
			break;
		if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
			break;
		if ((Frame.id == SQUISH_MAGIC) && (Frame.frame_type == SQUISH_FRAME_NORMAL))
			if (fread((char*)&Header, sizeof(AreaItem_Squish_Header), 1, b_obj->SQD) == 1)
			{
				TMessage *it = AppendArea_Squish(Base, false);

				qstrncpy((char*)&it->from, (char*)&Header.from, MAX_FROM_NAME_LEN + 1);
				qstrncpy((char*)&it->to, (char*)&Header.to, MAX_TO_NAME_LEN + 1);
				qstrncpy((char*)&it->subj, (char*)&Header.subj, MAX_SUBJ_LEN + 1);

				it->origaddr.zone = Header.origaddr.zone;
				it->origaddr.net = Header.origaddr.net;
				it->origaddr.node = Header.origaddr.node;
				it->origaddr.point = Header.origaddr.point;
				it->destaddr.zone = Header.destaddr.zone;
				it->destaddr.net = Header.destaddr.net;
				it->destaddr.node = Header.destaddr.node;
				it->destaddr.point = Header.destaddr.point;

				uint32_t tmp = Header.date_written;
				if (!Opus2DateTime(tmp, &it->dt))
					Fts2DateTime(&Header.fts_date[0], &it->dt);

				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_PVT, FLAG_PVT);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_RECEIVED, FLAG_RECV);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_SENT, FLAG_SENT);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_ATTACH, FLAG_FILE);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_TRANSIT, FLAG_FWD);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_ORPHAN, FLAG_ORP);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_KILLSENT, FLAG_KILL);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_LOCAL, FLAG_LOC);

				if ((Header.attr & (SQUISH_FLAG_CRASH | SQUISH_FLAG_HOLD)) != (SQUISH_FLAG_CRASH | SQUISH_FLAG_HOLD))
				{
					Flags2Global(it->attr, Header.attr, SQUISH_FLAG_CRASH, FLAG_CRASH);
					Flags2Global(it->attr, Header.attr, SQUISH_FLAG_HOLD, FLAG_HLD);
				} else
					it->attr |= FLAG_DIR;

				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_FREQ, FLAG_FRQ);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_RETRECREQ, FLAG_RRQ);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_RETREC, FLAG_CPT);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_AUDITREQ, FLAG_ARQ);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_UPDREQ, FLAG_URQ);
				Flags2Global(it->attr, Header.attr, SQUISH_FLAG_LOK, FLAG_LOK);

				if ((Header.attr & SQUISH_FLAG_LOCAL) == 0)
					Flags2Global(it->attr, BaseIndex.hash, 0x80000000, FLAG_READ);

				it->size = Frame.msg_length - sizeof(AreaItem_Squish_Header);

				it->uid = Header.umsgid;
				b_obj->MaxUID = QMAX(it->uid, b_obj->MaxUID);
			}
	}

	b_obj->MaxUID = QMAX(b_obj->BaseInfo.num_msg, b_obj->MaxUID);

	return true;
}

bool ReadArea_Squish(TArea *Base, uint32_t Index)
{
	bool ret = false;
	AreaItem_Squish_Index BaseIndex;
	AreaItem_Squish_Frame Frame;
	TMessage *it = Base->at(Index);
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	if (fseek(b_obj->SQI, Index * sizeof(AreaItem_Squish_Index), SEEK_SET) == 0)
	{
		if (fread((char*)&BaseIndex, sizeof(AreaItem_Squish_Index), 1, b_obj->SQI) == 1)
		{
			fseek(b_obj->SQD, BaseIndex.ofs, SEEK_SET);
			if (fread((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
				return false;

			int32_t CtlLen = Frame.clen - 1;
			if (CtlLen > 0)
			{
				fseek(b_obj->SQD, sizeof(AreaItem_Squish_Header) + 1, SEEK_CUR);
				Base->CtlBuff = new uint8_t[CtlLen + 1];
				if ((int32_t)fread(Base->CtlBuff, 1, CtlLen, b_obj->SQD) == CtlLen)
				{
					*(Base->CtlBuff + CtlLen) = '\0';

					char *tmp = (char*)Base->CtlBuff;
					while ((tmp = strchr(tmp, '\001')) != NULL)
						*tmp++ = '\n';

					it->attr = ExtendFlags(it->attr, GetKludge(Base->CtlBuff, "FLAGS "));
				}
			}

			int32_t TxtLen = Frame.msg_length - (Frame.clen + sizeof(AreaItem_Squish_Header));
			fseek(b_obj->SQD, BaseIndex.ofs + sizeof(AreaItem_Squish_Frame) + sizeof(AreaItem_Squish_Header) + CtlLen + 1, SEEK_SET);
			Base->TxtBuff = new uint8_t[TxtLen + 1];
			if ((int32_t)fread(Base->TxtBuff, 1, TxtLen, b_obj->SQD) == TxtLen)
			{
				*(Base->TxtBuff + TxtLen) = '\0';

				Fts2CRLF((char*)Base->TxtBuff);

				ret = true;
			}
		}
	} else
		debugmessage("Can't seek Squish message index " + QString::number(Index) + " in area \"" + Base->Name + "\"!");

	return ret;
}

bool WriteArea_Squish(TArea *Base, uint32_t Index)
{
	bool ret = false;
	AreaItem_Squish_Index BaseIndex;
	AreaItem_Squish_Frame Frame;
	AreaItem_Squish_Header Header;
	TMessage *it = Base->at(Index);
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	memset((char*)&Header, '\0', sizeof(AreaItem_Squish_Header));

	Header.umsgid = it->uid;

	qstrncpy((char*)&Header.from, (char*)&it->from, MAX_FROM_NAME_LEN + 1);
	qstrncpy((char*)&Header.to, (char*)&it->to, MAX_TO_NAME_LEN + 1);
	qstrncpy((char*)&Header.subj, (char*)&it->subj, MAX_SUBJ_LEN + 1);

	memcpy(&Header.fts_date, DateTime2Fts(it->dt).ascii(), MAX_DATE_LEN);
	uint32_t tmp;
	DateTime2Opus(it->dt, &tmp);
	Header.date_written = tmp;

	Header.origaddr.zone = it->origaddr.zone;
	Header.origaddr.net = it->origaddr.net;
	Header.origaddr.node = it->origaddr.node;
	Header.origaddr.point = it->origaddr.point;
	Header.destaddr.zone = it->destaddr.zone;
	Header.destaddr.net = it->destaddr.net;
	Header.destaddr.node = it->destaddr.node;
	Header.destaddr.point = it->destaddr.point;

	Global2Flags(Header.attr, it->attr, FLAG_PVT, SQUISH_FLAG_PVT);
	Global2Flags(Header.attr, it->attr, FLAG_RECV, SQUISH_FLAG_RECEIVED);
	Global2Flags(Header.attr, it->attr, FLAG_SENT, SQUISH_FLAG_SENT);
	Global2Flags(Header.attr, it->attr, FLAG_FILE, SQUISH_FLAG_ATTACH);
	Global2Flags(Header.attr, it->attr, FLAG_FWD, SQUISH_FLAG_TRANSIT);
	Global2Flags(Header.attr, it->attr, FLAG_ORP, SQUISH_FLAG_ORPHAN);
	Global2Flags(Header.attr, it->attr, FLAG_KILL, SQUISH_FLAG_KILLSENT);
	Global2Flags(Header.attr, it->attr, FLAG_LOC, SQUISH_FLAG_LOCAL);

	if ((it->attr & FLAG_DIR) == 0)
	{
		Global2Flags(Header.attr, it->attr, FLAG_CRASH, SQUISH_FLAG_CRASH);
		Global2Flags(Header.attr, it->attr, FLAG_HLD, SQUISH_FLAG_HOLD);
	} else
		Header.attr = Header.attr | (SQUISH_FLAG_CRASH | SQUISH_FLAG_HOLD);

	Global2Flags(Header.attr, it->attr, FLAG_FRQ, SQUISH_FLAG_FREQ);
	Global2Flags(Header.attr, it->attr, FLAG_RRQ, SQUISH_FLAG_RETRECREQ);
	Global2Flags(Header.attr, it->attr, FLAG_CPT, SQUISH_FLAG_RETREC);
	Global2Flags(Header.attr, it->attr, FLAG_ARQ, SQUISH_FLAG_AUDITREQ);
	Global2Flags(Header.attr, it->attr, FLAG_URQ, SQUISH_FLAG_UPDREQ);
	Global2Flags(Header.attr, it->attr, FLAG_LOK, SQUISH_FLAG_LOK);

	QString AppendBuff = Base->NormalizeCtl(it, (uint8_t*)Base->CtlBuff);

	if (!AppendBuff.isEmpty())
		AppendBuff.append("\n").append((char*)Base->CtlBuff);
	else
		AppendBuff = (char*)Base->CtlBuff;

	if ((it->attr & FLAG_IMM) == FLAG_IMM)
		AppendBuff = HandleFlag(AppendBuff, FLAG_IMM);

	char *Ctl = qstrdup((const char*)Base->Append001(AppendBuff).append("\n").ascii());
	CRLF2Fts(Ctl);

	char *Txt = qstrdup((const char*)Base->TxtBuff);
	CRLF2Fts(Txt);

	for (;;)
	{
		/* Mark existing item as free */
		if (fseek(b_obj->SQI, 0, SEEK_END) != 0)
			break;
		if ((int32_t)(Index * sizeof(AreaItem_Squish_Index)) > ftell(b_obj->SQI))
		{
			if (fseek(b_obj->SQI, Index * sizeof(AreaItem_Squish_Index), SEEK_SET) != 0)
				break;
			if (fread((char*)&BaseIndex, sizeof(AreaItem_Squish_Index), 1, b_obj->SQI) != 1)
				break;
			if (!RemoveFrameFromChain(Base, BaseIndex.ofs, false))
				break;
			if (!AddFrameToChain(Base, BaseIndex.ofs, true))
				break;
		}

		uint32_t ItemOfs = FindFreeFrame(Base, sizeof(AreaItem_Squish_Header) + strlen(Ctl) + strlen(Txt));

		if (ItemOfs != b_obj->BaseInfo.end_frame)
			if (!RemoveFrameFromChain(Base, ItemOfs, true))
				break;

		/* Fill new frame info */
		memset((char*)&Frame, '\0', sizeof(AreaItem_Squish_Frame));
		Frame.id = SQUISH_MAGIC;
		Frame.clen = strlen(Ctl);
		Frame.frame_length = sizeof(AreaItem_Squish_Header) + strlen(Ctl) + strlen(Txt);
		Frame.msg_length = sizeof(AreaItem_Squish_Header) + strlen(Ctl) + strlen(Txt);

		/* Write new frame */
		if (fseek(b_obj->SQD, ItemOfs, SEEK_SET) != 0)
			break;
		if (fwrite((char*)&Frame, sizeof(AreaItem_Squish_Frame), 1, b_obj->SQD) != 1)
			break;

		/* Write header */
		if (fwrite((char*)&Header, sizeof(AreaItem_Squish_Header), 1, b_obj->SQD) != 1)
			break;

		/* Write control info */
		if (fwrite(Ctl, 1, strlen(Ctl), b_obj->SQD) != strlen(Ctl))
			break;

		/* Write text */
		if (fwrite(Txt, 1, strlen(Txt), b_obj->SQD) != strlen(Txt))
			break;

		/* Add frame to normal chain */
		if (!AddFrameToChain(Base, ItemOfs, false))
			break;

		/* Update base info */
		if (fseek(b_obj->SQD, 0, SEEK_END) != 0)
			break;
		b_obj->BaseInfo.end_frame = ftell(b_obj->SQD);
		b_obj->BaseInfo.num_msg = Base->count();
		b_obj->BaseInfo.high_msg = b_obj->BaseInfo.num_msg;
		b_obj->BaseInfo.uid = b_obj->MaxUID;
		rewind(b_obj->SQD);
		if (fwrite((char*)&b_obj->BaseInfo, sizeof(AreaItem_Squish_Base), 1, b_obj->SQD) != 1)
			break;

		/* Rebuild index */
		ret = RegenerateSQI(Base);

		break;
	}

	delete Ctl;
	delete Txt;

	return ret;
}

TMessage *AppendArea_Squish(TArea *Base, bool isNew)
{
	TMessage *it = new TMessage(Base);
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	if (isNew)
		it->uid = ++b_obj->MaxUID;

	it->MessagePvtData = NULL;
	Base->append(it);

	return it;
}

bool DeleteArea_Squish(TArea *Base, uint32_t Index)
{
	AreaItem_Squish_Index BaseIndex;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	if (fseek(b_obj->SQI, Index * sizeof(AreaItem_Squish_Index), SEEK_SET) != 0)
		return false;

	if (fread((char*)&BaseIndex, sizeof(AreaItem_Squish_Index), 1, b_obj->SQI) != 1)
		return false;

	if (!RemoveFrameFromChain(Base, BaseIndex.ofs, false))
		return false;

	if (!AddFrameToChain(Base, BaseIndex.ofs, true))
		return false;

	rewind(b_obj->SQD);
	b_obj->BaseInfo.num_msg = b_obj->BaseInfo.num_msg - 1;
	b_obj->BaseInfo.high_msg = b_obj->BaseInfo.num_msg;
	if (fwrite((char*)&b_obj->BaseInfo, sizeof(AreaItem_Squish_Base), 1, b_obj->SQD) != 1)
		return false;

	return RegenerateSQI(Base);
}

bool KillArea_Squish(TArea *Base)
{
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	return QFile::remove(b_obj->SQDName) && QFile::remove(b_obj->SQIName) && b_obj->SQL.remove();
}

void CloseArea_Squish(TArea *Base)
{
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	fclose(b_obj->SQD);
	b_obj->SQD = NULL;
	fclose(b_obj->SQI);
	b_obj->SQI = NULL;
}

void DoneArea_Squish(TArea *Base)
{
	delete (TArea_Squish_PvtObject*)Base->AreaPvtData;
}

uint32_t GetLastReadArea_Squish(TArea *Base, uint32_t userid)
{
	uint32_t ret = 0;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	if (FileOpenFunc(&b_obj->SQL, IO_ReadOnly))
	{
		if (b_obj->SQL.at(userid * sizeof(uint32_t)))
			if (b_obj->SQL.readBlock((char*)&ret, sizeof(uint32_t)) != sizeof(uint32_t))
				ret = 0;
		FileCloseFunc(&b_obj->SQL);
	}

	return ret;
}

void SetLastReadArea_Squish(TArea *Base, uint32_t userid, uint32_t Index)
{
	uint32_t nul = 0;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	if (FileOpenFunc(&b_obj->SQL, IO_ReadWrite))
	{
		if (userid > 0) while (!b_obj->SQL.at(userid * sizeof(uint32_t)))
		{
			b_obj->SQL.at(b_obj->SQL.size());
			b_obj->SQL.writeBlock((char*)&nul, sizeof(uint32_t));
		}
		b_obj->SQL.writeBlock((char*)&Index, sizeof(uint32_t));
		FileCloseFunc(&b_obj->SQL);
	}
}

void MarkAsReadArea_Squish(TArea *Base, uint32_t Index)
{
	uint32_t ofs = Index * sizeof(AreaItem_Squish_Index);
	AreaItem_Squish_Index BaseIndex;
	TArea_Squish_PvtObject *b_obj = (TArea_Squish_PvtObject*)Base->AreaPvtData;

	if (fseek(b_obj->SQI, ofs, SEEK_SET) == 0)
		if (fread((char*)&BaseIndex, sizeof(AreaItem_Squish_Index), 1, b_obj->SQI) == 1)
		{
			BaseIndex.hash = BaseIndex.hash | 0x80000000;
			if (fseek(b_obj->SQI, ofs, SEEK_SET) == 0)
				fwrite((char*)&BaseIndex, sizeof(AreaItem_Squish_Index), 1, b_obj->SQI);
		}
}
