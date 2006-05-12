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

#include "tnodeitem.h"

TNodeItem::TNodeItem(QListView *parent, TNodeItem *after, TNode *_Node) : QListViewItem(parent, after)
{
	setMultiLinesEnabled(false);
	Node = _Node;
	Parsed = false;
}

TNodeItem::TNodeItem(TNodeItem *parent, TNodeItem *after, TNode *_Node) : QListViewItem(parent, after)
{
	setMultiLinesEnabled(false);
	Node = _Node;
	Parsed = false;
}

void TNodeItem::ParseItem()
{
	if (Parsed)
		return;

	Addr = addr2str1(&Node->addr);

	switch (Node->type)
	{
		case RECORD_TYPE_ZONE:
			setText(0, QString("Zone %1").arg(Node->addr.zone));
			break;
		case RECORD_TYPE_REGION:
			setText(0, QString("Region %1:%2").arg(Node->addr.zone).arg(Node->addr.net));
			break;
		case RECORD_TYPE_HOST:
			setText(0, QString("Host %1:%2").arg(Node->addr.zone).arg(Node->addr.net));
			break;
		case RECORD_TYPE_HUB:
			setText(0, QString("Hub %1:%2/%3").arg(Node->addr.zone).arg(Node->addr.net).arg(Node->addr.node));
			break;
		case RECORD_TYPE_PVT:
			setText(0, QString("Pvt %1:%2/%3").arg(Node->addr.zone).arg(Node->addr.net).arg(Node->addr.node));
			break;
		case RECORD_TYPE_HOLD:
			setText(0, QString("Hold %1:%2/%3").arg(Node->addr.zone).arg(Node->addr.net).arg(Node->addr.node));
			break;
		case RECORD_TYPE_DOWN:
			setText(0, QString("Down %1:%2/%3").arg(Node->addr.zone).arg(Node->addr.net).arg(Node->addr.node));
			break;
		case RECORD_TYPE_NODE:
			setText(0, QString("Node %1:%2/%3").arg(Node->addr.zone).arg(Node->addr.net).arg(Node->addr.node));
			break;
		case RECORD_TYPE_POINT:
			setText(0, "Point " + Addr);
			break;
		default:
			qDebug("Error TNodeItem::ParseItem()");
			break;
	}

	if (tokencount(Node->line, ",") >= 5)
	{
		Name = gettoken(Node->line, 1, ",").replace('_', ' ');
		Town = gettoken(Node->line, 2, ",").replace('_', ' ');
		SysOp = gettoken(Node->line, 3, ",").replace('_', ' ');
		Phone = gettoken(Node->line, 4, ",");
		Speed = gettoken(Node->line, 5, ",");
		Flags = Node->line.mid(Name.length() + Town.length() + SysOp.length() + Phone.length() + Speed.length() + 5);
	}

	Parsed = true;
}

int TNodeItem::compare(QListViewItem *It, int, bool) const
{
	return compareaddr(&Node->addr, &((TNodeItem*)It)->Node->addr);
}

void TNodeItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
	if (!Parsed)
		ParseItem();
	QListViewItem::paintCell(p, cg, column, width, alignment | Qt::AlignTop);
}
