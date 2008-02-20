/***************************************************************************
 *   Copyright (C) 2005-2008 by Alexander Shiyan                           *
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

void TAttachForm::init()
{
	setIcon(*Config->FindPixmap("icon_attach"));

	FileButton->setIconSet(QIconSet(*Config->FindPixmap("icon_load")));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnCancel->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));

	Config->GetPos(this);
}

void TAttachForm::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	e->accept();
}

void TAttachForm::FileButtonClicked()
{
	QFileDialog* fd = new QFileDialog(this);
	fd->setDir(Config->GetStr(QString::null, CONFIG_LASTOPENSAVE, QString::null));
	fd->setFilter(QObject::tr("All Files (*)"));
	fd->setCaption(QObject::tr("Select file..."));
	fd->setIcon(*Config->FindPixmap("icon_load"));
	fd->setMode(QFileDialog::ExistingFile);
	fd->setInfoPreviewEnabled(true);
	if (fd->exec() == QDialog::Accepted)
	{
		Config->SetStr(QString::null, CONFIG_LASTOPENSAVE, fd->dir()->absPath());
		FileEdit->setText(fd->selectedFile());
		BtnOk->setEnabled(true);
	}
	delete fd;
}
