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

#ifndef _TMESSAGEITEM_H_
#define _TMESSAGEITEM_H_

#include <qtimer.h>
#include <qpainter.h>
#include <qdatetime.h>
#include <qlistview.h>

class Qt;
class TMessageItem;

#include "utils.h"

#include "tarea.h"
#include "tconfig.h"
#include "tareaitem.h"
#include "tftncodec.h"

#define MARK_MESSAGE_MARKED	1

class TMessageItem : public QListViewItem
{
public:
	TMessageItem(QListView*, TAreaItem*, uint32_t);
	virtual ~TMessageItem();

	TMessage *Message;

	inline void UpdateMessage()
	{
		if (isVisible())
			repaint();
	}
	inline bool isMarked()
	{
		return (MarkState & MARK_MESSAGE_MARKED) != 0;
	}
	void StartTimer();
	void ToggleMarked();
	void ParseData();

private:
	TAreaItem *AreaItem;
	QDateTime dt;
	int MarkState;
	bool Parsed;
//private slots:
	void MarkMessageAsRead();

protected:
	virtual int compare(QListViewItem*, int, bool) const;
	virtual void paintCell(QPainter*, const QColorGroup&, int, int, int);
};

#endif /* _TMESSAGEITEM_H_ */
