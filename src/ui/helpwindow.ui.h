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

void THelpWindow::init()
{
	FirstTimeInit = false;

        setIcon(*Config->FindPixmap("icon_help"));

	BtnHome->setIconSet(QIconSet(*Config->FindPixmap("icon_home")));
	BtnBack->setIconSet(QIconSet(*Config->FindPixmap("icon_left")));
	BtnForw->setIconSet(QIconSet(*Config->FindPixmap("icon_right")));
	BtnAbout->setIconSet(QIconSet(*Config->FindPixmap("icon_about")));

	Config->GetSize(this, size().width(), size().height());
	Config->GetPos(this);
}

void THelpWindow::showEvent(QShowEvent*)
{
	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	if (Config->IsDevelopMode())
	{
		qDebug("Develop Mode");
		QString startpage = FindMatchedFile(Config->HelpPath, QString("index_%1.html").arg(GET_CURRENT_LOCALE()));
		if (!startpage.isEmpty())
		{
			startpage = Config->HelpPath + QDir::separator() + startpage;

			//TODO:
			//
			qDebug("StartPage is %s", startpage.ascii());
			//
		} else
			HelpBrowser->setText("<font size=\"+1\"><p align=\"center\"><b>" + QObject::tr("Help files not found. Check configuration or reinstall program.") + "</b></p></font>");
	} else
		HelpBrowser->setText("<font size=\"+1\"><p align=\"center\"><b>This window is under construction yet.</b></p></font>");
}

void THelpWindow::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	Config->SetSize(this);
	e->accept();
}

void THelpWindow::AboutClicked()
{
	TAbout *About = new TAbout(this);
	About->exec();
	delete About;
}

void THelpWindow::HomeClicked()
{
	HelpBrowser->home();
}

void THelpWindow::BackClicked()
{
	HelpBrowser->backward();
}

void THelpWindow::ForwClicked()
{
	HelpBrowser->forward();
}

void THelpWindow::BrowserForwardAvailable(bool Value)
{
	BtnForw->setEnabled(Value);
}

void THelpWindow::BrowserBackwardAvailable(bool Value)
{
	BtnBack->setEnabled(Value);
}

void THelpWindow::HelpBrowserLinkClicked(const QString &Link)
{
	//TODO: Made link clicked, such as in TAbout
	//(TMainWindow*)(qApp->mainWidget())->LinkProcess(Link, false);
	Q_UNUSED(Link);
	//
}
