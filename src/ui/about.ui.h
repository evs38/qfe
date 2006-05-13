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

void TAbout::init()
{
        setIcon(*Config->FindPixmap("icon_about"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));

	ProgramIcon->setPixmap(QFE_LOGO);

	ProgramLabel->setText(QString("<b><font color=\"#0f007a\" size=\"+2\">QFE</font><br>%1</b>").arg(QObject::tr("Qt FTN Message Editor")));

	QString Txt = QString("QFE/%1 " + QObject::tr("Version") + ": %2<br>").arg(PLATFORM_NAME).arg(QFE_VERSION);

	if (QString(QFE_VERSION).find("svn") > -1)
		if (tokencount(QFE_REVISION) == 3)
			Txt.append(QString("Rev %1<br>").arg(gettoken(QFE_REVISION, 2)));

	Txt.append("<br>");
	Txt.append(QObject::tr("Written and distributed under the GNU General Public License") + "<br><br>");
	Txt.append("<nobr>" + QObject::tr("Written by") + " Alexander Shiyan</nobr><br><br>");
	Txt.append("<nobr><b>Web:</b> <a href=\"" + QString(MAIN_URL) + "\">" + QString(MAIN_URL) + "</a></nobr><br>");
	Txt.append("<nobr><b>EMail:</b> <a href=\"" + QString(AUTHOR_EMAIL) + "\">" + QString(AUTHOR_EMAIL) + "</a></nobr><br>");
	AboutText->setText(Txt.prepend("<center>").append("</center>"));

	Config->GetPos(this);
}

void TAbout::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	e->accept();
}

void TAbout::ActionClose()
{
	WidgetCloseFunc(this);
}

void TAbout::AboutTextClicked(const QString &Link)
{
	((TMainWindow*)qApp->mainWidget())->LinkProcess(Link, false);
	AboutText->setSource(QString::null);
}
