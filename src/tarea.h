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

#ifndef _TAREA_H_
#define _TAREA_H_

#include <qlist.h>
#include <qmutex.h>

#include "ttypes.h"

class TArea;

#include "tmessage.h"

class TArea : public QList<TMessage>
{
public:
	TArea(Area_Type, Base_Type, uint16_t);
	~TArea();

	Area_Type AreaType;

	void FreeBuffers();

	bool Init(QString, QString, QString, QString, QString, QString, QString, QString, char*);
	bool Open(bool = false);
	bool Rescan();
	bool Read(uint32_t);
	bool Write(uint32_t);
	TMessage *Append();
	bool Delete(uint32_t);
	bool Kill();
	bool Purge();
	void Close(bool = false);
	void Done();

	uint32_t GetLastRead();
	void SetLastRead(uint32_t);
	void MarkAsRead(uint32_t);

	uint8_t *CtlBuff;
	uint8_t *TxtBuff;

	QString Aka;
	QString Name;
	QString Desc;
	QString Path;
	char *User;

	QString Group;
	QString Uplink;
	QString UplinkPwd;
	QString UplinkRbt;

	Base_Type GetBaseType();

	uint16_t DefZone;
	uint32_t UnReadCnt;
	uint32_t BaseSize;

	void *AreaPvtData;

	int32_t UID2Index(uint32_t);

	QString NormalizeCtl(TMessage*, uint8_t*, bool = true);
	QString Append001(QString);

private:
	Base_Type BaseType;

#if defined(QT_THREAD_SUPPORT)
	int32_t OpenCounter;
	QMutex Mutex;
#else
	volatile int32_t OpenCounter;
#endif

	int32_t LastIndex;
	int32_t dummyLastRead;

	void CalculateUnread();
	inline bool isOpened()
	{
		return OpenCounter > 0;
	}

	/* Base Functions */
	bool (*InitArea_)(TArea*);
	bool (*OpenArea_)(TArea*);
	bool (*RescanArea_)(TArea*);
	bool (*ReadArea_)(TArea*, uint32_t);
	bool (*WriteArea_)(TArea*, uint32_t);
	TMessage* (*AppendArea_)(TArea*, bool);
	bool (*DeleteArea_)(TArea*, uint32_t);
	bool (*KillArea_)(TArea*);
	void (*PurgeArea_)(TArea*);
	void (*CloseArea_)(TArea*);
	void (*DoneArea_)(TArea*);
	uint32_t (*GetLastReadArea_)(TArea*, uint32_t);
	void (*SetLastReadArea_)(TArea*, uint32_t, uint32_t);
	void (*MarkAsReadArea_)(TArea*, uint32_t);
};

#if defined(QT_THREAD_SUPPORT)
#	define WaitMutex()	QMutexLocker MutexLocker(&Mutex)
#else
#	define WaitMutex()	{}
#endif

#define isNetMailArea(x)	(x->AreaType == AREATYPE_NETMAIL)
#define isEchoMailArea(x)	(x->AreaType == AREATYPE_ECHOMAIL)

#endif /* _TAREA_H_ */
