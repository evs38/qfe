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

#ifndef _TPIXMAPVIEW_H_
#define _TPIXMAPVIEW_H_

#include <qurl.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qfiledialog.h>
#include <qscrollview.h>

class TPixmapView : public QScrollView, public QFilePreview
{
	Q_OBJECT
public:
	TPixmapView(QWidget*);
	void setPixmap(const QPixmap&);
	void drawContents(QPainter*, int, int, int, int);
	void previewUrl(const QUrl&);

private:
	QPixmap pixmap;
};

#endif /* _TPIXMAPVIEW_H_ */
