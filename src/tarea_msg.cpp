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

#include "tarea_msg.h"

bool InitArea_Msg(TArea *Base)
{
	TArea_Msg_PvtObject *b_obj = new TArea_Msg_PvtObject();

	b_obj->MsgPath.setPath(Base->Path);

	b_obj->MsgPath.setNameFilter("*.msg");
	b_obj->MsgPath.setFilter(QDir::Files | QDir::Readable);
	b_obj->MsgPath.setSorting(QDir::IgnoreCase);

	b_obj->LastRead.setName(b_obj->MsgPath.filePath("lastread"));

	b_obj->MaxUID = 0;

	Base->AreaPvtData = b_obj;

	return b_obj->MsgPath.exists();
}

bool OpenArea_Msg(TArea *Base)
{
	TArea_Msg_PvtObject *b_obj = (TArea_Msg_PvtObject*)Base->AreaPvtData;

	return b_obj->MsgPath.isReadable();
}

bool RescanArea_Msg(TArea *Base)
{
	int32_t sz, id;
	QString s;
	FILE *pf;
	AreaItem_Msg_Header Header;
	TArea_Msg_PvtObject *b_obj = (TArea_Msg_PvtObject*)Base->AreaPvtData;

	b_obj->MsgPath.refresh();
	QStringList sl(b_obj->MsgPath.entryList());

	for (uint32_t i = 0; i < sl.count(); i++)
		sl[i] = sl[i].mid(0, sl[i].length() - 4).rightJustify(8, '0', true) + " " + sl[i];
	sl.sort();

	Base->BaseSize = 0;

	for (uint32_t i = 0; i < sl.count(); i++)
	{
		id = atoi(gettoken(sl[i], 1).ascii());
		s = b_obj->MsgPath.filePath(gettoken(sl[i], 2));
		if ((id > 0) && ((pf = fopen(s.ascii(), "rb")) != NULL))
		{
			if (fread((char*)&Header, sizeof(AreaItem_Msg_Header), 1, pf) == 1)
			{
				fseek(pf, 0, SEEK_END);
				sz = ftell(pf);
				Base->BaseSize += sz;
				sz -= sizeof(AreaItem_Msg_Header);
				if (sz > 0)
				{
					TMessage *it = AppendArea_Msg(Base, false);
					TAreaItem_Msg_PvtObject *obj = (TAreaItem_Msg_PvtObject*)it->MessagePvtData;
					obj->FileName = s;

					qstrncpy((char*)&it->from, (char*)&Header.from, MAX_FROM_NAME_LEN + 1);
					qstrncpy((char*)&it->to, (char*)&Header.to, MAX_TO_NAME_LEN + 1);
					qstrncpy((char*)&it->subj, (char*)&Header.subj, MAX_SUBJ_LEN + 1);

					Fts2DateTime(&Header.datetime[0], &it->dt);

					it->origaddr.zone = Base->DefZone;
					it->origaddr.net = Header.orignet;
					it->origaddr.node = Header.orignode;
					it->origaddr.point = 0;
					it->destaddr.zone = Base->DefZone;
					it->destaddr.net = Header.destnet;
					it->destaddr.node = Header.destnode;
					it->destaddr.point = 0;

					if (((Header.timesread | Header.cost) == (uint16_t)-1) && (Header.timesread != 0))
						it->origaddr.point = Header.timesread;

					Flags2Global(it->attr, Header.attr, MSG_FLAG_PVT, FLAG_PVT);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_CRASH, FLAG_CRASH);

					if ((Header.attr & (MSG_FLAG_KILLSENT | MSG_FLAG_LOCAL)) == 0)
						Flags2Global(it->attr, Header.attr, MSG_FLAG_RECEIVED, FLAG_RECV);

					Flags2Global(it->attr, Header.attr, MSG_FLAG_SENT, FLAG_SENT);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_ATTACH, FLAG_FILE);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_TRANSIT, FLAG_FWD);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_ORPHAN, FLAG_ORP);

					if ((Header.attr & MSG_FLAG_RECEIVED) == 0)
						Flags2Global(it->attr, Header.attr, MSG_FLAG_KILLSENT, FLAG_KILL);

					Flags2Global(it->attr, Header.attr, MSG_FLAG_LOCAL, FLAG_LOC);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_HOLD, FLAG_HLD);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_RESERVED, FLAG_DIR);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_FREQ, FLAG_FRQ);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_RETRECREQ, FLAG_RRQ);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_RETREC, FLAG_CPT);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_AUDITREQ, FLAG_ARQ);
					Flags2Global(it->attr, Header.attr, MSG_FLAG_UPDREQ, FLAG_URQ);

					if ((Header.attr & (MSG_FLAG_KILLSENT | MSG_FLAG_RECEIVED)) == (MSG_FLAG_KILLSENT | MSG_FLAG_RECEIVED))
						it->attr |= FLAG_READ;

					it->size = sz;

					it->uid = id;
					b_obj->MaxUID = QMAX(it->uid, b_obj->MaxUID);
				}
			}
			fclose(pf);
		} else
			debugmessage("Can`t open file " + s);
	}

	return true;
}

bool ReadArea_Msg(TArea *Base, uint32_t Index)
{
	FILE *pf;
	TMessage *it = Base->at(Index);
	TAreaItem_Msg_PvtObject *obj = (TAreaItem_Msg_PvtObject*)it->MessagePvtData;

	if ((pf = fopen(obj->FileName.ascii(), "rb")) != NULL)
	{
		fseek(pf, sizeof(AreaItem_Msg_Header), SEEK_SET);
		char *Buffer = new char[it->size + 1];
		fread(Buffer, 1, it->size, pf);
		*(Buffer + it->size) = '\0';
		Fts2CRLF(Buffer);

		int32_t CtlLen = 0;
		char *Last = NULL, *Ptr = Buffer + 1;

		if ((*Buffer == '\001') || (strncmp(Buffer, "AREA:", 5) == 0))
			while ((Ptr = strchr(Ptr, '\n')) != NULL)
			{
				Last = Ptr;
				if (*(Ptr + 1) != '\001')
					break;
				Ptr += 2;
			}

		if (Last != NULL)
			CtlLen = Last - Buffer;

		if (CtlLen > 0)
		{
			Base->CtlBuff = new uint8_t[CtlLen + 1];
			qstrncpy((char*)Base->CtlBuff, Buffer, CtlLen + 1);
			*(Base->CtlBuff + CtlLen) = '\0';

			char *tmp = (char*)Base->CtlBuff;
			while ((tmp = strchr(tmp, '\001')) != NULL)
			{
				qmemmove((void*)tmp, (void*)(tmp + 1), strlen(tmp + 1) + 1);
				tmp++;
			}
		}

		Base->TxtBuff = (uint8_t*)qstrdup(Buffer + CtlLen);

		if (!obj->AddrFlagsOK)
		{
			uint16_t tmppnt;
			fidoaddr tmpaddr;

			QString kl = GetKludge(Base->CtlBuff, "REPLY: ");
			if (!kl.isEmpty())
				if (str2addr(kl.mid(7), &tmpaddr))
				{
					it->destaddr.zone = tmpaddr.zone;
					it->destaddr.point = tmpaddr.point;
				}

			kl = GetKludge(Base->CtlBuff, "INTL ");
			if (!kl.isEmpty())
			{
				if (str2addr(kl.mid(5), &tmpaddr))
					it->destaddr.zone = tmpaddr.zone;
				if (str2addr(gettoken(kl.mid(5), 2), &tmpaddr))
					it->origaddr.zone = tmpaddr.zone;
			}

			kl = GetKludge(Base->CtlBuff, "MSGID: ");
			if (!kl.isEmpty())
				if (str2addr(kl.mid(7), &tmpaddr))
				{
					it->origaddr.zone = tmpaddr.zone;
					it->origaddr.point = tmpaddr.point;
				}

			kl = GetKludge(Base->CtlBuff, "TOPT ");
			if (!kl.isEmpty())
			{
				tmppnt = kl.mid(5).toUShort();
				if (tmppnt > 0)
					it->destaddr.point = tmpaddr.point;
			}

			kl = GetKludge(Base->CtlBuff, "FMPT ");
			if (!kl.isEmpty())
			{
				tmppnt = kl.mid(5).toUShort();
				if (tmppnt > 0)
					it->origaddr.point = tmpaddr.point;
			}

			it->attr = ExtendFlags(it->attr, GetKludge(Base->CtlBuff, "FLAGS "));

			obj->AddrFlagsOK = true;
		}

		delete Buffer;
		fclose(pf);
		return true;
	}

	return false;
}

bool WriteArea_Msg(TArea *Base, uint32_t Index)
{
	bool ret = false;
	FILE *pf;
	TMessage *it = Base->at(Index);
	TAreaItem_Msg_PvtObject *obj = (TAreaItem_Msg_PvtObject*)it->MessagePvtData;

	if ((pf = fopen(obj->FileName.ascii(), "wb")) != NULL)
	{
		AreaItem_Msg_Header Header;

		memset((char*)&Header, '\0', sizeof(AreaItem_Msg_Header));

		qstrncpy((char*)&Header.from, (char*)&it->from, MAX_FROM_NAME_LEN + 1);
		qstrncpy((char*)&Header.to, (char*)&it->to, MAX_TO_NAME_LEN + 1);
		qstrncpy((char*)&Header.subj, (char*)&it->subj, MAX_SUBJ_LEN + 1);

		memcpy(&Header.datetime, DateTime2Fts(it->dt).ascii(), MAX_DATE_LEN);

		Header.orignet = it->origaddr.net;
		Header.orignode = it->origaddr.node;
		Header.destnet = it->destaddr.net;
		Header.destnode = it->destaddr.node;

		if (it->origaddr.point > 0)
		{
			Header.timesread = it->origaddr.point;
			Header.cost = ~Header.timesread;
		}

		Global2Flags(Header.attr, it->attr, FLAG_PVT, MSG_FLAG_PVT);
		Global2Flags(Header.attr, it->attr, FLAG_CRASH, MSG_FLAG_CRASH);

		if ((it->attr & (FLAG_KILL | FLAG_LOC)) == 0)
			Global2Flags(Header.attr, it->attr, FLAG_RECV, MSG_FLAG_RECEIVED);

		Global2Flags(Header.attr, it->attr, FLAG_SENT, MSG_FLAG_SENT);
		Global2Flags(Header.attr, it->attr, FLAG_FILE, MSG_FLAG_ATTACH);
		Global2Flags(Header.attr, it->attr, FLAG_FWD, MSG_FLAG_TRANSIT);
		Global2Flags(Header.attr, it->attr, FLAG_ORP, MSG_FLAG_ORPHAN);

		if ((it->attr & FLAG_RECV) == 0)
			Global2Flags(Header.attr, it->attr, FLAG_KILL, MSG_FLAG_KILLSENT);

		Global2Flags(Header.attr, it->attr, FLAG_LOC, MSG_FLAG_LOCAL);
		Global2Flags(Header.attr, it->attr, FLAG_HLD, MSG_FLAG_HOLD);
		Global2Flags(Header.attr, it->attr, FLAG_DIR, MSG_FLAG_RESERVED);
		Global2Flags(Header.attr, it->attr, FLAG_FRQ, MSG_FLAG_FREQ);
		Global2Flags(Header.attr, it->attr, FLAG_RRQ, MSG_FLAG_RETRECREQ);
		Global2Flags(Header.attr, it->attr, FLAG_CPT, MSG_FLAG_RETREC);
		Global2Flags(Header.attr, it->attr, FLAG_ARQ, MSG_FLAG_AUDITREQ);
		Global2Flags(Header.attr, it->attr, FLAG_URQ, MSG_FLAG_UPDREQ);

		if ((it->attr & (FLAG_READ | FLAG_LOC)) == FLAG_READ)
			Header.attr = Header.attr | (MSG_FLAG_KILLSENT | MSG_FLAG_RECEIVED);

		if (fwrite((char*)&Header, sizeof(AreaItem_Msg_Header), 1, pf) == 1)
		{
			QString AppendBuff = Base->NormalizeCtl(it, (uint8_t*)Base->CtlBuff);

			if (!AppendBuff.isEmpty())
				AppendBuff.append("\n").append((char*)Base->CtlBuff);
			else
				AppendBuff = (char*)Base->CtlBuff;

			if ((it->attr & FLAG_IMM) == FLAG_IMM)
				AppendBuff = HandleFlag(AppendBuff, FLAG_IMM);

			char *Ctl = qstrdup((const char*)Base->Append001(AppendBuff).append("\n").ascii());
			CRLF2Fts(Ctl);

			if (fwrite(Ctl, 1, strlen(Ctl), pf) == strlen(Ctl))
			{
				char *Txt = qstrdup((const char*)Base->TxtBuff);
				CRLF2Fts(Txt);

				if (fwrite(Txt, 1, strlen(Txt) + 1, pf) == (strlen(Txt) + 1))
					ret = true;

				delete Txt;
			}

			delete Ctl;
		}

		fclose(pf);
	}

	return ret;
}

TMessage *AppendArea_Msg(TArea *Base, bool isNew)
{
	TMessage *it = new TMessage(Base);
	TArea_Msg_PvtObject *b_obj = (TArea_Msg_PvtObject*)Base->AreaPvtData;
	TAreaItem_Msg_PvtObject *obj = new TAreaItem_Msg_PvtObject();

	if (isNew)
		for (;;)
		{
			it->uid = ++b_obj->MaxUID;
			obj->FileName = b_obj->MsgPath.filePath(QString::number(b_obj->MaxUID) + ".msg");
			if (!QFile::exists(obj->FileName))
				break;
		}

	obj->AddrFlagsOK = false;

	it->MessagePvtData = obj;
	Base->append(it);

	return it;
}

bool DeleteArea_Msg(TArea *Base, uint32_t Index)
{
	TAreaItem_Msg_PvtObject *obj = (TAreaItem_Msg_PvtObject*)Base->at(Index)->MessagePvtData;

	return QFile::remove(obj->FileName);
}

bool KillArea_Msg(TArea *Base)
{
	bool ret = true;

	TArea_Msg_PvtObject *b_obj = (TArea_Msg_PvtObject*)Base->AreaPvtData;

	for (uint32_t i = 0; i < Base->count(); i++)
		ret = ret && QFile::remove(((TAreaItem_Msg_PvtObject*)(Base->at(i)->MessagePvtData))->FileName);

	return ret && b_obj->LastRead.remove();
}

void CloseArea_Msg(TArea*)
{
	/* Dummy */
}

void DoneArea_Msg(TArea *Base)
{
	for (uint32_t i = 0; i < Base->count(); i++)
		delete (TAreaItem_Msg_PvtObject*)Base->at(i)->MessagePvtData;

	delete (TArea_Msg_PvtObject*)Base->AreaPvtData;
}

uint32_t GetLastReadArea_Msg(TArea *Base, uint32_t userid)
{
	uint16_t ret = 0;
	TArea_Msg_PvtObject *b_obj = (TArea_Msg_PvtObject*)Base->AreaPvtData;

	if (FileOpenFunc(&b_obj->LastRead, IO_ReadOnly))
	{
		if (b_obj->LastRead.at(userid * sizeof(uint16_t)))
			if (b_obj->LastRead.readBlock((char*)&ret, sizeof(uint16_t)) != sizeof(uint16_t))
				ret = 0;
		FileCloseFunc(&b_obj->LastRead);
	}

	return (uint32_t)ret;
}

void SetLastReadArea_Msg(TArea *Base, uint32_t userid, uint32_t Index)
{
	uint16_t tmp = (uint16_t)Index, nul = 0;
	TArea_Msg_PvtObject *b_obj = (TArea_Msg_PvtObject*)Base->AreaPvtData;

	if (FileOpenFunc(&b_obj->LastRead, IO_ReadWrite))
	{
		if (userid > 0) while (!b_obj->LastRead.at(userid * sizeof(uint16_t)))
		{
			b_obj->LastRead.at(b_obj->LastRead.size());
			b_obj->LastRead.writeBlock((char*)&nul, sizeof(uint16_t));
		}
		b_obj->LastRead.writeBlock((char*)&tmp, sizeof(uint16_t));
		FileCloseFunc(&b_obj->LastRead);
	}
}

void MarkAsReadArea_Msg(TArea *Base, uint32_t Index)
{
	FILE *pf;
	AreaItem_Msg_Header Header;
	TMessage *it = Base->at(Index);
	TAreaItem_Msg_PvtObject *obj = (TAreaItem_Msg_PvtObject*)it->MessagePvtData;

	if ((pf = fopen(obj->FileName.ascii(), "r+b")) != NULL)
	{
		if (fread((char*)&Header, sizeof(AreaItem_Msg_Header), 1, pf) == 1)
			if ((Header.attr & MSG_FLAG_LOCAL) == 0)
			{
				Header.attr = Header.attr | (MSG_FLAG_KILLSENT | MSG_FLAG_RECEIVED);
				rewind(pf);
				fwrite((char*)&Header, sizeof(AreaItem_Msg_Header), 1, pf);
			}

		fclose(pf);
	}
}
