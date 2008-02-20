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

#include "tpixmapview.h"

TPixmapView::TPixmapView(QWidget *parent) : QScrollView(parent)
{
	viewport()->setBackgroundMode(PaletteBase);
}

void TPixmapView::setPixmap(const QPixmap &pix)
{
	pixmap = pix;
	resizeContents(pixmap.size().width(), pixmap.size().height());
	viewport()->repaint(false);
}

void TPixmapView::drawContents(QPainter *p, int cx, int cy, int cw, int ch)
{
	p->fillRect(cx, cy, cw, ch, colorGroup().brush(QColorGroup::Base));
	p->drawPixmap(0, 0, pixmap);
}

void TPixmapView::previewUrl(const QUrl &u)
{
	if (u.isLocalFile())
	{
		QPixmap pix(u.path());
		setPixmap(pix.isNull() ? NULL : pix);

	} else
		setPixmap(NULL);
}
