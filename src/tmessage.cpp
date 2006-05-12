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

#include "tmessage.h"

TMessage::TMessage(TArea *_Area)
{
	Area = _Area;
	AreaIndex = _Area->count();

#warning "CtlBuff & TxtBuff must be moved to TArea"
	CtlBuff = NULL;
	TxtBuff = NULL;

	memset(from, '\0', MAX_FROM_NAME_LEN + 1);
	memset(to, '\0', MAX_TO_NAME_LEN + 1);
	memset(subj, '\0', MAX_SUBJ_LEN + 1);

	attr = 0;

	MessageItem = NULL;
};

TMessage::~TMessage()
{
	FreeBuffers();
}

bool TMessage::Read()
{
	return Area->Read(AreaIndex);
};

void TMessage::MarkAsRead()
{
	Area->MarkAsRead(AreaIndex);
};

void TMessage::FreeBuffers()
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

void TMessage::CopyAttributesTo(TMessage *Dest)
{
	qstrcpy((char*)Dest->from, (char*)from);
	qstrcpy((char*)Dest->to, (char*)to);
	qstrcpy((char*)Dest->subj, (char*)subj);

	qstrncpy((char*)&Dest->origaddr, (const char*)&origaddr, sizeof(fidoaddr));
	qstrncpy((char*)&Dest->destaddr, (const char*)&destaddr, sizeof(fidoaddr));

	Dest->dt = dt;
	Dest->attr = attr;
}
