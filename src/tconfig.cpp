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

#include "tconfig.h"

#include "config.h"
#include "stdconfig.h"

#include <setjmp.h>

#define CONFIG_VERSION		"Version"

#define CONFIG_DRAFTAREA	"DraftArea"
#define CONFIG_ARCHIVEAREA	"ArchiveArea"

#define CONFIG_WINDOWPOSX	"%1PosX"
#define CONFIG_WINDOWPOSY	"%1PosY"
#define CONFIG_WINDOWWIDTH	"%1Width"
#define CONFIG_WINDOWHEIGHT	"%1Height"

#define CONFIG_LISTWIDTH	"%1Width%2"
#define CONFIG_LISTSORT		"%1Sort"
#define CONFIG_LISTORDER	"%1Order"

#define CONFIG_TAGCOLOR		"%1Color"
#define CONFIG_TAGSTYLE		"%1Style"

#define CONFIG_ACCEL		"Accel%1"

#define CONFIG_TOOLBAR_SHOW	"%1ToolBarView"
#define CONFIG_TOOLBAR_DOCK	"%1ToolBarDock"
#define CONFIG_TOOLBAR_ORIENT	"%1ToolBarOrient"
#define CONFIG_TOOLBAR_POSX	"%1ToolBarPosX"
#define CONFIG_TOOLBAR_POSY	"%1ToolBarPosY"

#define CONFIG_LIST_COUNTER	"%1Count"
#define CONFIG_LIST_ITEM	"%1Item%2"

#define QFE_INI_HOMEDIR		".qfe"

jmp_buf __env;
bool atexit_need = false;

TConfig::TConfig(QDir *Home, QDir *ManHome, QString UserHome)
{
	Inited = false;

	FidoConfLock = 0;
	FidoConfig = NULL;

	QDir CfgRoot = QDir::homeDirPath();
	if (UserHome.isEmpty())
		CfgRoot.mkdir(QFE_INI_HOMEDIR, false);
	else
		CfgRoot = QDir(UserHome);

	ConfigPath = CfgRoot.path() + QDir::separator() + QFE_INI_HOMEDIR + QDir::separator();

#if defined(ENABLE_DEBUG)
	/* Remove old debug entries in log */
	QFile::remove(ConfigPath + DEBUG_LOG_FILE);
#endif

	Settings = new TIniSettings(ConfigPath + "qfe.ini");

#if defined(ENABLE_DEBUG)
	/* Force debug in develop-mode */
	if (IsDevelopMode())
		dodebug = true;
#endif

	HomePath = Home->absPath();

	HelpPath = ManHome->absPath();

	ScreenRect = QDesktopWidget().screenGeometry(0);

	CheckTemplates(GetStr(QString::null, CONFIG_VERSION, QFE_VERSION));

	/* ******************** Config-file clean ******************** */
	if (CompareVersions("0.4.2-cvs", GetStr(QString::null, CONFIG_VERSION, QFE_VERSION)) > 0)
	{
		Settings->RemoveString(GetSection(QString::null), "HtmlBrowser");
		Settings->RemoveString(GetSection(QString::null), "EMailClient");
		debugmessage("Check external Html & Email program settings!");
	}

	SetStr(QString::null, CONFIG_VERSION, QFE_VERSION);

	TextCodec = QTextCodec::codecForName(GetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCODEPAGE, "IBM 866").latin1());
	FontDatabase = new QFontDatabase();
	AddressBook = new TBook(ConfigPath + ADDRESSBOOK_NAME);

	MediaDir = GetStr(QString::null, CONFIG_MEDIA_DIR, ConfigPath + "media");
}

TConfig::~TConfig()
{
	delete Settings;
	delete FontDatabase;
	delete AddressBook;
}

void TConfig::RestoreFile(QString Name)
{
	QByteArray data = qembed_findData(Name.ascii());

	if (data.size() > 0)
	{
		QFile f(ConfigPath + Name);

		if (f.exists())
		{
			QDir d(ConfigPath);
			QString AppSuff = QString::null, OldName = Name + ".old";
			uint32_t i = 1;
			while (d.exists(OldName + AppSuff, false))
			{
				AppSuff = QString(".%1").arg(i);
				i++;
			}
			OldName += AppSuff;
			d.rename(Name, OldName, false);
			fprintf(stdout, "File %s renamed to %s due update.\n", Name.ascii(), OldName.ascii());
		}

		if (FileOpenFunc(&f, IO_Raw | IO_WriteOnly))
		{
			f.writeBlock(data.data(), data.size());
			FileCloseFunc(&f);
		}
	}
}

void TConfig::FlushIni()
{
	Settings->Flush();
}

void TConfig::CheckTemplates(QString OldVer)
{
	if (!testexists(ConfigPath + TPL_MESSAGE_TPL))
		RestoreFile(TPL_MESSAGE_TPL);
	if (!testexists(ConfigPath + TPL_REPLY_TPL))
		RestoreFile(TPL_REPLY_TPL);
	if (!testexists(ConfigPath + TPL_FORWARD_TPL))
		RestoreFile(TPL_FORWARD_TPL);
	if (!testexists(ConfigPath + TPL_UNSUBSCRIBE_TPL))
		RestoreFile(TPL_UNSUBSCRIBE_TPL);
	if (!testexists(ConfigPath + TPL_CONFIRM_TPL))
		RestoreFile(TPL_CONFIRM_TPL);

	// CompareVersions will be removed in latest versions
	if (!testexists(ConfigPath + TPL_ARCHIVE_TPL) || (CompareVersions("0.4.6-cvs", OldVer) > 0))
		RestoreFile(TPL_ARCHIVE_TPL);

	if (!testexists(ConfigPath + TPL_SAVETXT_TPL))
		RestoreFile(TPL_SAVETXT_TPL);

	if (!testexists(ConfigPath + TPL_SAVEHTM_TPL))
		RestoreFile(TPL_SAVEHTM_TPL);

	if (!testexists(ConfigPath + TPL_CLIPBOARD_TPL))
		RestoreFile(TPL_CLIPBOARD_TPL);

	if (!testexists(ConfigPath + TPL_ORIGIN_LST))
		RestoreFile(TPL_ORIGIN_LST);
	if (!testexists(ConfigPath + TPL_KLUDGES_LST))
		RestoreFile(TPL_KLUDGES_LST);
}

void atexit_handler(void)
{
	if (atexit_need)
	{
		atexit_need = false;
		debugmessage("atexit_handler() execution...");
		longjmp(__env, EX_CONFIG);
	}
}

uint32_t TConfig::InitFido()
{
	uint32_t ret = ERROR_INITFIDO_OK;
//	QFile errlog;

	if (!Inited)
		debugmessage("FidoConfig initialization.");
	QString fcf = GetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCONFNAME, getConfigFileName());
	setvar("module", "qfe");

//	if (!FileOpenFunc(&errlog, IO_ReadOnly, stdout))
//		debugmessage("Could not open stdout.");
//	QTextStream errstream(&errlog);

	if (!Inited)
	{
		debugmessage("Trying to read FidoConfig" + (fcf.isEmpty() ? QString::null : QString(" from %1").arg(fcf)) + ".");
		atexit(&atexit_handler);
		if (setjmp(__env) != 0)
		{
//			if (errlog.isOpen())
//			{
//				if (ExtInfo != NULL) while (!errstream.atEnd())
//					ExtInfo->append(errstream.readLine() + "\n");
//				FileCloseFunc(&errlog);
//			}
			return ERROR_INITFIDO_CONFIG;
		}
	}

	atexit_need = true;
	FidoConfig = readConfig(fcf.isEmpty() ? NULL : fcf.ascii());
	atexit_need = false;

	if (FidoConfig != NULL)
	{
		if (FidoConfig->lockfile != NULL)
		{
			if (!Inited)
				debugmessage(QString("Creating lock-file %1.").arg((char*)FidoConfig->lockfile));
			FidoConfLock = LockFile_(FidoConfig->lockfile, FidoConfig->advisoryLock);
			if (FidoConfLock <= 0)
			{
				debugmessage("Can`t create lock-file.");
				disposeConfig(FidoConfig);
				FidoConfig = NULL;
				ret = ERROR_INITFIDO_LOCK;
			} else
				if (!Inited)
					debugmessage("Lock-file created.");
		}
	} else
		ret = ERROR_INITFIDO_CONFIG;

//	if (errlog.isOpen())
//		FileCloseFunc(&errlog);

	Inited = true;

	return ret;
}

void TConfig::DoneFido()
{
	if (FidoConfig != NULL)
	{
		if ((FidoConfig->lockfile != NULL) && (FidoConfLock != 0))
		{
			UnLockFile_(FidoConfig->lockfile, FidoConfLock);
			FidoConfLock = 0;
		}
		disposeConfig(FidoConfig);
		FidoConfig = NULL;
	}
}

QString TConfig::CharsetForMessages()
{
	QString tmp = GetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCODEPAGE, "IBM 866");
	for (uint32_t i = 0; i < CODEPAGES_COUNT; i++)
		if (strcompare(CharSetRecords[i].Name, tmp))
			return CharSetRecords[i].CHRS;

	return "IBMPC 2";
}

QPixmap *TConfig::FindPixmap(QString Name)
{
	static QPixmap p;
	QPixmap tmp;

	if (!Name.endsWith(".xpm"))
		Name = Name.append(".xpm");
	if (QPixmapCache::find(Name, p))
		if (!p.isNull())
			return &p;
	tmp = QPixmap::fromMimeSource(Name);
	if (!tmp.isNull())
	{
		if (!QPixmapCache::insert(Name, tmp))
			fprintf(stdout, "Fail to insert pixmap in cache. Please report to developers.\n");
		p = tmp;
		return &p;
	}

	fprintf(stdout, "Pixmap %s not found. Please report to developers.\n", Name.ascii());

	return NULL;
}

void TConfig::ChangeTextCodec(QString Value)
{
	if (Value != QString(TextCodec->name()))
		TextCodec = QTextCodec::codecForName(Value);
}

QString TConfig::toUTF(char *Value, QString CodecName)
{
	QString tmp = TextCodec->name();
	ChangeTextCodec(CodecName);
	QString ret = (Value == NULL) ? QString::null : toUTF(Value);
	ChangeTextCodec(tmp);
	return ret;
}

QString TConfig::toUTF(char *Value, uint32_t ValueSize)
{
	return (ValueSize == 0) ? TextCodec->toUnicode(Value) : TextCodec->toUnicode(Value, ValueSize);
}

QByteArray TConfig::fromUTF(QString Value)
{
	return TextCodec->fromUnicode(Value);
}

int TConfig::GetInt(QString Section, QString aKey, int Default)
{
	return Settings->GetString(GetSection(Section), aKey, QString::number(Default)).toInt();
}

bool TConfig::GetBool(QString Section, QString aKey, bool Default)
{
	return strcompare(Settings->GetString(GetSection(Section), aKey, Default ? "true" : "false"), "true");
}

QString TConfig::GetStr(QString Section, QString aKey, QString Default)
{
	return Settings->GetString(GetSection(Section), aKey, Default);
}

void TConfig::GetStrList(QString Section, QString ListName, QStringList *Value)
{
	int i, cnt = GetInt(Section, QString(CONFIG_LIST_COUNTER).arg(ListName), 0);
	QString tmp;

	Value->clear();
	for (i = 0; i < cnt; i++)
	{
		tmp = GetStr(Section, QString(CONFIG_LIST_ITEM).arg(ListName).arg(i + 1), QString::null).stripWhiteSpace();
		if (!tmp.isEmpty())
			Value->append(tmp);
	}
}

QString TConfig::GetDraftArea()
{
	ps_area Area = NULL;
	QString tmp = GetStr(QString::null, CONFIG_DRAFTAREA, QString::null);

	if (!tmp.isEmpty())
		Area = getArea(FidoConfig, (char*)tmp.ascii());

	return ((Area == NULL) ? QString::null : tmp);
}

QString TConfig::GetArchiveArea()
{
	ps_area Area = NULL;
	QString tmp = GetStr(QString::null, CONFIG_ARCHIVEAREA, QString::null);

	if (!tmp.isEmpty())
		Area = getArea(FidoConfig, (char*)tmp.ascii());

	return ((Area == NULL) ? QString::null : tmp);
}

void TConfig::GetPos(QWidget *Wnd)
{
	Wnd->move(GetInt(QString::null, QString(CONFIG_WINDOWPOSX).arg(Wnd->name()), Wnd->pos().x()), GetInt(QString::null, QString(CONFIG_WINDOWPOSY).arg(Wnd->name()), Wnd->pos().y()));
}

void TConfig::GetPos(QWidget *Wnd, int DefX, int DefY)
{
	Wnd->move(GetInt(QString::null, QString(CONFIG_WINDOWPOSX).arg(Wnd->name()), DefX), GetInt(QString::null, QString(CONFIG_WINDOWPOSY).arg(Wnd->name()), DefY));
}

void TConfig::GetSize(QWidget *Wnd, int DefW, int DefH)
{
	Wnd->resize(QMIN(ScreenRect.width(), GetInt(QString::null, QString(CONFIG_WINDOWWIDTH).arg(Wnd->name()), DefW)), QMIN(ScreenRect.height(), GetInt(QString::null, QString(CONFIG_WINDOWHEIGHT).arg(Wnd->name()), DefH)));
}

void TConfig::GetSplitter(QString Section, QSplitter *Value, const int *Default)
{
	int i;
	size_t j;
	QValueList<int> list = Value->sizes();

	for (j = 0; j < list.size(); j++)
	{
		i = GetInt(Section, QString("%1%2").arg(Value->name()).arg(j), Default ? *(Default + j) : -1);
		if (i == -1) return;
		QValueList<int>::Iterator it = list.at(j);
		*it = i;
	}
	Value->setSizes(list);
}

void TConfig::GetList(QString Section, QListView *Value, const int *Default)
{
	for (int i = 0; i < Value->columns(); i++)
		Value->setColumnWidth(i, QMAX(GetInt(Section, QString(CONFIG_LISTWIDTH).arg(Value->name()).arg(i), Default ? *(Default + i) : Value->columnWidth(i)), 0));
	Value->setSorting(QMAX(GetInt(Section, QString(CONFIG_LISTSORT).arg(Value->name()), 0), 0), GetBool(Section, QString(CONFIG_LISTORDER).arg(Value->name()), false));

}

QString TConfig::GetAccel(QString Section, QAction *Value, QString DefAccel)
{
	QString key = GetStr(Section, QString(CONFIG_ACCEL).arg(Value->name()), DefAccel);
	if (key == "None")
		key = QString::null;
	return key;
}

void TConfig::GetStyleSheetItem(QString Section, QStyleSheetItem *Value, QColor DefaultColor, int DefStyle)
{
	Value->setColor(QColor(GetInt(Section, QString(CONFIG_TAGCOLOR).arg(Value->name()), DefaultColor.rgb())));
	int style = GetInt(Section, QString(CONFIG_TAGSTYLE).arg(Value->name()), DefStyle);
	Value->setFontWeight((style & STYLE_BOLD) ? QFont::Bold : QFont::Normal);
	Value->setFontItalic(style & STYLE_ITALIC);
	Value->setFontUnderline(style & STYLE_UNDERLINE);
}

void TConfig::GetToolBar(QWidget *Wnd, QToolBar *Value)
{
	int x = GetInt(QString::null, QString(CONFIG_TOOLBAR_POSX).arg(Wnd->name()), -1);
	int y = GetInt(QString::null, QString(CONFIG_TOOLBAR_POSY).arg(Wnd->name()), -1);
	while (x > ScreenRect.width())
		x -= 10;
	while (y > ScreenRect.height())
		y -= 10;
	if ((x != -1) && (y != -1))
	{
		Value->undock();
		Value->move(x, y);
		Value->setOrientation(GetBool(QString::null, QString(CONFIG_TOOLBAR_ORIENT).arg(Wnd->name()), true) ? Qt::Horizontal : Qt::Vertical);
		if (GetBool(QString::null, QString(CONFIG_TOOLBAR_DOCK).arg(Wnd->name()), true))
			Value->dock();
	}
	Value->setShown(GetBool(QString::null, QString(CONFIG_TOOLBAR_SHOW).arg(Wnd->name()), true));
}

void TConfig::SetInt(QString Section, QString aKey, int Value)
{
	Settings->SetString(GetSection(Section), aKey, QString::number(Value));
}

void TConfig::SetBool(QString Section, QString aKey, bool Value)
{
	Settings->SetString(GetSection(Section), aKey, Value ? "true" : "false");
}

void TConfig::SetStr(QString Section, QString aKey, QString Value)
{
	Settings->SetString(GetSection(Section), aKey, Value);
}

void TConfig::SetStrList(QString Section, QString ListName, QStringList *Value)
{
	int i, cnt = Value->count();

	SetInt(Section, QString(CONFIG_LIST_COUNTER).arg(ListName), cnt);
	for (i = 0; i < cnt; i++)
		SetStr(Section, QString(CONFIG_LIST_ITEM).arg(ListName).arg(i + 1), *Value->at(i));
}

void TConfig::SetDraftArea(QString Value)
{
	SetStr(QString::null, CONFIG_DRAFTAREA, Value);
}

void TConfig::SetArchiveArea(QString Value)
{
	SetStr(QString::null, CONFIG_ARCHIVEAREA, Value);
}

void TConfig::SetPos(QWidget *Wnd)
{
	SetInt(QString::null, QString(CONFIG_WINDOWPOSX).arg(Wnd->name()), Wnd->pos().x());
	SetInt(QString::null, QString(CONFIG_WINDOWPOSY).arg(Wnd->name()), Wnd->pos().y());
}

void TConfig::SetSize(QWidget *Wnd)
{
	SetInt(QString::null, QString(CONFIG_WINDOWWIDTH).arg(Wnd->name()), Wnd->size().width());
	SetInt(QString::null, QString(CONFIG_WINDOWHEIGHT).arg(Wnd->name()), Wnd->size().height());
}

void TConfig::SetSplitter(QString Section, QSplitter *Value)
{
	int i = 0;

	QValueList<int> list = Value->sizes();
	for (QValueList<int>::Iterator it = list.begin(); it != list.end(); it++)
		SetInt(Section, QString("%1%2").arg(Value->name()).arg(i++), *it);
}

void TConfig::SetList(QString Section, QListView *Value)
{
	for (int i = 0; i < Value->columns(); i++)
		SetInt(Section, QString(CONFIG_LISTWIDTH).arg(Value->name()).arg(i), Value->columnWidth(i));
	SetInt(QString::null, QString(CONFIG_LISTSORT).arg(Value->name()), Value->sortColumn());
	SetBool(QString::null, QString(CONFIG_LISTORDER).arg(Value->name()), (Value->sortOrder() == Qt::Ascending) ? true : false);
}

void TConfig::SetAccel(QString Section, QAction *Value)
{
	QString key = Value->accel();
	SetStr(Section, QString(CONFIG_ACCEL).arg(Value->name()), key.isEmpty() ? "None" : key);
}

void TConfig::SetStyleSheetItem(QString Section, QStyleSheetItem *Value)
{
	SetInt(Section, QString(CONFIG_TAGCOLOR).arg(Value->name()), Value->color().rgb());
	int style = (Value->fontWeight() == QFont::Bold) ? STYLE_BOLD : 0;
	if (Value->fontItalic()) style |= STYLE_ITALIC;
	if (Value->fontUnderline()) style |= STYLE_UNDERLINE;
	SetInt(Section, QString(CONFIG_TAGSTYLE).arg(Value->name()), style);
}

void TConfig::SetToolBar(QWidget *Wnd, QToolBar *Value)
{
	SetBool(QString::null, QString(CONFIG_TOOLBAR_SHOW).arg(Wnd->name()), Value->isShown());
	SetBool(QString::null, QString(CONFIG_TOOLBAR_DOCK).arg(Wnd->name()), Value->place() == QDockWindow::InDock);
	SetBool(QString::null, QString(CONFIG_TOOLBAR_ORIENT).arg(Wnd->name()), Value->orientation() == Qt::Horizontal);
	SetInt(QString::null, QString(CONFIG_TOOLBAR_POSX).arg(Wnd->name()), Value->pos().x());
	SetInt(QString::null, QString(CONFIG_TOOLBAR_POSY).arg(Wnd->name()), Value->pos().y());
}

bool TConfig::IsDevelopMode()
{
#if defined(ENABLE_DEBUG)
	return GetBool(QString::null, "Develop", false);
#else
	return false;
#endif
}

QString TConfig::GetAddress(uint32_t idx)
{
	if (idx < FidoConfig->addrCount)
		return addr2str2(*(FidoConfig->addr + idx));

	return QString::null;
}
