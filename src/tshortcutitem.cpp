/***************************************************************************
 *   Copyright (C) 2005-2008 by Alexander Shiyan                           *
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

#include "tshortcutitem.h"

TShortCutItem::TShortCutItem(QListViewItem *parent, TShortCutItem *after, TAction *_Action) : QListViewItem(parent, after)
{
	setMultiLinesEnabled(false);
	Action = _Action;
	if (!Action->Action->iconSet().isNull())
		setPixmap(0, Action->Action->iconSet().pixmap());

	setText(0, Action->Action->text());
	SetAccel(Action->Action->accel());
}

bool TShortCutItem::SetAccel(QString _key)
{
	bool ok = true;

	Accel = QString::null;
	if (!_key.isEmpty())
	{
		QListViewItemIterator it(listView());
		for (; *it; it++)
			if (((QListViewItem*)*it)->isSelectable())
				if ((TShortCutItem*)*it != this)
					if (((TShortCutItem*)*it)->parent() == parent())
						if (_key.compare(((TShortCutItem*)*it)->GetAccel()) == 0)
						{
							ok = false;
							break;
						}
		if (ok)
			Accel = _key;
	}

	repaint();
	return ok;
}

void TShortCutItem::ClearAccel()
{
	SetAccel(QString::null);
}

void TShortCutItem::Apply()
{
	Action->SetAccel(Accel);
}

void TShortCutItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
	if (column == 1)
		setText(1, Accel);
	QListViewItem::paintCell(p, cg, column, width, alignment | Qt::AlignTop);
}
