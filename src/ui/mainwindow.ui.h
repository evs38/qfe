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

const int DefSplitterHWidth[] = {247, 536};
const int DefSplitterVWidth[] = {207, 304};

extern TAreas *Areas;
extern TConfig *Config;
extern QSplashScreen *Splash;
extern bool GlobalQfeError;

extern QString StartArea;

const QColor DefGoldedColors[] = {
	QColor(255, 255, 255),
	QColor(0, 0, 255),
	QColor(0, 255, 255),
	QColor(255, 255, 0),
	QColor(255, 0, 0),
	QColor(0, 170, 0),
	QColor(0, 255, 0),
	QColor(128, 128, 128),
	QColor(77, 142, 148),
	QColor(85, 85, 255),
	QColor(85, 255, 255),
	QColor(255, 170, 0),
	QColor(255, 85, 0),
	QColor(85, 170, 0),
	QColor(85, 255, 0)
};

struct smile_t
{
	const QString occ;
	const QString img;
};

const smile_t smilies_table[] =
{
	{ ":D", "smile_icon1" },
	{ ":-D", "smile_icon1" },
	{ ":I", "smile_icon2" },
	{ ":-I", "smile_icon2" },
	{ ";(", "smile_icon3" },
	{ ";-(", "smile_icon3" },
	{ ":o", "smile_icon4" },
	{ ":-o", "smile_icon4" },
	{ ":)", "smile_icon5" },
	{ ":-)", "smile_icon5" },
	{ "8)", "smile_icon6" },
	{ "8-)", "smile_icon6" },
	{ ":p", "smile_icon7" },
	{ ":(", "smile_icon8" },
	{ ":-(", "smile_icon8" },
	{ ";)", "smile_icon9" },
	{ ";-)", "smile_icon9" },
	{ QString::null, QString::null }
};

void TMainWindow::init()
{
	int i;

	FirstTimeInit = false;

	setIcon(QIconSet(*Config->FindPixmap("qfe"), QIconSet::Large).pixmap(QIconSet::Small, QIconSet::Normal));

	MainActions = new TActions();

	MainActions->AddAction(FileMenu, MainMenuBar->text(1), OpenAction, "icon_load", QObject::tr("Open Message File for View"), QObject::tr("Supported Formats is MSG, PKT"), "Ctrl+O", true);
	MainActions->AddAction(FileMenu, MainMenuBar->text(1), PrintAction, "icon_print", QObject::tr("Print Current Message"), QObject::tr("You must print entrie message, or only selected region"), "Ctrl+P");
	MainActions->AddAction(FileMenu, MainMenuBar->text(1), SaveAction, "icon_save", QObject::tr("Save Current Message to File"), QObject::tr("Supported Formats is TXT, HTML"), "Ctrl+S");
	MainActions->AddAction(FileMenu, MainMenuBar->text(1), DataBaseAction, "icon_database", QObject::tr("Save Current Message to DataBase"), QString::null, QString::null, true);
	MainActions->AddAction(FileMenu, MainMenuBar->text(1), ArchiveAction, "icon_archive", QObject::tr("Save Current Message to Archive Area"), QString::null, QString::null, true);
	MainActions->AddAction(FileMenu, MainMenuBar->text(1), OptionsAction, "icon_options", QObject::tr("Edit Program Options"), QObject::tr("In \"Options\" dialog You must define fonts, colors and other settings for best view"));
	MainActions->AddAction(FileMenu, MainMenuBar->text(1), ExitAction, "icon_exit", QObject::tr("Quit Program"), QString::null, "Ctrl+Q");

	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), NewAction, "icon_newmail", QObject::tr("Create New Message in Current Area"), QString::null, "Ctrl+N");
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), EditAction, "icon_edit", QObject::tr("Edit Current Message"));
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), ReplyAction, "icon_reply", QObject::tr("Create Reply to Current Message"), QString::null, "Ctrl+R");
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), ReplyNetmailAction, "icon_replynet", QObject::tr("Create Reply to Current Message in NetMail Area"));
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), ForwardAction, "icon_forward", QObject::tr("Forward Current Message to Another Area"));
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), DeleteAction, "icon_delmail", QObject::tr("Delete Current Message from MessageBase"), QString::null, "Del");
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), PreviousMessAction, "icon_left", QObject::tr("Go to Previous Message in Current Area"), QString::null, "Up");
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), NextMessAction, "icon_right", QObject::tr("Go to Next Message in Current Area"), QString::null, "Down");
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), NextUnreadAction, "icon_unread", QObject::tr("Go to Next Unread Message in Current Area"), QString::null, "Space");
#if defined(Q_OS_MACX)
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), MarkMessAction, "icon_mark", QObject::tr("Mark/Unmark Current Message"));
#else
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), MarkMessAction, "icon_mark", QObject::tr("Mark/Unmark Current Message"), QString::null, "Ins");
#endif
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), AddToBookAction, "icon_addtobook", QObject::tr("Add sender to AddressBook"));
	MainActions->AddAction(MessageMenu, MainMenuBar->text(2), MessagePropAction, "icon_prop", QObject::tr("View Message Properties"), QString::null, QString::null, true);

	MainActions->AddAction(AreaMenu, MainMenuBar->text(3), RescanAction, "icon_rescan", QObject::tr("Rescan Area"));
	MainActions->AddAction(AreaMenu, MainMenuBar->text(3), PreviousAreaAction, "icon_up", QObject::tr("Go to Previous Area"), QString::null, "Left");
	MainActions->AddAction(AreaMenu, MainMenuBar->text(3), NextAreaAction, "icon_down", QObject::tr("Go to Next Area"), QString::null, "Right");
	MainActions->AddAction(AreaMenu, MainMenuBar->text(3), MarkAllAsReadAction, "icon_flag", QObject::tr("Mark All Messages in Current Area As Read"));
	MainActions->AddAction(AreaMenu, MainMenuBar->text(3), UnsubscribeAction, "icon_unsubscribe", QObject::tr("Unsubscribe from Current Area"));
	MainActions->AddAction(AreaMenu, MainMenuBar->text(3), AreaPropAction, "icon_prop", QObject::tr("View Area Properties"));

	MainActions->AddAction(ViewMenu, MainMenuBar->text(4), ViewToolBarAction, QString::null, QObject::tr("Show/Hide ToolBar"));
	MainActions->AddAction(ViewMenu, MainMenuBar->text(4), ViewStatusBarAction, QString::null, QObject::tr("Show/Hide StatusBar"));
	MainActions->AddAction(ViewMenu, MainMenuBar->text(4), ViewKludgesAction, QString::null, QObject::tr("Show/Hide Kludge lines from Message"));
	MainActions->AddAction(ViewMenu, MainMenuBar->text(4), ViewHighlightAction, QString::null, QObject::tr("Show/Hide Color Message Highlighting"));
	MainActions->AddAction(ViewMenu, MainMenuBar->text(4), WordWrapAction, QString::null, QObject::tr("Wrap Text On/Off in View Window"));

	MainActions->AddAction(ToolsMenu, MainMenuBar->text(5), RescanAllAction, "icon_rescan", QObject::tr("Rescan All Areas"));
	MainActions->AddAction(ToolsMenu, MainMenuBar->text(5), FindAction, "icon_find", QObject::tr("Search for Messages"), QObject::tr("Searching allowed in One Area, or in All Areas"), "Ctrl+F");
	MainActions->AddAction(ToolsMenu, MainMenuBar->text(5), DecodeAction, "icon_decode", QObject::tr("Decode UUE from Marked Messages"), QObject::tr("Supported Formats is UUE, Base64"));
	MainActions->AddAction(ToolsMenu, MainMenuBar->text(5), NodelistBrowserAction, "icon_browser", QObject::tr("Run Extended Nodelist Browser"), QString::null, QString::null, true);
	MainActions->AddAction(ToolsMenu, MainMenuBar->text(5), AddressBookAction, "icon_addressbook", QObject::tr("View AddessBook"), QString::null, "Ctrl+A");
	MainActions->AddAction(ToolsMenu, MainMenuBar->text(5), ExtensionsAction, "icon_plugins", QObject::tr("Execute Program Extensions"), QString::null, QString::null, true);

	MainActions->AddAction(HelpMenu, MainMenuBar->text(6), HelpAction, "icon_help", QObject::tr("View Program Help"), QString::null, "F1", true);
	MainActions->AddAction(HelpMenu, MainMenuBar->text(6), AboutAction, "icon_about", QObject::tr("Some Information About this Program"));
	MainActions->AddAction(HelpMenu, MainMenuBar->text(6), AboutQtAction, "icon_aboutqt", QObject::tr("Some Information About Qt-Library, used in this Program"));
	MainActions->AddAction(HelpMenu, MainMenuBar->text(6), BugReportAction, "icon_bug", QObject::tr("Report a Bug in Program"));
	MainActions->AddAction(HelpMenu, MainMenuBar->text(6), WhatsThisAction, "icon_whatsthis", QObject::tr("Enter in \"What is This?\"-Mode"), QString::null, "Shift+F1");

	AreaListPopup = new QPopupMenu(AreaList);
	RescanAction->addTo(AreaListPopup);
	AreaListPopup->insertSeparator();
	MarkAllAsReadAction->addTo(AreaListPopup);
	AreaListPopup->insertSeparator();
	UnsubscribeAction->addTo(AreaListPopup);
	AreaListPopup->insertSeparator();
	AreaPropAction->addTo(AreaListPopup);

	MessListPopup = new QPopupMenu(MessageList);
	EditAction->addTo(MessListPopup);
	MessListPopup->insertSeparator();
	ReplyAction->addTo(MessListPopup);
	ReplyNetmailAction->addTo(MessListPopup);
	ForwardAction->addTo(MessListPopup);
	DeleteAction->addTo(MessListPopup);
	MessListPopup->insertSeparator();
	MarkMessAction->addTo(MessListPopup);
	MessListPopup->insertSeparator();
	AddToBookAction->addTo(MessListPopup);
	MessListPopup->insertSeparator();
	MessagePropAction->addTo(MessListPopup);

	WhatsThisAction->addTo(MainToolBar);
	MainToolBar->setCloseMode(QDockWindow::Undocked);

	Config->GetToolBar(this, MainToolBar);
	ViewToolBarAction->setOn(MainToolBar->isShown());

	ViewStatusBarAction->setOn(Config->GetBool(QString::null, CONFIG_VIEWSTATUSBAR, true));
	ActionViewStatusBar();

	ViewKludgesAction->setOn(Config->GetBool(QString::null, CONFIG_VIEWKLUDGES, false));
	ViewHighlightAction->setOn(Config->GetBool(QString::null, CONFIG_VIEWHIGHLIGHT, true));
	WordWrapAction->setOn(Config->GetBool(QString::null, CONFIG_VIEWWORDWRAP, true));
	ActionWordWrap();

	Config->GetSize(this, Config->ScreenRect.width(), Config->ScreenRect.height());
	Config->GetPos(this, 0, 0);

	Config->GetSplitter(QString::null, HorSplitter, &DefSplitterHWidth[0]);
	Config->GetSplitter(QString::null, VerSplitter, &DefSplitterVWidth[0]);

	setWindowState(Config->GetInt(QString::null, CONFIG_WINDOWSTATE, Qt::WindowMaximized) & ~Qt::WindowActive);

	AreaList->setColumnText(0, QIconSet(*Config->FindPixmap("icon_index")), "N");
	AreaList->setColumnText(1, QIconSet(*Config->FindPixmap("icon_area")), QObject::tr("Area"));
	AreaList->setColumnText(2, QIconSet(*Config->FindPixmap("icon_desc")), QObject::tr("Description"));
	Config->GetList(QString::null, AreaList);

	MessageList->setColumnText(0, QIconSet(*Config->FindPixmap("icon_index")), "N");
	MessageList->setColumnText(1, QIconSet(*Config->FindPixmap("icon_mark")), "");
	MessageList->setColumnText(2, QIconSet(*Config->FindPixmap("icon_name")), QObject::tr("From"));
	MessageList->setColumnText(3, QIconSet(*Config->FindPixmap("icon_name")), QObject::tr("To"));
	MessageList->setColumnText(4, QIconSet(*Config->FindPixmap("icon_subject")), QObject::tr("Subject"));
	MessageList->setColumnText(5, QIconSet(*Config->FindPixmap("icon_size")), QObject::tr("Size"));
	MessageList->setColumnText(6, QIconSet(*Config->FindPixmap("icon_date")), QObject::tr("Date"));
	Config->GetList(QString::null, MessageList);

	KludgeItem = new QStyleSheetItem(TextEdit->styleSheet(), "ktag");
	OriginItem = new QStyleSheetItem(TextEdit->styleSheet(), "otag");
	TaglineItem = new QStyleSheetItem(TextEdit->styleSheet(), "ltag");
	TearlineItem = new QStyleSheetItem(TextEdit->styleSheet(), "rtag");
	QuoteItem = new QStyleSheetItem(TextEdit->styleSheet(), "qtag");
	UrlItem = new QStyleSheetItem(TextEdit->styleSheet(), "utag");

	for (i = 0; i < 15; i++)
		GoldedColorTextItem[i] = new QStyleSheetItem(TextEdit->styleSheet(), QString("gtag%1").arg(i + 1));
	for (i = 0; i < 3; i++)
		FipsColorTextItem[i] = new QStyleSheetItem(TextEdit->styleSheet(), QString("ftag%1").arg(i + 1));

	LastAreaName = Config->GetStr(QString::null, CONFIG_LASTAREA, "NETMAIL");
	LoadSettings();
	Areas = new TAreas();
	ActiveEditCnt = 0;

	Current = NULL;

	HelpWindow = NULL;
	PhotoWnd = NULL;

	PositionStatusBar = new QLabel(statusBar());
	PositionStatusBar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	PositionStatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	PositionStatusBar->setLineWidth(0);
	PositionStatusBar->setFixedWidth(75);
	PositionStatusBar->setMargin(2);
	PositionStatusBar->setMidLineWidth(0);
	statusBar()->addWidget(PositionStatusBar, 1, true);

	GeekStatusBar = new QLabel(statusBar());
	GeekStatusBar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	GeekStatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	GeekStatusBar->setLineWidth(0);
	GeekStatusBar->setFixedWidth(20);
	GeekStatusBar->setMargin(2);
	GeekStatusBar->setMidLineWidth(0);
	statusBar()->addWidget(GeekStatusBar, 1, true);

	HtmlStatusBar = new QLabel(statusBar());
	HtmlStatusBar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	HtmlStatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	HtmlStatusBar->setLineWidth(0);
	HtmlStatusBar->setFixedWidth(20);
	HtmlStatusBar->setMargin(2);
	HtmlStatusBar->setMidLineWidth(0);
	statusBar()->addWidget(HtmlStatusBar, 1, true);

	ChrsStatusBar = new QLabel(statusBar());
	ChrsStatusBar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	ChrsStatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	ChrsStatusBar->setLineWidth(0);
	ChrsStatusBar->setFixedWidth(60);
	ChrsStatusBar->setMargin(2);
	ChrsStatusBar->setMidLineWidth(0);
	statusBar()->addWidget(ChrsStatusBar, 1, true);

	MessageStatusBar = new QLabel(statusBar());
	MessageStatusBar->setAlignment(Qt::AlignTop);
	MessageStatusBar->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	MessageStatusBar->setLineWidth(0);
	MessageStatusBar->setMargin(2);
	MessageStatusBar->setMidLineWidth(0);
	statusBar()->addWidget(MessageStatusBar, 1, true);
	connect(statusBar(), SIGNAL(messageChanged(const QString &)), SLOT(statusBarMessageChanged(const QString &)));

	ProgressBar = new QProgressBar(statusBar());
	ProgressBar->hide();
	statusBar()->addWidget(ProgressBar, 1, true);

	connect(MainToolBar, SIGNAL(visibilityChanged(bool)), this, SLOT(ToolBarVisibilityChanged(bool)));
	GeekStatusBar->installEventFilter(this);
}

void TMainWindow::destroy()
{
	if (HelpWindow != NULL)
	{
		if (HelpWindow->isShown())
			WidgetCloseFunc(HelpWindow);
		delete HelpWindow;
	}
	if (PhotoWnd != NULL)
	{
		if (PhotoWnd->isShown())
			PhotoWnd->hide();
		delete PhotoWnd;
	}

	delete Areas;
	delete MainActions;

	statusBar()->removeWidget(ProgressBar);
	statusBar()->removeWidget(MessageStatusBar);
	statusBar()->removeWidget(ChrsStatusBar);
	statusBar()->removeWidget(HtmlStatusBar);
	statusBar()->removeWidget(GeekStatusBar);
	statusBar()->removeWidget(PositionStatusBar);
	delete ProgressBar;
	delete MessageStatusBar;
	delete ChrsStatusBar;
	delete HtmlStatusBar;
	delete GeekStatusBar;
	delete PositionStatusBar;
}

void TMainWindow::statusBarMessageChanged(const QString &string)
{
	MessageStatusBar->setText(string.stripWhiteSpace().remove("\n"));
}

void TMainWindow::ShowProgress(uint Percent, QString Name, uint ProgressType)
{
	if (!ProgressBar->isShown())
		ProgressBar->show();
	if (!Name.isEmpty())
	{
		switch (ProgressType)
		{
			case PROGRESS_TYPE_RESCAN:
				MessageStatusBar->setText(QObject::tr("Rescan:") + " " + Name);
				break;
			case PROGRESS_TYPE_SEARCH:
				MessageStatusBar->setText(QObject::tr("Search in:") + " " + Name);
				break;
			case PROGRESS_TYPE_COMPILE:
				MessageStatusBar->setText(QObject::tr("Compile:") + " " + Name);
				break;
			case PROGRESS_TYPE_SAVENODES:
				MessageStatusBar->setText(QObject::tr("Save:") + " " + Name);
				break;
			case PROGRESS_TYPE_LOADNODES:
				MessageStatusBar->setText(QObject::tr("Load:") + " " + Name);
				break;
			default:
				MessageStatusBar->clear();
				break;
		}
	} else
		MessageStatusBar->clear();
	ProgressBar->setProgress(Percent, 100);
	qApp->processEvents();
}

void TMainWindow::HideProgress()
{
	QTimer::singleShot(500, this, SLOT(ActionHideProgress()));
}

void TMainWindow::ActionHideProgress()
{
	if (ProgressBar->isShown())
		ProgressBar->hide();
}

void TMainWindow::LoadSettings()
{
	int i;

	Config->ChangeTextCodec(Config->GetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCODEPAGE, "IBM 866"));

	TextEdit->setFont(QFont(Config->GetStr(QString::null, CONFIG_FONTNAME, DEFAULT_VIEW_FONT), Config->GetInt(QString::null, CONFIG_FONTSIZE, DEFAULT_VIEW_FONTSIZE)));
	TextEdit->setPaletteBackgroundColor(QColor(Config->GetInt(QString::null, CONFIG_FONTBACK, QColor(0, 0, 0).rgb())));
	QColor FColor = QColor(Config->GetInt(QString::null, CONFIG_FONTCOLOR, QColor(220, 220, 220).rgb()));
	TextEdit->setPaletteForegroundColor(FColor);

	Config->GetStyleSheetItem(QString::null, KludgeItem, QColor(111, 111, 111).rgb());
	Config->GetStyleSheetItem(QString::null, OriginItem, Qt::red);
	Config->GetStyleSheetItem(QString::null, TaglineItem, Qt::green);
	Config->GetStyleSheetItem(QString::null, TearlineItem, Qt::blue);
	Config->GetStyleSheetItem(QString::null, QuoteItem, QColor(175, 175, 175).rgb());
	Config->GetStyleSheetItem(QString::null, UrlItem, Qt::blue, STYLE_UNDERLINE);
	for (i = 0; i < 15; i++)
		Config->GetStyleSheetItem(QString::null, GoldedColorTextItem[i], DefGoldedColors[i]);
	Config->GetStyleSheetItem(QString::null, FipsColorTextItem[0], QColor(0, 255, 0), STYLE_BOLD);
	Config->GetStyleSheetItem(QString::null, FipsColorTextItem[1], FColor, STYLE_UNDERLINE);
	Config->GetStyleSheetItem(QString::null, FipsColorTextItem[2], FColor, STYLE_ITALIC);
}

void TMainWindow::showEvent(QShowEvent *)
{
	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	qApp->flush();
	sound_init();
	QTimer::singleShot(200, this, SLOT(ActionRescanAll()));
}

void TMainWindow::closeEvent(QCloseEvent *e)
{
	if (!GlobalQfeError)
	{
		if (ActiveEditCnt)
		{
			informationmessage(this, QObject::tr("You have one or more opened Editors. Quit canceled."));
			return;
		}
		if (!extquestionmessage(this, QObject::tr("Are You really want to quit?"), CONFIG_CONFIRM_QUIT))
			return;
	}

	if (HelpWindow != NULL)
		HelpWindow->hide();

	sound_quit();

	Config->SetStr(QString::null, CONFIG_LASTAREA, LastAreaName);
	Config->SetToolBar(this, MainToolBar);
	Config->SetBool(QString::null, CONFIG_VIEWSTATUSBAR, statusBar()->isShown());
	Config->SetBool(QString::null, CONFIG_VIEWKLUDGES, ViewKludgesAction->isOn());
	Config->SetBool(QString::null, CONFIG_VIEWHIGHLIGHT, ViewHighlightAction->isOn());
	Config->SetBool(QString::null, CONFIG_VIEWWORDWRAP, WordWrapAction->isOn());

	Config->SetList(QString::null, AreaList);
	Config->SetList(QString::null, MessageList);

	Config->SetInt(QString::null, CONFIG_WINDOWSTATE, windowState());
	hide();
	setWindowState(Qt::WindowNoState);

	Config->SetSplitter(QString::null, HorSplitter);
	Config->SetSplitter(QString::null, VerSplitter);

	Config->SetPos(this);
	Config->SetSize(this);
	e->accept();
}

void TMainWindow::customEvent(QCustomEvent *e)
{
	if (e->type() == TCloseEventID)
	{
		TEditWindow *EditWnd = (TEditWindow*)((TCloseEvent*)e)->Wnd;
		while (EditWnd->isShown())
			qApp->processEvents();
		ActiveEditCnt--;
		OptionsAction->setEnabled(ActiveEditCnt == 0);
		delete EditWnd;

		QStringList RescanList = QStringList::split("\n", ((TCloseEvent*)e)->RescanArea);
		for (uint32_t i = 0; i < RescanList.count(); i++)
		{
			bool found = false;

			QListViewItemIterator It = QListViewItemIterator(AreaList);
			while (It.current())
			{
				if (((TAreaItem*)It.current())->Area->Name == RescanList[i])
				{
					found = true;
					break;
				}
				++It;
			}
			if (found)
				RescanArea((TAreaItem*)It.current(), true);//(TAreaItem*)It.current() == (TAreaItem*)AreaList->selectedItem());
		}
	}
}

bool TMainWindow::eventFilter(QObject *o, QEvent *e)
{
	if (o == GeekStatusBar)
	{
		if ((e->type() == QEvent::MouseButtonPress) && !MessageGeekBuffer.isEmpty())
		{
			TGeekForm *GeekWnd = new TGeekForm(this);
			if (GeekWnd->SetUp(MessageGeekBuffer, MessageGeekR))
				GeekWnd->exec();
			delete GeekWnd;

			return true;
		}
	}
	return QMainWindow::eventFilter(o, e);
}

void TMainWindow::ActionClose()
{
	WidgetCloseFunc(this);
}

void TMainWindow::AreaListContextMenuRequested(QListViewItem *Item, const QPoint &Pt, int)
{
	if (Item)
		AreaListPopup->popup(Pt);
}

void TMainWindow::MessageListContextMenuRequested(QListViewItem *Item, const QPoint &Pt, int)
{
	if (Item)
		MessListPopup->popup(Pt);
}

void TMainWindow::ActionOpen()
{
	if (Config->IsDevelopMode())
	{
		qDebug("Develop Mode");
		QFileDialog* fd = new QFileDialog(this);
		fd->setDir(Config->GetStr(QString::null, CONFIG_LASTOPENSAVE, QString::null));
		fd->setFilter(QObject::tr("SQUISH messagebase (*.sqd)"));
		fd->setCaption(QObject::tr("Select file..."));
		fd->setIcon(*Config->FindPixmap("icon_load"));
		fd->setMode(QFileDialog::ExistingFile);
		fd->setInfoPreviewEnabled(true);
		if (fd->exec() == QDialog::Accepted)
		{
			Config->SetStr(QString::null, CONFIG_LASTOPENSAVE, fd->dir()->absPath());
			//
			//TODO:...
			//
		}
		delete fd;
	} else
		qDebug("TMainWindow::ActionOpen() Not implemented yet...");
}

void TMainWindow::LoadTemplate(QString Name, QString Area, QString *Buff)
{
	QString tmp = Config->ConfigPath + Name + "." + Area;

	*Buff = QString::null;

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
			Buff->append(stream.readLine().append("\n"));
		FileCloseFunc(&tpl);
	}
}

void TMainWindow::ActionSave()
{
	TMessage *Message = ((TMessageItem*)MessageList->selectedItem())->Message;
	QFileDialog* fd = new QFileDialog(this);

	fd->setDir(Config->GetStr(QString::null, CONFIG_LASTOPENSAVE, QString::null));
	fd->setFilter(QObject::tr("Text Files (*.txt)"));
	fd->addFilter(QObject::tr("HTML Files (*.htm *.html)"));
	fd->setSelectedFilter(0);
	fd->setCaption(QObject::tr("Select file..."));
	fd->setIcon(*Config->FindPixmap("icon_save"));
	fd->setMode(QFileDialog::AnyFile);
	fd->setInfoPreviewEnabled(true);
	if (fd->exec() == QDialog::Accepted)
	{
		Config->SetStr(QString::null, CONFIG_LASTOPENSAVE, fd->dir()->absPath());

		if (Message->Area->Open())
		{
			if (Message->Read())
			{
				QString Lines, Buffer = Config->toUTF((char*)Message->TxtBuff);
				QStringList BufferList = QStringList::split("\n", Buffer, true);

				int findex = -1, i = BufferList.count() - 1;
				while (i >= 0)
					if (isLowerKludge(&BufferList[i]))
						BufferList.remove(BufferList.at(i));
					else
						if (!BufferList[i].isEmpty())
							break;
						else
							i--;

				QString RealName = fd->selectedFile();

				if (fd->selectedFilter().find("txt") > 0)
				{
					findex = 0;
					if (!RealName.endsWith(".txt", false))
						RealName = RealName.append(".txt");
					LoadTemplate(TPL_SAVETXT_TPL, Message->Area->Name, &Lines);
				} else if (fd->selectedFilter().find("html") > 0)
				{
					for (i = 0; i < (int)BufferList.count(); i++)
						BufferList[i] = convert2qhtm(QStyleSheet::escape(BufferList[i]));

					if (!RealName.endsWith(".htm", false) && !RealName.endsWith(".html", false))
						RealName = RealName.append(".htm");
					findex = 1;
					LoadTemplate(TPL_SAVEHTM_TPL, Message->Area->Name, &Lines);
				} else
					errormessage(this, QObject::tr("Unknown format."));

				switch (findex)
				{
					case 0:
					case 1:
						{
							ExpandMacros(Message->Area, Message, &Lines, Message->Area->Name);
							ExpandMacros2(this, &Lines);
							Lines.replace("@text@", BufferList.join(((findex == 0) ? "\n" : "<br>\n")));

							QFile SavedFile(RealName);
							if (SavedFile.exists())
								if (!questionmessage(this, QObject::tr("File already exists. Overwrite?")))
									break;

							if (FileOpenFunc(&SavedFile, IO_WriteOnly | IO_Truncate))
							{
								QTextStream Stream(&SavedFile);
								Stream.setEncoding(QTextStream::UnicodeUTF8);
								Stream << Lines;
								FileCloseFunc(&SavedFile);
								statusBar()->message(QObject::tr("File saved"), 1000);
							} else
								errormessage(this, QObject::tr("Unable to save file."));
						}
						break;
					default:
						break;
				}
			}
		} else
			errormessage(this, QObject::tr("Can`t Read Message."));
		Message->Area->Close();
	} else
		errormessage(this, QObject::tr("Can`t Open Area") + " " + Message->Area->Name + ".");
	delete fd;
}

void TMainWindow::ActionSaveToDataBase()
{
	qDebug("TMainWindow::ActionSaveToDataBase() Not implemented yet...");
}

void TMainWindow::ActionSaveToArchive()
{
	int tmpIdx = Areas->Find(Config->GetArchiveArea());
	if (tmpIdx < 0)
	{
		errormessage(this, QObject::tr("Can't find Archive Area."));
		return;
	}

	TArea *ArchArea = Areas->at(tmpIdx);

	TMessage *OrigMessage = ((TMessageItem*)MessageList->selectedItem())->Message;
	if (OrigMessage->Area->Open())
	{
		if (ArchArea->Open())
		{
			if (OrigMessage->Read())
			{
				TMessage *DestMessage = ArchArea->Append();

				DestMessage->CtlBuff = (uint8_t*)qstrdup((char*)OrigMessage->CtlBuff);

				QString Lines, Buffer = Config->toUTF((char*)OrigMessage->TxtBuff);
				LoadTemplate(TPL_ARCHIVE_TPL, OrigMessage->Area->Name, &Lines);
				ExpandMacros(OrigMessage->Area, OrigMessage, &Lines, OrigMessage->Area->Name);
				ExpandMacros2(this, &Lines);
				Lines.replace("@text@", Buffer);

				DestMessage->TxtBuff = (uint8_t*)qstrdup(Config->fromUTF(Lines));

				OrigMessage->CopyAttributesTo(DestMessage);

				if (!ArchArea->Write(DestMessage->AreaIndex))
					errormessage(this, QObject::tr("Can't Write Message!"));
				else
					ArchArea->Rescan();
			}
			ArchArea->Close();
		}
		OrigMessage->Area->Close();
	}
}

void TMainWindow::RunEditor(TArea *Area, TMessage *Message, uint Operation, void *tmp)
{
	TEditWindow *EditWnd = new TEditWindow(this);

	if (EditWnd->SetUp(Area, Message, Operation, tmp))
	{
		EditWnd->show();
		ActiveEditCnt++;
		OptionsAction->setEnabled(false);
	} else
		delete EditWnd;
}

void TMainWindow::ActionNew()
{
	RunEditor(((TAreaItem*)AreaList->selectedItem())->Area, NULL, MSG_OPERATION_NEW, NULL);
}

void TMainWindow::ActionEdit()
{
	RunEditor(((TAreaItem*)AreaList->selectedItem())->Area, ((TMessageItem*)MessageList->selectedItem())->Message, MSG_OPERATION_EDIT, NULL);
}

void TMainWindow::ActionReply()
{
	RunEditor(((TAreaItem*)AreaList->selectedItem())->Area, ((TMessageItem*)MessageList->selectedItem())->Message, MSG_OPERATION_REPLY, NULL);
}

void TMainWindow::ActionReplyNetmail()
{
	TAreaSelect *SelectWnd = new TAreaSelect(this);
	for (uint32_t i = 0; i < Areas->count(); i++)
	{
		TArea *Area = Areas->at(i);
		if (isNetMailArea(Area))
			SelectWnd->AddArea((void*)Area);
	}
	if (SelectWnd->AreasCnt > 1)
	{
		if (SelectWnd->exec() == QDialog::Accepted)
			RunEditor(((TAreaSelectItem*)SelectWnd->SelectAreaList->selectedItem())->Area, ((TMessageItem*)MessageList->selectedItem())->Message, MSG_OPERATION_REPLYNET, NULL);
	} else if (SelectWnd->AreasCnt == 1)
		RunEditor(((TAreaSelectItem*)SelectWnd->SelectAreaList->firstChild())->Area, ((TMessageItem*)MessageList->selectedItem())->Message, MSG_OPERATION_REPLYNET, NULL);
	delete SelectWnd;
}

void TMainWindow::ActionForward()
{
	TAreaSelect *SelectWnd = new TAreaSelect(this);
	for (uint32_t i = 0; i < Areas->count(); i++)
	{
		TArea *Area = Areas->at(i);
		if (isNetMailArea(Area) || isEchoMailArea(Area))
			if (isNetMailArea(Area) || (Area != ((TAreaItem*)AreaList->selectedItem())->Area))
				SelectWnd->AddArea((void*)Area);
	}
	if (SelectWnd->AreasCnt)
		if (SelectWnd->exec() == QDialog::Accepted)
			RunEditor(((TAreaSelectItem*)SelectWnd->SelectAreaList->selectedItem())->Area, ((TMessageItem*)MessageList->selectedItem())->Message, MSG_OPERATION_FORWARD, NULL);
	delete SelectWnd;
}

void TMainWindow::ActionDelete()
{
	if (extquestionmessage(this, QObject::tr("Do You really want delete current message?"), CONFIG_CONFIRM_DELETE))
	{
		TMessage *Message = ((TMessageItem*)MessageList->selectedItem())->Message;

		TMessageItem *TmpItem = (TMessageItem*)MessageList->selectedItem()->itemBelow();
		if (TmpItem == NULL)
			TmpItem = (TMessageItem*)MessageList->selectedItem()->itemAbove();
		if (TmpItem != NULL)
			Message->Area->SetLastRead(TmpItem->Message->uid);

		if (Message->Area->Delete(Message->AreaIndex))
		{
			Current = NULL;
			RescanArea((TAreaItem*)AreaList->selectedItem(), true);
			((TAreaItem*)AreaList->selectedItem())->UpdateArea();
		} else
			errormessage(this, QObject::tr("Can't Delete Message!"));
	}
}

void TMainWindow::ActionFind()
{
	TFind *FindWnd = new TFind(this);

	FindWnd->exec();

	delete FindWnd;
}

void TMainWindow::ActionPreviousMess()
{
	MessageList->setSelected(MessageList->selectedItem()->itemAbove(), true);
}

void TMainWindow::ActionNextMess()
{
	MessageList->setSelected(MessageList->selectedItem()->itemBelow(), true);
}

void TMainWindow::ActionNextUnread()
{
	QListViewItemIterator It;

	if (MessageList->selectedItem()->itemBelow() != NULL)
	{
		It = QListViewItemIterator(MessageList->selectedItem()->itemBelow());
		while (It.current())
		{
			if (!isReaded(((TMessageItem*)It.current())->Message))
			{
				MessageList->setSelected(It.current(), true);
				return;
			}
			++It;
		}
	}
	It = QListViewItemIterator(MessageList);
	while (It.current())
	{
		if (!isReaded(((TMessageItem*)It.current())->Message))
		{
			MessageList->setSelected(It.current(), true);
			return;
		}
		++It;
	}
}

void TMainWindow::ActionMarkMessage()
{
	((TMessageItem*)MessageList->selectedItem())->ToggleMarked();
	if (NextMessAction->isEnabled())
		ActionNextMess();

	bool found = false;
	QListViewItemIterator It = QListViewItemIterator(MessageList);
	while (It.current())
	{
		if (((TMessageItem*)It.current())->isMarked())
		{
			found = true;
			break;
		}
		++It;
	}
	DecodeAction->setEnabled(found);
}

void TMainWindow::ActionAddToBook()
{
	TAddressBook *AddressBook = new TAddressBook(this);
	AddressBook->SetUp(false);

	TMessage *Message = ((TMessageItem*)MessageList->selectedItem())->Message;
	Message->Area->Open();
	Message->Read();
	AddressBook->AddExternalAddr(Config->toUTF((char*)Message->from), addr2str1(&Message->origaddr, Message->CtlBuff));
	Message->Area->Close();

	if (AddressBook->exec() == QDialog::Accepted)
		Config->AddressBook->Save();
	else
		Config->AddressBook->Read();

	delete AddressBook;
}

void TMainWindow::ActionDecode()
{
	uint32_t completed = 0;
	TFTNCoDec *Decoder = new TFTNCoDec();
	QByteArray *Array = new QByteArray();
	QString FileName;

	if (((TAreaItem*)AreaList->selectedItem())->Area->Open())
	{
		QListViewItemIterator It = QListViewItemIterator(MessageList);
		while (It.current())
		{
			if (((TMessageItem*)It.current())->isMarked())
				Decoder->DecodeMessage(((TMessageItem*)It.current())->Message);
			++It;
		}
		((TAreaItem*)AreaList->selectedItem())->Area->Close();

		while (Decoder->MakeFile(Array, &FileName))
		{
			QFileDialog* fd = new QFileDialog(this);
			fd->setDir(Config->GetStr(QString::null, CONFIG_LASTOPENSAVE, QString::null));
			fd->setFilter(QObject::tr("All Files (*)"));
			fd->setCaption(QObject::tr("Select file..."));
			fd->setIcon(*Config->FindPixmap("icon_save"));
			fd->setMode(QFileDialog::AnyFile);
			fd->setInfoPreviewEnabled(true);
			if (!FileName.isEmpty())
				fd->setSelection(FileName);

			bool Retry = true;
			while (Retry)
			{
				Retry = false;
				if (fd->exec() == QDialog::Accepted)
				{
					QFileInfo fi(fd->selectedFile());
					if (!ValidateFileName(fi.fileName()))
						if (questionmessage(this, QObject::tr("File name contain invalid characters. Do You want set another file name?")))
						{
							Retry = true;
							continue;
						}
					Config->SetStr(QString::null, CONFIG_LASTOPENSAVE, fd->dir()->absPath());
					QFile UUE(fd->selectedFile());
					if (UUE.exists())
					{
						if (!questionmessage(this, QObject::tr("File already exists. Overwrite?")))
						{
							Retry = true;
							continue;
						}
					}
					if (FileOpenFunc(&UUE, IO_WriteOnly | IO_Truncate))
					{
						UUE.writeBlock(Array->data(), Array->size());
						FileCloseFunc(&UUE);
						completed++;
					} else
						errormessage(this, QObject::tr("Unable to save file."));
				}
			}
		}

		if (completed)
			statusBar()->message(QObject::tr("Succesfully decoded %1 files.").arg(completed), 1000);

		It = QListViewItemIterator(MessageList);
		while (It.current())
		{
			if (((TMessageItem*)It.current())->isMarked())
				((TMessageItem*)It.current())->ToggleMarked();
			++It;
		}
	}

	delete Array;
	delete Decoder;
}

void TMainWindow::ActionWordWrap()
{
	TextEdit->setWordWrap(WordWrapAction->isOn() ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
}

void TMainWindow::ActionMessageProp()
{
	qDebug("TMainWindow::ActionMessageProp() Not implemented yet...");
}

void TMainWindow::ActionPreviousArea()
{
	AreaList->setSelected(AreaList->selectedItem()->itemAbove(), true);
}

void TMainWindow::ActionNextArea()
{
	AreaList->setSelected(AreaList->selectedItem()->itemBelow(), true);
}

void TMainWindow::ActionMarkAllAsRead()
{
	TArea *Area = ((TAreaItem*)AreaList->selectedItem())->Area;
	if (Area->Open())
	{
		for (uint32_t i = 0; i < Area->count(); i++)
			if (!isReaded(Area->at(i)))
				Area->at(i)->MarkAsRead();

		Area->Close();

		((TAreaItem*)AreaList->selectedItem())->UpdateArea();
		MarkAllAsReadAction->setEnabled(Area->UnReadCnt > 0);
	}
}

void TMainWindow::ActionUnsubscribe()
{
	TAreaSelect *SelectWnd = new TAreaSelect(this);
	for (uint32_t i = 0; i < Areas->count(); i++)
	{
		TArea *Area = Areas->at(i);
		if (isNetMailArea(Area))
			SelectWnd->AddArea((void*)Area);
	}
	if (SelectWnd->AreasCnt > 1)
	{
		if (SelectWnd->exec() == QDialog::Accepted)
			RunEditor(((TAreaSelectItem*)SelectWnd->SelectAreaList->selectedItem())->Area, NULL, MSG_OPERATION_UNSUBSCRIBE, (void*)((TAreaItem*)AreaList->selectedItem())->Area);
	} else if (SelectWnd->AreasCnt == 1)
		RunEditor(((TAreaSelectItem*)SelectWnd->SelectAreaList->firstChild())->Area, NULL, MSG_OPERATION_UNSUBSCRIBE, (void*)((TAreaItem*)AreaList->selectedItem())->Area);
	delete SelectWnd;
}

void TMainWindow::ActionAreaProp()
{
	if (AreaList->selectedItem())
	{
		TArea *Area = ((TAreaItem*)AreaList->selectedItem())->Area;

		TAreaProp *PropWnd = new TAreaProp(this);

		PropWnd->SetUp(Area);
		PropWnd->exec();

		delete PropWnd;
	}
}

#if !defined(QT_NO_PRINTER)
#	include <qpainter.h>
#	include <qprinter.h>
#	include <qsimplerichtext.h>
#	include <qpaintdevicemetrics.h>
#endif // QT_NO_PRINTER

void TMainWindow::ActionPrint()
{
#if !defined(QT_NO_PRINTER)
	QPrinter Printer(QPrinter::HighResolution);
	Printer.setFullPage(true);

	if (Printer.setup(this))
	{
		QPainter Painter;

		statusBar()->message(QObject::tr("Printing..."));

		if(!Painter.begin(&Printer))
		{
			statusBar()->message(QObject::tr("Printing aborted"), 1000);
			return;
        	}

		qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
		qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);

		QPaintDeviceMetrics Metrics(Painter.device());
		int dpiy = Metrics.logicalDpiY();
		int margin = (int)((2 / 2.54) * dpiy);
		QRect view(margin, margin, Metrics.width() - 2 * margin, Metrics.height() - 2 * margin);

		QSimpleRichText RTF(TextEdit->hasSelectedText() ? TextEdit->selectedText() : TextEdit->text(), TextEdit->font(), TextEdit->context(), TextEdit->styleSheet(), TextEdit->mimeSourceFactory(), view.height(), UrlItem->color(), false);
		RTF.setWidth(&Painter, view.width());

		int page = 1;
		do {
			qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
			RTF.draw(&Painter, margin, margin, view, colorGroup());
			view.moveBy(0, view.height());
			Painter.translate(0 , -view.height());
			Painter.drawText(view.right() - Painter.fontMetrics().width(QString::number(page)), view.bottom() + Painter.fontMetrics().ascent() + 5, QString::number(page));
			if ((view.top() - margin) >= RTF.height())
				break;
			Printer.newPage();
			page++;
		} while (true);

		Painter.end();

		qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
		qApp->restoreOverrideCursor();

		statusBar()->message(QObject::tr("Printing completed."), 1000);
	};
#else // QT_NO_PRINTER
	informationmessage(this, QObject::tr("Printer support disabled by Qt-config."));
#endif // QT_NO_PRINTER
}

void TMainWindow::ActionNodelistBrowser()
{
	TNodelistBrowser *NodelistBrowser = new TNodelistBrowser(this);
	if (NodelistBrowser->TestLists())
	{
		NodelistBrowser->SetSelectMode(false);
		NodelistBrowser->exec();
	} else
		informationmessage(this, QObject::tr("No Nodelists in config."));
	delete NodelistBrowser;
}

void TMainWindow::ActionAddressBook()
{
	TAddressBook *AddressBook = new TAddressBook(this);
	AddressBook->SetUp(false);
	if (AddressBook->exec() == QDialog::Accepted)
		Config->AddressBook->Save();
	else
		Config->AddressBook->Read();
	delete AddressBook;
}

void TMainWindow::ActionExtensions()
{
	qDebug("TMainWindow::ActionExtensions() Not implemented yet...");
}

void TMainWindow::ActionOptions()
{
	TOptions *Options = new TOptions(this);
	if (Options->exec() == QDialog::Accepted)
	{
		TArea *CurrentArea = (AreaList->selectedItem() != NULL) ? ((TAreaItem*)AreaList->selectedItem())->Area : NULL;
//		Areas->Close();
		Config->DoneFido();
		Config->InitFido();
		LoadSettings();
		if (CurrentArea != NULL)
		{
#warning "Redraw on config change disabled!"
//			Areas->Open(CurrentArea);
			ActionRedrawMessage();
		}
	}
	delete Options;
}

void TMainWindow::ActionAboutQt()
{
	qApp->aboutQt();
}

void TMainWindow::ActionHelp()
{
	HelpIndex();
}

void TMainWindow::ActionAbout()
{
	TAbout *About = new TAbout(this);
	About->exec();
	delete About;
}

void TMainWindow::ActionWhatsThis()
{
	QWhatsThis::enterWhatsThisMode();
}

void TMainWindow::ToolBarVisibilityChanged(bool)
{
	if (isShown())
		ViewToolBarAction->setOn(MainToolBar->isShown());
}

void TMainWindow::ActionViewToolBar()
{
	MainToolBar->setShown(ViewToolBarAction->isOn());
}

void TMainWindow::ActionViewStatusBar()
{
	statusBar()->setShown(ViewStatusBarAction->isOn());
}

void TMainWindow::ClearMessageView()
{
	if (PhotoWnd != NULL)
	{
		if (PhotoWnd->isShown())
			PhotoWnd->hide();
		PhotoWnd->SysopImage->clear();
	}

	sound_stop();

	Current = NULL;

	PositionStatusBar->clear();
	ChrsStatusBar->clear();
	SetGeek(false);
	SetHtml(false);

	FNLabel->clear();
	FALabel->clear();
	TNLabel->clear();
	TALabel->clear();
	SLabel->clear();
	FLabel->clear();
	DLabel->clear();

	// Restore colors after HTML-mode
	TextEdit->setFont(QFont(Config->GetStr(QString::null, CONFIG_FONTNAME, DEFAULT_VIEW_FONT), Config->GetInt(QString::null, CONFIG_FONTSIZE, DEFAULT_VIEW_FONTSIZE)));
	QColor FColor = QColor(Config->GetInt(QString::null, CONFIG_FONTBACK, QColor(0, 0, 0).rgb()));
	TextEdit->setPaletteBackgroundColor(FColor);
	TextEdit->setPaletteForegroundColor(QColor(Config->GetInt(QString::null, CONFIG_FONTCOLOR, QColor(220, 220, 220).rgb())));
	TextEdit->setPaper(QBrush(FColor));

	TextEdit->clear();

	DeleteAction->setEnabled(false);
	PreviousMessAction->setEnabled(false);
	NextMessAction->setEnabled(false);
	NextUnreadAction->setEnabled(false);
	MarkMessAction->setEnabled(false);

	MessageGeekBuffer = QString::null;
	MessageGeekR = false;
}

void TMainWindow::ClearMessageList()
{
	ClearMessageView();
	MessageList->clear();
	NewAction->setEnabled(false);
	EditAction->setEnabled(false);
	ReplyAction->setEnabled(false);
	ReplyNetmailAction->setEnabled(false);
	ForwardAction->setEnabled(false);
	PrintAction->setEnabled(false);
	SaveAction->setEnabled(false);
	ArchiveAction->setEnabled(false);
	DecodeAction->setEnabled(false);
	AddToBookAction->setEnabled(false);
}

void TMainWindow::RescanArea(TAreaItem *It, bool DoItActive)
{
#warning "RescanArea need test"
	if (It->Area->Open())
	{
		It->Area->Rescan();
		Current = NULL;
		if (DoItActive)
			AreaChanged((QListViewItem*)It);
		It->Area->Close();
	}
	//AreaList->repaintItem(It);
}

void TMainWindow::ActionRescan()
{
	if (AreaList->selectedItem() != NULL)
		RescanArea((TAreaItem*)AreaList->selectedItem(), true);
}

void TMainWindow::AddBase(void *FconfBase, Area_Type atype)
{
	Base_Type bt = BASETYPE_UNKNOWN;
	s_area *Base = (s_area*)FconfBase;

	switch (Base->msgbType)
	{
		case MSGTYPE_JAM:
			bt = BASETYPE_JAM;
			break;
		case MSGTYPE_SQUISH:
			bt = BASETYPE_SQUISH;
			break;
		case MSGTYPE_SDM:
			bt = BASETYPE_MSG;
			break;
		default:
			return;
	}

	QString Uplink = QString::null;
	QString UplinkPwd = QString::null;
	QString UplinkRbt = "AREAFIX";

	for (uint32_t i = 0; i < Base->downlinkCount; i++)
		if (Base->downlinks[i]->defLink)
		{
			Uplink = addr2str2(Base->downlinks[i]->link->hisAka);
#if defined(FIDOCONF_VER1_4)
			if (Base->downlinks[i]->link->AreaFix != 0)
			{
				UplinkPwd = Base->downlinks[i]->link->areaFixPwd;
#else
			if (Base->downlinks[i]->link->areafix.on)
			{
				UplinkPwd = Base->downlinks[i]->link->areafix.pwd;
				if (Base->downlinks[i]->link->areafix.name)
					UplinkRbt = Base->downlinks[i]->link->areafix.name;
#endif
			} else
				UplinkPwd = Base->downlinks[i]->link->defaultPwd;
		}

	TArea *Area = Areas->Add(Base->areaName, Config->toUTF(Base->description), Base->fileName, atype, bt, Config->GetInt(CONFIG_SECTION_FIDO, CONFIG_FIDODEFZONE, 2), addr2str2(*Base->useAka), Base->group, Uplink, UplinkPwd, UplinkRbt, (char*)Config->GetSysopName().ascii());
	if (Area != NULL)
		new TAreaItem(AreaList, Area);
	else
		errormessage(this, QString(QObject::tr("Error load area \"%1\". Area skipped. See debug log for additional information.")).arg(Base->areaName));
}

void TMainWindow::ActionRescanAll()
{
	int i;
	uint32_t j = 0, cnt;

	debugmessage("Rescan Areas...");
	statusBar()->message(QObject::tr("Rescan Areas..."));

	if ((Splash != NULL) && (Splash->isShown()))
		Splash->message(QObject::tr("Rescan Areas..."), Qt::AlignLeft, Qt::white);

	ClearMessageList();
	AreaList->clear();
	Areas->clear();

	AreaList->setUpdatesEnabled(false);
	MessageList->blockSignals(true);

	int scol = AreaList->sortColumn();
	AreaList->setSortColumn(-1);

	PreviousAreaAction->setEnabled(false);
	NextAreaAction->setEnabled(false);
	MarkAllAsReadAction->setEnabled(false);
	UnsubscribeAction->setEnabled(false);
	AreaPropAction->setEnabled(false);
	FindAction->setEnabled(false);

	cnt = Config->FidoConfig->netMailAreaCount;
	debugmessage(QString("Netmail areas count=%1").arg(cnt));
	if ((cnt == 0) || (cnt > 100))
	{
		errormessage(this, QObject::tr("Configuration incorrect. Can`t continue."));
		GlobalQfeError = true;
		MessageList->blockSignals(false);
		AreaList->setUpdatesEnabled(true);
		AreaList->triggerUpdate();
		WidgetCloseFunc(this);
		return;
	}

	cnt += 2 + Config->FidoConfig->localAreaCount + Config->FidoConfig->echoAreaCount;

	for (i = 0; i < (int)Config->FidoConfig->netMailAreaCount; i++)
	{
		ShowProgress((j * 100) / cnt, Config->FidoConfig->netMailAreas[i].areaName, PROGRESS_TYPE_RESCAN);
		j++;
		AddBase(&Config->FidoConfig->netMailAreas[i], AREATYPE_NETMAIL);
	}

	ShowProgress((j * 100) / cnt, Config->FidoConfig->dupeArea.areaName, PROGRESS_TYPE_RESCAN);
	j++;
	AddBase(&Config->FidoConfig->dupeArea, AREATYPE_DUPEMAIL);

	ShowProgress((j * 100) / cnt, Config->FidoConfig->badArea.areaName, PROGRESS_TYPE_RESCAN);
	j++;
	AddBase(&Config->FidoConfig->badArea, AREATYPE_BADMAIL);

	for (i = 0; i < (int)Config->FidoConfig->localAreaCount; i++)
	{
		ShowProgress((j * 100) / cnt, Config->FidoConfig->localAreas[i].areaName, PROGRESS_TYPE_RESCAN);
		j++;
		AddBase(&Config->FidoConfig->localAreas[i], AREATYPE_LOCALMAIL);
	}

	for (i = 0; i < (int)Config->FidoConfig->echoAreaCount; i++)
	{
		ShowProgress((j * 100) / cnt, Config->FidoConfig->echoAreas[i].areaName, PROGRESS_TYPE_RESCAN);
		j++;
		AddBase(&Config->FidoConfig->echoAreas[i], AREATYPE_ECHOMAIL);
	}

	QToolTip::remove(AreaList);

	AreaList->setSortColumn(scol);

	FindAction->setEnabled(Areas->count() > 0);

	MessageList->blockSignals(false);
	AreaList->setUpdatesEnabled(true);
	AreaList->triggerUpdate();

	ShowProgress(100, QString::null, PROGRESS_TYPE_RESCAN);
	debugmessage("Rescan Done.");
	HideProgress();

	i = Areas->Find(StartArea.isEmpty() ? LastAreaName : StartArea);
	StartArea = QString::null;
	AreaList->setSelected(AreaList->findItem(QString::number((i < 0) ? 1 : i + 1), 0), true);
	statusBar()->message(QObject::tr("Rescan Done."), 1000);
}

void TMainWindow::ActionRedrawMessage()
{
	Current = NULL;
	MessageChanged(MessageList->selectedItem());
}

void TMainWindow::FindCallBack(TFindItem *Item)
{
	bool found = false;

	QListViewItemIterator It = QListViewItemIterator(AreaList);
	while (It.current())
	{
		if (((TAreaItem*)It.current())->Area == Item->Area)
		{
			AreaList->setSelected(It.current(), true);
			found = true;
			break;
		}
		++It;
	}

	if (found)
	{
		It = QListViewItemIterator(MessageList);
		while (It.current())
		{
			if (((TMessageItem*)It.current())->Message == Item->Message)
			{
				MessageList->setSelected(It.current(), true);
				break;
			}
			++It;
		}
	}
}

void TMainWindow::AreaChanged(QListViewItem *Item)
{
	if (Item == NULL)
	{
		ClearMessageList();
		return;
	}

	TArea *Area = ((TAreaItem*)Item)->Area;

	if (Current != NULL)
		if (Current->Area == Area)
			return;

	MessageList->setUpdatesEnabled(false);
	MessageList->blockSignals(true);

	int scol = MessageList->sortColumn();
	MessageList->setSortColumn(-1);

	ClearMessageList();

	AreaList->ensureItemVisible(Item);

	NewAction->setEnabled(isNetMailArea(Area) || isEchoMailArea(Area));

	PreviousAreaAction->setEnabled(Item != AreaList->firstChild());
	NextAreaAction->setEnabled(Item != AreaList->lastItem());
	UnsubscribeAction->setEnabled(isEchoMailArea(Area));

	AreaPropAction->setEnabled(true);
	LastAreaName = Area->Name;

	for (uint32_t i = 0; i < Area->count(); i++)
		new TMessageItem(MessageList, (TAreaItem*)Item, i);

	QToolTip::remove(MessageList);

	MessageList->setSortColumn(scol);

	int lr = Area->UID2Index(Area->GetLastRead());

	MessageList->blockSignals(false);
	MessageList->setUpdatesEnabled(true);

	if (lr < 0)
		MessageList->setSelected(MessageList->firstChild(), true);
	else
		MessageList->setSelected(Area->at(lr)->MessageItem, true);

	MarkAllAsReadAction->setEnabled(Area->UnReadCnt > 0);

	MessageList->triggerUpdate();
}

void TMainWindow::ParseColorLine(QString *Line, uint Start)
{
	//TODO: Parser Need Full Rewrite !!!

	bool InScope = false, hstyle = Config->GetBool(QString::null, CONFIG_VIEWSTYLE, true);
	byte StartTag = 0, EndTag = 0;
	int j, k;
	uint32_t i = Start, StartTagPos = 0, EndTagPos = 0;

//qDebug("String Start=%i =%s", Start, Line->ascii());

	if (!ViewHighlightAction->isOn()) return;

	if (Line->find("<a href=") < 0)
	{
		QRegExp rx;
		rx.setPattern(REGEXP_URL);
		j = rx.search(*Line, Start);
		if (j > -1)
		{
			k = Line->find("&gt;", j);
			if ((k < 0) || ((k - j) >= rx.matchedLength()))
				k = rx.matchedLength();
			else
				k -= j;
			Line->insert(j + k, "</utag></a>");
			Line->insert(j, "<a href=\"" + rx.cap().stripWhiteSpace() + "\"><utag>");
		}
		rx.setPattern(REGEXP_EMAIL);
		j = rx.search(*Line, Start);
		if (j > -1)
		{
			k = Line->find("&gt;", j);
			if ((k < 0) || ((k - j) >= rx.matchedLength()))
				k = rx.matchedLength();
			else
				k -= j;
			Line->insert(j + k, "</utag></a>");
			Line->insert(j, "<a href=\"" + rx.cap().stripWhiteSpace() + "\"><utag>");
		}
	}

/* This code is work fine. Later I remake this function as below.
QRegExp rx("(^|\\B)[*].+[*]($|\\B)");
j = rx.search(*Line, Start);
if (j > -1)
{
	Line->remove(j + rx.matchedLength() - 1, 1);
	Line->insert(j + rx.matchedLength() - 1, "</utag>");
	Line->remove(j, 1);
	Line->insert(j, "<utag>");
}
return;
*/

	while (i < Line->length())
	{
		QChar CurChar = Line->at(i);
		if (CurChar == QChar('<'))
		{
			j = Line->find('>', i + 1);
			if (j > -1)
			{
				i += j + 1;
				j = Line->find('<', i);
				if (j > -1)
				{
					i += j + 1;
					j = Line->find('>', i);
					if (j > -1)
						i += j + 1;
				}
				continue;
			}
		}
		if (hstyle)
		{
			/* GoldEd Style Highlight */
			if (CurChar == QChar('*'))
			{
				if (!InScope)
				{
//					if ((i == Start) || ((i > Start) && QString("*/_#").find(Line->at(i - 1)) > -1))
//					{
						InScope = true;
						StartTag |= 1;
						EndTag = 0;
						EndTagPos = 0;
						if (!StartTagPos) StartTagPos = i;
//					}
				} else {
					EndTag |= 1;
					if (!EndTagPos) EndTagPos = i;
				}
			} else if (CurChar == QChar('/'))
			{
				if (!InScope)
				{
//					if ((i == Start) || ((i > Start) && QString("*/_#").find(Line->at(i - 1)) > -1))
//					{
						InScope = true;
						StartTag |= 2;
						EndTag = 0;
						EndTagPos = 0;
						if (!StartTagPos) StartTagPos = i;
//					}
				} else {
					EndTag |= 2;
					if (!EndTagPos) EndTagPos = i;
				}
			} else if (CurChar == QChar('_'))
			{
				if (!InScope)
				{
//					if ((i == Start) || ((i > Start) && QString("*/_#").find(Line->at(i - 1)) > -1))
//					{
						InScope = true;
						StartTag |= 4;
						EndTag = 0;
						EndTagPos = 0;
						if (!StartTagPos) StartTagPos = i;
//					}
				} else {
					EndTag |= 4;
					if (!EndTagPos) EndTagPos = i;
				}
			} else if (CurChar == QChar('#'))
			{
				if (!InScope)
				{
//					if ((i == Start) || ((i > Start) && QString("*/_#").find(Line->at(i - 1)) > -1))
//					{
						InScope = true;
						StartTag |= 8;
						EndTag = 0;
						EndTagPos = 0;
						if (!StartTagPos) StartTagPos = i;
//					}
				} else {
					EndTag |= 8;
					if (!EndTagPos) EndTagPos = i;
				}
			} else {
				if (EndTagPos)
				{
					if (((EndTagPos - StartTagPos) > 1) && ((i == (Line->length() - 1)) || CurChar.isSpace() || CurChar.isPunct()))
					{
						break;
					} else {
						InScope = false;
						i = StartTagPos + 1;
						StartTagPos = 0;
						EndTagPos = 0;
						StartTag = 0;
						continue;
					}
				}
			}
		} else {
			/* FIPS Style Highlight */
			if (CurChar == QChar('*'))
			{
				if (!InScope)
				{
					InScope = true;
					StartTag |= 1;
					EndTag = 0;
					EndTagPos = 0;
					if (!StartTagPos) StartTagPos = i;
				} else {
					EndTag |= 1;
					if (!EndTagPos) EndTagPos = i;
				}
			} else if (CurChar == QChar('_'))
			{
				if (!InScope)
				{
					InScope = true;
					StartTag |= 2;
					EndTag = 0;
					EndTagPos = 0;
					if (!StartTagPos) StartTagPos = i;
				} else {
					EndTag |= 2;
					if (!EndTagPos) EndTagPos = i;
				}
			//
			//
			//
			} else {
				if (EndTagPos)
				{
					if (((EndTagPos - StartTagPos) > 1) && ((i == (Line->length() - 1)) || CurChar.isSpace() || CurChar.isPunct()))
					{
						break;
					} else {
						InScope = false;
						i = StartTagPos + 1;
						StartTagPos = 0;
						EndTagPos = 0;
						StartTag = 0;
						continue;
					}
				}
			}
		}
		i++;
	}
//qDebug("Finally StartTagPos=%i", StartTagPos);
//qDebug("Finally EndTagPos=%i", EndTagPos);
	StartTag &= EndTag;
	if (InScope && (EndTagPos > StartTagPos) && StartTag)
	{
		i = 0;

		if (StartTag & 1) i++;
		if (StartTag & 2) i++;
		if (StartTag & 4) i++;
		if (StartTag & 8) i++;
		// hstyle ?
		if (EndTagPos > (StartTagPos + i))
		{
			Line->remove(EndTagPos, i);
			Line->insert(EndTagPos, QString("</gtag%1>").arg(StartTag));
			Line->remove(StartTagPos, i);
			Line->insert(StartTagPos, QString("<gtag%1>").arg(StartTag));
			EndTagPos += QString("<gtag%1>").arg(StartTag).length() * 2 + 1 + 1 - i * 2;
		} else
			EndTagPos = StartTagPos + 1;
		ParseColorLine(Line, EndTagPos);
	} else if (StartTagPos && (StartTagPos < Line->length()))
		ParseColorLine(Line, StartTagPos + 1);
}

void TMainWindow::SetUserPixmap(QPixmap pix)
{
	if (PhotoWnd == NULL)
		PhotoWnd = new TPhotoWnd(this, "", Qt::WType_TopLevel | Qt::WStyle_Customize | Qt::WStyle_DialogBorder);

	if (!pix.isNull() && (PhotoWnd->SysopImage->pixmap() != NULL))
		if (!PhotoWnd->SysopImage->pixmap()->isNull())
			return;

	PhotoWnd->SysopImage->setPixmap(ScalePixmap(pix, SYSOPIMAGE_WIDTH, SYSOPIMAGE_HEIGHT));
	if (!PhotoWnd->isShown())
		PhotoWnd->show();
}

void TMainWindow::SetGeek(bool DoSet)
{
	if (DoSet)
	{
		GeekStatusBar->setPixmap(*Config->FindPixmap("icon_geek"));
		QToolTip::add(GeekStatusBar, QObject::tr("Message contain Geek-code"));
	} else {
		GeekStatusBar->clear();
		QToolTip::remove(GeekStatusBar);
	}
}

void TMainWindow::SetHtml(bool DoSet)
{
	if (DoSet)
	{
		HtmlStatusBar->setPixmap(*Config->FindPixmap("icon_html"));
		QToolTip::add(HtmlStatusBar, QObject::tr("Message in HTML-format"));
	} else {
		HtmlStatusBar->clear();
		QToolTip::remove(HtmlStatusBar);
	}
}

void TMainWindow::ReplaceSmilies(QString *Buffer)
{
	int j, k;
	QString tmp;

	if (Config->GetBool(CONFIG_SECTION_EDIT, CONFIG_SMILIES_REPLACE, false))
	for (k = 0;; k++)
	{
		if (smilies_table[k].occ.isEmpty())
			break;
		j = Buffer->find(smilies_table[k].occ, 0);
		if (j > -1)
		{
			if (!Buffer->at(j - 1).isLetterOrNumber() && !Buffer->at(j + smilies_table[k].occ.length()).isLetterOrNumber())
			{
				tmp = QString("<img src=\"%1.xpm\">").arg(smilies_table[k].img);
				Buffer->replace(smilies_table[k].occ, tmp);
			}
		}
	}
}

void TMainWindow::DecodeGeek(uint8_t *Buffer)
{
	int i, j;
	QString gc;

	for (i = 0; i < 999; i++)
	{
		gc = GetKludge(Buffer, "GC:", i);
		if (!gc.isEmpty())
		{
			j = gc.find("Geek-R 1.3");
			if (j > -1)
			{
				gc = gc.mid(j + (10 - 3));
				MessageGeekR = true;
			}
			MessageGeekBuffer.append((MessageGeekBuffer.isEmpty() ? QString::null : " ") + gc.mid(3).stripWhiteSpace());
		} else
			break;
	}

	for (i = 0; i < 999; i++)
	{
		gc = GetKludge(Buffer, "Geek-R:", i);
		if (!gc.isEmpty())
		{
			MessageGeekR = true;
			MessageGeekBuffer.append((MessageGeekBuffer.isEmpty() ? QString::null : " ") + gc.mid(7).stripWhiteSpace());
		} else
			break;
	}

	if (!MessageGeekBuffer.isEmpty())
		SetGeek(true);
}

void TMainWindow::MessageChanged(QListViewItem *Item)
{
	bool isHtml = false;
	QString gc, gif, charset = QString::null;

	if (Item == NULL)
	{
		ClearMessageView();
		return;
	}

	TMessageItem *CurrentItem = (TMessageItem*)Item;
	if (Current != NULL)
		if (Current == CurrentItem->Message)
			return;

	ClearMessageView();
	MessageList->ensureItemVisible(Item);

	CurrentItem->ParseData();

	PreviousMessAction->setEnabled(Item->itemAbove() != NULL);
	NextMessAction->setEnabled(Item->itemBelow() != NULL);

	Current = CurrentItem->Message;
	if (Current == NULL)
		return;

	if (Current->Area->Open())
	{
		if (Current->Read())
		{
			PositionStatusBar->setText(QString("%1/%2").arg(Current->AreaIndex + 1).arg(Current->Area->count()));

			EditAction->setEnabled((isNetMail(Current) || isEchoMail(Current) || (strcompare(Current->Area->Name, Config->GetDraftArea()))) && ((Current->attr & FLAG_LOC) != 0) && ((Current->attr & (FLAG_SENT | FLAG_CPT)) == 0) && (!strcompare(Current->Area->Name, Config->GetArchiveArea())));

			ReplyAction->setEnabled(isNetMail(Current) || isEchoMail(Current));
			ReplyNetmailAction->setEnabled(isEchoMail(Current));
			ForwardAction->setEnabled(!strcompare(Current->Area->Name, Config->GetDraftArea()));
			DeleteAction->setEnabled(true);
			PrintAction->setEnabled(true);
			SaveAction->setEnabled(true);
			ArchiveAction->setEnabled((!strcompare(Current->Area->Name, Config->GetArchiveArea())) && !Config->GetArchiveArea().isEmpty());
			MarkMessAction->setEnabled(true);
			AddToBookAction->setEnabled(true);

			//TODO: Process CHRS
			//QString chrs = GetKludge(Current->CtlBuff, "CHRS: ");
			//QString chrs = GetKludge(Current->CtlBuff, "CHARSET: ");
			//if (!chrs.isEmpty())
			//{
			//	chrs.mid(6)
			//	ChrsStatusBar
			//}

			FNLabel->setText(CurrentItem->text(2));

			FALabel->setText(addr2str1(&Current->origaddr, Current->CtlBuff));

			TNLabel->setText(CurrentItem->text(3));
			if (isNetMail(Current))
				TALabel->setText(addr2str1(&Current->destaddr));
			SLabel->setText(CurrentItem->text(4));

			QDateTime dt;
			dt.setTime_t(Current->dt);
			DLabel->setText(dt.toString("hh:mm:ss dd MMM yyyy"));

			FLabel->setText(flags2str(Current->attr, GetKludge(Current->CtlBuff, "FLAGS ")));

			gc = GetKludge(Current->CtlBuff, "GC:");
			if (!gc.isEmpty())
			{
				DecodeGeek(Current->CtlBuff);
			} else {
				gc = GetKludge(Current->CtlBuff, "Geek-R");
				if (!gc.isEmpty())
					DecodeGeek(Current->CtlBuff);
			}

			QString Kludges = ViewKludgesAction->isOn() ? QStyleSheet::escape(Config->toUTF((char*)Current->CtlBuff)) : QString::null;
			QString tmp, Buffer = Config->toUTF((char*)Current->TxtBuff);
			ReplaceUnicodeChars(&Buffer);

			int i = Buffer.find("<html>", 0, false);
			int j = Buffer.find("</html>", i + 100, false);
			if ((i > -1) && (j > -1))
			{
				SetHtml(true);
				isHtml = !ViewKludgesAction->isOn();
				Buffer.truncate(j + 7);
				Buffer = Buffer.remove(0, i);
			}

			if (isHtml)
			{
				QRegExp chrx("charset=([-\\d\\w]{3,}|[\"][-\\d\\w]{3,}[\"])", false);
				int s = chrx.search(Buffer);
				if (s > -1)
				{
					charset = Buffer.mid(s + 8, chrx.matchedLength() - 8);
					charset.replace('"', QString::null);
					qDebug("Charset is %s", charset.ascii());
					//TODO:
					//
				}
			}

			if (!isHtml)
			{
				Buffer = QStyleSheet::escape(Buffer);
				ReplaceSmilies(&Buffer);
				QStringList BufferList = QStringList::split("\n", Buffer, true);
				Buffer = QString::null;
				int s = 0;

				for (i = BufferList.count() - 1; i >= 0;)
				{
					if (isLowerKludge(&BufferList[i]))
					{
						if (!ViewKludgesAction->isOn())
						{
							BufferList.remove(BufferList.at(i));
							continue;
						}
						if (ViewHighlightAction->isOn())
							BufferList[i].prepend("<ktag>").append("</ktag>");
					} else if (ViewHighlightAction->isOn())
					{
						if (BufferList[i].startsWith(" * Origin: "))
						{
							BufferList[i].prepend("<otag>").append("</otag>");
							s++;
						} else if (BufferList[i].startsWith("... "))
						{
							BufferList[i].prepend("<rtag>").append("</rtag>");
						} else if (BufferList[i].startsWith("--- "))
						{
							BufferList[i].prepend("<ltag>").append("</ltag>");
						} else if (!BufferList[i].isEmpty())
							break;
					} else if (!BufferList[i].isEmpty())
						break;
					i--;
				}

				for (i = 0; i < (int)BufferList.count(); i++)
				{
					if (i < ((int)BufferList.count() - s))
					{
						j = BufferList[i].find("&gt;");
						if ((j > -1) && (j < 5))
							BufferList[i].prepend("<qtag>").append("</qtag>");
						ParseColorLine(&BufferList[i], 0);
					}
					Buffer += ((Buffer == QString::null) ? QString::null : "<br>") + BufferList[i];
				}
				TextEdit->setText(convert2qhtm(Buffer.prepend(ViewKludgesAction->isOn() ? Kludges.prepend(ViewHighlightAction->isOn() ? "<ktag>" : QString::null).append(ViewHighlightAction->isOn() ? "</ktag><br>" : QString::null) : QString::null)));
			} else {
				ReplaceSmilies(&Buffer);
				TextEdit->setText(Buffer);
			}

			CurrentItem->StartTimer();
			MarkAllAsReadAction->setEnabled(Current->Area->UnReadCnt > 0);
			NextUnreadAction->setEnabled(Current->Area->UnReadCnt > 0);

			QStringList fmts = QImage::inputFormatList();
			if (Config->GetBool(QString::null, CONFIG_MEDIA_IMAGES, true))
			{
				// Find Photo in AddressBook
				TBookItem *BookItem = Config->AddressBook->FindUser(FALabel->text(), FNLabel->text());
				if (BookItem != NULL)
					if (testexists(BookItem->Photo))
						SetUserPixmap(QPixmap(BookItem->Photo));

				// Find Photo from "GIF"-kludge
				gif = GetKludge(Current->CtlBuff, "GIF: ");
				if (!gif.isEmpty())
				{
					Buffer = gif.mid(5).stripWhiteSpace();
					QFileInfo fi(Buffer);
					if (fi.extension().isEmpty())
					{
						tmp = QString::null;
						if (fmts.findIndex("GIF") > -1)
							tmp = FindMatchedFile(QDir(Config->MediaDir), Buffer + ".gif");
						if (fmts.findIndex("BMP") > -1)
							if (tmp.isEmpty())
								tmp = FindMatchedFile(QDir(Config->MediaDir), Buffer + ".bmp");
						if (fmts.findIndex("JPEG") > -1)
						{
							if (tmp.isEmpty())
								tmp = FindMatchedFile(QDir(Config->MediaDir), Buffer + ".jpg");
							if (tmp.isEmpty())
								tmp = FindMatchedFile(QDir(Config->MediaDir), Buffer + ".jpeg");
						}
					} else
						tmp = FindMatchedFile(QDir(Config->MediaDir), Buffer);
					Buffer = tmp;
					if (!Buffer.isEmpty())
					{
						Buffer.prepend(Config->MediaDir + QDir::separator());
						SetUserPixmap(QPixmap(Buffer));
					}
				}
			}

			// Find Photo from FIPS-style code
			QRegExp rx(REGEXP_FIPS_MM);
			i = rx.search(TextEdit->text());
			while (i > -1)
			{
				tmp = rx.cap(0).stripWhiteSpace();
				if (((tmp.find("1") == 4) || tmp.find("2") == 4) || (tmp.find("4") == 4))
				{
					if (tmp.find("W", 0, false) > -1)
					{
						if (Config->GetBool(QString::null, CONFIG_MEDIA_WAVES, true))
						{
							Buffer = FindMatchedFile(QDir(Config->MediaDir), tmp.mid(5).append(".wav"));
							if (!Buffer.isEmpty())
							{
								Buffer.prepend(Config->MediaDir + QDir::separator());
								sound_play(Buffer);
							}
						}
					} else if (Config->GetBool(QString::null, CONFIG_MEDIA_IMAGES, true) && (fmts.findIndex("BMP") > -1))
					{
						Buffer = FindMatchedFile(QDir(Config->MediaDir), tmp.mid(5).append(".bmp"));
						if (!Buffer.isEmpty())
						{
							Buffer.prepend(Config->MediaDir + QDir::separator());
							SetUserPixmap(QPixmap(Buffer));
						}
					}
				}
				i = rx.search(TextEdit->text(), i + tmp.length() + 1);
			}

			if (isNetMail(Current) && ((Current->attr & (FLAG_RRQ | FLAG_LOC)) == FLAG_RRQ))
			{
				if (extquestionmessage(this, QObject::tr("Message contain 'return receipt request'. Do You want to create this one?"), CONFIG_CONFIRM_RRQ))
					RunEditor(Current->Area, Current, MSG_OPERATION_CONFIRM, NULL);
				Current->attr &= ~FLAG_RRQ;
				Current->Area->Write(Current->AreaIndex);
			}
		} else
			errormessage(this, QObject::tr("Can`t Read Message."));
		Current->Area->Close();
	} else
		errormessage(this, QObject::tr("Can`t Open Area") + " " + Current->Area->Name + ".");
}

void TMainWindow::HelpIndex()
{
	if (HelpWindow == NULL)
		HelpWindow = new THelpWindow(this);
	if (!HelpWindow->isShown())
		HelpWindow->show();
}

void TMainWindow::ActionBugReport()
{
	LinkProcess(BUG_REPORT_URL, false);
}

void TMainWindow::LinkProcess(QString Link, bool InsertData)
{
	bool urldone = false;
	int i, i1, findex, tc;
	QString Tok = QString::null, Cmd = QString::null, Lnk = Link;
	QRegExp rx;

	rx.setPattern(REGEXP_EMAIL);
	if (rx.exactMatch(Lnk))
	{
		Cmd = Config->GetStr(QString::null, CONFIG_EXTEMAIL_CMD, QString::null);
		if (!Cmd.isEmpty())
		{
			QProcess proc(this);
			QFileInfo CmdExec(Cmd);
			proc.setWorkingDirectory(CmdExec.dir(true));
			proc.addArgument(CmdExec.fileName());
			Cmd = Config->GetStr(QString::null, CONFIG_EXTEMAIL_PRM, QString::null);
			tc = tokencount(Cmd, " ");
			for (i = 0; i < tc; i++)
			{
				Tok = gettoken(Cmd, i + 1, " ");
				if (Tok.find("%t") > -1)
				{
					Tok = Tok.replace("%t", Lnk);
					urldone = true;
				}
				if (InsertData)
				{
					if (Tok.find("%s") > -1)
						Tok = Tok.replace("%s", convert2extern(SLabel->text()));
					if (Tok.find("%B") > -1)
					{
						TMessage *Message = ((TMessageItem*)MessageList->selectedItem())->Message;
						QString Lines, Buffer = Config->toUTF((char*)Message->TxtBuff);
						QStringList BufferList = QStringList::split("\n", Buffer, true);
							findex = -1;
						i1 = BufferList.count() - 1;
						while (i1 >= 0)
							if (isLowerKludge(&BufferList[i1]))
								BufferList.remove(BufferList.at(i1));
							else
								if (!BufferList[i1].isEmpty())
									break;
								else
									i1--;
							Tok = Tok.replace("%B", /*convert2extern(*/BufferList.join("\n")/*)*/);
					}
				} else
				{
					if (Tok.find("%s") > -1)
						Tok = Tok.replace("%s", "");
					if (Tok.find("%B") > -1)
						Tok = Tok.replace("%B", "");
				}
				proc.addArgument(Tok);
			}

			debugmessage(QString("Run command: %1").arg(proc.arguments().join(" ")));
			if (urldone)
			{
				if (!proc.start())
				{
					statusBar()->message(QObject::tr("Error starting EMail client."), 1000);
					debugmessage("Run fail.");
				}
			} else
				informationmessage(this, QObject::tr("Incorrect settings for external EMail client."));
		} else
			informationmessage(this, QObject::tr("External EMail client not set."));
	} else {
		Cmd = Config->GetStr(QString::null, CONFIG_EXTBROWSER_CMD, QString::null);
		if (!Cmd.isEmpty())
		{
			QProcess proc(this);
			QFileInfo CmdExec(Cmd);
			proc.setWorkingDirectory(CmdExec.dir(true));
			proc.addArgument(CmdExec.fileName());
			Cmd = Config->GetStr(QString::null, CONFIG_EXTBROWSER_PRM, QString::null);
			tc = tokencount(Cmd, " ");
			if (Lnk.find("//", 0) < 2)
				Lnk = Lnk.prepend("http://");
			for (i = 0; i < tc; i++)
			{
				Tok = gettoken(Cmd, i + 1, " ");
				if (Tok.find("%u") > -1)
				{
					Tok = Tok.replace("%u", Lnk);
					urldone = true;
				}
				proc.addArgument(Tok);
			}
			debugmessage(QString("Run command: %1").arg(proc.arguments().join(" ")));
			if (!urldone)
				proc.addArgument(Lnk);
			if (!proc.start())
			{
				statusBar()->message(QObject::tr("Error starting HTML browser."), 1000);
				debugmessage("Run fail.");
			}
		} else
			informationmessage(this, QObject::tr("External browser not set."));
	}

}

void TMainWindow::TextEditLinkClicked(const QString &Link)
{
	LinkProcess(Link, true);
	TextEdit->setSource(QString::null);
}
