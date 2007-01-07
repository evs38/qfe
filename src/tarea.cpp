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

#include "tarea.h"

#include "tarea_msg.h"
#include "tarea_jam.h"
#include "tarea_squish.h"

#define DEFAULT_USERID		0

TArea::TArea(Area_Type _AType, Base_Type _BType, uint16_t _DefZone)
{
	setAutoDelete(true);

	AreaPvtData = NULL;
	AreaType = _AType;
	BaseType = _BType;
	DefZone = _DefZone;

	UnReadCnt = 0;

	BaseSize = 0;

	OpenCounter = 0;
	dummyLastRead = -1;
	User = NULL;

	CtlBuff = NULL;
	TxtBuff = NULL;

	LastIndex = -1;

	switch (BaseType)
	{
		case BASETYPE_MSG:
			InitArea_ = InitArea_Msg;
			OpenArea_ = OpenArea_Msg;
			RescanArea_ = RescanArea_Msg;
			ReadArea_ = ReadArea_Msg;
			WriteArea_ = WriteArea_Msg;
			AppendArea_ = AppendArea_Msg;
			DeleteArea_ = DeleteArea_Msg;
			KillArea_ = KillArea_Msg;
			PurgeArea_ = NULL; //TODO:
			CloseArea_ = CloseArea_Msg;
			DoneArea_ = DoneArea_Msg;
			GetLastReadArea_ = GetLastReadArea_Msg;
			SetLastReadArea_ = SetLastReadArea_Msg;
			MarkAsReadArea_ = MarkAsReadArea_Msg;
			break;
		case BASETYPE_JAM:
			InitArea_ = InitArea_Jam;
			OpenArea_ = OpenArea_Jam;
			RescanArea_ = RescanArea_Jam;
			ReadArea_ = ReadArea_Jam;
			WriteArea_ = WriteArea_Jam;
			AppendArea_ = AppendArea_Jam;
			DeleteArea_ = DeleteArea_Jam;
			KillArea_ = KillArea_Jam;
			PurgeArea_ = NULL; //TODO:
			CloseArea_ = CloseArea_Jam;
			DoneArea_ = DoneArea_Jam;
			GetLastReadArea_ = GetLastReadArea_Jam;
			SetLastReadArea_ = SetLastReadArea_Jam;
			MarkAsReadArea_ = MarkAsReadArea_Jam;
			break;
		case BASETYPE_SQUISH:
			InitArea_ = InitArea_Squish;
			OpenArea_ = OpenArea_Squish;
			RescanArea_ = RescanArea_Squish;
			ReadArea_ = ReadArea_Squish;
			WriteArea_ = WriteArea_Squish;
			AppendArea_ = AppendArea_Squish;
			DeleteArea_ = DeleteArea_Squish;
			KillArea_ = KillArea_Squish;
			PurgeArea_ = NULL; //TODO:
			CloseArea_ = CloseArea_Squish;
			DoneArea_ = DoneArea_Squish;
			GetLastReadArea_ = GetLastReadArea_Squish;
			SetLastReadArea_ = SetLastReadArea_Squish;
			MarkAsReadArea_ = MarkAsReadArea_Squish;
			break;
		default:
			InitArea_ = NULL;
			OpenArea_ = NULL;
			RescanArea_ = NULL;
			ReadArea_ = NULL;
			WriteArea_ = NULL;
			AppendArea_ = NULL;
			DeleteArea_ = NULL;
			KillArea_ = NULL;
			PurgeArea_ = NULL;
			CloseArea_ = NULL;
			DoneArea_ = NULL;
			GetLastReadArea_ = NULL;
			SetLastReadArea_ = NULL;
			MarkAsReadArea_ = NULL;
			break;
	}
}

TArea::~TArea()
{
	Done();
	if (User != NULL)
		delete User;
	FreeBuffers();
}

Base_Type TArea::GetBaseType()
{
	return BaseType;
}

void TArea::FreeBuffers()
{
	if (CtlBuff != NULL)
	{
		delete CtlBuff;
		CtlBuff = NULL;
	}
	if (TxtBuff != NULL)
	{
		delete TxtBuff;
		TxtBuff = NULL;
	}
}

bool TArea::Init(QString _Name, QString _Desc, QString _Path, QString _Aka, QString _Group, QString _Uplink, QString _UplinkPwd, QString _UplinkRbt, char *_User)
{
	bool ret = false;

	Aka = _Aka;
	Name = _Name;
	Desc = _Desc;
	Path = _Path;
	Group = _Group;

	Uplink = _Uplink;
	UplinkPwd = _UplinkPwd;
	UplinkRbt = _UplinkRbt;

	User = qstrdup(_User);

	if (InitArea_ != NULL)
		ret = InitArea_(this);

	if (!ret)
		debugmessage("Init() fail. Path=" + Path);

	return ret;
}

bool TArea::Open(bool ignorelock)
{
	if (!ignorelock)
		WaitMutex();

	bool ret = false;

	if (++OpenCounter == 1)
	{
		if (OpenArea_ != NULL)
			ret = OpenArea_(this);

		if (!ret)
		{
			debugmessage("Open() fail. Path=" + Path);
			Close(true);
		}
	} else
		ret = true;

	return ret;
}

bool TArea::Rescan()
{
	WaitMutex();
	bool ret = false;

	if (isOpened())
	{
		clear();

		if (RescanArea_ != NULL)
			ret = RescanArea_(this);

		if (!ret)
			debugmessage("Rescan() fail. Path=" + Path);

		CalculateUnread();
	} else
		debugmessage("Rescan() fail. Area " + Name + " closed.");

	return ret;
}

bool TArea::Read(uint32_t Index)
{
	WaitMutex();
	bool ret = false;

	if (isOpened())
	{
		if (LastIndex != (int32_t)Index)
		{
			if (LastIndex != -1)
				FreeBuffers();
			LastIndex = Index;

			if (ReadArea_ != NULL)
				ret = ReadArea_(this, Index);
		} else
			ret = true;
	} else
		debugmessage("Read() fail. Area " + Name + " closed.");

	return ret;
}

bool TArea::Write(uint32_t Index)
{
	WaitMutex();
	bool ret = false;

	if (isOpened())
	{
		if (LastIndex != (int32_t)Index)
		{
			if (LastIndex != -1)
				FreeBuffers();
			LastIndex = Index;
		}

		if (WriteArea_ != NULL)
			ret = WriteArea_(this, Index);
	} else
		debugmessage("Write() fail. Area " + Name + " closed.");

	return ret;
}

TMessage *TArea::Append()
{
	WaitMutex();
	TMessage *ret = NULL;

	if (Open(true))
	{
		if (AppendArea_ != NULL)
		{
			ret = AppendArea_(this, true);

			ret->attr = FLAG_LOC | FLAG_READ;
			ret->dt = QDateTime::currentDateTime().toTime_t();
		}
		Close(true);
	}

	return ret;
}

bool TArea::Delete(uint32_t Index)
{
	WaitMutex();
	bool ret = false;

	if (Open(true))
	{
		if (LastIndex == (int32_t)Index)
			LastIndex = -1;
		FreeBuffers();

		if (DeleteArea_ != NULL)
			ret = DeleteArea_(this, Index);

		if (ret)
			remove(Index);
		else
			debugmessage("Delete() fail.");

		Close(true);
	}

	CalculateUnread();

	return ret;
}

bool TArea::Kill()
{
	WaitMutex();
	bool ret = false;

	Done();

	if (KillArea_ != NULL)
		ret = KillArea_(this);

	return ret;
}

bool TArea::Purge()
{
	WaitMutex();
	bool ret = false;

	if (isOpened())
	{
		if (PurgeArea_ != NULL)
			PurgeArea_(this);

		ret = true;
	} else
		debugmessage("Purge() fail. Area " + Name + " closed.");

	return ret;
}

void TArea::Close(bool ignorelock)
{
	if (!ignorelock)
		WaitMutex();

	if (OpenCounter > 0)
		if (--OpenCounter == 0)
		{
			if (LastIndex != -1)
			{
				FreeBuffers();
				LastIndex = -1;
			}

			if (CloseArea_ != NULL)
				CloseArea_(this);
		}
}

void TArea::Done()
{
	while (isOpened())
		Close();

	if (DoneArea_ != NULL)
		DoneArea_(this);

	clear();
}

uint32_t TArea::GetLastRead()
{
	if (dummyLastRead == -1)
		if (GetLastReadArea_ != NULL)
			dummyLastRead = GetLastReadArea_(this, DEFAULT_USERID);

	return (uint32_t)QMAX(dummyLastRead, 0);
}

void TArea::SetLastRead(uint32_t Index)
{
	if ((dummyLastRead == -1) || (dummyLastRead != (int32_t)Index))
	{
		dummyLastRead = Index;

		if (SetLastReadArea_ != NULL)
			SetLastReadArea_(this, DEFAULT_USERID, Index);
	}
}

void TArea::MarkAsRead(uint32_t Index)
{
	if (!isReaded(at(Index)))
	{
		at(Index)->attr |= FLAG_READ;
		if (Open())
		{
			if (MarkAsReadArea_ != NULL)
				MarkAsReadArea_(this, Index);

			Close();
		}

		if (UnReadCnt > 0)
			UnReadCnt--;
		if (at(Index)->MessageItem != NULL)
			at(Index)->MessageItem->UpdateMessage();
	}
};

int32_t TArea::UID2Index(uint32_t Uid)
{
	for (uint32_t i = 0; i < count(); i++)
		if (at(i)->uid == Uid)
			return i;

	return -1;
}

void TArea::CalculateUnread()
{
	UnReadCnt = 0;
	for (uint32_t i = 0; i < count(); i++)
		if (!isReaded(at(i)))
			UnReadCnt++;
}

QString TArea::NormalizeCtl(TMessage *Message, uint8_t *Ctl, bool addPID)
{
	QString ret = QString::null;

	if (AreaType == AREATYPE_NETMAIL)
	{
		if (GetKludge(Ctl, "INTL ").isEmpty())
			ret = QString("INTL %1:%2/%3 %4:%5/%6").arg(Message->destaddr.zone).arg(Message->destaddr.net).arg(Message->destaddr.node).arg(Message->origaddr.zone).arg(Message->origaddr.net).arg(Message->origaddr.node);

		if (GetKludge(Ctl, "FMPT ").isEmpty() && (Message->origaddr.point > 0))
			ret.append((ret.isEmpty() ? QString::null : "\n") + QString("FMPT %1").arg(Message->origaddr.point));

		if (GetKludge(Ctl, "TOPT ").isEmpty() && (Message->destaddr.point > 0))
			ret.append((ret.isEmpty() ? QString::null : "\n") + QString("TOPT %1").arg(Message->destaddr.point));
	}

	if (addPID && GetKludge(Ctl, "PID: ").isEmpty())
		ret.append((ret.isEmpty() ? QString::null : "\n") + QString("PID: QFE/%1 %2").arg(SHORT_PLATFORM_NAME).arg(QFE_VERSION));

	return ret;
}

QString TArea::Append001(QString Ctl)
{
	return Ctl.replace('\n', "\n\001").prepend(Ctl.isEmpty() ? QString::null : "\001");
}
