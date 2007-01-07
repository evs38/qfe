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

#include "tbookviewitem.h"

TBookViewItem::TBookViewItem(QListView *parent, TBookItem *Item) : QListViewItem(parent)
{
	setMultiLinesEnabled(false);
	BookItem = Item;
}

void TBookViewItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
	if (BookItem->Name.isEmpty())
	{
		QColorGroup _cg(cg);

		_cg.setColor(QColorGroup::Text, Qt::red);
		setText(0, "!");
		QListViewItem::paintCell(p, _cg, column, width, alignment | Qt::AlignTop);
	} else {
		setText(0, BookItem->Name);
		QListViewItem::paintCell(p, cg, column, width, alignment | Qt::AlignTop);
	}
}
