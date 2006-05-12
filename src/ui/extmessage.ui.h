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

void TExtMessage::init()
{
        setIcon(*Config->FindPixmap("icon_question"));

	IconLabel->setPixmap(*Config->FindPixmap("icon_question"));

	BtnYes->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnNo->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));
}

void TExtMessage::destroy()
{
	Config->SetBool(CONFIG_SECTION_CONFIRM, BoolParam, CheckBox->isChecked());
}

bool TExtMessage::SetUp(QString Mess, QString _BoolParam)
{
	bool tmp;

	MessLabel->setText(Mess);
	BoolParam = _BoolParam;
	tmp = Config->GetBool(CONFIG_SECTION_CONFIRM, BoolParam, true);
	CheckBox->setChecked(tmp);

	return tmp;
}
