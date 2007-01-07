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

extern TConfig *Config;

void TReplace::init()
{
	FirstTimeInit = false;

        setIcon(*Config->FindPixmap("icon_replace"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnCancel->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));

	Config->GetPos(this);
}

void TReplace::showEvent(QShowEvent *)
{
	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	BtnOk->setEnabled(false);
}

void TReplace::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	e->accept();
}

void TReplace::TextChanged(const QString&)
{
	BtnOk->setEnabled(!TextEdit->text().stripWhiteSpace().isEmpty() && !ReplaceEdit->text().stripWhiteSpace().isEmpty());
}
