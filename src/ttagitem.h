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

#ifndef _TTAGITEM_H_
#define _TTAGITEM_H_

#include <qpainter.h>
#include <qlistview.h>
#include <qstylesheet.h>

#include "ttypes.h"

class TTagItem : public QListViewItem
{
public:
	TTagItem(QListView*, uint32_t, QString, QFont*, QColor*, QStyleSheetItem*);

	uint32_t Index;
	QStyleSheetItem *Style;
	QColor *Background;

	void SetFont(QFont*);

private:
	QFont Font;

protected:
	virtual int compare(QListViewItem*, int, bool) const;
	virtual void paintCell(QPainter*, const QColorGroup&, int, int, int);
};

#endif /* _TTAGITEM_H_ */
