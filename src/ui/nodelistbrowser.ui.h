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

void TNodelistBrowser::init()
{
	FirstTimeInit = false;

	setIcon(*Config->FindPixmap("icon_browser"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnCancel->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));

	BtnFindAddr->setIconSet(QIconSet(*Config->FindPixmap("icon_find")));
	BtnFindName->setIconSet(QIconSet(*Config->FindPixmap("icon_find")));

	Config->GetPos(this);

	Nodes = new TNodes();
	NodeTree->setSortColumn(-1);

	SelectMode = false;
	Finded = NULL;
	Selected = NULL;
}

void TNodelistBrowser::destroy()
{
	delete Nodes;
}

void TNodelistBrowser::showEvent(QShowEvent *)
{
	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	NodeTree->setUpdatesEnabled(false);
	NodeTree->blockSignals(true);

	QString CompiledName = Config->ConfigPath + COMPILED_NODELIST_FILE;
	if (Nodes->CheckModified(CompiledName))
	{
		Nodes->Compile();
		Nodes->SaveToStream(CompiledName);
	} else
		if (!Nodes->LoadFromStream(CompiledName))
		{
			Nodes->Compile();
			Nodes->SaveToStream(CompiledName);
		}

	Nodes->AssignListView(NodeTree);
	NodeTree->blockSignals(false);
	NodeTree->setUpdatesEnabled(true);
	NodeTree->update();

	FindAddrEdit->setValidator(new QRegExpValidator(QRegExp(REGEXP_ADDRESS), this));
}

void TNodelistBrowser::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	Config->SetSize(this);
	e->accept();
}

void TNodelistBrowser::SetSelectMode(bool Value)
{
	SelectMode = Value;
}

bool TNodelistBrowser::TestLists()
{
	return (Nodes->TestCount() > 0);
}

void TNodelistBrowser::ClearResult()
{
	Finded = NULL;
	ResultLabel->clear();
}

void TNodelistBrowser::TreeSelectionChanged(QListViewItem *It)
{
	NameEdit->setText(QString::null);
	SysOpEdit->setText(QString::null);
	LocationEdit->setText(QString::null);
	PhoneEdit->setText(QString::null);
	SpeedEdit->setText(QString::null);
	FlagsEdit->setText(QString::null);
	BtnOk->setEnabled(false);

	Selected = NULL;
	ClearResult();

	if (It != NULL)
	{
		TNodeItem *Item = (TNodeItem*)It;
		NameEdit->setText(Item->Name);
		SysOpEdit->setText(Item->SysOp);
		LocationEdit->setText(Item->Town);
		PhoneEdit->setText(Item->Phone);
		SpeedEdit->setText(Item->Speed);
		FlagsEdit->setText(Item->Flags);

		Selected = Item;

		BtnOk->setEnabled(SelectMode);
	}
}

void TNodelistBrowser::TreeDoubleClicked(QListViewItem *It)
{
	if ((It != NULL) && BtnOk->isEnabled())
		accept();
}

void TNodelistBrowser::FindAddrEditChanged(const QString &txt)
{
	int pos = 0;
	bool rxvalid = FindAddrEdit->validator()->validate((QString&)txt, pos) == QValidator::Acceptable;

	FindAddrEdit->setPaletteForegroundColor(rxvalid ? Qt::black : Qt::red);
	BtnFindAddr->setEnabled(rxvalid);
	ClearResult();
}

void TNodelistBrowser::BtnFindAddrClicked()
{
	hs_addr faddr;
	QString txt = FindAddrEdit->text().stripWhiteSpace();

	/* Cut domain && append .0 for node-addresses */
	/* str -> hs_addr -> str */
	if (!str2addr(txt, &faddr))
		return;

	txt = addr2str2(faddr, true);

	QListViewItemIterator it = QListViewItemIterator(NodeTree);

	if (Finded != NULL)
		while (it.current())
		{
			if (Finded == (TNodeItem*)*it)
			{
				++it;
				break;
			}
			++it;
		}

	Finded = NULL;

	while (it.current())
	{		
		if (addr2str1(&((TNodeItem*)*it)->Node->addr, NULL, true) == txt)
		{
			((TNodeItem*)*it)->ParseItem();
			NodeTree->ensureItemVisible(it.current());
			NodeTree->setSelected(it.current(), true);
			Finded = (TNodeItem*)*it;
			break;
		}
		++it;
	}
	if (Finded == NULL)
		ResultLabel->setText("<nobr><font color=\"#ff0000\"><b>" + QObject::tr("Address not found.") + "</b></font></nobr>");
	BtnFindAddr->setEnabled(false);
}

void TNodelistBrowser::FindNameEditChanged(const QString &txt)
{
	BtnFindName->setEnabled(!txt.stripWhiteSpace().isEmpty());
	ClearResult();
}

void TNodelistBrowser::BtnFindNameClicked()
{
	QString txt = FindNameEdit->text().stripWhiteSpace();

	QListViewItemIterator it = QListViewItemIterator(NodeTree);

	if (Finded != NULL)
		while (it.current())
		{
			if (Finded == (TNodeItem*)*it)
			{
				++it;
				break;
			}
			++it;
		}

	Finded = NULL;

	while (it.current())
	{		
		((TNodeItem*)*it)->ParseItem();
		if (((TNodeItem*)*it)->SysOp.find(txt, 0, false) > -1)
		{
			NodeTree->ensureItemVisible(it.current());
			NodeTree->setSelected(it.current(), true);
			Finded = (TNodeItem*)*it;
			break;
		}
		++it;
	}

	if (Finded == NULL)
		ResultLabel->setText("<nobr><font color=\"#ff0000\"><b>" + QObject::tr("Address not found.") + "</b></font></nobr>");
	BtnFindName->setEnabled(false);
}
