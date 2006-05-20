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

#include "tarea_jam.h"

bool InitArea_Jam(TArea *Base)
{
	TArea_Jam_PvtObject *b_obj = new TArea_Jam_PvtObject();

	b_obj->JDTName = Base->Path + ".jdt";
	b_obj->JDXName = Base->Path + ".jdx";
	b_obj->JHRName = Base->Path + ".jhr";

	b_obj->JDT = NULL;
	b_obj->JDX = NULL;
	b_obj->JHR = NULL;

	b_obj->JLR.setName(Base->Path + ".jlr");

	memset((char*)&b_obj->HdrInfo, '\0', sizeof(AreaItem_Jam_HeaderInfo));

	b_obj->MaxUID = 0;

	Base->AreaPvtData = b_obj;

	QFileInfo fi(b_obj->JDTName);

	return (fi.dir(true).exists() && fi.dir(true).isReadable());
}

bool OpenArea_Jam(TArea *Base)
{
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	b_obj->JDT = fopen(b_obj->JDTName.ascii(), /* QFile::exists(b_obj->JDTName) ? */"r+b"/* : "w+b"*/);
	b_obj->JDX = fopen(b_obj->JDXName.ascii(), /* QFile::exists(b_obj->JDXName) ? */"r+b"/* : "w+b"*/);
	b_obj->JHR = fopen(b_obj->JHRName.ascii(), /* QFile::exists(b_obj->JHRName) ? */"r+b"/* : "w+b"*/);

	return (b_obj->JDT != NULL) && (b_obj->JDX != NULL) && (b_obj->JHR != NULL);
}

bool RescanArea_Jam(TArea *Base)
{
	AreaItem_Jam_Index JamIndex;
	AreaItem_Jam_Header JamHeader;
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	rewind(b_obj->JHR);
	if (fread((char*)&b_obj->HdrInfo, sizeof(AreaItem_Jam_HeaderInfo), 1, b_obj->JHR) == 1)
	{
		if (strncmp((const char*)&b_obj->HdrInfo.signature, JAM_MAGIC, 4) == 0)
		{
			if (fseek(b_obj->JDX, (QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) * sizeof(AreaItem_Jam_Index), SEEK_SET) == 0)
			{
				uint32_t i;
				for (i = 0; i < b_obj->HdrInfo.activemsgs;)
				{
					if (fread((char*)&JamIndex, sizeof(AreaItem_Jam_Index), 1, b_obj->JDX) != 1)
						break;
					if (JamIndex.hdroffset == 0xffffffff)
						break;
					if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) != 0)
						break;
					if (fread((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR) != 1)
						break;
					if ((memcmp((const char*)&JamHeader.signature, JAM_MAGIC, 4) != 0) && (JamHeader.revision == JAM_REVISION))
						break;

					if ((JamHeader.attribute & JAM_FLAG_DELETED) != 0)
						continue;

					uint32_t sptr = 0;
					char *SubBuffer = new char[JamHeader.subfieldlen];

					if (fread(SubBuffer, 1, JamHeader.subfieldlen, b_obj->JHR) != JamHeader.subfieldlen)
					{
						delete SubBuffer;
						break;
					}

					TMessage *it = AppendArea_Jam(Base, false);

					while (sptr <= (JamHeader.subfieldlen - sizeof(AreaItem_Jam_SubfieldHeader)))
					{
						AreaItem_Jam_SubfieldHeader *SubfieldHeader = (AreaItem_Jam_SubfieldHeader*)(SubBuffer + sptr);
						AreaItem_Jam_Subfield * Subfield = (AreaItem_Jam_Subfield*)SubfieldHeader;

						if (SubfieldHeader->datlen > JAM_MAX_DATA_LEN)
							break;

						switch (SubfieldHeader->loid)
						{
							case JAMSFLD_OADDRESS:
							case JAMSFLD_DADDRESS:
								break;
							case JAMSFLD_SENDERNAME:
								qstrncpy((char*)it->from, (char*)&Subfield->data, QMIN(MAX_FROM_NAME_LEN, SubfieldHeader->datlen));
								break;
							case JAMSFLD_RECVRNAME:
								qstrncpy((char*)it->to, (char*)&Subfield->data, QMIN(MAX_TO_NAME_LEN, SubfieldHeader->datlen));
								break;
							case JAMSFLD_MSGID:
							case JAMSFLD_REPLYID:
								break;
							case JAMSFLD_SUBJECT:
								qstrncpy((char*)it->subj, (char*)&Subfield->data, QMIN(MAX_SUBJ_LEN, SubfieldHeader->datlen));
								break;
							case JAMSFLD_PID:
							case JAMSFLD_TRACE:
							case JAMSFLD_ENCLFILE:
							case JAMSFLD_ENCLFWALIAS:
							case JAMSFLD_ENCLFREQ:
							case JAMSFLD_ENCLFILEWC:
							case JAMSFLD_ENCLINDFILE:
							case JAMSFLD_EMBINDAT:
							case JAMSFLD_FTSKLUDGE:
							case JAMSFLD_SEENBY2D:
							case JAMSFLD_PATH2D:
							case JAMSFLD_FLAGS:
							case JAMSFLD_TZUTCINFO:
							case JAMSFLD_UNKNOWN:
							default:
								break;
						}

						sptr += sizeof(AreaItem_Jam_SubfieldHeader) + SubfieldHeader->datlen;
					}
					delete SubBuffer;

					it->dt = (time_t)JamHeader.datewritten;

					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_LOCAL, FLAG_LOC);

					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_PRIVATE, FLAG_PVT);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_RCVD, FLAG_RECV);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_SENT, FLAG_SENT);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_KILLSENT, FLAG_KILL);

					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_HOLD, FLAG_HLD);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_CRASH, FLAG_CRASH);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_IMMEDIATE, FLAG_IMM);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_DIRECT, FLAG_DIR);

					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_FILEREQUEST, FLAG_FRQ);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_FILEATTACH, FLAG_FILE);

					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_RECEIPTREQ, FLAG_RRQ);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_CONFIRMREQ, FLAG_CPT);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_ORPHAN, FLAG_ORP);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_ENCRYPT, FLAG_ENC);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_COMPRESS, FLAG_CMP);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_ESCAPED, FLAG_ESC);
					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_FPU, FLAG_IMM);

					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_READ, FLAG_READ);

					Flags2Global(it->attr, JamHeader.attribute, JAM_FLAG_LOCKED, FLAG_LOK);

					it->size = JamHeader.txtlen;

					it->uid = JamHeader.messagenumber;
					b_obj->MaxUID = QMAX(it->uid, b_obj->MaxUID);
					i++;
				}
				if (b_obj->HdrInfo.activemsgs != i)
					debugmessage(QString("HdrInfo.activemsgs(%1) & index records(%2) not equal in area \"%3\"!").arg(b_obj->HdrInfo.activemsgs).arg(i).arg(Base->Name));
			}
		}
	}

	return true;
}

bool ReadArea_Jam(TArea *Base, uint32_t Index)
{
	bool ret = false;
	AreaItem_Jam_Index JamIndex;
	AreaItem_Jam_Header JamHeader;
	TMessage *it = Base->at(Index);
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	if (fseek(b_obj->JDX, ((QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) + Index) * sizeof(AreaItem_Jam_Index), SEEK_SET) != 0)
		return false;

	if (fread((char*)&JamIndex, sizeof(AreaItem_Jam_Index), 1, b_obj->JDX) != 1)
		return false;

	if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) != 0)
		return false;

	if (fread((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR) == 1)
	{
		uint32_t sptr = 0;
		char *SubBuffer = new char[JamHeader.subfieldlen];

		if (fread(SubBuffer, 1, JamHeader.subfieldlen, b_obj->JHR) == JamHeader.subfieldlen)
		{
			bool fa = false, ta = false;
			QString tmpstr;
			QStringList KludgeBuff, PathBuff;

			while (sptr <= (JamHeader.subfieldlen - sizeof(AreaItem_Jam_SubfieldHeader)))
			{
				AreaItem_Jam_SubfieldHeader *SubfieldHeader = (AreaItem_Jam_SubfieldHeader*)(SubBuffer + sptr);
				AreaItem_Jam_Subfield * Subfield = (AreaItem_Jam_Subfield*)SubfieldHeader;

				if (SubfieldHeader->datlen > JAM_MAX_DATA_LEN)
					break;

				fidoaddr addr;
				switch (SubfieldHeader->loid)
				{
					case JAMSFLD_OADDRESS:
						if (str2addr((char*)&Subfield->data, &addr, SubfieldHeader->datlen))
						{
							it->origaddr.zone = addr.zone;
							it->origaddr.net = addr.net;
							it->origaddr.node = addr.node;
							it->origaddr.point = addr.point;
							fa = true;
						}
						break;
					case JAMSFLD_DADDRESS:
						if (str2addr((char*)&Subfield->data, &addr, SubfieldHeader->datlen))
						{
							it->destaddr.zone = addr.zone;
							it->destaddr.net = addr.net;
							it->destaddr.node = addr.node;
							it->destaddr.point = addr.point;
							ta = true;
						}
						break;
					case JAMSFLD_SENDERNAME:
						qstrncpy((char*)it->from, (char*)&Subfield->data, QMIN(MAX_FROM_NAME_LEN, SubfieldHeader->datlen));
						break;
					case JAMSFLD_RECVRNAME:
						qstrncpy((char*)it->to, (char*)&Subfield->data, QMIN(MAX_TO_NAME_LEN, SubfieldHeader->datlen));
						break;
					case JAMSFLD_MSGID:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "MSGID: " + tmpstr;
						break;
					case JAMSFLD_REPLYID:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "REPLY: " + tmpstr;
						break;
					case JAMSFLD_SUBJECT:
						qstrncpy((char*)it->subj, (char*)&Subfield->data, QMIN(MAX_SUBJ_LEN, SubfieldHeader->datlen));
						break;
					case JAMSFLD_PID:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "PID: " + tmpstr;
						break;
					case JAMSFLD_TRACE:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						PathBuff << "\001Via " + tmpstr;
						break;
					case JAMSFLD_ENCLFILE:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "ENCLFILE: " + tmpstr;
						break;
					case JAMSFLD_ENCLFWALIAS:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "ENCLFILEWALIAS: " + tmpstr;
						break;
					case JAMSFLD_ENCLFREQ:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "ENCLFREQ: " + tmpstr;
						break;
					case JAMSFLD_ENCLFILEWC:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "ENCLFILEWCARD: " + tmpstr;
						break;
					case JAMSFLD_ENCLINDFILE:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "ENCLINDIRFILE: " + tmpstr;
						break;
					case JAMSFLD_EMBINDAT:
						break;
					case JAMSFLD_FTSKLUDGE:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						if (tmpstr.startsWith("Via ") || tmpstr.startsWith("Recd") || tmpstr.startsWith("Forwarded"))
							PathBuff << "\001" + tmpstr;
						else
							KludgeBuff << tmpstr;
						break;
					case JAMSFLD_SEENBY2D:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						WrapLines(&PathBuff, tmpstr, "SEEN-BY: ", 80 - (1 + 9 + 1));
						break;
					case JAMSFLD_PATH2D:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						WrapLines(&PathBuff, tmpstr, "\001PATH: ", 80 - (1 + 6 + 1));
						break;
					case JAMSFLD_FLAGS:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "FLAGS " + tmpstr;
						it->attr = ExtendFlags(it->attr, " " + tmpstr);
						break;
					case JAMSFLD_TZUTCINFO:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						KludgeBuff << "TZUTC: " + tmpstr;
						break;
					case JAMSFLD_UNKNOWN:
					default:
						break;
				}
					sptr += sizeof(AreaItem_Jam_SubfieldHeader) + SubfieldHeader->datlen;
			}

			if (fa && ta)
			{
				tmpstr = Base->NormalizeCtl(it, NULL, false);
				if (!tmpstr.isEmpty())
					KludgeBuff << tmpstr;
			}

			Base->CtlBuff = (uint8_t*)qstrdup(KludgeBuff.join("\n"));
			Fts2CRLF((char*)Base->CtlBuff);

			if (fseek(b_obj->JDT, JamHeader.offset, SEEK_SET) == 0)
			{
				char *Txt = new char[JamHeader.txtlen + 1];
				if (fread(Txt, 1, JamHeader.txtlen, b_obj->JDT) == JamHeader.txtlen)
				{
					*(Txt + JamHeader.txtlen) = '\0';
					PathBuff.prepend(qstrdup(Txt));

					Base->TxtBuff = (uint8_t*)qstrdup(PathBuff.join("\n"));
					Fts2CRLF((char*)Base->TxtBuff);

					ret = true;
				}
				delete Txt;
			}
		}
		delete SubBuffer;
	}

	return ret;
}

bool WriteArea_Jam(TArea *Base, uint32_t Index)
{
	bool ret = false;
	AreaItem_Jam_Index JamIndex;
	AreaItem_Jam_Header JamHeader;
	TMessage *it = Base->at(Index);
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	memset((char*)&JamHeader, '\0', sizeof(AreaItem_Jam_Header));

	Global2Flags(JamHeader.attribute, it->attr, FLAG_LOC, JAM_FLAG_LOCAL);

	Global2Flags(JamHeader.attribute, it->attr, FLAG_PVT, JAM_FLAG_PRIVATE);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_RECV, JAM_FLAG_RCVD);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_SENT, JAM_FLAG_SENT);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_KILL, JAM_FLAG_KILLSENT);

	Global2Flags(JamHeader.attribute, it->attr, FLAG_HLD, JAM_FLAG_HOLD);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_CRASH, JAM_FLAG_CRASH);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_IMM, JAM_FLAG_IMMEDIATE);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_DIR, JAM_FLAG_DIRECT);

	Global2Flags(JamHeader.attribute, it->attr, FLAG_FRQ, JAM_FLAG_FILEREQUEST);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_FILE, JAM_FLAG_FILEATTACH);

	Global2Flags(JamHeader.attribute, it->attr, FLAG_RRQ, JAM_FLAG_RECEIPTREQ);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_CPT, JAM_FLAG_CONFIRMREQ);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_ORP, JAM_FLAG_ORPHAN);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_ENC, JAM_FLAG_ENCRYPT);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_CMP, JAM_FLAG_COMPRESS);
	Global2Flags(JamHeader.attribute, it->attr, FLAG_ESC, JAM_FLAG_ESCAPED);

	Global2Flags(JamHeader.attribute, it->attr, FLAG_READ, JAM_FLAG_READ);

	Global2Flags(JamHeader.attribute, it->attr, FLAG_LOK, JAM_FLAG_LOCKED);

	QString AppendBuff = Base->NormalizeCtl(it, (uint8_t*)Base->CtlBuff);

	if (!AppendBuff.isEmpty())
		AppendBuff.append("\n").append((char*)Base->CtlBuff);
	else
		AppendBuff = (char*)Base->CtlBuff;

	char *Ctl = qstrdup((const char*)AppendBuff.ascii());
	CRLF2Fts(Ctl);

	char *Txt = qstrdup((const char*)Base->TxtBuff);
	CRLF2Fts(Txt);

	for (;;)
	{
/*
		if (fseek(b_obj->JDX, ((QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) + Index) * sizeof(AreaItem_Jam_Index), SEEK_SET) != 0)
			break;

		if (fread((char*)&JamIndex, sizeof(AreaItem_Jam_Index), 1, b_obj->JDX) != 1)
			break;

		if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) != 0)
			break;
*/

/*
	TODO
remember JAM_FLAG_TYPELOCAL,JAM_FLAG_TYPEECHO,JAM_FLAG_TYPENET !!!
*/

/*
		if (obj->Ptr == 0)
		{
			//
		} else {
			//
		}
		ret = true;
*/
		break;
	}

	delete Ctl;
	delete Txt;

	return ret;
}

TMessage *AppendArea_Jam(TArea *Base, bool isNew)
{
	TMessage *it = new TMessage(Base);
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	if (isNew)
		it->uid = ++b_obj->MaxUID;

	it->MessagePvtData = NULL;
	Base->append(it);

	return it;
}

bool DeleteArea_Jam(TArea *Base, uint32_t Index)
{
	AreaItem_Jam_Index JamIndex;
	AreaItem_Jam_Header JamHeader;
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	if (fseek(b_obj->JDX, ((QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) + Index) * sizeof(AreaItem_Jam_Index), SEEK_SET) != 0)
		return false;

	if (fread((char*)&JamIndex, sizeof(AreaItem_Jam_Index), 1, b_obj->JDX) != 1)
		return false;

	if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) != 0)
		return false;

	if (fread((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR) == 1)
	{
		JamHeader.attribute = JamHeader.attribute | JAM_FLAG_DELETED;
		if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) == 0)
			if (fwrite((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR) == 1)
			{
				b_obj->HdrInfo.activemsgs = b_obj->HdrInfo.activemsgs - 1;
				rewind(b_obj->JHR);
				fwrite((char*)&b_obj->HdrInfo, sizeof(AreaItem_Jam_HeaderInfo), 1, b_obj->JHR);
				return true;
			}
	}

	return false;
}

bool KillArea_Jam(TArea *Base)
{
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	return QFile::remove(b_obj->JDTName) && QFile::remove(b_obj->JDXName) && QFile::remove(b_obj->JHRName) && b_obj->JLR.remove();
}

void CloseArea_Jam(TArea *Base)
{
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	fclose(b_obj->JDT);
	b_obj->JDT = NULL;
	fclose(b_obj->JDX);
	b_obj->JDX = NULL;
	fclose(b_obj->JHR);
	b_obj->JHR = NULL;
}

void DoneArea_Jam(TArea *Base)
{
	delete (TArea_Jam_PvtObject*)Base->AreaPvtData;
}

uint32_t GetLastReadArea_Jam(TArea *Base, uint32_t userid)
{
	int32_t cnt;
	uint32_t ret = 0;
	AreaItem_Jam_Lastread LastRead;
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	if (FileOpenFunc(&b_obj->JLR, IO_ReadOnly))
	{
		cnt = b_obj->JLR.size() / sizeof(AreaItem_Jam_Lastread);
		while (cnt-- > 0)
			if (b_obj->JLR.readBlock((char*)&LastRead, sizeof(AreaItem_Jam_Lastread)) == sizeof(AreaItem_Jam_Lastread))
			{
				if (LastRead.userid == userid)
				{
					ret = LastRead.lastread;
					break;
				}
			} else
				break;
		FileCloseFunc(&b_obj->JLR);
	}

	return ret;
}

void SetLastReadArea_Jam(TArea *Base, uint32_t userid, uint32_t Index)
{
	int32_t cnt;
	bool found = false;
	AreaItem_Jam_Lastread LastRead;
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	if (FileOpenFunc(&b_obj->JLR, IO_ReadWrite))
	{

		cnt = b_obj->JLR.size() / sizeof(AreaItem_Jam_Lastread);

		while (cnt-- > 0)
			if (b_obj->JLR.readBlock((char*)&LastRead, sizeof(AreaItem_Jam_Lastread)) == sizeof(AreaItem_Jam_Lastread))
			{
				if (LastRead.userid == userid)
				{
					b_obj->JLR.at(b_obj->JLR.at() - sizeof(AreaItem_Jam_Lastread));
					found = true;
					break;
				}
			} else
				break;

		if (!found)
		{
			memset((char*)&LastRead, '\0', sizeof(AreaItem_Jam_Lastread));
			LastRead.userid = userid;
			LastRead.usercrc = Calculate_CRC((const uint8_t*)Base->User);
		}

		LastRead.lastread = Index;
		b_obj->JLR.writeBlock((char*)&LastRead, sizeof(AreaItem_Jam_Lastread));

		FileCloseFunc(&b_obj->JLR);
	}
}

void MarkAsReadArea_Jam(TArea *Base, uint32_t Index)
{
	AreaItem_Jam_Index JamIndex;
	AreaItem_Jam_Header JamHeader;
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	if (fseek(b_obj->JDX, ((QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) + Index) * sizeof(AreaItem_Jam_Index), SEEK_SET) == 0)
		if (fread((char*)&JamIndex, sizeof(AreaItem_Jam_Index), 1, b_obj->JDX) == 1)
			if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) == 0)
				if (fread((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR) == 1)
				{
					JamHeader.attribute = JamHeader.attribute | JAM_FLAG_READ;
					if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) == 0)
						fwrite((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR);
				}
}
