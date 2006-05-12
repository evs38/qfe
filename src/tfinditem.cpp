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

#include "tfinditem.h"

extern TConfig *Config;

TFindItem::TFindItem(QListView *parent, TArea *_Area, uint32_t _idx) : QListViewItem(parent)
{
	Area = _Area;
	Message = Area->at(_idx);
	setText(0, Area->Name);
	setText(1, Config->toUTF((char*)Message->from));
	setText(2, Config->toUTF((char*)Message->to));
	setText(3, Config->toUTF((char*)Message->subj));
}

void TFindItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
	if (!isReaded(Message))
	{
		QFont f(p->font());
		f.setBold(true);
		p->setFont(f);
	}
	QListViewItem::paintCell(p, cg, column, width, alignment | Qt::AlignTop);
}
