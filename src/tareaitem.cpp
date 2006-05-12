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

#include "tareaitem.h"

TAreaItem::TAreaItem(QListView *parent, TArea *_Area) : QListViewItem(parent)
{
	setMultiLinesEnabled(false);
	Area = _Area;
	Index = parent->childCount();
	setText(0, QString::number(Index));
	setText(2, Area->Desc);
}

void TAreaItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
	if (column == 1)
	{
		setText(1, QString("%1 (%2/%3)").arg(Area->Name).arg(Area->count()).arg(Area->UnReadCnt));
		if (Area->UnReadCnt)
		{
			QFont f(p->font());
			f.setBold(true);
			p->setFont(f);
		}
	}
	QListViewItem::paintCell(p, cg, column, width, ((column == 0) ? Qt::AlignHCenter : alignment) | Qt::AlignTop);
}

int TAreaItem::compare(QListViewItem *i, int col, bool ascending) const
{
	int ret = 0;

	switch (col)
	{
		case 0:
			ret = ((Index > ((TAreaItem*)i)->Index) ? -1 : ((Index < ((TAreaItem*)i)->Index) ? 1 : 0));
			break;
		case 1:
		case 2:
			ret = QListViewItem::compare(i, col, ascending);
			break;
		default:
			break;
	}

	return ret;
}
