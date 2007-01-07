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

#include "ttagitem.h"

TTagItem::TTagItem(QListView *parent, uint32_t idx, QString Name, QFont *_Font, QColor *_Background, QStyleSheetItem *_Style) : QListViewItem(parent, Name)
{
	setMultiLinesEnabled(false);
	Index = idx;
	Style = _Style;
	Background = _Background;
	Font = *_Font;
	setText(1, QObject::tr("Example"));
}

void TTagItem::SetFont(QFont *_Font)
{
	Font = *_Font;
	repaint();
}

void TTagItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
	if (column == 1)
	{
		QColorGroup _cg(cg);

		_cg.setColor(QColorGroup::Text, Style->color());
		_cg.setColor(QColorGroup::Base, *Background);
		Font.setWeight(Style->fontWeight());
		Font.setItalic(Style->fontItalic());
		Font.setUnderline(Style->fontUnderline());
		p->setFont(Font);
		QListViewItem::paintCell(p, _cg, column, width, Qt::AlignHCenter | Qt::AlignTop);
	} else
		QListViewItem::paintCell(p, cg, column, width, alignment | Qt::AlignTop);
}

int TTagItem::compare(QListViewItem *i, int, bool) const
{
	return (Index > ((TTagItem*)i)->Index) ? 1 : ((Index < ((TTagItem*)i)->Index) ? -1 : 0);
}
