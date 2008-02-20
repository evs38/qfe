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

#ifndef _TAREAITEM_H_
#define _TAREAITEM_H_

#include <qpainter.h>
#include <qlistview.h>

class Qt;
class TAreaItem;

#include "tarea.h"

class TAreaItem : public QListViewItem
{
public:
	TAreaItem(QListView*, TArea*);

	TArea *Area;

	inline void UpdateArea()
	{
		if (isVisible())
			repaint();
	};

private:
	uint32_t Index;

protected:
	virtual int compare(QListViewItem*, int, bool) const;
	virtual void paintCell(QPainter*, const QColorGroup&, int, int, int);
};

#endif /* _TAREAITEM_H_ */
