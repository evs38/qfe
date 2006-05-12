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

QFileIconProvider *imageIconProvider = NULL;

void TAddressBook::init()
{
	FirstTimeInit = false;

        setIcon(*Config->FindPixmap("icon_addressbook"));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnCancel->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));

	BtnAdd->setIconSet(QIconSet(*Config->FindPixmap("icon_additem")));
	BtnDel->setIconSet(QIconSet(*Config->FindPixmap("icon_delitem")));

	BtnPhoto->setIconSet(QIconSet(*Config->FindPixmap("icon_photo")));
	BtnErase->setIconSet(QIconSet(*Config->FindPixmap("icon_erase")));

	AddressList->setColumnText(0, QIconSet(*Config->FindPixmap("icon_name")), QObject::tr("User"));

	Config->GetPos(this);
	SelectMode = false;
	Selected = NULL;

	UserEdit->setValidator(new QRegExpValidator(QRegExp(REGEXP_FROMTO), this));
	EMailEdit->setValidator(new QRegExpValidator(QRegExp(REGEXP_EMAIL), this));
	AddressEdit->setValidator(new QRegExpValidator(QRegExp(REGEXP_ADDRESS), this));
}

void TAddressBook::SetUp(bool DoSelect)
{
	SelectMode = DoSelect;
}

void TAddressBook::showEvent(QShowEvent *)
{
	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	BtnOk->setEnabled(!SelectMode);
	for (uint32_t i = 0; i < Config->AddressBook->count(); i++)
	{
		TBookViewItem *it = new TBookViewItem(AddressList, Config->AddressBook->at(i));
		if (Config->AddressBook->at(i) == Selected)
			AddressList->setSelected(it, true);
	}

	BtnAdd->setEnabled(!SelectMode);
	UserEdit->setReadOnly(SelectMode);
	EMailEdit->setReadOnly(SelectMode);
	AddressEdit->setReadOnly(SelectMode);
	CommentEdit->setReadOnly(SelectMode);
}

void TAddressBook::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	e->accept();
}

void TAddressBook::ActionAdd()
{
	TBookItem *Item = Config->AddressBook->Add();
	Item->Name = "SysOp";
	Item->Comment = QObject::tr("Added") + " " + QDateTime::currentDateTime().toString();
	AddressList->setSelected(new TBookViewItem(AddressList, Item), true);
}

void TAddressBook::AddExternalAddr(QString Name, QString Address)
{
	Selected = Config->AddressBook->Add();
	Selected->Name = Name;
	Selected->Address = Address;
	Selected->Comment = QObject::tr("Added") + " " + QDateTime::currentDateTime().toString();
}

void TAddressBook::CleanData()
{
	BtnOk->setEnabled(!SelectMode);
	BtnPhoto->setEnabled(false);
	BtnErase->setEnabled(false);

	UserEdit->setEnabled(false);
	EMailEdit->setEnabled(false);
	AddressEdit->setEnabled(false);
	CommentEdit->setEnabled(false);

	UserEdit->setText(QString::null);
	EMailEdit->setText(QString::null);
	AddressEdit->setText(QString::null);
	CommentEdit->setText(QString::null);

	Selected = NULL;
}

void TAddressBook::ActionRemove()
{
	if (Selected == NULL)
		return;
	int idx = Config->AddressBook->Index(Selected);
	if (idx > -1)
	{
		CleanData();

		Config->AddressBook->Delete(idx);
		AddressList->removeItem(AddressList->selectedItem());
		BtnDel->setEnabled(Config->AddressBook->count() > 0);
		if (SelectMode)
			BtnOk->setEnabled(Config->AddressBook->count() > 0);
	} else
		AddressList->removeItem(AddressList->selectedItem());
}

void TAddressBook::ListSelectionChanged(QListViewItem *it)
{
	CleanData();

	if (it != NULL)
	{
		BtnOk->setEnabled(true);
		BtnDel->setEnabled(!SelectMode);
		BtnPhoto->setEnabled(!SelectMode);

		Selected = ((TBookViewItem*)it)->BookItem;

		UserEdit->setText(Selected->Name);
		EMailEdit->setText(Selected->EMail);
		AddressEdit->setText(Selected->Address);
		CommentEdit->setText(Selected->Comment);
		if (!Selected->Photo.isEmpty())
		{
			if (testexists(Selected->Photo))
			{
				SysOpPhoto->setPixmap(ScalePixmap(QPixmap(Selected->Photo), SYSOPIMAGE_WIDTH, SYSOPIMAGE_HEIGHT));
				BtnErase->setEnabled(!SelectMode);
			} else
				SysOpPhoto->clear();
		} else
			SysOpPhoto->clear();

		UserEdit->setEnabled(true);
		EMailEdit->setEnabled(true);
		AddressEdit->setEnabled(true);
		CommentEdit->setEnabled(true);
	}
}

void TAddressBook::UserChanged(const QString&)
{
	CommentChanged();
}

void TAddressBook::CommentChanged()
{
	if (UserEdit->isEnabled() && (Selected != NULL))
	{
		Selected->Name = UserEdit->text().stripWhiteSpace();
		Selected->EMail = EMailEdit->text().stripWhiteSpace();
		Selected->Address = AddressEdit->text().stripWhiteSpace();
		Selected->Comment = CommentEdit->text().stripWhiteSpace();

		int pos = 0;
		UserEdit->setPaletteForegroundColor(UserEdit->validator()->validate((QString&)Selected->Name, pos) == QValidator::Acceptable ? Qt::black : Qt::red);
		pos = 0;
		EMailEdit->setPaletteForegroundColor(EMailEdit->validator()->validate((QString&)Selected->EMail, pos) == QValidator::Acceptable ? Qt::black : Qt::red);
		pos = 0;
		AddressEdit->setPaletteForegroundColor(AddressEdit->validator()->validate((QString&)Selected->Address, pos) == QValidator::Acceptable ? Qt::black : Qt::red);

		AddressList->selectedItem()->repaint();
	}
}

void TAddressBook::ListDoubleClicked(QListViewItem*)
{
	if (SelectMode)
		accept();
}

void TAddressBook::BrowsePhoto()
{
	QString allfmts = QString::null;

	if (!QFileDialog::iconProvider())
	{
		if (imageIconProvider == NULL)
			imageIconProvider = new QFileIconProvider();
		QFileDialog::setIconProvider(imageIconProvider);
	}

	QFileDialog* fd = new QFileDialog(this);
	fd->setContentsPreviewEnabled(true);

	TPixmapView *pw = new TPixmapView(fd);
	fd->setContentsPreview(pw, pw);
	fd->setViewMode(QFileDialog::List);
	fd->setPreviewMode(QFileDialog::Contents);
	fd->setMode(QFileDialog::ExistingFile);
	fd->setInfoPreviewEnabled(true);

	fd->setDir(Config->GetStr(QString::null, CONFIG_LASTOPENSAVE, QString::null));
	fd->setFilter(QObject::tr("All Files (*)"));
	QStringList fmts = QImage::inputFormatList();
	for (uint32_t i = 0; i < fmts.count(); i++)
	{
		if (fmts[i] == "BMP")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.bmp");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.bmp)");
		} else if (fmts[i] == "GIF")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.gif");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.gif)");
		} else if (fmts[i] == "JPEG")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.jpg *.jpeg");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.jpg *jpeg)");
		} else if (fmts[i] == "MNG")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.mng");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.mng)");
		} else if (fmts[i] == "PBM")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.pbm");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.pbm)");
		} else if (fmts[i] == "PGM")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.pgm");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.pgm)");
		} else if (fmts[i] == "PNG")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.png");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.png)");
		} else if (fmts[i] == "PPM")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.ppm");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.ppm)");
		} else if (fmts[i] == "XBM")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.xbm");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.xbm)");
		} else if (fmts[i] == "XPM")
		{
			allfmts.append((allfmts.isEmpty() ? QString::null : " ") + "*.xpm");
			fd->addFilter(fmts[i] + " " + QObject::tr("Images") + "(*.xpm)");
		} else
			fprintf(stdout, "Unhandled image format %s. Please report to developers.\n", fmts[i].ascii());
	}
	fd->addFilter(QObject::tr("All Image Files") + " (" + allfmts + ")");
	fd->setCaption(QObject::tr("Select file..."));
	fd->setIcon(*Config->FindPixmap("icon_load"));
	if (fd->exec() == QDialog::Accepted)
	{
		Config->SetStr(QString::null, CONFIG_LASTOPENSAVE, fd->dir()->absPath());

		Selected = ((TBookViewItem*)AddressList->selectedItem())->BookItem;
		Selected->Photo = fd->selectedFile();
		SysOpPhoto->setPixmap(ScalePixmap(QPixmap(Selected->Photo), SYSOPIMAGE_WIDTH, SYSOPIMAGE_HEIGHT));
		BtnErase->setEnabled(true);
	}

	delete pw;
	delete fd;
}


void TAddressBook::BtnEraseClicked()
{
	Selected = ((TBookViewItem*)AddressList->selectedItem())->BookItem;
	Selected->Photo = QString::null;
	SysOpPhoto->clear();
	BtnErase->setEnabled(false);
}
