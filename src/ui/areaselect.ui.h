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

extern TConfig *Config;

void TAreaSelect::init()
{
	setIcon(*Config->FindPixmap("icon_browser"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnCancel->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));
	SelectAreaList->setColumnText(0, QIconSet(*Config->FindPixmap("icon_index")), "N");
	SelectAreaList->setColumnText(1, QIconSet(*Config->FindPixmap("icon_area")), QObject::tr("Area"));
	SelectAreaList->setColumnText(2, QIconSet(*Config->FindPixmap("icon_desc")), QObject::tr("Description"));
	Config->GetList(QString::null, SelectAreaList);

	Config->GetPos(this);

	AreasCnt = 0;
}

void TAreaSelect::closeEvent(QCloseEvent *e)
{
	Config->SetList(QString::null, SelectAreaList);
	Config->SetPos(this);
	e->accept();
}

void TAreaSelect::AddArea(void *Area)
{
	new TAreaSelectItem(SelectAreaList, (TArea*)Area);
	AreasCnt++;
}

void TAreaSelect::AreaSelectionChanged(QListViewItem *it)
{
	BtnOk->setEnabled(it != NULL);
}

void TAreaSelect::AreaDoubleClicked(QListViewItem *it)
{
	if (it != NULL)
		accept();
}
