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

#include "tarea_jam.h"

void CreateEmptyJHR(TArea *Base)
{
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;
	AreaItem_Jam_HeaderInfo _HdrInfo;

	b_obj->JHR = freopen(b_obj->JHRName.ascii(), "w+b", b_obj->JHR);
	if (b_obj->JHR != NULL)
	{
		memset(&_HdrInfo, 0, sizeof(AreaItem_Jam_HeaderInfo));

		memcpy(&_HdrInfo.signature, JAM_MAGIC, 4);
		_HdrInfo.datecreated = QDateTime::currentDateTime().toTime_t();
		_HdrInfo.modcounter = 1;
		_HdrInfo.passwordcrc = 0xffffffff;
		_HdrInfo.basemsgnum = 1;

		fwrite((char*)&_HdrInfo, sizeof(AreaItem_Jam_HeaderInfo), 1, b_obj->JHR);
		b_obj->JHR = freopen(b_obj->JHRName.ascii(), "r+b", b_obj->JHR);
	}
}

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

	b_obj->JDT = fopen(b_obj->JDTName.ascii(), "r+b");
	b_obj->JDX = fopen(b_obj->JDXName.ascii(), "r+b");
	b_obj->JHR = fopen(b_obj->JHRName.ascii(), "r+b");

	if (b_obj->JHR != NULL)
		if (fseek(b_obj->JHR, 0, SEEK_END) == 0)
			if (ftell(b_obj->JHR) < (long)sizeof(AreaItem_Jam_HeaderInfo))
				CreateEmptyJHR(Base);

	return (b_obj->JDT != NULL) && (b_obj->JDX != NULL) && (b_obj->JHR != NULL);
}

bool RescanArea_Jam(TArea *Base)
{
	AreaItem_Jam_Index JamIndex;
	AreaItem_Jam_Header JamHeader;
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	Base->BaseSize = 0;
	if (fseek(b_obj->JHR, 0, SEEK_END) == 0)
		Base->BaseSize += ftell(b_obj->JHR);
	if (fseek(b_obj->JDT, 0, SEEK_END) == 0)
		Base->BaseSize += ftell(b_obj->JDT);
	if (fseek(b_obj->JDX, 0, SEEK_END) == 0)
		Base->BaseSize += ftell(b_obj->JDX);

	rewind(b_obj->JHR);
	if (fread((char*)&b_obj->HdrInfo, sizeof(AreaItem_Jam_HeaderInfo), 1, b_obj->JHR) == 1)
	{
		if (memcmp(&b_obj->HdrInfo.signature, JAM_MAGIC, 4) == 0)
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
						AreaItem_Jam_Subfield *Subfield = (AreaItem_Jam_Subfield*)SubfieldHeader;

						if (SubfieldHeader->datlen > JAM_MAX_DATA_LEN)
							break;

						switch (SubfieldHeader->loid)
						{
							case JAMSFLD_OADDRESS:
							case JAMSFLD_DADDRESS:
								break;
							case JAMSFLD_SENDERNAME:
								qstrncpy((char*)&it->from, (char*)&Subfield->data, QMIN(MAX_FROM_NAME_LEN, SubfieldHeader->datlen + 1));
								break;
							case JAMSFLD_RECVRNAME:
								qstrncpy((char*)&it->to, (char*)&Subfield->data, QMIN(MAX_TO_NAME_LEN, SubfieldHeader->datlen + 1));
								break;
							case JAMSFLD_MSGID:
							case JAMSFLD_REPLYID:
								break;
							case JAMSFLD_SUBJECT:
								qstrncpy((char*)&it->subj, (char*)&Subfield->data, QMIN(MAX_SUBJ_LEN, SubfieldHeader->datlen + 1));
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

	if (fseek(b_obj->JDX, ((QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) + (it->uid - 1)) * sizeof(AreaItem_Jam_Index), SEEK_SET) != 0)
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
				AreaItem_Jam_Subfield *Subfield = (AreaItem_Jam_Subfield*)SubfieldHeader;

				if (SubfieldHeader->datlen > JAM_MAX_DATA_LEN)
					break;

				fidoaddr addr;
				switch ((SubfieldHeader->hiid << 16) + SubfieldHeader->loid)
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
						qstrncpy((char*)&it->from, (char*)&Subfield->data, QMIN(MAX_FROM_NAME_LEN, SubfieldHeader->datlen + 1));
						break;
					case JAMSFLD_RECVRNAME:
						qstrncpy((char*)&it->to, (char*)&Subfield->data, QMIN(MAX_TO_NAME_LEN, SubfieldHeader->datlen + 1));
						break;
					case JAMSFLD_MSGID:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						if (!fa)
							if (str2addr(tmpstr, &addr))
							{
								it->origaddr.zone = addr.zone;
								it->origaddr.net = addr.net;
								it->origaddr.node = addr.node;
								it->origaddr.point = addr.point;
							}
						KludgeBuff << "MSGID: " + tmpstr;
						break;
					case JAMSFLD_REPLYID:
						tmpstr = (char*)&Subfield->data;
						tmpstr.truncate(SubfieldHeader->datlen);
						if (!ta)
							if (str2addr(tmpstr, &addr))
							{
								it->destaddr.zone = addr.zone;
								it->destaddr.net = addr.net;
								it->destaddr.node = addr.node;
								it->destaddr.point = addr.point;
							}
						KludgeBuff << "REPLY: " + tmpstr;
						break;
					case JAMSFLD_SUBJECT:
						qstrncpy((char*)&it->subj, (char*)&Subfield->data, QMIN(MAX_SUBJ_LEN, SubfieldHeader->datlen + 1));
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
	uint32_t IdxOfs;
	AreaItem_Jam_Index JamIndex;
	AreaItem_Jam_Header JamHeader;
	AreaItem_Jam_SubfieldHeader Subfield;

	TMessage *it = Base->at(Index);
	TArea_Jam_PvtObject *b_obj = (TArea_Jam_PvtObject*)Base->AreaPvtData;

	fseek(b_obj->JDX, ((QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) + (it->uid - 1)) * sizeof(AreaItem_Jam_Index), SEEK_SET);
	IdxOfs = ftell(b_obj->JDX);

	if (Index < b_obj->HdrInfo.activemsgs)
	{
		/* Mark existing item as deleted */
		if (!DeleteArea_Jam(Base, Index))
			return false;
		Base->remove(Index);
		it->uid = ++b_obj->MaxUID;
	}

	memset((char*)&JamHeader, '\0', sizeof(AreaItem_Jam_Header));

	memcpy(&JamHeader.signature, JAM_MAGIC, 4);
	JamHeader.revision = JAM_REVISION;
	JamHeader.msgidcrc = 0xffffffff;
	JamHeader.replycrc = 0xffffffff;
	JamHeader.passwordcrc = 0xffffffff;
	JamHeader.messagenumber = it->uid;
	JamHeader.datewritten = (uint32_t)it->dt;

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

	switch (Base->AreaType)
	{
		case AREATYPE_NETMAIL:
			JamHeader.attribute = JamHeader.attribute | JAM_FLAG_TYPENET;
			break;
		case AREATYPE_BADMAIL:
		case AREATYPE_DUPEMAIL:
		case AREATYPE_CARBON:
		case AREATYPE_LOCALMAIL:
			JamHeader.attribute = JamHeader.attribute | JAM_FLAG_TYPELOCAL;
			break;
		case AREATYPE_ECHOMAIL:
			JamHeader.attribute = JamHeader.attribute | JAM_FLAG_TYPEECHO;
			break;
		default:
			break;
	}

	/* Write new index record */
	if (fseek(b_obj->JHR, 0, SEEK_END) != 0)
		return false;

	JamIndex.hdroffset = ftell(b_obj->JHR);
	JamIndex.usercrc = Calculate_CRC(it->from);

	if (fseek(b_obj->JDX, IdxOfs, SEEK_SET) != 0)
		return false;
	if (fwrite((char*)&JamIndex, sizeof(AreaItem_Jam_Index), 1, b_obj->JDX) != 1)
		return false;

	/* Set up text offset */
	if (fseek(b_obj->JDT, 0, SEEK_END) != 0)
		return false;
	JamHeader.offset = ftell(b_obj->JDT);

	/* Write header */
	if (fwrite((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR) != 1)
		return false;

	/* Fill subfields */
	memset(&Subfield, 0, sizeof(AreaItem_Jam_SubfieldHeader));

	QString AppendBuff = addr2str1(&it->origaddr, NULL);
	Subfield.loid = JAMSFLD_OADDRESS;
	Subfield.datlen = QMIN(AppendBuff.length(), JAM_MAX_DATA_LEN);
	if (fwrite((char*)&Subfield, sizeof(AreaItem_Jam_SubfieldHeader), 1, b_obj->JHR) != 1)
		return false;
	if (fwrite(AppendBuff.ascii(), Subfield.datlen, 1, b_obj->JHR) != 1)
		return false;

	AppendBuff = addr2str1(&it->destaddr, NULL);
	Subfield.loid = JAMSFLD_DADDRESS;
	Subfield.datlen = QMIN(AppendBuff.length(), JAM_MAX_DATA_LEN);
	if (fwrite((char*)&Subfield, sizeof(AreaItem_Jam_SubfieldHeader), 1, b_obj->JHR) != 1)
		return false;
	if (fwrite(AppendBuff.ascii(), Subfield.datlen, 1, b_obj->JHR) != 1)
		return false;

	Subfield.loid = JAMSFLD_SENDERNAME;
	Subfield.datlen = QMIN(qstrlen((const char*)&it->from), JAM_MAX_DATA_LEN);
	if (fwrite((char*)&Subfield, sizeof(AreaItem_Jam_SubfieldHeader), 1, b_obj->JHR) != 1)
		return false;
	if (fwrite(&it->from, Subfield.datlen, 1, b_obj->JHR) != 1)
		return false;

	Subfield.loid = JAMSFLD_RECVRNAME;
	Subfield.datlen = QMIN(qstrlen((const char*)&it->to), JAM_MAX_DATA_LEN);
	if (fwrite((char*)&Subfield, sizeof(AreaItem_Jam_SubfieldHeader), 1, b_obj->JHR) != 1)
		return false;
	if (fwrite(&it->to, Subfield.datlen, 1, b_obj->JHR) != 1)
		return false;

	Subfield.loid = JAMSFLD_SUBJECT;
	Subfield.datlen = QMIN(qstrlen((const char*)&it->subj), JAM_MAX_DATA_LEN);
	if (fwrite((char*)&Subfield, sizeof(AreaItem_Jam_SubfieldHeader), 1, b_obj->JHR) != 1)
		return false;
	if (fwrite(&it->subj, Subfield.datlen, 1, b_obj->JHR) != 1)
		return false;

	/* Parse Kludges to subfields */
	AppendBuff = Base->NormalizeCtl(it, (uint8_t*)Base->CtlBuff);

	if (!AppendBuff.isEmpty())
		AppendBuff.append("\n").append((char*)Base->CtlBuff);
	else
		AppendBuff = (char*)Base->CtlBuff;

	QStringList ParseList = QStringList::split("\n", AppendBuff, true);
	while (ParseList.count() > 0)
	{
		AppendBuff = ParseList[0].stripWhiteSpace();

		memset(&Subfield, 0, sizeof(AreaItem_Jam_SubfieldHeader));
		if (AppendBuff.startsWith("INTL ") || AppendBuff.startsWith("FMPT ") || AppendBuff.startsWith("TOPT ") || AppendBuff.isEmpty())
		{
			ParseList.remove(ParseList.at(0));
			continue;
		} else if (AppendBuff.startsWith("MSGID: "))
		{
			AppendBuff = AppendBuff.mid(7);
			Subfield.loid = JAMSFLD_MSGID;
			JamHeader.msgidcrc = Calculate_CRC((const uint8_t*)AppendBuff.ascii());
		} else if (AppendBuff.startsWith("REPLY: "))
		{
			AppendBuff = AppendBuff.mid(7);
			Subfield.loid = JAMSFLD_REPLYID;
			JamHeader.replycrc = Calculate_CRC((const uint8_t*)AppendBuff.ascii());
		} else if (AppendBuff.startsWith("PID: "))
		{
			AppendBuff = AppendBuff.mid(5);
			Subfield.loid = JAMSFLD_PID;
		} else if (AppendBuff.startsWith("ENCLFILE: "))
		{
			AppendBuff = AppendBuff.mid(10);
			Subfield.loid = JAMSFLD_ENCLFILE;
		} else if (AppendBuff.startsWith("ENCLFILEWALIAS: "))
		{
			AppendBuff = AppendBuff.mid(16);
			Subfield.loid = JAMSFLD_ENCLFWALIAS;
		} else if (AppendBuff.startsWith("ENCLFREQ: "))
		{
			AppendBuff = AppendBuff.mid(10);
			Subfield.loid = JAMSFLD_ENCLFREQ;
		} else if (AppendBuff.startsWith("ENCLFILEWCARD: "))
		{
			AppendBuff = AppendBuff.mid(15);
			Subfield.loid = JAMSFLD_ENCLFILEWC;
		} else if (AppendBuff.startsWith("ENCLINDIRFILE: "))
		{
			AppendBuff = AppendBuff.mid(15);
			Subfield.loid = JAMSFLD_ENCLINDFILE;
		} else if (AppendBuff.startsWith("FLAGS "))
		{
			AppendBuff = AppendBuff.mid(6);
			Subfield.loid = JAMSFLD_FLAGS;
		} else if (AppendBuff.startsWith("TZUTC: "))
		{
			AppendBuff = AppendBuff.mid(7);
			Subfield.loid = JAMSFLD_TZUTCINFO;
		} else
			Subfield.loid = JAMSFLD_FTSKLUDGE;

		Subfield.datlen = QMIN(AppendBuff.length(), JAM_MAX_DATA_LEN);
		if (fwrite((char*)&Subfield, sizeof(AreaItem_Jam_SubfieldHeader), 1, b_obj->JHR) != 1)
			return false;
		if (fwrite(AppendBuff.ascii(), Subfield.datlen, 1, b_obj->JHR) != 1)
			return false;
		ParseList.remove(ParseList.at(0));
	}

	/* Parse Text to subfields */
	ParseList = QStringList::split("\n", (const char*)Base->TxtBuff, true);
	for (uint32_t i = 0; i < ParseList.count();)
	{
		AppendBuff = ParseList[i];

		memset(&Subfield, 0, sizeof(AreaItem_Jam_SubfieldHeader));

		if (AppendBuff.startsWith("SEEN-BY: "))
		{
			AppendBuff = AppendBuff.mid(9);
			Subfield.loid = JAMSFLD_SEENBY2D;
		} else if (AppendBuff.startsWith("PATH: "))
		{
			AppendBuff = AppendBuff.mid(6);
			Subfield.loid = JAMSFLD_PATH2D;
		} else {
			i++;
			continue;
		}

		Subfield.datlen = QMIN(AppendBuff.length(), JAM_MAX_DATA_LEN);
		ParseList.remove(ParseList.at(i));
		if (fwrite((char*)&Subfield, sizeof(AreaItem_Jam_SubfieldHeader), 1, b_obj->JHR) != 1)
			return false;
		if (fwrite(AppendBuff.ascii(), Subfield.datlen, 1, b_obj->JHR) != 1)
			return false;
	}

	/* Update subfield length */
	JamHeader.subfieldlen = ftell(b_obj->JHR) - (JamIndex.hdroffset + sizeof(AreaItem_Jam_Header));

	/* Write Text */
	char *Txt = qstrdup(ParseList.join("\r").ascii());
	CRLF2Fts(Txt);

	if (fwrite(Txt, qstrlen(Txt), 1, b_obj->JDT) != 1)
		return false;
	JamHeader.txtlen = qstrlen(Txt);

	/* Update header */
	if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) != 0)
		return false;
	if (fwrite((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR) != 1)
		return false;

	/* Update message counter */
	b_obj->HdrInfo.activemsgs = b_obj->HdrInfo.activemsgs + 1;
	rewind(b_obj->JHR);
	fwrite((char*)&b_obj->HdrInfo, sizeof(AreaItem_Jam_HeaderInfo), 1, b_obj->JHR);

	return true;
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
	TMessage *it = Base->at(Index);

	if (fseek(b_obj->JDX, ((QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) + (it->uid - 1)) * sizeof(AreaItem_Jam_Index), SEEK_SET) != 0)
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
	TMessage *it = Base->at(Index);

	if (fseek(b_obj->JDX, ((QMAX(b_obj->HdrInfo.basemsgnum, 1) - 1) + (it->uid - 1)) * sizeof(AreaItem_Jam_Index), SEEK_SET) == 0)
		if (fread((char*)&JamIndex, sizeof(AreaItem_Jam_Index), 1, b_obj->JDX) == 1)
			if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) == 0)
				if (fread((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR) == 1)
				{
					JamHeader.attribute = JamHeader.attribute | JAM_FLAG_READ;
					if (fseek(b_obj->JHR, JamIndex.hdroffset, SEEK_SET) == 0)
						fwrite((char*)&JamHeader, sizeof(AreaItem_Jam_Header), 1, b_obj->JHR);
				}
}
