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

extern TAreas *Areas;
extern TConfig *Config;

void TFind::init()
{
	FirstTimeInit = false;

        setIcon(*Config->FindPixmap("icon_find"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));

	Config->GetSize(this, size().width(), size().height());
	Config->GetPos(this);
}

void TFind::showEvent(QShowEvent*)
{
	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	AreaCombo->insertItem(QObject::tr("<<< All Areas >>>"));
	for (uint32_t i = 0; i < Areas->count(); i++)
		AreaCombo->insertItem(Areas->at(i)->Name);

	WhereCombo->insertItem(QObject::tr("<<< Anywhere >>>"));
	WhereCombo->insertItem(QObject::tr("From"));
	WhereCombo->insertItem(QObject::tr("To"));
	WhereCombo->insertItem(QObject::tr("Subject"));
	WhereCombo->insertItem(QObject::tr("Text"));
	WhereCombo->insertItem(QObject::tr("Kludges"));

	FindList->setColumnText(0, QIconSet(*Config->FindPixmap("icon_area")), QObject::tr("Area"));
	FindList->setColumnText(1, QIconSet(*Config->FindPixmap("icon_name")), QObject::tr("From"));
	FindList->setColumnText(2, QIconSet(*Config->FindPixmap("icon_name")), QObject::tr("To"));
	FindList->setColumnText(3, QIconSet(*Config->FindPixmap("icon_subject")), QObject::tr("Subject"));

	TextEdit->setText(Config->GetStr(QString::null, CONFIG_SEARCHTEXT, QString::null));
	CaseSensCheck->setChecked(Config->GetBool(QString::null, CONFIG_SEARCHCASE, false));
	RegExpCheck->setChecked(Config->GetBool(QString::null, CONFIG_SEARCHREGEXP, false));

	QListBoxItem *It;
	It = AreaCombo->listBox()->findItem(Config->GetStr(QString::null, CONFIG_SEARCHAREA, QString::null), Qt::ExactMatch);
	AreaCombo->setCurrentItem((It == NULL) ? 0 : AreaCombo->listBox()->index(It));
	It = WhereCombo->listBox()->findItem(Config->GetStr(QString::null, CONFIG_SEARCHWHERE, QString::null), Qt::ExactMatch);
	WhereCombo->setCurrentItem((It == NULL) ? 0 : WhereCombo->listBox()->index(It));
}

void TFind::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	Config->SetSize(this);
	e->accept();
}

void TFind::TextChanged(const QString&)
{
	BtnOk->setEnabled(!TextEdit->text().stripWhiteSpace().isEmpty());
}

bool TFind::FindInString(QString Value)
{
	if (!Value.isEmpty())
	{
		if (!RegExpCheck->isChecked())
			return (Value.find(TextEdit->text(), 0, CaseSensCheck->isChecked()) > -1);
		return (RegExp.search(Value) > -1);
	}

	return false;
}

bool TFind::FindInMsgText(TMessage *Message)
{
	if (Message->Read())
		return FindInString(Config->toUTF((char*)Message->Area->TxtBuff));
	return false;
}

bool TFind::FindInMsgKludges(TMessage *Message)
{
	if (Message->Read())
		return FindInString(Config->toUTF((char*)Message->Area->CtlBuff));
	return false;
}

bool TFind::FindInMessage(TMessage *Message)
{
	switch (WhereCombo->currentItem())
	{
		case 1:
			return FindInString(Config->toUTF((char*)Message->from));
		case 2:
			return FindInString(Config->toUTF((char*)Message->to));
		case 3:
			return FindInString(DecodeMIMELine((char*)Message->subj));
		case 4:
			return FindInMsgText(Message);
		case 5:
			return FindInMsgKludges(Message);
		default:
			if (FindInString(Config->toUTF((char*)Message->from)))
				return true;
			if (FindInString(Config->toUTF((char*)Message->to)))
				return true;
			if (FindInString(DecodeMIMELine((char*)Message->subj)))
				return true;
			if (FindInMsgText(Message))
				return true;
	}
	return FindInMsgKludges(Message);
}

void TFind::FindInArea(TArea *Area, bool SingleProgress)
{
	if (Area->Open())
	{
		for (uint32_t i = 0; i < Area->count(); i++)
		{
			if (SingleProgress)
				((TMainWindow*)qApp->mainWidget())->ShowProgress((i * 100) / Area->count(), Area->Name, PROGRESS_TYPE_SEARCH);
			if (FindInMessage(Area->at(i)))
				new TFindItem(FindList, Area, i);
		}
		Area->Close();
	}
}

void TFind::Find()
{
	int i;

	FindList->clear();
	if (RegExpCheck->isChecked())
	{
		RegExp.setPattern(TextEdit->text());
		RegExp.setCaseSensitive(CaseSensCheck->isChecked());
		if (!RegExp.isValid())
		{
			errormessage(this, QObject::tr("Invalid regular expression."));
			return;
		}
	}

	AreaCombo->setEnabled(false);
	WhereCombo->setEnabled(false);
	TextEdit->setEnabled(false);
	CaseSensCheck->setEnabled(false);
	RegExpCheck->setEnabled(false);
	BtnOk->setEnabled(false);

	if (AreaCombo->currentItem() != 0)
	{
		i = Areas->Find(AreaCombo->currentText());
		if (i >= 0)
			FindInArea(Areas->at(i), true);
	} else
		for (i = 0; i < (int)Areas->count(); i++)
		{
			((TMainWindow*)qApp->mainWidget())->ShowProgress((i * 100) / Areas->count(), Areas->at(i)->Name, PROGRESS_TYPE_SEARCH);
			FindInArea(Areas->at(i), false);
			qApp->processEvents();
		}
	((TMainWindow*)qApp->mainWidget())->ShowProgress(100, QString::null, PROGRESS_TYPE_SEARCH);
	((TMainWindow*)qApp->mainWidget())->HideProgress();

	AreaCombo->setEnabled(true);
	WhereCombo->setEnabled(true);
	TextEdit->setEnabled(true);
	CaseSensCheck->setEnabled(true);
	RegExpCheck->setEnabled(true);
	BtnOk->setEnabled(true);

	Config->SetStr(QString::null, CONFIG_SEARCHTEXT, TextEdit->text());
	Config->SetBool(QString::null, CONFIG_SEARCHCASE, CaseSensCheck->isChecked());
	Config->SetBool(QString::null, CONFIG_SEARCHREGEXP, RegExpCheck->isChecked());
	Config->SetStr(QString::null, CONFIG_SEARCHAREA, AreaCombo->currentText());
	Config->SetStr(QString::null, CONFIG_SEARCHWHERE, WhereCombo->currentText());
}

void TFind::FindListClicked(QListViewItem *It)
{
	if (It == NULL)
		return;
	WidgetCloseFunc(this);
}

void TFind::FindListSelected(QListViewItem *It)
{
	if (It != NULL)
		((TMainWindow*)qApp->mainWidget())->FindCallBack((TFindItem*)It);
}
