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

void TMessAttr::init()
{
        setIcon(*Config->FindPixmap("icon_flag"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnCancel->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));

	Config->GetPos(this);

	Allow = 0;
}

void TMessAttr::SetCheckBox(QCheckBox *box, QString Name, QString Hint, bool _Allow, bool State)
{
	box->setText(Name);
	box->setEnabled(_Allow);
	box->setChecked(State);
	QToolTip::add(box, Hint);
}

void TMessAttr::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	e->accept();
}

void TMessAttr::SetUp(uint _Allow, uint State)
{
	Allow = _Allow;
	SetCheckBox(Flag1, "Private", flag2desc(FLAG_PVT), Allow & FLAG_PVT, State & FLAG_PVT);
	SetCheckBox(Flag2, "Crash", flag2desc(FLAG_CRASH), Allow & FLAG_CRASH, State & FLAG_CRASH);
	SetCheckBox(Flag3, "Sent", flag2desc(FLAG_SENT), Allow & FLAG_SENT, State & FLAG_SENT);
	SetCheckBox(Flag4, "File", flag2desc(FLAG_FILE), Allow & FLAG_FILE, State & FLAG_FILE);
	SetCheckBox(Flag5, "Forward", flag2desc(FLAG_FWD), Allow & FLAG_FWD, State & FLAG_FWD);
	SetCheckBox(Flag6, "Orphan", flag2desc(FLAG_ORP), Allow & FLAG_ORP, State & FLAG_ORP);
	SetCheckBox(Flag7, "Kill", flag2desc(FLAG_KILL), Allow & FLAG_KILL, State & FLAG_KILL);
	SetCheckBox(Flag8, "Local", flag2desc(FLAG_LOC), Allow & FLAG_LOC, State & FLAG_LOC);
	SetCheckBox(Flag9, "Hold", flag2desc(FLAG_HLD), Allow & FLAG_HLD, State & FLAG_HLD);
	SetCheckBox(Flag10, "Direct", flag2desc(FLAG_DIR), Allow & FLAG_DIR, State & FLAG_DIR);
	SetCheckBox(Flag11, "File Request", flag2desc(FLAG_FRQ), Allow & FLAG_FRQ, State & FLAG_FRQ);
	SetCheckBox(Flag12, "Receipt Request", flag2desc(FLAG_RRQ), Allow & FLAG_RRQ, State & FLAG_RRQ);
	SetCheckBox(Flag13, "Is Return Receipt", flag2desc(FLAG_CPT), Allow & FLAG_CPT, State & FLAG_CPT);
	SetCheckBox(Flag14, "Audit Request", flag2desc(FLAG_ARQ), Allow & FLAG_ARQ, State & FLAG_ARQ);
	SetCheckBox(Flag15, "Update Request", flag2desc(FLAG_URQ), Allow & FLAG_URQ, State & FLAG_URQ);
	SetCheckBox(Flag16, "Immediate", flag2desc(FLAG_IMM), Allow & FLAG_IMM, State & FLAG_IMM);
}

uint TMessAttr::GetState()
{
	uint32_t tmp = 0;

	tmp |= Flag1->isChecked() ? FLAG_PVT : 0;
	tmp |= Flag2->isChecked() ? FLAG_CRASH : 0;
	tmp |= Flag3->isChecked() ? FLAG_SENT : 0;
	tmp |= Flag4->isChecked() ? FLAG_FILE : 0;
	tmp |= Flag5->isChecked() ? FLAG_FWD : 0;
	tmp |= Flag6->isChecked() ? FLAG_ORP : 0;
	tmp |= Flag7->isChecked() ? FLAG_KILL : 0;
	tmp |= Flag8->isChecked() ? FLAG_LOC : 0;
	tmp |= Flag9->isChecked() ? FLAG_HLD : 0;
	tmp |= Flag10->isChecked() ? FLAG_DIR : 0;
	tmp |= Flag11->isChecked() ? FLAG_FRQ : 0;
	tmp |= Flag12->isChecked() ? FLAG_RRQ : 0;
	tmp |= Flag13->isChecked() ? FLAG_CPT : 0;
	tmp |= Flag14->isChecked() ? FLAG_ARQ : 0;
	tmp |= Flag15->isChecked() ? FLAG_URQ : 0;
	tmp |= Flag16->isChecked() ? FLAG_IMM : 0;

	return (tmp & Allow);
}
