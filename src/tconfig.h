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

#ifndef _TCONFIG_H_
#define _TCONFIG_H_

#include <qdir.h>
#include <qaction.h>
#include <qobject.h>
#include <qtoolbar.h>
#include <qlistview.h>
#include <qsplitter.h>
#include <qpopupmenu.h>
#include <qtextcodec.h>
#include <qstylesheet.h>
#include <qpixmapcache.h>
#include <qfontdatabase.h>
#include <qdesktopwidget.h>

#include "funchider.h"

class TConfig;

#include "utils.h"

#include "tarea.h"
#include "tbook.h"
#include "tfidoconfig.h"
#include "tinisettings.h"

#define CONFIG_SECTION_MAIN		"General"
#define CONFIG_SECTION_CONF		"Config"
#define CONFIG_SECTION_FIDO		"Fido"
#define CONFIG_SECTION_EDIT		"Editor"
#define CONFIG_SECTION_STYL		"StyleDlg"
#define CONFIG_SECTION_KEYB		"Keyboard"
#define CONFIG_SECTION_CONFIRM		"Confirm"

#define CONFIG_FIRSTRUN			"FirstRun"

#define CONFIG_WINDOWSTATE		"WindowState"

#define CONFIG_VIEWSTATUSBAR		"ViewStatusBar"
#define CONFIG_VIEWKLUDGES		"ViewKludges"
#define CONFIG_VIEWHIGHLIGHT		"ViewHighlight"
#define CONFIG_VIEWWORDWRAP		"ViewWordWrap"
#define CONFIG_VIEWORIGIN		"ViewOrigin"

#define CONFIG_FIDOCODEPAGE		"DefCodePage"
#define CONFIG_FIDODEFZONE		"DefZone"
#define CONFIG_FIDOCONFNAME		"ConfigName"

#define CONFIG_EXTBROWSER_CMD		"HtmlBrowserCommand"
#define CONFIG_EXTBROWSER_PRM		"HtmlBrowserParams"
#define CONFIG_EXTEMAIL_CMD		"EMailClientCommand"
#define CONFIG_EXTEMAIL_PRM		"EMailClientParams"

#define CONFIG_FONTNAME			"FontName"
#define CONFIG_FONTSIZE			"FontSize"
#define CONFIG_FONTCOLOR		"FontColor"
#define CONFIG_FONTBACK			"BackColor"
#define CONFIG_VIEWSTYLE		"HighlightStyle"

#define CONFIG_SEARCHAREA		"SearchArea"
#define CONFIG_SEARCHTEXT		"SearchText"
#define CONFIG_SEARCHCASE		"SearchCase"
#define CONFIG_SEARCHWHERE		"SearchWhere"
#define CONFIG_SEARCHREGEXP		"SearchRegExp"

#define CONFIG_REPLACELIST		"Replace"

#define CONFIG_SHOWKLUDGES		"ShowKldgesInEditor"

#define CONFIG_SMILIES_REPLACE		"ReplaceSmilies"

#define CONFIG_LASTAREA			"LastArea"
#define CONFIG_LASTOPENSAVE		"LastDir"

#define CONFIG_MEDIA_DIR		"MediaDir"
#define CONFIG_MEDIA_IMAGES		"MediaBmp"
#define CONFIG_MEDIA_WAVES		"MediaWav"

#define CONFIG_CONFIRM_DELETE		"DeleteMessage"
#define CONFIG_CONFIRM_QUIT		"Quit"
#define CONFIG_CONFIRM_QUITSAVE		"QuitSave"
#define CONFIG_CONFIRM_RRQ		"RRQ"

#define STYLE_BOLD			(1<<0)
#define STYLE_ITALIC			(1<<1)
#define STYLE_UNDERLINE			(1<<2)
#define STYLE_INVERSE			(1<<3)

#define MSG_OPERATION_NEW		(1<<0)
#define MSG_OPERATION_EDIT		(1<<1)
#define MSG_OPERATION_REPLY		(1<<2)
#define MSG_OPERATION_REPLYNET		(1<<3)
#define MSG_OPERATION_FORWARD		(1<<4)
#define MSG_OPERATION_UNSUBSCRIBE	(1<<5)
#define MSG_OPERATION_CONFIRM		(1<<6)

#define SYSOPIMAGE_WIDTH		110
#define SYSOPIMAGE_HEIGHT		110

#define TPL_MESSAGE_TPL			"message.tpl"
#define TPL_REPLY_TPL			"reply.tpl"
#define TPL_FORWARD_TPL			"forward.tpl"
#define TPL_UNSUBSCRIBE_TPL		"unsubscribe.tpl"
#define TPL_CONFIRM_TPL			"confirm.tpl"

#define TPL_ARCHIVE_TPL			"archive.tpl"

#define TPL_SAVETXT_TPL			"save_txt.tpl"
#define TPL_SAVEHTM_TPL			"save_htm.tpl"

#define TPL_CLIPBOARD_TPL		"clipboard.tpl"

#define TPL_ORIGIN_LST			"origin.lst"
#define TPL_KLUDGES_LST			"kludges.lst"

#define PROGRESS_TYPE_RESCAN		1
#define PROGRESS_TYPE_SEARCH		2
#define PROGRESS_TYPE_COMPILE		3
#define PROGRESS_TYPE_SAVENODES		4
#define PROGRESS_TYPE_LOADNODES		5

#define ADDRESSBOOK_NAME		"addressbook.xml"

#define DEBUG_LOG_FILE			"debug.log"

#define COMPILED_NODELIST_FILE		"nodelist.dat"

#define REGEXP_ADDRESS			"^[\\d]{1,4}:[\\d]{1,4}/[\\d]{1,4}(\\.[\\d]{1,4}|\\b)(@[.\\S\\w]{1,15}|\\b)"
#define REGEXP_FROMTO			"^[\\S].{1,36}$"
#define REGEXP_EMAIL			"\\b([-_./\\d\\w]|[\\d][:]){1,20}[@][-_\\d\\w]{1,10}(\\.[-_\\d\\w]{2,12}){1,5}"
#define REGEXP_URL			"((ftp|http|https)://|www\\.)([\\d\\S\\w]{2,}\\.)+[-/?&%_+=;:~a-zA-Z0-9]{2,}($|\\b)"

#define REGEXP_FIPS_MM			"FmM[BW](2|4|2[\\x20]).?[\\d]{7,7}"

#define REGEXP_UNICODE_CHRS		"&#[\\d]{2,5};"

#define MAIN_URL			"http://qfe.sourceforge.net/"
#define AUTHOR_EMAIL			"shc@users.sourceforge.net"
#define BUG_REPORT_URL			"http://sourceforge.net/tracker/?group_id=132811"

#define ERROR_INITFIDO_OK		0
#define ERROR_INITFIDO_CONFIG		1
#define ERROR_INITFIDO_LOCK		2

#if defined(Q_OS_MACX)
#	warning "Put true values in fields bellow!!!"
#	define PLATFORM_NAME		"MacOS"
#	define SHORT_PLATFORM_NAME	"Mac"
#	define DEFAULT_VIEW_FONT	"Monaco"
#	define DEFAULT_VIEW_FONTSIZE	12
#	define DEFAULT_EXT_BROWSER	"/Applications/Safari.app/Contents/MacOS/Safari"
#	define DEFAULT_EXT_BROWSER_PRM	""
#	define DEFAULT_EXT_EMAIL	"/Applications/Mail.app/Contents/MacOS/Mail"
#	define DEFAULT_EXT_EMAIL_PRM	""
#elif defined (Q_OS_WIN)
#	define PLATFORM_NAME		"Windows"
#	define SHORT_PLATFORM_NAME	"Win"
#	define DEFAULT_VIEW_FONT	"Lucida Console"
#	define DEFAULT_VIEW_FONTSIZE	10
#	define DEFAULT_EXT_BROWSER	"explorer.exe"
#	define DEFAULT_EXT_BROWSER_PRM	"%u"
#	define DEFAULT_EXT_EMAIL	"msimn.exe"
#	define DEFAULT_EXT_EMAIL_PRM	"/mailurl:mailto:%t?Subject=%s&Body=%B"
#elif defined(Q_OS_UNIX)
#	if defined(__x86_64__)
#		define PLATFORM_NAME	"Linux(64Bit)"
#		define SHORT_PLATFORM_NAME	"Lnx64"
#	else
#		define PLATFORM_NAME	"Linux"
#		define SHORT_PLATFORM_NAME	"Lnx"
#	endif
#	define DEFAULT_VIEW_FONT	"Console"
#	define DEFAULT_VIEW_FONTSIZE	12
#	define DEFAULT_EXT_BROWSER	"mozilla"
#	define DEFAULT_EXT_BROWSER_PRM	"%u"
#	define DEFAULT_EXT_EMAIL	"kmail"
#	define DEFAULT_EXT_EMAIL_PRM	"-s %s --body %B %t"
#else
#	error "Unable to determine target operating system!"
#endif

#if defined(Q_OS_WIN)
#	define FIXED_FILE_CASE(x)	x.lower()
#else
#	define FIXED_FILE_CASE(x)	x
#endif

#define QFE_LOGO_NAME	"qfe_logo.xpm"
#define QFE_LOGO	QPixmap::fromMimeSource(QFE_LOGO_NAME)

typedef struct _TProgressCallBack_ {
	void *InitProgress(uint32_t);
	void *ProcProgress(uint32_t);
	void *DoneProgress();
} TProgressCallBack;

#define GET_CURRENT_LOCALE()	QTextCodec::locale()

class TConfig
{
public:
	TConfig(QDir*, QDir*, QString = QString::null);
	~TConfig();

	ps_fidoconfig FidoConfig;

	QString ConfigPath;
	QString HomePath;
	QString HelpPath;
	QString MediaDir;

	QRect ScreenRect;
	QFontDatabase *FontDatabase;

	TBook *AddressBook;

	void FlushIni();

	uint32_t InitFido();
	void DoneFido();

	QString CharsetForMessages();

	QPixmap *FindPixmap(QString Name);

	void ChangeTextCodec(QString);
	QString toUTF(char*, QString);
	QString toUTF(char*, uint32_t = 0);
	QByteArray fromUTF(QString);

	int GetInt(QString, QString, int);
	bool GetBool(QString , QString, bool);
	QString GetStr(QString, QString, QString = QString::null);

	void GetStrList(QString, QString, QStringList*);

	QString GetDraftArea();
	QString GetArchiveArea();

	void GetPos(QWidget*);
	void GetPos(QWidget*, int, int);
	void GetSize(QWidget*, int, int);
	void GetSplitter(QString, QSplitter*, const int* = NULL);
	void GetList(QString, QListView*, const int* = NULL);
	QString GetAccel(QString, QAction*, QString);
	void GetStyleSheetItem(QString, QStyleSheetItem*, QColor, int = 0);
	void GetToolBar(QWidget*, QToolBar *Value);

	void SetInt(QString, QString, int);
	void SetBool(QString, QString, bool);
	void SetStr(QString, QString, QString);

	void SetStrList(QString, QString, QStringList*);

	void SetDraftArea(QString);
	void SetArchiveArea(QString);

	void SetPos(QWidget*);
	void SetSize(QWidget*);
	void SetSplitter(QString, QSplitter*);
	void SetList(QString, QListView*);
	void SetAccel(QString, QAction*);
	void SetStyleSheetItem(QString, QStyleSheetItem*);
	void SetToolBar(QWidget*, QToolBar *Value);

	bool IsDevelopMode();

	QString GetAddress(uint32_t);
	inline QString GetEchoTossLog()
	{
		return FidoConfig->echotosslog;
	}
	inline QString GetSysopName()
	{
		return FidoConfig->sysop;
	}

private:
	bool Inited;

	int FidoConfLock;

	TIniSettings *Settings;
	QTextCodec *TextCodec;

	void RestoreFile(QString);

	inline QString GetSection(QString Section)
	{
		return (Section.isEmpty() ? CONFIG_SECTION_MAIN : Section);
	}

	void CheckTemplates(QString);
};

#endif /* _TCONFIG_H_ */
