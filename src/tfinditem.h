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

#ifndef _TFINDITEM_H_
#define _TFINDITEM_H_

#include <qpainter.h>
#include <qlistview.h>

#include "tarea.h"

class TFindItem: public QListViewItem
{
public:
	TFindItem(QListView*, TArea*, uint32_t);

	TArea *Area;
	TMessage *Message;

protected:
	virtual void paintCell(QPainter*, const QColorGroup&, int, int, int);
};

#endif /* _TFINDITEM_H_ */
