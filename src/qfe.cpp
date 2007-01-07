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

#include <qdir.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qfiledialog.h>
#include <qtranslator.h>
#include <qapplication.h>
#include <qsplashscreen.h>

#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
#	if !defined(ENABLE_BINRELOC)
#		error "Missed ENABLE_BINRELOC define!"
#	endif
#	if !defined(HOME_PATH)
#		define HOME_PATH	"/usr/local"
#	endif
#	include "binreloc.h"
#else
#	if defined(ENABLE_BINRELOC)
#		error "ENABLE_BINRELOC must not be defined for this platform!"
#	endif
#endif

#if defined(Q_OS_MACX)
#	include <CoreFoundation/CFURL.h>
#	include <CoreFoundation/CFBundle.h>
#endif

#if defined(Q_OS_UNIX) || defined(Q_OS_MACX)
#	include <signal.h>
#endif

#include "config.h"
#include "utils.h"
#include "tareas.h"
#include "tconfig.h"

#include "mainwindow.h"

#define PROGRAM_PATH_BASE	0
#define PROGRAM_PATH_MANUAL	1
#define PROGRAM_PATH_LOCALE	2
#define PROGRAM_PATH_EXTENSIONS	3

TConfig *Config = NULL;
TAreas *Areas = NULL;
QSplashScreen *Splash = NULL;

bool GlobalQfeError = false;
bool UseGUI = true;

QString StartArea = QString::null;
QTranslator *translator;

#if defined(Q_OS_WIN)
#	define QFEWindowIdent	"QFEWindowIdent"
#endif

QString Get_OS_Specific_Path(uint32_t PathType)
{
	bool NotInstalled = false;
	QString tmp;
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
	tmp = br_find_exe_dir(HOME_PATH"/bin");
	if (tmp.endsWith("/src"))
		NotInstalled = true;
	switch (PathType)
	{
		case PROGRAM_PATH_BASE:
			return tmp;
		case PROGRAM_PATH_MANUAL:
			if (NotInstalled)
				return tmp + "/../doc/manual";
			else
				return QString(br_find_data_dir(HOME_PATH"/share")) + "/qfe/manual";
		case PROGRAM_PATH_LOCALE:
			if (NotInstalled)
				return tmp + "/ts";
			else
				return QString(br_find_data_dir(HOME_PATH"/share")) + "/qfe/translations";
		default:
			break;
	}
#endif
#if defined(Q_OS_MACX)
	CFURLRef UrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFStringRef MacPath = CFURLCopyFileSystemPath(UrlRef, kCFURLPOSIXPathStyle);
	tmp = CFStringGetCStringPtr(MacPath, CFStringGetSystemEncoding());
	CFRelease(UrlRef);
	CFRelease(MacPath);
	if (tmp.contains("/src/") && tmp.endsWith(".app"))
		NotInstalled = true;
	switch (PathType)
	{
		case PROGRAM_PATH_BASE:
			return tmp;
		case PROGRAM_PATH_MANUAL:
			if (NotInstalled)
				return tmp + "/../../doc/manual";
			else
				return tmp + "/Contents/Resources/manual";
		case PROGRAM_PATH_LOCALE:
			if (NotInstalled)
				return tmp + "/../ts";
			else
				return tmp + "/Contents/Resources/translations";
		default:
			break;
	}
#endif
#if defined(Q_OS_WIN)
	tmp = qApp->applicationDirPath();
	if (tmp.endsWith("/src"))
		NotInstalled = true;
	switch (PathType)
	{
		case PROGRAM_PATH_BASE:
			return tmp;
		case PROGRAM_PATH_MANUAL:
			if (NotInstalled)
				return tmp + "\\..\\doc\\manual";
			else
				return tmp + "\\..\\share\\qfe\\manual";
		case PROGRAM_PATH_LOCALE:
			if (NotInstalled)
				return tmp + "\\ts";
			else
				return tmp + "\\..\\share\\qfe\\translations";
		default:
			break;
	}
#endif
	return QString::null;
}

void StartUpMessage(bool do_UseGUI, QString mess)
{
	if (do_UseGUI)
		informationmessage(NULL, mess);
	else
		fprintf(stdout, mess.append("\n").local8Bit());
}

void AppMsgOutput(QtMsgType type, const char *msg)
{
	switch (type)
	{
		case QtDebugMsg:
			if (debugmessage(QString("Debug: %1").arg(msg)))
				fprintf(stderr, "Debug: %s\n", msg);
			break;
		case QtWarningMsg:
			if (debugmessage(QString("Warning: %1").arg(msg)))
				fprintf(stderr, "Warning: %s\n", msg);
			break;
		case QtFatalMsg:
			if (debugmessage(QString("Fatal: %1").arg(msg)))
				fprintf(stderr, "Fatal: %s\n", msg);
			abort();
			break;
		default:
			break;
	}
}

int main(int argc, char ** argv)
{
	int32_t i, ret = 1, fres;
	bool ffound = false, showsplash = true, allowroot = false;
	QString UserHome = QString::null;
#if defined(Q_OS_WIN)
	HANDLE Mutex = 0;
#endif
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
	BrInitError BRerror = BR_INIT_ERROR_DISABLED;

	UseGUI = !QString(getenv("DISPLAY")).isEmpty();
	if (!br_init(&BRerror))
		StartUpMessage(UseGUI, QString("BinReloc failed to initialize. Error: %1").arg(BRerror));
#endif

	qInstallMsgHandler(AppMsgOutput);

	qApp = new QApplication(argc, argv, UseGUI);

	QDir tsdir = QDir(Get_OS_Specific_Path(PROGRAM_PATH_LOCALE));

	/* Language override */
	QString arg, tmp = QString::null;
	for (i = 1; i < qApp->argc(); i++)
	{
		arg = qApp->argv()[i];
		if ((arg == "-l") || (arg == "--lang"))
		{
			if (qApp->argc() > (i + 1))
				tmp = qApp->argv()[i + 1];
			break;
		}
	}
	if (tmp.isEmpty())
		tmp = GET_CURRENT_LOCALE();

	translator = new QTranslator(0);
	if (translator->load(tsdir.absPath().append(QDir::separator()).append("qfe_").append(tmp)))
		qApp->installTranslator(translator);
	else
		debugmessage(QString("Can`t load translator for locale \"%1\". Locale path is %2.").arg(QTextCodec::locale()).arg(tsdir.absPath()));

	QDir hlpdir = QDir(Get_OS_Specific_Path(PROGRAM_PATH_MANUAL));

	for (i = 1; i < qApp->argc(); i++)
	{
		arg = qApp->argv()[i];
		if ((arg == "-c") || (arg == "--configdir"))
		{
			if (qApp->argc() > (i + 1))
			{
				UserHome = qApp->argv()[i + 1];
				i++;
			} else {
				StartUpMessage(UseGUI, QObject::tr("Option '--configdir' require path as parameter."));
				return 1;
			}
		} else if ((arg == "-ns") || (arg == "--nosplash"))
		{
			showsplash = false;
		} else if ((arg == "-V") || (arg == "--version"))
		{
			StartUpMessage(UseGUI, "QFE - " + QObject::tr("Qt FTN Message Editor") + "\n" + QString(QObject::tr("Version") + ": %1").arg(QFE_VERSION));
			return 0;
		} else if ((arg == "-r") || (arg == "--root"))
		{
			allowroot = true;
		} else if ((arg == "-d") || (arg == "--debug"))
		{
			dodebug = true;
#if !defined(ENABLE_DEBUG)
			StartUpMessage(false, QObject::tr("Program compiled without debug messages."));
#endif
		} else if (arg.startsWith("-e", false))
		{
			tmp = arg.mid(2);
			if (!tmp.isEmpty())
				StartArea = tmp;
		} else if ((arg == "-h") || (arg == "--help"))
		{
			tmp = "QFE - " + QObject::tr("Qt FTN Message Editor") + "\n" + QString(QObject::tr("Version") + ": %1").arg(QFE_VERSION) + "\n\n";
			tmp.append(QObject::tr("Usage: %1 [options]").arg(argv[0]) + "\n");
			tmp.append(QObject::tr("Available options:") + "\n");
			tmp.append(QObject::tr(" -c,--configdir \tUse alternate directory with user settings.") + "\n");
			tmp.append(QObject::tr(" -ns,--nosplash \tDisable splash screen at startup.") + "\n");
			tmp.append(QObject::tr(" -l,--lang      \tOverride system language for interface.") + "\n");
			tmp.append(QObject::tr(" -V,--version   \tShow program version and exit.") + "\n");
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
			tmp.append(QObject::tr(" -r,--root      \tAllow run program by superuser.") + "\n");
#endif
			tmp.append(QObject::tr(" -eAREANAME     \tShow area AREANAME after start.") + "\n");
			tmp.append(QObject::tr(" -d,--debug     \tPring a lot of debug info into log file.") + "\n");
			tmp.append(QObject::tr(" -h,--help      \tThis help.") + "\n");
			StartUpMessage(UseGUI, tmp);
			return 0;
		} else if ((arg == "-l") || (arg == "--lang"))
		{
			if (qApp->argc() > (i + 1))
			{
				/* Dummy here... */
				i++;
			} else {
				StartUpMessage(UseGUI, QObject::tr("Option '--lang' require language code as parameter."));
				return 1;
			}
		} else
			StartUpMessage(false, QObject::tr("Incorrect option") + ": " + arg);
	}

	if (!UseGUI)
	{
		StartUpMessage(false, QObject::tr("Program must be run only in graphic mode."));
		return 1;
	}

	QPixmapCache::setCacheLimit(256);

	Config = new TConfig(&tsdir, &hlpdir, UserHome);

#if defined(ENABLE_DEBUG)
	QString debstart;
	debugmessage(debstart.fill('-', 79));
	debugmessage("QFE starting " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss") + ".");
#endif

#if !defined(Q_OS_WIN)
	pid_t qfepid;
	QFile PID(Config->ConfigPath + "qfe.pid");
	if (PID.exists())
	{
		if (FileOpenFunc(&PID, IO_ReadOnly))
		{
			if (PID.readBlock((char*)&qfepid, sizeof(qfepid)) == sizeof(qfepid))
				if (kill(qfepid, 0) == 0)
					ffound = true;
			FileCloseFunc(&PID);
		} else
			ffound = true;
	}
	if (!ffound)
		if (FileOpenFunc(&PID, IO_WriteOnly))
		{
			qfepid = getpid();
			PID.writeBlock((char*)&qfepid, sizeof(qfepid));
			FileCloseFunc(&PID);
		}
#else
	SetLastError(0);
	Mutex = CreateMutexA(NULL, true, QFEWindowIdent);
	if (Mutex == 0)
	{
		debugmessage("Can't create mutex.");
		return 1;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ReleaseMutex(Mutex);
		ffound = true;
	}
#endif
	if (ffound)
	{
		informationmessage(NULL, QObject::tr("QFE alredy running."));
		return 1;
	}

#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
	if ((getuid()) == 0 && !allowroot)
	{
		informationmessage(NULL, QObject::tr("This program must run as user."));
		return 1;
	}
#else
	Q_UNUSED(allowroot);
#endif

	if (Config->GetBool(CONFIG_SECTION_MAIN, CONFIG_FIRSTRUN, true))
	{
		informationmessage(NULL, QObject::tr("<p align=\"center\"><b>Welcome!</b></p><blockquote>This is first start of QFE - Qt fidonet editor.<br><b><font color=\"#ff0000\">Note:</font></b> All fidonet specific parameters must be set in HUSKY configuration file. This file, by default, is searched from <b>FIDOCONFIG</b> environment variable. Another way to set up different HUSKY configuration file for use with QFE - set up it manually by editing <b>~/.qfe/qfe.ini</b> file and write full path to config in parameter \"<b>ConfigName</b>\" in section \"<b>[Fido]</b>\".</blockquote>Have fun!"));
		Config->SetBool(CONFIG_SECTION_MAIN, CONFIG_FIRSTRUN, false);
		Config->FlushIni();
	}

	ffound = false;

	QFileDialog *fd = new QFileDialog(NULL);
	fd->setFilter(QObject::tr("All Files (*)"));
	fd->addFilter(QObject::tr("Configuration Files (*.cfg)"));
	fd->setSelectedFilter(0);
	fd->setCaption(QObject::tr("Select file..."));
	fd->setIcon(*Config->FindPixmap("icon_load"));
	fd->setMode(QFileDialog::ExistingFile);
	fd->setInfoPreviewEnabled(true);

	while (!ffound)
	{
		if (!testexists(Config->GetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCONFNAME, getConfigFileName())))
		{
			QString cval = getConfigFileName();
			debugmessage(QString("Husky config not found. ").append(cval.isEmpty() ? "No compiled in value." : QString("Compiled value is %1.").arg(cval)));
			if (questionmessage(NULL, QObject::tr("Husky config not found. Do You want select Config-file now?")))
			{
				if (fd->exec() == QDialog::Accepted)
				{
					Config->SetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCONFNAME, fd->selectedFile());
					Config->FlushIni();
				}
			} else
				break;
		} else {
			fres = Config->InitFido();
			if (fres == ERROR_INITFIDO_CONFIG)
			{
				Config->DoneFido();
				if (questionmessage(NULL, QObject::tr("Can't open config. Do You want select Config-file now?")))
				{
					if (fd->exec() == QDialog::Accepted)
					{
						Config->SetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCONFNAME, fd->selectedFile());
						Config->FlushIni();
					}
				} else
					break;
			} else if (fres == ERROR_INITFIDO_LOCK)
			{
				Config->DoneFido();
				errormessage(NULL, QObject::tr("Can't create lock file.") + " " + QObject::tr("Program terminated."));
				break;
			} else
				ffound = true;
		}
	}
	delete fd;

	if (ffound)
	{
		if ((Config->ScreenRect.width() < 800) || (Config->ScreenRect.height() < 600))
		{
			errormessage(NULL, QObject::tr("Program work with sceeen size not lower 800x600."));
		} else {
			if (showsplash)
			{
				Splash = new QSplashScreen(ScalePixmap(QFE_LOGO, (int32_t)((double)QFE_LOGO.width() * 1.5), (int32_t)((double)QFE_LOGO.height() * 1.5)));
				Splash->show();
				Splash->message(QObject::tr("Initializing..."), Qt::AlignLeft, Qt::white);
				QTimer::singleShot(1650, Splash, SLOT(hide()));
			}

			qApp->setMainWidget(new TMainWindow());
			qApp->mainWidget()->setCaption(QObject::tr("Qt FTN Message Editor"));
			qApp->connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));

			qApp->mainWidget()->show();

			ret = qApp->exec();

			if (Splash != NULL)
				delete Splash;

			delete qApp->mainWidget();
		}
		Config->DoneFido();
	}

	delete Config;
	Config = NULL;

	delete translator;

#if !defined(Q_OS_WIN)
	PID.remove();
#else
	CloseHandle(Mutex);
#endif
	return ret;
}
