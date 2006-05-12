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

#include "tactionlist.h"

TActionList::TActionList(TActions *_Actions, QPopupMenu *_Popup, QString _Group)
{
	Actions = _Actions;
	Popup = _Popup;
	Group = _Group;
	setAutoDelete(true);
}

void TActionList::AddAction(QAction *_Action, QString _Pixmap, QString _ToolTip, QString _DescAdd, QString _Accel, bool _NotComplete)
{
	append(new TAction(this, _Action, _Pixmap, _ToolTip, _DescAdd, _Accel, _NotComplete));
}

void TActionList::Save()
{
	for (uint32_t i = 0; i < count(); i++)
		at(i)->Save();
}
