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

	OpenCounter = 0;
	dummyLastRead = -1;
	User = NULL;

	LastIndex = -1;
}

TArea::~TArea()
{
	if (User != NULL)
		delete User;
	Done();
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

	switch (BaseType)
	{
		case BASETYPE_MSG:
			ret = InitArea_Msg(this);
			break;
		case BASETYPE_JAM:
			ret = InitArea_Jam(this);
			break;
		case BASETYPE_SQUISH:
			ret = InitArea_Squish(this);
			break;
		default:
			break;
	}

	if (!ret)
		debugmessage("Init() fail. Path=" + Path);

	return ret;
}

bool TArea::Open()
{
	QMutexLocker MutexLocker(&Mutex);
	bool ret = false;

	if (++OpenCounter == 1)
	{
		switch (BaseType)
		{
			case BASETYPE_MSG:
				ret = OpenArea_Msg(this);
				break;
			case BASETYPE_JAM:
				ret = OpenArea_Jam(this);
				break;
			case BASETYPE_SQUISH:
				ret = OpenArea_Squish(this);
				break;
			default:
				break;
		}

		if (!ret)
		{
			debugmessage("Open() fail. Path=" + Path);
			Close();
		}
	} else
		ret = true;

	return ret;
}

bool TArea::Rescan()
{
	QMutexLocker MutexLocker(&Mutex);
	bool ret = false;

	if (isOpened())
	{
		clear();
		switch (BaseType)
		{
			case BASETYPE_MSG:
				ret = RescanArea_Msg(this);
				break;
			case BASETYPE_JAM:
				ret = RescanArea_Jam(this);
				break;
			case BASETYPE_SQUISH:
				ret = RescanArea_Squish(this);
				break;
			default:
				break;
		}

		if (!ret)
			debugmessage("Rescan() fail. Path=" + Path);

		CalculateUnread();
	} else
		debugmessage("Rescan() fail. Area " + Name + " closed.");

	return ret;
}

bool TArea::Read(uint32_t Index)
{
	QMutexLocker MutexLocker(&Mutex);
	bool ret = false;

	if (isOpened())
	{
		if (LastIndex != (int32_t)Index)
		{
			if (LastIndex != -1)
				at(LastIndex)->FreeBuffers();
			LastIndex = Index;

			switch (BaseType)
			{
				case BASETYPE_MSG:
					ret = ReadArea_Msg(this, Index);
					break;
				case BASETYPE_JAM:
					ret = ReadArea_Jam(this, Index);
					break;
				case BASETYPE_SQUISH:
					ret = ReadArea_Squish(this, Index);
					break;
				default:
					break;
			}
		} else
			ret = true;
	} else
		debugmessage("Read() fail. Area " + Name + " closed.");

	return ret;
}

bool TArea::Write(uint32_t Index)
{
	QMutexLocker MutexLocker(&Mutex);
	bool ret = false;

	if (isOpened())
	{
		if (LastIndex != (int32_t)Index)
		{
			if (LastIndex != -1)
				at(LastIndex)->FreeBuffers();
			LastIndex = Index;
		}

		switch (BaseType)
		{
			case BASETYPE_MSG:
				ret = WriteArea_Msg(this, Index);
				break;
			case BASETYPE_JAM:
				ret = WriteArea_Jam(this, Index);
				break;
			case BASETYPE_SQUISH:
				ret = WriteArea_Squish(this, Index);
				break;
			default:
				break;
		}
	} else
		debugmessage("Write() fail. Area " + Name + " closed.");

	return ret;
}

TMessage *TArea::Append()
{
	QMutexLocker MutexLocker(&Mutex);
	TMessage *ret = NULL;

	if (Open())
	{
		switch (BaseType)
		{
			case BASETYPE_MSG:
				ret = AppendArea_Msg(this, true);
				break;
			case BASETYPE_JAM:
				ret = AppendArea_Jam(this, true);
				break;
			case BASETYPE_SQUISH:
				ret = AppendArea_Squish(this, true);
				break;
			default:
				break;
		}
		ret->attr = FLAG_LOC | FLAG_READ;
		ret->dt = QDateTime::currentDateTime().toTime_t();
		Close();
	}

	return ret;
}

bool TArea::Delete(uint32_t Index)
{
	QMutexLocker MutexLocker(&Mutex);
	bool ret = false;

	if (Open())
	{
		if (LastIndex == (int32_t)Index)
			LastIndex = -1;
		at(Index)->FreeBuffers();

		switch (BaseType)
		{
			case BASETYPE_MSG:
				ret = DeleteArea_Msg(this, Index);
				break;
			case BASETYPE_JAM:
				ret = DeleteArea_Jam(this, Index);
				break;
			case BASETYPE_SQUISH:
				ret = DeleteArea_Squish(this, Index);
				break;
			default:
				break;
		}

		if (ret)
			remove(Index);
		else
			debugmessage("Delete() fail.");

		Close();
	}

	CalculateUnread();

	return ret;
}

bool TArea::Kill()
{
	QMutexLocker MutexLocker(&Mutex);
	bool ret = false;

	Done();

	switch (BaseType)
	{
		case BASETYPE_MSG:
			ret = KillArea_Msg(this);
			break;
		case BASETYPE_JAM:
			ret = KillArea_Jam(this);
			break;
		case BASETYPE_SQUISH:
			ret = KillArea_Squish(this);
			break;
		default:
			break;
	}

	return ret;
}

void TArea::Close()
{
	QMutexLocker MutexLocker(&Mutex);
	if (OpenCounter > 0)
		if (--OpenCounter == 0)
		{
			if (LastIndex != -1)
			{
				at(LastIndex)->FreeBuffers();
				LastIndex = -1;
			}

			switch (BaseType)
			{
				case BASETYPE_MSG:
					CloseArea_Msg(this);
					break;
				case BASETYPE_JAM:
					CloseArea_Jam(this);
					break;
				case BASETYPE_SQUISH:
					CloseArea_Squish(this);
					break;
				default:
					break;
			}
		}
}

void TArea::Done()
{
	while (isOpened())
		Close();

	switch (BaseType)
	{
		case BASETYPE_MSG:
			DoneArea_Msg(this);
			break;
		case BASETYPE_JAM:
			DoneArea_Jam(this);
			break;
		case BASETYPE_SQUISH:
			DoneArea_Squish(this);
			break;
		default:
			break;
	}
	clear();
}

uint32_t TArea::GetLastRead()
{
	if (dummyLastRead == -1)
		switch (BaseType)
		{
			case BASETYPE_MSG:
				dummyLastRead = GetLastReadArea_Msg(this, DEFAULT_USERID);
				break;
			case BASETYPE_JAM:
				dummyLastRead = GetLastReadArea_Jam(this, DEFAULT_USERID);
				break;
			case BASETYPE_SQUISH:
				dummyLastRead = GetLastReadArea_Squish(this, DEFAULT_USERID);
				break;
			default:
				break;
		}

	return (uint32_t)QMAX(dummyLastRead, 0);
}

void TArea::SetLastRead(uint32_t Index)
{
	if ((dummyLastRead == -1) || (dummyLastRead != (int32_t)Index))
	{
		dummyLastRead = Index;

		switch (BaseType)
		{
			case BASETYPE_MSG:
				SetLastReadArea_Msg(this, DEFAULT_USERID, Index);
				break;
			case BASETYPE_JAM:
				SetLastReadArea_Jam(this, DEFAULT_USERID, Index);
				break;
			case BASETYPE_SQUISH:
				SetLastReadArea_Squish(this, DEFAULT_USERID, Index);
				break;
			default:
				break;
		}
	}
}

void TArea::MarkAsRead(uint32_t Index)
{
	if (!isReaded(at(Index)))
	{
		at(Index)->attr |= FLAG_READ;
		if (Open())
		{
			switch (BaseType)
			{
				case BASETYPE_MSG:
					MarkAsReadArea_Msg(this, Index);
					break;
				case BASETYPE_JAM:
					MarkAsReadArea_Jam(this, Index);
					break;
				case BASETYPE_SQUISH:
					MarkAsReadArea_Squish(this, Index);
					break;
				default:
					break;
			}
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
