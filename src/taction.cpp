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

#include "taction.h"

extern TConfig *Config;

TAction::TAction(TActionList *_ActionList, QAction *_Action, QString _Pixmap, QString _ToolTip, QString _DescAdd, QString _Accel, bool _NotComplete)
{
	Action = _Action;
	ActionList = _ActionList;
	Pixmap = _Pixmap;
	ToolTip = _ToolTip;
	DescAdd = _DescAdd;
	NotComplete = _NotComplete;
	if (!Pixmap.isEmpty())
	{
		QPixmap *ActionPix = Config->FindPixmap(Pixmap);
		if (ActionPix != NULL)
			if (!ActionPix->isNull())
				Action->setIconSet(QIconSet(*ActionPix, QIconSet::Small));
	}
	Action->setToolTip(ToolTip);
	Action->setStatusTip(ToolTip);
	SetAccel(Config->GetAccel(ActionList->Actions->Section, Action, _Accel));
}

void TAction::SetAccel(QString _Accel)
{
	QString PixName = Pixmap;
	if (!Pixmap.isEmpty())
	{
		if (!PixName.endsWith(".xpm"))
			PixName = PixName.append(".xpm");
		PixName = QString("<img src=\"%1\">").arg(PixName);
	} else
		PixName = QString::null;
	QString WhatIs = QString("<table><tr><td>%1</td><td><nobr><b>%2 | %3<b></nobr></td></tr>").arg(PixName).arg(ActionList->Group).arg(Action->text());
	QString _ToolTip = ToolTip;
	if (!_ToolTip.endsWith("."))
		_ToolTip.append(".");
	WhatIs.append(QString("<tr><td></td><td>%1</td></tr>").arg(_ToolTip));
	if (!DescAdd.isEmpty())
	{
		QString _DescAdd = DescAdd;
		if (!_DescAdd.endsWith("."))
			_DescAdd.append(".");
		WhatIs.append(QString("<tr><td></td><td>%1</td></tr>").arg(_DescAdd));
	}
	if (!_Accel.isEmpty())
		WhatIs.append(QString("<tr><td></td><td><nobr>%1: <b>\"%2\"</b></nobr></td></tr>").arg(QObject::tr("Shortcut key for this action is")).arg((QString)_Accel));
	if (NotComplete)
		WhatIs.append("<tr><td></td><td><nobr><font color=\"#ff0000\"><b>" + QObject::tr("This action is under construction yet.") + "</b></font></nobr></td></tr>");
	WhatIs.append(QString("</table><br><hr><nobr><p align=\"right\"><font size=\"-1\">%1 <a href=\"Help\">%2</a> %3</font></p></nobr>").arg(QObject::tr("See")).arg(QObject::tr("Manual")).arg(QObject::tr("for Additional Information.")));
	Action->setWhatsThis(WhatIs);
	Action->setAccel(_Accel);
}

void TAction::Save()
{
	Config->SetAccel(ActionList->Actions->Section, Action);
}
