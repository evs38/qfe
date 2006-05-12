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

#include "tareaselectitem.h"

TAreaSelectItem::TAreaSelectItem(QListView *parent, TArea *_Area) : QListViewItem(parent)
{
	setMultiLinesEnabled(false);
	Area = _Area;
	setText(0, QString::number(parent->childCount()));
	parent->setColumnAlignment(0, Qt::AlignHCenter | Qt::AlignTop);
	parent->setColumnAlignment(1, Qt::AlignLeft | Qt::AlignTop);
	parent->setColumnAlignment(2, Qt::AlignLeft | Qt::AlignTop);
	setText(1, Area->Name);
	setText(2, Area->Desc);
}

int TAreaSelectItem::compare(QListViewItem *i, int col, bool ascending) const
{
	int ret = 0;

	switch (col)
	{
		case 0:
		{
			bool Test1 = false, Test2 = false;
			int Value1, Value2;

			Value1 = key(col, ascending).toInt(&Test1);
			Value2 = i->key(col, ascending).toInt(&Test2);
			if (Test2 && Test2)
				ret = ((Value1 > Value2) ? -1 : ((Value1 < Value2) ? 1 : 0));
			break;
		}
		case 1:
		case 2:
			ret = QListViewItem::compare(i, col, ascending);
			break;
		default:
			break;
	}

	return ret;
}
