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

extern TAreas *Areas;
extern TConfig *Config;

void TAreaProp::init()
{
        setIcon(*Config->FindPixmap("icon_prop"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));

	Config->GetPos(this);
}

void TAreaProp::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	e->accept();
}

void TAreaProp::SetUp(void *_Area)
{
	uint32_t i, cntall = 0, cntunread = 0;
	TArea *Area = (TArea*)_Area;

	AreaNameEdit->setText(Area->Name);
	AreaPathEdit->setText(Area->Path);
	AreaDescEdit->setText(Area->Desc);
	AreaAkaEdit->setText(Area->Aka);
#warning "Add Area->Uplink"
	switch (Area->BaseType)
	{
		case BASETYPE_JAM:
			AreaTypeEdit->setText("Jam");
			break;
		case BASETYPE_SQUISH:
			AreaTypeEdit->setText("Squish");
			break;
		case BASETYPE_MSG:
			AreaTypeEdit->setText("Msg");
			break;
		default:
			AreaTypeEdit->setText("Unknown");
			break;
	}
	AreaGroupEdit->setText(Area->Group);

#if !defined(FIDOCONF_VER1_4)
	for (i = 0; i < Config->FidoConfig->groupCount; i++)
		if (strcompare(Config->FidoConfig->group[i].name, Area->Group))
		{
			if (!QString(Config->FidoConfig->group[i].name).isEmpty())
			{
				GroupDescLabel->setEnabled(true);
				AreaGroupDescEdit->setEnabled(true);
				AreaGroupDescEdit->setText(Config->FidoConfig->group[i].name);
			}
			break;
		}
#endif

	SumLabel1->setText("<b>" + QObject::tr("Summary areas:") + "</b> " + QString::number(Areas->count()));

	for (i = 0; i < Areas->count(); i++)
	{
		cntall += Areas->at(i)->count();
		cntunread += Areas->at(i)->UnReadCnt;
	}
	SumLabel2->setText("<b>" + QObject::tr("Summary messages:") + "</b> " + QString::number(cntall));
	SumLabel3->setText("<b>" + QObject::tr("Summary unread:") + "</b> " + QString::number(cntunread));
}
