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

#define MESS_NOTAREA		QObject::tr("Can't find Area.")
#define MESS_NOTMESSAGE		QObject::tr("Can't find Message.")

#define QUOTE_BREAK_POS		78

void TEditWindow::init()
{
	FirstTimeInit = false;

	setIcon(*Config->FindPixmap("icon_edit"));

	BtnAttributes->setIconSet(QIconSet(*Config->FindPixmap("icon_flag")));
	BtnAddrBook->setIconSet(QIconSet(*Config->FindPixmap("icon_addressbook")));
	BtnNodeList->setIconSet(QIconSet(*Config->FindPixmap("icon_browser")));

	EditActions = new TActions();

	EditActions->AddAction(MessageMenu, EditMenuBar->text(1), SaveMessageAction, "icon_sent", QObject::tr("Save Message to MessageBase"), QString::null, "Ctrl+S");
	EditActions->AddAction(MessageMenu, EditMenuBar->text(1), SaveDraftAction, "icon_draft", QObject::tr("Save Message to Draft Folder"), QObject::tr("You must continue editing this message later."), "Ctrl+D");
	EditActions->AddAction(MessageMenu, EditMenuBar->text(1), QuitAction, "icon_exit", QObject::tr("Exit from Editor without Save"), QString::null, "Ctrl+Q");

	EditActions->AddAction(EditMenu, EditMenuBar->text(2), UndoAction, "icon_undo", QObject::tr("Undo Last Operation"), QString::null, "Ctrl+Z");
	EditActions->AddAction(EditMenu, EditMenuBar->text(2), RedoAction, "icon_redo", QObject::tr("Redo Last Operation"), QString::null, "Ctrl+Shift+Z");
	EditActions->AddAction(EditMenu, EditMenuBar->text(2), CopyAction, "icon_copy", QObject::tr("Copy selected text to Clipboard"), QString::null, "Ctrl+C");
	EditActions->AddAction(EditMenu, EditMenuBar->text(2), CutAction, "icon_cut", QObject::tr("Cut selected text to Clipboard"), QString::null, "Ctrl+X");
	EditActions->AddAction(EditMenu, EditMenuBar->text(2), DelAction, "icon_delete", QObject::tr("Delete selected text"), QString::null, "Ctrl+D");
	EditActions->AddAction(EditMenu, EditMenuBar->text(2), PasteAction, "icon_paste", QObject::tr("Paste text from Clipboard"), QString::null, "Ctrl+V");
	EditActions->AddAction(EditMenu, EditMenuBar->text(2), InsertAction, "icon_insert", QObject::tr("Insert Text file into Current Message"));
	EditActions->AddAction(EditMenu, EditMenuBar->text(2), AttachAction, "icon_attach", QObject::tr("Insert File into Current Message in UU-Format"), QString::null, QString::null, true);

	EditActions->AddAction(ViewMenu, EditMenuBar->text(3), ViewToolBarAction, QString::null, QObject::tr("Show/Hide ToolBar"));
	EditActions->AddAction(ViewMenu, EditMenuBar->text(3), ViewStatusBarAction, QString::null, QObject::tr("Show/Hide StatusBar"));
	EditActions->AddAction(ViewMenu, EditMenuBar->text(3), ViewOriginAction, QString::null, QObject::tr("Show/Hide Origin Line"));

	EditActions->AddAction(ToolMenu, EditMenuBar->text(4), SpellCheckAction, "icon_spell", QObject::tr("Check Message for Spelling Errors"), QString::null, QString::null, true);

	EditActions->AddAction(HelpMenu, EditMenuBar->text(5), HelpAction, "icon_help", QObject::tr("View Program Help"), QString::null, "F1", true);
	EditActions->AddAction(HelpMenu, EditMenuBar->text(5), WhatsThisAction, "icon_whatsthis", QObject::tr("Enter in \"What is This?\"-Mode"), QString::null, "Shift+F1");

	WhatsThisAction->addTo(EditToolBar);
	EditToolBar->setCloseMode(QDockWindow::Undocked);

	Config->GetSize(this, size().width(), size().height());
	Config->GetPos(this);

	ToNameCombo->setValidator(new QRegExpValidator(QRegExp(REGEXP_FROMTO), this));
	ToAddrEdit->setValidator(new QRegExpValidator(QRegExp(REGEXP_ADDRESS), this));
	FromNameCombo->setValidator(new QRegExpValidator(QRegExp(REGEXP_FROMTO), this));

	Config->GetToolBar(this, EditToolBar);
	ViewToolBarAction->setOn(EditToolBar->isShown());

	ViewStatusBarAction->setOn(Config->GetBool(CONFIG_SECTION_EDIT, CONFIG_VIEWSTATUSBAR, true));
	ActionViewStatusBar();

	ViewOriginAction->setOn(Config->GetBool(CONFIG_SECTION_EDIT, CONFIG_VIEWORIGIN, true));
	ActionViewOrigin();

	MessageEdit->setFont(QFont(Config->GetStr(CONFIG_SECTION_EDIT, CONFIG_FONTNAME, DEFAULT_VIEW_FONT), Config->GetInt(CONFIG_SECTION_EDIT, CONFIG_FONTSIZE, DEFAULT_VIEW_FONTSIZE)));
	MessageEdit->setPaletteBackgroundColor(QColor(Config->GetInt(CONFIG_SECTION_EDIT, CONFIG_FONTBACK, QColor(0, 0, 0).rgb())));
	MessageEdit->setPaletteForegroundColor(QColor(Config->GetInt(CONFIG_SECTION_EDIT, CONFIG_FONTCOLOR, QColor(220, 220, 220).rgb())));

	OriginLabel->setText("Origin:");

	PosStatusBar = new QLabel(statusBar());
	PosStatusBar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	PosStatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	PosStatusBar->setLineWidth(0);
	PosStatusBar->setFixedWidth(65);
	PosStatusBar->setMargin(2);
	PosStatusBar->setMidLineWidth(0);
	statusBar()->addWidget(PosStatusBar, 1, true);

	MessageStatusBar = new QLabel(statusBar());
	MessageStatusBar->setAlignment(Qt::AlignTop);
	MessageStatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	MessageStatusBar->setLineWidth(0);
	MessageStatusBar->setMargin(2);
	MessageStatusBar->setMidLineWidth(0);
	statusBar()->addWidget(MessageStatusBar, 1, true);
	connect(statusBar(), SIGNAL(messageChanged(const QString &)), SLOT(statusBarMessageChanged(const QString &)));

	MessageEdit->installEventFilter(this);

	OrigArea = QString::null;
	DestArea = QString::null;
	UniqueID = 0;
	SetupDone = false;
	Result = QString::null;
}

void TEditWindow::destroy()
{
	statusBar()->removeWidget(MessageStatusBar);
	statusBar()->removeWidget(PosStatusBar);
	delete MessageStatusBar;
	delete PosStatusBar;

	SpellCheckDone();
	delete EditActions;
}

void TEditWindow::statusBarMessageChanged(const QString &string)
{
	MessageStatusBar->setText(string.stripWhiteSpace().remove("\n"));
}

bool TEditWindow::SetUp(TArea *Area, TMessage *Message, uint Operation, void *addval)
{
	QListBoxItem *tmpit;
	int i, textcnt = 0;
	QString tmp, tmpaddr;
	QPoint pos;

	MsgOperation = Operation;

	Kludges.clear();
	MessageEdit->clear();

	ToNameCombo->clear();

	FromAddrCombo->clear();
	FromAddrCombo->insertItem(Area->Aka);
	for (i = 0;; i++)
	{
		tmp = Config->GetAddress(i);
		if (tmp.isEmpty())
			break;
		if (!strcompare(tmp, FromAddrCombo->text(0)))
			FromAddrCombo->insertItem(tmp);
	}

	FromNameCombo->clear();
	FromNameCombo->insertItem(Config->GetSysopName());

	OriginCombo->clear();
	MessFlags = FLAG_LOC;

	switch (MsgOperation)
	{
		case MSG_OPERATION_NEW:
		{
			OrigArea = Area->Name;
			DestArea = Area->Name;

			if (Area->AreaType == AREATYPE_ECHOMAIL)
			{
				ToNameCombo->insertItem("All", 0);
				AllowFlags = FLAG_KILL;

			} else {
				ToNameCombo->insertItem("", 0);
				ToAddrEdit->setEnabled(true);
				MessFlags |= FLAG_PVT;
				AllowFlags = FLAG_PVT | FLAG_CRASH | FLAG_FILE | FLAG_KILL | FLAG_HLD | FLAG_DIR | FLAG_FRQ | FLAG_RRQ | FLAG_ARQ | FLAG_URQ | FLAG_IMM;
			}

			LoadTemplate(TPL_MESSAGE_TPL, &pos);
			ParseTemplate(Area, NULL, false);
			break;
		}
		case MSG_OPERATION_EDIT:
		{
			OrigArea = Message->Area->Name;
			if (strcompare(Area->Name, Config->GetDraftArea()))
			{
				tmp = GetKludge(Message->CtlBuff, "AREA: ");
				if (tmp.isEmpty())
				{
					errormessage(this, MESS_NOTAREA);
					return false;
				}
				DestArea = tmp.mid(6);
			} else
				DestArea = Area->Name;

			MessFlags |= Message->attr;

			i = Areas->Find(DestArea);
			if (i < 0)
			{
				errormessage(this, MESS_NOTAREA);
				return false;
			}
			TArea *DstArea = Areas->at(i);
			if (DstArea->AreaType == AREATYPE_NETMAIL)
			{
				ToAddrEdit->setEnabled(true);
				ToAddrEdit->setText(addr2str1(&Message->destaddr, Message->CtlBuff));
				AllowFlags = FLAG_PVT | FLAG_CRASH | FLAG_FILE | FLAG_KILL | FLAG_HLD | FLAG_DIR | FLAG_FRQ | FLAG_RRQ | FLAG_ARQ | FLAG_URQ | FLAG_IMM;
			} else
				AllowFlags = FLAG_KILL;

			tmp = addr2str1(&Message->origaddr);
			tmpit = FromAddrCombo->listBox()->findItem(tmp);
			if (tmpit != NULL)
				FromAddrCombo->setCurrentItem(FromAddrCombo->listBox()->index(tmpit));
			tmp = (char*)Message->from;
			tmpit = FromNameCombo->listBox()->findItem(tmp);
			if (tmpit == NULL)
			{
				FromNameCombo->insertItem(tmp, 0);
				FromNameCombo->setCurrentItem(0);
			} else
				FromNameCombo->setCurrentItem(FromNameCombo->listBox()->index(tmpit));

			ToNameCombo->insertItem((char*)Message->to, 0);
			SubjEdit->setText(DecodeMIMELine((char*)Message->subj));

			if (DstArea->AreaType != AREATYPE_NETMAIL)
			{
				KeepKludge("RFC-References:", Message->CtlBuff, &Kludges);
				KeepKludge("RFC-Message-ID:", Message->CtlBuff, &Kludges);
			}
			KeepKludge("REPLY:", Message->CtlBuff, &Kludges);

			UniqueID = Message->uid;

			MessageEdit->append("@text@");
			textcnt = LoadText(Message);
			break;
		}
		case MSG_OPERATION_REPLY:
		{
			OrigArea = Message->Area->Name;
			DestArea = Area->Name;

			ToNameCombo->insertItem(Config->toUTF((char*)Message->from), 0);
			if (Area->AreaType == AREATYPE_NETMAIL)
			{
				ToAddrEdit->setText(addr2str1(&Message->origaddr, Message->CtlBuff));
				ToAddrEdit->setEnabled(true);
				MessFlags |= FLAG_PVT;
				AllowFlags = FLAG_PVT | FLAG_CRASH | FLAG_FILE | FLAG_KILL | FLAG_HLD | FLAG_DIR | FLAG_FRQ | FLAG_RRQ | FLAG_ARQ | FLAG_URQ | FLAG_IMM;
			} else
				AllowFlags = FLAG_KILL;

			tmp = DecodeMIMELine((char*)Message->subj);
			if (!tmp.startsWith("Re", false))
				tmp = "Re: " + tmp;
			SubjEdit->setText(tmp);

			Message->Area->Open();
			Message->Read();
			tmp = GetKludge(Message->CtlBuff, "MSGID:");
			if (!tmp.isEmpty())
				Kludges.append("REPLY" + tmp.mid(5));

			QString rfc = QString::null;
			tmp = GetKludge(Message->CtlBuff, "RFC-Message-ID:");
			if (!tmp.isEmpty())
				rfc = tmp.mid(16);

			tmp = GetKludge(Message->CtlBuff, "RFC-References:");
			if (!tmp.isEmpty())
			{
				tmp = tmp.mid(16);
				if (rfc.isEmpty())
				{
					Kludges.append("RFC-Message-ID: " + GetRfcMessageMsgId(Config->GetAddress(0)));
					rfc = tmp;
				} else
					rfc.prepend(tmp + " ");
			}

			if (!rfc.isEmpty())
				Kludges.append("RFC-References: " + rfc);

			LoadTemplate(TPL_REPLY_TPL, &pos);
			ParseTemplate(Area, Message, false);
			textcnt = LoadText(Message);
			Message->Area->Close();
			break;
		}
		case MSG_OPERATION_REPLYNET:
		{
			OrigArea = Message->Area->Name;
			DestArea = Area->Name;

			MessFlags |= FLAG_PVT;
			AllowFlags = FLAG_PVT | FLAG_CRASH | FLAG_FILE | FLAG_KILL | FLAG_HLD | FLAG_DIR | FLAG_FRQ | FLAG_RRQ | FLAG_ARQ | FLAG_URQ | FLAG_IMM;

			ToAddrEdit->setText(addr2str1(&Message->origaddr, Message->CtlBuff));
			ToAddrEdit->setEnabled(true);
			ToNameCombo->insertItem(Config->toUTF((char*)Message->from), 0);

			tmp = DecodeMIMELine((char*)Message->subj);
			if (!tmp.startsWith("Re", false))
				tmp = "Re: " + tmp;
			SubjEdit->setText(tmp);

			LoadTemplate(TPL_REPLY_TPL, &pos);
			ParseTemplate(Area, Message, false);
			textcnt = LoadText(Message);
			break;
		}
		case MSG_OPERATION_FORWARD:
		{
			OrigArea = Message->Area->Name;
			DestArea = Area->Name;

			if (Area->AreaType == AREATYPE_ECHOMAIL)
			{
				ToNameCombo->insertItem("All", 0);
				AllowFlags = FLAG_KILL;
			} else {
				ToNameCombo->insertItem("", 0);
				ToAddrEdit->setEnabled(true);
				MessFlags |= FLAG_PVT;
				AllowFlags = FLAG_PVT | FLAG_CRASH | FLAG_FILE | FLAG_KILL | FLAG_HLD | FLAG_DIR | FLAG_FRQ | FLAG_RRQ | FLAG_ARQ | FLAG_URQ | FLAG_IMM;
			}

			tmp = DecodeMIMELine((char*)Message->subj);
			if (!tmp.startsWith("Fwd", false))
				tmp = "Fwd: " + tmp;
			SubjEdit->setText(tmp);

			LoadTemplate(TPL_FORWARD_TPL, &pos);
			ParseTemplate(Area, Message, false);
			textcnt = LoadText(Message);
			break;
		}
		case MSG_OPERATION_UNSUBSCRIBE:
		{
			OrigArea = ((TArea*)addval)->Name;
			DestArea = Area->Name;

			MessFlags |= FLAG_PVT| FLAG_KILL | FLAG_DIR;
			AllowFlags = FLAG_PVT | FLAG_CRASH | FLAG_KILL | FLAG_HLD | FLAG_DIR | FLAG_IMM;

			ToAddrEdit->setEnabled(true);
			ToAddrEdit->setText(((TArea*)addval)->Uplink);
			SubjEdit->setText(((TArea*)addval)->UplinkPwd);

			ToNameCombo->insertItem(((TArea*)addval)->UplinkRbt, 0);

			LoadTemplate(TPL_UNSUBSCRIBE_TPL, &pos);
			ParseTemplate(Area, NULL, false);
			tmp = MessageEdit->text();
			tmp.replace("@origarea@", OrigArea);
			MessageEdit->setText(tmp);
			break;
		}
		case MSG_OPERATION_CONFIRM:
			OrigArea = Message->Area->Name;
			DestArea = Area->Name;

			MessFlags |= FLAG_PVT | FLAG_KILL | FLAG_CPT;
			AllowFlags = FLAG_PVT | FLAG_CRASH | FLAG_KILL | FLAG_DIR | FLAG_CPT | FLAG_IMM;

			ToAddrEdit->setText(addr2str1(&Message->origaddr, Message->CtlBuff));
			//ToAddrEdit->setEnabled(true);
			ToNameCombo->insertItem(Config->toUTF((char*)Message->from), 0);

			SubjEdit->setText("CONFIRMATION MESSAGE");

			LoadTemplate(TPL_CONFIRM_TPL, &pos);
			ParseTemplate(Area, Message, false);

			BtnAddrBook->setEnabled(false);
			BtnNodeList->setEnabled(false);

			MessageTextChanged();

			break;
		default:
			return false;
	}

	setCaption(QString(QObject::tr("Message Editor") + " [" + QObject::tr("Area") + ": %1]").arg(DestArea));

	if (pos.isNull())
		pos.setY(MessageEdit->paragraphs() - 1);
	else
		pos.setY(pos.y() + textcnt);
	MessageEdit->setCursorPosition(pos.y(), pos.x());

	ClearModified();

	if ((MsgOperation != MSG_OPERATION_UNSUBSCRIBE) && (MsgOperation != MSG_OPERATION_CONFIRM) && ((MsgOperation != MSG_OPERATION_EDIT) || strcompare(Area->Name, Config->GetDraftArea())))
	{
		tmp = Config->ConfigPath + TPL_ORIGIN_LST + "." + DestArea;
		if (!testexists(tmp))
			tmp = Config->ConfigPath + TPL_ORIGIN_LST;
		if (testexists(tmp))
		{
			QFile origins(tmp);
			if (FileOpenFunc(&origins, IO_ReadOnly))
			{
				OriginCombo->insertItem("");

				QTextStream stream(&origins);
				while (!stream.atEnd())
				{
					tmp = stream.readLine().stripWhiteSpace();
					if (!tmp.isEmpty())
						OriginCombo->insertItem(tmp);
				}
				FileCloseFunc(&origins);

				if (OriginCombo->listBox()->count() > 1)
					OriginCombo->setCurrentItem(1);
			}
		}
	} else {
		ViewOriginAction->setEnabled(false);
		ViewOriginAction->setOn(false);
		ActionViewOrigin();
	}

	if ((MsgOperation != MSG_OPERATION_UNSUBSCRIBE) && (MsgOperation != MSG_OPERATION_CONFIRM))
	{
		QStringList UserLst;
		Config->AddressBook->MakeUserLst(&UserLst);
		if (UserLst.count() > 0)
			ToNameCombo->insertStringList(UserLst);
	}
	ToNameCombo->setCurrentItem(0);

	SetupDone = true;
	return true;
}

void TEditWindow::LoadTemplate(QString Name, QPoint *pos)
{
	int p;
	QPoint ret(0, 0);
	QString tmp, Line;

	tmp = Config->ConfigPath + Name + "." + DestArea;
	if (!testexists(tmp))
	{
		tmp = Config->ConfigPath + Name;
		if (!testexists(tmp))
			return;
	}

	QFile tpl(tmp);
	if (FileOpenFunc(&tpl, IO_ReadOnly))
	{
		QTextStream stream(&tpl);
		while (!stream.atEnd())
		{
			Line = stream.readLine().append("\n");
			p = Line.find("@cursor@");
			if (p > -1)
			{
				pos->setX(p);
				pos->setY(MessageEdit->paragraphs() - 1);
				Line.replace("@cursor@", QString::null);
			}
			MessageEdit->append(Line);
		}
		FileCloseFunc(&tpl);
	}
}

void TEditWindow::LoadKludges(QStringList *Kludges)
{
	QString tmp, Line;
	QStringList ret;

	tmp = Config->ConfigPath + TPL_KLUDGES_LST + "." + DestArea;
	if (!testexists(tmp))
	{
		tmp = Config->ConfigPath + TPL_KLUDGES_LST;
		if (!testexists(tmp))
			return;
	}

	QFile tpl(tmp);
	if (FileOpenFunc(&tpl, IO_ReadOnly))
	{
		QTextStream stream(&tpl);
		while (!stream.atEnd())
		{
			Line = stream.readLine().stripWhiteSpace();
			if (tokencount(Line, ": ") > 1)
			{
				ret = Kludges->grep(QRegExp("^" + Line, false));
				if (ret.count() == 0)
					Kludges->append(Line);
			}
		}
		FileCloseFunc(&tpl);
	}
}

QString TEditWindow::ComposeOrigin(void *_addr)
{
	ps_addr addr = (ps_addr)_addr;
	QString origline, addrstr = QString("%1:%2/%3").arg(addr->zone).arg(addr->net).arg(addr->node);

	if (addr->point > 0)
		addrstr.append(QString(".%1").arg(addr->point));
	addrstr.prepend(" (").append(")");

	if (OriginCombo->isShown() && OriginCombo->listBox()->count() > 2)
		origline = OriginCombo->text(RandomNumber(1, OriginCombo->listBox()->count() - 1)).stripWhiteSpace();
	else
		origline = OriginCombo->currentText().stripWhiteSpace();

	if (origline.isEmpty())
		return QString::null;

	while (ParseCommand(this, &origline)) {};
	if (origline.length() > (68 - addrstr.length()))
		origline.setLength(68 - addrstr.length());

	return QString(" * Origin: %1%2\n").arg(origline).arg(addrstr);
}

void TEditWindow::ParseTemplate(TArea *Area, TMessage *Message, bool isFinal)
{
	QString Line = MessageEdit->text();

	Line.replace("@areadesc@", Area->Desc);
	Line.replace("@area@", DestArea);
	if (!isFinal)
		ExpandMacros(Area, Message, &Line, OrigArea);
	else {
		Line.replace("@to@", ToNameCombo->currentText());
		Line.replace("@toname@", gettoken(ToNameCombo->currentText(), 1));
		if (Area->AreaType == AREATYPE_NETMAIL)
			Line.replace("@toaddr@", ToAddrEdit->text());
		Line.replace("@from@", FromNameCombo->currentText());
		Line.replace("@fromname@", gettoken(FromNameCombo->currentText(), 1));
		Line.replace("@fromaddr@", FromAddrCombo->currentText());
		Line.replace("@subject@", SubjEdit->text());
		QDateTime dt = QDateTime::currentDateTime();
		Line.replace("@date@", dt.toString("dd.MM.yyyy"));
		Line.replace("@time@", dt.toString("hh:mm:ss"));
		Line.replace("@attr@", flags2str(MessFlags));
		ExpandMacros2(this, &Line);
	}
	MessageEdit->setText(Line);
}

int TEditWindow::LoadText(TMessage *Message)
{
	int q;
	uint32_t i, j;
	QString Initials, Line, Text = Config->toUTF((char*)Message->TxtBuff);

	if (Config->GetBool(CONFIG_SECTION_EDIT, CONFIG_SHOWKLUDGES, false) && (MsgOperation != MSG_OPERATION_EDIT) && (MsgOperation != MSG_OPERATION_UNSUBSCRIBE) && (MsgOperation != MSG_OPERATION_CONFIRM))
		Text.prepend(Config->toUTF((char*)Message->CtlBuff));

	ReplaceUnicodeChars(&Text);
	QStringList InputList = QStringList::split("\n", Text, true);
	Text = MessageEdit->text();

	while ((InputList.count() > 0) && InputList[0].isEmpty())
		InputList.remove(InputList.at(0));

	if (!(Config->GetBool(CONFIG_SECTION_EDIT, CONFIG_SHOWKLUDGES, false) && (MsgOperation != MSG_OPERATION_EDIT) && (MsgOperation != MSG_OPERATION_UNSUBSCRIBE) && (MsgOperation != MSG_OPERATION_CONFIRM)))
	{
		while ((InputList.count() > 0) && (isLowerKludge(&InputList[InputList.count() - 1]) || InputList[InputList.count() - 1].isEmpty()))
			InputList.remove(InputList.at(InputList.count() - 1));
		if ((InputList.count() > 0) && InputList[InputList.count() - 1].startsWith(" * Origin: "))
			InputList.remove(InputList.at(InputList.count() - 1));
		while ((InputList.count() > 0) && (InputList[InputList.count() - 1].startsWith("--- ") || InputList[InputList.count() - 1].startsWith("... ")))
			InputList.remove(InputList.at(InputList.count() - 1));
		while ((InputList.count() > 0) && InputList[InputList.count() - 1].isEmpty())
			InputList.remove(InputList.at(InputList.count() - 1));
	}

	if ((MsgOperation == MSG_OPERATION_REPLY) || (MsgOperation == MSG_OPERATION_REPLYNET))
	{
		Initials = QString::null;
		for (i = 1; i <= QMIN((uint32_t)tokencount((char*)Message->from), 3); i++)
		{
			Line = gettoken((char*)Message->from, i);
			if (!Line.isEmpty())
				if (Line.at(0).isLetter())
					Initials.append(Line.at(0));
		}

		for (i = 0; i < InputList.count(); i++)
		{
			QString tmpq = " " + Initials + "> ";

			Line = InputList[i].stripWhiteSpace();
			if (Line.isEmpty())
				continue;

			q = Line.find('>');
			if (q > -1)
			{
				if (q < 5)
				{
					for (j = q + 1; j < Line.length(); j++)
						if (Line.at(j) != '>')
							break;
					tmpq = Line.left(j) + "> ";
					Line.remove(0, tmpq.length() - 1);
				}
			}

			Line.prepend(tmpq);
			while (Line.length() > QUOTE_BREAK_POS)
			{
				int FixWrap = QUOTE_BREAK_POS;
				for (j = QUOTE_BREAK_POS - 1; j > QUOTE_BREAK_POS - 15; j--)
					if (Line.at(j).isSpace())
					{
						FixWrap = j;
						break;
					}
				InputList.insert(InputList.at(i) , Line.left(FixWrap));
				Line.remove(0, FixWrap);
				Line = Line.stripWhiteSpace().prepend(tmpq);
				i++;
			}
			InputList[i] = Line;
		}
		Text.replace("@quotetext@", InputList.join("\n"));
	} else
		Text.replace("@text@", InputList.join("\n"));

	MessageEdit->setText(Text);

	return ((InputList.count() > 1) ? (InputList.count() - 1) : 0);
}

void TEditWindow::showEvent(QShowEvent *)
{
	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	switch (MsgOperation)
	{
		case MSG_OPERATION_EDIT:
		case MSG_OPERATION_REPLY:
		case MSG_OPERATION_REPLYNET:
		case MSG_OPERATION_UNSUBSCRIBE:
			MessageEdit->setFocus();
			break;
		default:
			break;
	}

	PasteTest();
	connect(qApp->clipboard(), SIGNAL(dataChanged()), this, SLOT(PasteTest()));

	if (SpellCheckInit())
	{
		SpellCheckSetup();
		//SpellCheckAction->setEnabled(true);
	}
}

void TEditWindow::closeEvent(QCloseEvent *e)
{
	if (MessageEdit->isModified())
		if (!extquestionmessage(this, QObject::tr("Do You really want Quit without Save Message?"), CONFIG_CONFIRM_QUITSAVE))
			return;

	disconnect(qApp->clipboard(), SIGNAL(dataChanged()), this, SLOT(PasteTest()));

	Config->SetToolBar(this, EditToolBar);
	Config->SetBool(CONFIG_SECTION_EDIT, CONFIG_VIEWSTATUSBAR, statusBar()->isShown());
	Config->SetBool(CONFIG_SECTION_EDIT, CONFIG_VIEWORIGIN, OriginCombo->isShown());

	Config->SetPos(this);
	Config->SetSize(this);
	if (SetupDone)
		qApp->postEvent(parent(), new TCloseEvent(this, Result));
	e->accept();
}

void TEditWindow::ClearModified()
{
	MessageEdit->setModified(false);

	MessageEdit->setUndoRedoEnabled(false);
	MessageEdit->setUndoRedoEnabled(true);

	SaveMessageAction->setEnabled((MsgOperation == MSG_OPERATION_FORWARD) || (MsgOperation == MSG_OPERATION_UNSUBSCRIBE));
	SaveDraftAction->setEnabled(MsgOperation == MSG_OPERATION_FORWARD);

	RedoAction->setEnabled(false);
	UndoAction->setEnabled(false);
	CopyAction->setEnabled(false);
	CutAction->setEnabled(false);
	DelAction->setEnabled(false);
}

void TEditWindow::ToAddrChanged(const QString &s)
{
	int pos = 0;
	ToAddrEdit->setPaletteForegroundColor(ToAddrEdit->validator()->validate((QString&)s, pos) == QValidator::Acceptable ? Qt::black : Qt::red);
}

QString TEditWindow::TextReplace(QString Text)
{
	QStringList Replace;

	Config->GetStrList(CONFIG_SECTION_EDIT, CONFIG_REPLACELIST, &Replace);
	for (uint32_t i = 0; i < Replace.count(); i++)
		if (tokencount(Replace[i], "\t") == 2)
			Text.replace(gettoken(Replace[i], 1, "\t"), gettoken(Replace[i], 2, "\t"));

	return Text;
}

void TEditWindow::Save(bool isDraft)
{
	int tmpIdx;
	TArea *Area, *Draft = NULL;
	TMessage *Message = NULL;
	QString tmp;
	QByteArray _tmp;

	if (isDraft && !Config->GetDraftArea().isEmpty())
	{
		tmpIdx = Areas->Find(Config->GetDraftArea());
		if (tmpIdx < 0)
		{
			errormessage(this, MESS_NOTAREA);
			return;
		}
		Draft = Areas->at(tmpIdx);
	}

	if (!isDraft)
	{
		tmpIdx = Areas->Find(DestArea);
		if (tmpIdx < 0)
		{
			errormessage(this, MESS_NOTAREA);
			return;
		}
		Area = Areas->at(tmpIdx);
	} else
		Area = Draft;

	if ((MsgOperation == MSG_OPERATION_EDIT) && !strcompare(OrigArea, Config->GetDraftArea()))
	{
		tmpIdx = Area->UID2Index(UniqueID);
		if (tmpIdx < 0)
		{
			errormessage(this, MESS_NOTMESSAGE);
			return;
		}
		Message = Area->at(tmpIdx);
	} else
		Message = Area->Append();

	ps_addr faddr = getAddr(Config->FidoConfig, (char*)FromAddrCombo->currentText().ascii());
	hs_addr taddr;

	if (faddr == NULL)
	{
		Area->Delete(Message->AreaIndex);
		errormessage(this, QObject::tr("Sender address is invalid. Please, select valid address."));
		return;
	}

	if (Areas->at(Areas->Find(DestArea))->AreaType == AREATYPE_ECHOMAIL)
		memcpy(&taddr, faddr, sizeof(hs_addr));
	else
	{
		if (!str2addr(ToAddrEdit->text(), &taddr))
		{
			errormessage(this, QObject::tr("Receiver address is invalid. Please, select valid address."));
			return;
		}
	}

	tmp = SubjEdit->text();
	if (tmp.stripWhiteSpace().isEmpty())
	{
		errormessage(this, QObject::tr("Message Subject is Empty."));
		return;
	}

	Area->Open();
	if (!isDraft)
		ParseTemplate(Area, Message, true);

	tmp = QString("%1:%2/%3").arg(faddr->zone).arg(faddr->net).arg(faddr->node);
	if (faddr->point > 0)
		tmp.append(QString(".%1").arg(faddr->point));
	if (faddr->domain != NULL)
		tmp.append(QString("@%1").arg(faddr->domain));
	tmp.prepend("MSGID: ").append(" " + GetMessageMsgId());

	Kludges.prepend(tmp);

	tmp = Config->CharsetForMessages();
	if (!tmp.isEmpty())
		Kludges.append(QString("CHRS: %1").arg(tmp));

	addr2addr(&Message->origaddr, faddr);
	addr2addr(&Message->destaddr, &taddr);
	_tmp = Config->fromUTF(FromNameCombo->currentText());
	qstrcpy((char*)Message->from, _tmp.data());
	_tmp = Config->fromUTF(ToNameCombo->currentText());
	qstrcpy((char*)Message->to, _tmp.data());
	tmp = isDraft ? SubjEdit->text().stripWhiteSpace() : TextReplace(SubjEdit->text().stripWhiteSpace());
	_tmp = Config->fromUTF(tmp);
	qstrcpy((char*)Message->subj, _tmp.data());

	Message->attr = MessFlags;

	LoadKludges(&Kludges);
	Message->CtlBuff = (uint8_t*)qstrdup(Config->fromUTF(Kludges.join("\n")));

	tmp = isDraft ? MessageEdit->text() : TextReplace(MessageEdit->text());
	if (isDraft)
		tmp.prepend(QString("AREA: %1\n").arg(DestArea));
	else if ((MsgOperation != MSG_OPERATION_EDIT) && (MsgOperation != MSG_OPERATION_UNSUBSCRIBE))
	{
		if (!tmp.endsWith("\r") && !tmp.endsWith("\n"))
			tmp.append("\n");
		if ((Area->AreaType == AREATYPE_NETMAIL) && (tmp.find("---") < 0))
			tmp.append("---\n");
		tmp.append(isDraft ? ComposeOrigin(faddr) : TextReplace(ComposeOrigin(faddr)));
	}

	Message->TxtBuff = (uint8_t*)qstrdup(Config->fromUTF(tmp));

	bool w_res = Area->Write(Message->AreaIndex);
	Area->Close();
	if (!w_res)
	{
		errormessage(this, QObject::tr("Can't Write Message!"));
		return;
	} else if ((MsgOperation == MSG_OPERATION_EDIT) && !isDraft && strcompare(OrigArea, Config->GetDraftArea()))
	{
		Result = Config->GetDraftArea();
		tmpIdx = Draft->UID2Index(UniqueID);
		if (tmpIdx < 0)
		{
			errormessage(this, QObject::tr("Can't Delete Draft Message!"));
			return;
		} else
			Draft->Delete(tmpIdx);
	}

	ClearModified();
	if ((!Config->GetEchoTossLog().isEmpty()) && !isDraft)
	{
		bool found = false;
		QFile echotoss(Config->GetEchoTossLog());
		if (FileOpenFunc(&echotoss, IO_ReadWrite | IO_Append))
		{
			echotoss.reset();
			QTextStream stream(&echotoss);
			while (!stream.atEnd())
			{
				tmp = stream.readLine();
				if (strcompare(tmp, Message->Area->Name) && !found)
					found = true;
			}
			if (!found)
				stream << Area->Name << "\n";
			FileCloseFunc(&echotoss);
		}
	}
	Result.append((Result.isEmpty() ? QString::null : "\n") + DestArea);
	WidgetCloseFunc(this);
}

void TEditWindow::ActionClose()
{
	WidgetCloseFunc(this);
}

void TEditWindow::ActionSaveMessage()
{
	Save(false);
}

void TEditWindow::ActionSaveDraft()
{
	Save(true);
}

void TEditWindow::ActionViewToolBar()
{
	EditToolBar->setShown(ViewToolBarAction->isOn());
}

void TEditWindow::ActionViewStatusBar()
{
	statusBar()->setShown(ViewStatusBarAction->isOn());
}

void TEditWindow::ToolBarVisibilityChanged(bool)
{
	if (isShown())
		ViewToolBarAction->setOn(EditToolBar->isShown());
}

void TEditWindow::ActionViewOrigin()
{
	OriginCombo->setShown(ViewOriginAction->isOn());
	OriginLabel->setShown(ViewOriginAction->isOn());
}

void TEditWindow::ActionHelp()
{
	((TMainWindow*)qApp->mainWidget())->HelpIndex();
}

void TEditWindow::ActionWhatsThis()
{
	QWhatsThis::enterWhatsThisMode();
}

void TEditWindow::MessageTextChanged()
{
	if (isShown())
	{
		SaveMessageAction->setEnabled(true);
		SaveDraftAction->setEnabled((MsgOperation != MSG_OPERATION_UNSUBSCRIBE) && (MsgOperation != MSG_OPERATION_CONFIRM) && !Config->GetDraftArea().isEmpty());
	}
}

void TEditWindow::MessageEditRedoAvailable(bool Val)
{
	RedoAction->setEnabled(Val);
}

void TEditWindow::MessageEditUndoAvailable(bool Val)
{
	UndoAction->setEnabled(Val);
}

void TEditWindow::MessageEditCopyAvailable(bool Val)
{
	CopyAction->setEnabled(Val);
	CutAction->setEnabled(Val);
	DelAction->setEnabled(Val);
}

void TEditWindow::PasteTest()
{
	QString clip = qApp->clipboard()->text();

	if (!clip.isEmpty())
	{
		QString Lines;

		disconnect(qApp->clipboard(), SIGNAL(dataChanged()), this, SLOT(PasteTest()));

		((TMainWindow*)qApp->mainWidget())->LoadTemplate(TPL_CLIPBOARD_TPL, DestArea, &Lines);

		ExpandMacros2(this, &Lines);
		Lines.replace("@text@", clip);

		qApp->clipboard()->setText(Lines, QClipboard::Clipboard);

		connect(qApp->clipboard(), SIGNAL(dataChanged()), this, SLOT(PasteTest()));

		PasteAction->setEnabled(true);
	} else
		PasteAction->setEnabled(false);
}

void TEditWindow::ActionSpellCheck()
{
	qDebug("TEditWindow::ActionSpellCheck() Not implemented yet...");
}

void TEditWindow::ActionAttributes()
{
	TMessAttr *AttrWnd = new TMessAttr(this);
	AttrWnd->SetUp(AllowFlags, MessFlags);
	if (AttrWnd->exec() == QDialog::Accepted)
		MessFlags = AttrWnd->GetState() | FLAG_LOC;
	delete AttrWnd;
}

void TEditWindow::ActionInsertFile()
{
	QFileDialog* fd = new QFileDialog(this);
	fd->setDir(Config->GetStr(QString::null, CONFIG_LASTOPENSAVE, QString::null));
	fd->setFilter(QObject::tr("Text Files (*.txt)"));
	fd->addFilter(QObject::tr("All Files (*)"));
	fd->setSelectedFilter(0);
	fd->setCaption(QObject::tr("Select file..."));
	fd->setIcon(*Config->FindPixmap("icon_load"));
	fd->setMode(QFileDialog::ExistingFile);
	fd->setInfoPreviewEnabled(true);
	if (fd->exec() == QDialog::Accepted)
	{
		Config->SetStr(QString::null, CONFIG_LASTOPENSAVE, fd->dir()->absPath());

		int mpara, mindex;
		QFile txtfile(fd->selectedFile());
		if (FileOpenFunc(&txtfile, IO_ReadOnly))
		{
			if (txtfile.size() > 0)
			{
				QTextStream stream(&txtfile);
				MessageEdit->getCursorPosition(&mpara, &mindex);
				MessageEdit->insertParagraph(TextStreamReadFunc(&stream), mpara + ((mindex > 0) ? 1 : 0));
			}
			FileCloseFunc(&txtfile);
		}
	}
	delete fd;
}

void TEditWindow::ActionAttachFile()
{
/*
	TAttachForm *Attach = new TAttachForm(this);
	if (Attach->exec() == QDialog::Accepted)
	{
		//TODO:
		//
		//
	}
	delete Attach;
*/
	qDebug("TEditWindow::ActionAttachFile() Not implemented yet...");
}

void TEditWindow::ActionAddrBook()
{
        TAddressBook *AddressBook = new TAddressBook(this);
        AddressBook->SetUp(true);
        if (AddressBook->exec() == QDialog::Accepted)
        {
		ToNameCombo->insertItem(AddressBook->Selected->Name, 0);
		ToNameCombo->setCurrentItem(0);
		if (ToAddrEdit->isEnabled())
			ToAddrEdit->setText(AddressBook->Selected->Address);
	}
        delete AddressBook;
}

void TEditWindow::ActionNodeListBrowser()
{
	TNodelistBrowser *NodelistBrowser = new TNodelistBrowser(this);
	if (NodelistBrowser->TestLists())
	{
		NodelistBrowser->SetSelectMode(true);
		if (NodelistBrowser->exec() == QDialog::Accepted)
		{
			ToNameCombo->insertItem(NodelistBrowser->Selected->SysOp, 0);
			ToNameCombo->setCurrentItem(0);
			if (ToAddrEdit->isEnabled())
				ToAddrEdit->setText(NodelistBrowser->Selected->Addr);
		}
	} else
		informationmessage(this, QObject::tr("No Nodelists in config."));
	delete NodelistBrowser;
}

void TEditWindow::ActionUndo()
{
	MessageEdit->undo();
}

void TEditWindow::ActionRedo()
{
	MessageEdit->redo();
}

void TEditWindow::ActionCopy()
{
	MessageEdit->copy();
}

void TEditWindow::ActionCut()
{
	MessageEdit->cut();
}

void TEditWindow::ActionDel()
{
	MessageEdit->del();
}

void TEditWindow::ActionPaste()
{
	MessageEdit->paste();
}

bool TEditWindow::eventFilter(QObject *o, QEvent *e)
{
        if (o == MessageEdit)
        {
                if (e->type() == QEvent::Paint)
		{
			//TODO: Draw line at column 80
			//QPaintEvent *event = (QPaintEvent*)e;

			//QWidget::paintEvent(event);
			//QPainter paint(MessageEdit);
			//paint.setPen(Qt::red);
			//paint.drawLine(0,0,20,20);
			//return true;
		}
	}
	return false;
}

void TEditWindow::EditCursorPositionChanged(int x, int y)
{
	PosStatusBar->setText(QString("%1 : %2").arg(x).arg(y));
}
