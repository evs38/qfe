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

#ifndef _TACTION_H_
#define _TACTION_H_

#include <qaction.h>

class TAction;

#include "tconfig.h"
#include "tactionlist.h"

class TAction
{
public:
	TAction(TActionList*, QAction*, QString, QString, QString, QString, bool);

	QAction *Action;

	void SetAccel(QString);
	void Save();

private:
	TActionList *ActionList;
	QString Pixmap;
	QString ToolTip;
	QString DescAdd;
	bool NotComplete;
};

#endif /* _TACTION_H_ */
