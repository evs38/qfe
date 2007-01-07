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

#include "tmessageitem.h"

extern TConfig *Config;

TMessageItem::TMessageItem(QListView *parent, TAreaItem *_AreaItem, uint32_t Index) : QListViewItem(parent)
{
	setMultiLinesEnabled(false);
	AreaItem = _AreaItem;
	Message = AreaItem->Area->at(Index);
	Message->MessageItem = this;
	MarkState = 0;

	Parsed = false;
}

TMessageItem::~TMessageItem()
{
}

void TMessageItem::ParseData()
{
	if (!Parsed)
	{
		setText(0, QString::number(Message->AreaIndex + 1));
		setText(1, QString::null);
		setText(2, Config->toUTF((char*)Message->from));
		setText(3, Config->toUTF((char*)Message->to));
		setText(4, DecodeMIMELine((char*)Message->subj));
		setText(5, QString::number(Message->size));
		dt.setTime_t(Message->dt);
		setText(6, dt.toString("hh:mm:ss dd.MM.yyyy"));

		Parsed = true;
	}
}

void TMessageItem::ToggleMarked()
{
	if (isMarked())
	{
		MarkState &= ~MARK_MESSAGE_MARKED;
		setPixmap(1, NULL);
	} else {
		MarkState |= MARK_MESSAGE_MARKED;
		setPixmap(1, *Config->FindPixmap("icon_mark"));
	}
	UpdateMessage();
}

void TMessageItem::StartTimer()
{
	if (!isReaded(Message))
	{
		//QTimer::singleShot(1000, this, SLOT(MarkMessageAsRead()));
		MarkMessageAsRead();
	}
	Message->Area->SetLastRead(Message->uid);
}

void TMessageItem::MarkMessageAsRead()
{
	if (isSelected())
	{
		Message->MarkAsRead();
		AreaItem->UpdateArea();
		UpdateMessage();
	}
}

void TMessageItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
	if (column > 0)
	{
		QColorGroup _cg(cg);

		if (!isReaded(Message))
		{
			QFont f(p->font());
			f.setBold(true);
			p->setFont(f);
		}

		ParseData();

		if (Message->Area->AreaType == AREATYPE_ECHOMAIL)
		{
			if (strcompare(text(2), Config->FidoConfig->sysop))
				_cg.setColor(QColorGroup::Text, Qt::red);
			else
				if (strcompare(text(3), Config->FidoConfig->sysop))
					_cg.setColor(QColorGroup::Text, Qt::red);
		}
		QListViewItem::paintCell(p, _cg, column, width, alignment | Qt::AlignTop);
	} else
		QListViewItem::paintCell(p, cg, column, width, Qt::AlignHCenter | Qt::AlignTop);
}

int TMessageItem::compare(QListViewItem *i, int col, bool ascending) const
{
	int ret;

	switch (col)
	{
		case 0:
			ret = ((Message->AreaIndex > ((TMessageItem*)i)->Message->AreaIndex) ? -1 : ((Message->AreaIndex < ((TMessageItem*)i)->Message->AreaIndex) ? 1 : 0));
			break;
		case 5:
			ret = ((Message->size > ((TMessageItem*)i)->Message->size) ? -1 : ((Message->size < ((TMessageItem*)i)->Message->size) ? 1 : 0));
			break;
		case 6:
			ret = ((Message->dt > ((TMessageItem*)i)->Message->dt) ? -1 : ((Message->dt < ((TMessageItem*)i)->Message->dt) ? 1 : 0));
			break;
		case 2:
		case 3:
		case 4:
			((TMessageItem*)this)->ParseData();
			((TMessageItem*)i)->ParseData();
			ret = QListViewItem::compare(i, col, ascending);
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}
