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

extern TConfig *Config;

void TOptions::init()
{
	FirstTimeInit = false;

	setIcon(*Config->FindPixmap("icon_options"));

	OptionsTab->setTabIconSet(OptionsTab->page(0), QIconSet(*Config->FindPixmap("icon_general")));
	OptionsTab->setTabIconSet(OptionsTab->page(1), QIconSet(*Config->FindPixmap("icon_view")));
	OptionsTab->setTabIconSet(OptionsTab->page(2), QIconSet(*Config->FindPixmap("icon_edit")));
	OptionsTab->setTabIconSet(OptionsTab->page(3), QIconSet(*Config->FindPixmap("icon_keyboard")));
	OptionsTab->setTabIconSet(OptionsTab->page(4), QIconSet(*Config->FindPixmap("icon_template")));
	OptionsTab->setTabIconSet(OptionsTab->page(5), QIconSet(*Config->FindPixmap("icon_multimedia")));
	OptionsTab->setTabIconSet(OptionsTab->page(6), QIconSet(*Config->FindPixmap("icon_question")));

	BtnBrowseFidoconf->setIconSet(QIconSet(*Config->FindPixmap("icon_load")));
	BtnBrowseHtml->setIconSet(QIconSet(*Config->FindPixmap("icon_load")));
	BtnBrowseEMail->setIconSet(QIconSet(*Config->FindPixmap("icon_load")));
	BtnBrowseMM->setIconSet(QIconSet(*Config->FindPixmap("icon_load")));

	TagsTab->setTabIconSet(TagsTab->page(0), QIconSet(*Config->FindPixmap("icon_strings")));
	TagsTab->setTabIconSet(TagsTab->page(1), QIconSet(*Config->FindPixmap("icon_golded")));
	TagsTab->setTabIconSet(TagsTab->page(2), QIconSet(*Config->FindPixmap("icon_fips")));

	BtnViewForegroundColor->setIconSet(QIconSet(*Config->FindPixmap("icon_color")));
	BtnViewBackgroundColor->setIconSet(QIconSet(*Config->FindPixmap("icon_color")));

	BtnEditForegroundColor->setIconSet(QIconSet(*Config->FindPixmap("icon_color")));
	BtnEditBackgroundColor->setIconSet(QIconSet(*Config->FindPixmap("icon_color")));

	BtnEraseKey->setIconSet(QIconSet(*Config->FindPixmap("icon_erase")));

	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnCancel->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));

	BtnAdd->setIconSet(QIconSet(*Config->FindPixmap("icon_additem")));
	BtnDel->setIconSet(QIconSet(*Config->FindPixmap("icon_delitem")));

	BtnReload->setIconSet(QIconSet(*Config->FindPixmap("icon_rescan")));
	BtnSave->setIconSet(QIconSet(*Config->FindPixmap("icon_save")));

	ShortcutList->setSortColumn(-1);

	Config->GetSize(this, size().width(), size().height());
	Config->GetPos(this);

	KeyEdit->installEventFilter(this);
}

void TOptions::destroy()
{
	delete EditWindowCopy;
}

void TOptions::showEvent(QShowEvent *)
{
	int i;
	size_t i1, i2;
	QString tmp, tmp1;
	TActionList *List;
	TAction *Action;
	QListBoxItem *it;

	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	FidoconfPathEdit->setText(Config->GetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCONFNAME, getConfigFileName()));

	for (i = 0; i < CODEPAGES_COUNT; i++)
		CodepageCombo->insertItem(CharSetRecords[i].Name);
	CodepageCombo->setCurrentItem(CodepageCombo->listBox()->index(CodepageCombo->listBox()->findItem(Config->GetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCODEPAGE, "IBM 866"), Qt::ExactMatch)));

	ZoneBox->setValue(Config->GetInt(CONFIG_SECTION_FIDO, CONFIG_FIDODEFZONE, 2));

	DraftCombo->insertItem("");
	for (i1 = 0; i1 < Config->FidoConfig->localAreaCount; i1++)
		DraftCombo->insertItem(Config->FidoConfig->localAreas[i1].areaName);
	tmp = Config->GetDraftArea();
	it = DraftCombo->listBox()->findItem(tmp, Qt::ExactMatch);
	if ((it == NULL) || tmp.isEmpty())
		it = DraftCombo->listBox()->item(0);
	DraftCombo->setCurrentItem(DraftCombo->listBox()->index(it));

	ArchiveCombo->insertItem("");
	for (i1 = 0; i1 < Config->FidoConfig->localAreaCount; i1++)
		ArchiveCombo->insertItem(Config->FidoConfig->localAreas[i1].areaName);
	tmp = Config->GetArchiveArea();
	it = ArchiveCombo->listBox()->findItem(tmp, Qt::ExactMatch);
	if ((it == NULL) || tmp.isEmpty())
		it = ArchiveCombo->listBox()->item(0);
	ArchiveCombo->setCurrentItem(ArchiveCombo->listBox()->index(it));

	HtmlPathEdit->setText(Config->GetStr(QString::null, CONFIG_EXTBROWSER_CMD, DEFAULT_EXT_BROWSER));
	HtmlParamsEdit->setText(Config->GetStr(QString::null, CONFIG_EXTBROWSER_PRM, DEFAULT_EXT_BROWSER_PRM));

	EMailPathEdit->setText(Config->GetStr(QString::null, CONFIG_EXTEMAIL_CMD, DEFAULT_EXT_EMAIL));
	EMailParamsEdit->setText(Config->GetStr(QString::null, CONFIG_EXTEMAIL_PRM, DEFAULT_EXT_EMAIL_PRM));

	MMPathEdit->setText(Config->MediaDir);
	MMImageCheck->setChecked(Config->GetBool(QString::null, CONFIG_MEDIA_IMAGES, true));
	MMSoundsCheck->setChecked(Config->GetBool(QString::null, CONFIG_MEDIA_WAVES, true));

	ViewFontNameCombo->insertStringList(Config->FontDatabase->families(QFont::Cyrillic));
	ViewFontNameCombo->setCurrentItem(ViewFontNameCombo->listBox()->index(ViewFontNameCombo->listBox()->findItem(Config->GetStr(QString::null, CONFIG_FONTNAME, DEFAULT_VIEW_FONT), Qt::ExactMatch)));
	ViewFontSizeBox->setValue(Config->GetInt(QString::null, CONFIG_FONTSIZE, DEFAULT_VIEW_FONTSIZE));
	FColor = QColor(Config->GetInt(QString::null, CONFIG_FONTCOLOR, QColor(224, 224, 224).rgb()));
	BColor = QColor(Config->GetInt(QString::null, CONFIG_FONTBACK, QColor(0, 0, 0).rgb()));
	if (Config->GetBool(QString::null, CONFIG_VIEWSTYLE, true))
		GoldedCheck->setChecked(true);
	else
		FipsCheck->setChecked(true);
	TMainWindow *Wnd = (TMainWindow*)qApp->mainWidget();
	ViewFontChanged(QString::null);
	for (i1 = 1; i1 < 16; i1++)
		new TTagItem(GoldedTable, i1, QObject::tr("Macro") + ": " + ((i1 & 8) ? "#" : QString::null) + ((i1 & 4) ? "_" : QString::null) + ((i1 & 2) ? "/" : QString::null) + ((i1 & 1) ? "*" : QString::null), &ViewFont, &BColor, Wnd->GoldedColorTextItem[i1 - 1]);

	new TTagItem(FipsTable, 1, QObject::tr("Macro") + ": *", &ViewFont, &BColor, Wnd->FipsColorTextItem[0]);
	new TTagItem(FipsTable, 2, QObject::tr("Macro") + ": _", &ViewFont, &BColor, Wnd->FipsColorTextItem[1]);
	new TTagItem(FipsTable, 3, QObject::tr("Macro") + ": //", &ViewFont, &BColor, Wnd->FipsColorTextItem[2]);

	new TTagItem(StringsTable, 1, QObject::tr("Kludges"), &ViewFont, &BColor, Wnd->KludgeItem);
	new TTagItem(StringsTable, 2, QObject::tr("Origin"), &ViewFont, &BColor, Wnd->OriginItem);
	new TTagItem(StringsTable, 3, QObject::tr("Tagline"), &ViewFont, &BColor, Wnd->TaglineItem);
	new TTagItem(StringsTable, 4, QObject::tr("TearLine"), &ViewFont, &BColor, Wnd->TearlineItem);
	new TTagItem(StringsTable, 5, QObject::tr("Quote"), &ViewFont, &BColor, Wnd->QuoteItem);
	new TTagItem(StringsTable, 6, QObject::tr("Link"), &ViewFont, &BColor, Wnd->UrlItem);

	SmilesReplace->setChecked(Config->GetBool(CONFIG_SECTION_EDIT, CONFIG_SMILIES_REPLACE, false));

	EditFontNameCombo->insertStringList(Config->FontDatabase->families(QFont::Cyrillic));
	EditFontNameCombo->setCurrentItem(EditFontNameCombo->listBox()->index(EditFontNameCombo->listBox()->findItem(Config->GetStr(CONFIG_SECTION_EDIT, CONFIG_FONTNAME, DEFAULT_VIEW_FONT), Qt::ExactMatch)));
	EditFontSizeBox->setValue(Config->GetInt(CONFIG_SECTION_EDIT, CONFIG_FONTSIZE, DEFAULT_VIEW_FONTSIZE));
	FEColor = QColor(Config->GetInt(CONFIG_SECTION_EDIT, CONFIG_FONTCOLOR, QColor(224, 224, 224).rgb()));
	BEColor = QColor(Config->GetInt(CONFIG_SECTION_EDIT, CONFIG_FONTBACK, QColor(0, 0, 0).rgb()));

	QListViewItem *WndGroup, *KbdGroup = NULL;
	TShortCutItem *KbdItem;

	WndGroup = new QListViewItem(ShortcutList, QObject::tr("Main Window"));
	WndGroup->setPixmap(0, QIconSet(*Config->FindPixmap("qfe"), QIconSet::Large).pixmap(QIconSet::Small, QIconSet::Normal));
	WndGroup->setOpen(true);
	WndGroup->setSelectable(false);
	for (i1 = 0; i1 < Wnd->MainActions->count(); i1++)
	{
		List = Wnd->MainActions->at(i1);
		KbdGroup = new QListViewItem(WndGroup, KbdGroup, QObject::tr("Menu") + ": " + List->Group);
		KbdGroup->setPixmap(0, *Config->FindPixmap("icon_menu"));
		KbdGroup->setOpen(true);
		KbdGroup->setSelectable(false);
		KbdItem = NULL;
		for (i2 = 0; i2 < List->count(); i2++)
		{
			Action = List->at(i2);
			KbdItem = new TShortCutItem(KbdGroup, KbdItem, Action);
		}
	}

	EditWindowCopy = new TEditWindow(this);
	WndGroup = new QListViewItem(ShortcutList, WndGroup, QObject::tr("Edit Window"));
	WndGroup->setPixmap(0, *Config->FindPixmap("icon_edit"));
	WndGroup->setOpen(true);
	WndGroup->setSelectable(false);
	for (i1 = 0; i1 < EditWindowCopy->EditActions->count(); i1++)
	{
		List = EditWindowCopy->EditActions->at(i1);
		KbdGroup = new QListViewItem(WndGroup, KbdGroup, QObject::tr("Menu") + ": " + List->Group);
		KbdGroup->setPixmap(0, *Config->FindPixmap("icon_menu"));
		KbdGroup->setOpen(true);
		KbdGroup->setSelectable(false);
		KbdItem = NULL;
		for (i2 = 0; i2 < List->count(); i2++)
		{
			Action = List->at(i2);
			KbdItem = new TShortCutItem(KbdGroup, KbdItem, Action);
		}
	}

	QStringList Replace;
	Config->GetStrList(CONFIG_SECTION_EDIT, CONFIG_REPLACELIST, &Replace);
	for (i1 = 0; i1 < Replace.count(); i1++)
		if (tokencount(Replace[i1], "\t") == 2)
			new QListViewItem(ReplaceList, gettoken(Replace[i1], 1, "\t"), gettoken(Replace[i1], 2, "\t"));

	KludgesInEdit->setChecked(Config->GetBool(CONFIG_SECTION_EDIT, CONFIG_SHOWKLUDGES, false));

	TemplateCombo->insertItem(QObject::tr("New message"));
	TemplateCombo->insertItem(QObject::tr("Reply to message"));
	TemplateCombo->insertItem(QObject::tr("Forward message"));
	TemplateCombo->insertItem(QObject::tr("Unsubscribe message"));
	TemplateCombo->insertItem(QObject::tr("Copy message to archive area"));
	TemplateCombo->insertItem(QObject::tr("Save message as TXT"));
	TemplateCombo->insertItem(QObject::tr("Save message as HTML"));
	TemplateCombo->insertItem(QObject::tr("Origin lines"));
	TemplateCombo->insertItem(QObject::tr("User kludges"));

	DelMessConfirm->setChecked(Config->GetBool(CONFIG_SECTION_CONFIRM, CONFIG_CONFIRM_DELETE, true));
	QuitSaveConfirm->setChecked(Config->GetBool(CONFIG_SECTION_CONFIRM, CONFIG_CONFIRM_QUITSAVE, true));
	QuitConfirm->setChecked(Config->GetBool(CONFIG_SECTION_CONFIRM, CONFIG_CONFIRM_QUIT, true));
	RRQConfirm->setChecked(Config->GetBool(CONFIG_SECTION_CONFIRM, CONFIG_CONFIRM_RRQ, true));
}

void TOptions::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	Config->SetSize(this);
	e->accept();
}

void TOptions::BtnOkAction()
{
	QStringList Replace;
	QListViewItemIterator it;

	Config->SetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCONFNAME, ((FidoconfPathEdit->text() == QString(getConfigFileName())) ? QString::null : FidoconfPathEdit->text()));
	Config->SetStr(CONFIG_SECTION_FIDO, CONFIG_FIDOCODEPAGE, CodepageCombo->currentText());
	Config->SetInt(CONFIG_SECTION_FIDO, CONFIG_FIDODEFZONE, ZoneBox->value());
	Config->SetStr(CONFIG_SECTION_EDIT, CONFIG_FONTNAME, EditFontNameCombo->currentText());

	Config->SetStr(QString::null, CONFIG_EXTBROWSER_CMD, HtmlPathEdit->text());
	Config->SetStr(QString::null, CONFIG_EXTBROWSER_PRM, HtmlParamsEdit->text());
	Config->SetStr(QString::null, CONFIG_EXTEMAIL_CMD, EMailPathEdit->text());
	Config->SetStr(QString::null, CONFIG_EXTEMAIL_PRM, EMailParamsEdit->text());

	Config->MediaDir = MMPathEdit->text();
	Config->SetStr(QString::null, CONFIG_MEDIA_DIR, MMPathEdit->text());
	Config->SetBool(QString::null, CONFIG_MEDIA_IMAGES, MMImageCheck->isChecked());
	Config->SetBool(QString::null, CONFIG_MEDIA_WAVES, MMSoundsCheck->isChecked());

	Config->SetStr(QString::null, CONFIG_FONTNAME, ViewFontNameCombo->currentText());
	Config->SetInt(QString::null, CONFIG_FONTSIZE, ViewFontSizeBox->value());
	Config->SetInt(QString::null, CONFIG_FONTCOLOR, FColor.rgb());
	Config->SetInt(QString::null, CONFIG_FONTBACK, BColor.rgb());
	Config->SetBool(QString::null, CONFIG_VIEWSTYLE, GoldedCheck->isOn());

	Config->SetDraftArea(DraftCombo->currentText());

	Config->SetArchiveArea(ArchiveCombo->currentText());

	Config->SetInt(CONFIG_SECTION_EDIT, CONFIG_FONTSIZE, EditFontSizeBox->value());
	Config->SetInt(CONFIG_SECTION_EDIT, CONFIG_FONTCOLOR, FEColor.rgb());
	Config->SetInt(CONFIG_SECTION_EDIT, CONFIG_FONTBACK, BEColor.rgb());

	it = QListViewItemIterator(StringsTable);
	while (it.current())
	{
		Config->SetStyleSheetItem(QString::null, ((TTagItem*)*it)->Style);
		++it;
	}
	it = QListViewItemIterator(GoldedTable);
	while (it.current())
	{
		Config->SetStyleSheetItem(QString::null, ((TTagItem*)*it)->Style);
		++it;
	}
	it = QListViewItemIterator(FipsTable);
	while (it.current())
	{
		Config->SetStyleSheetItem(QString::null, ((TTagItem*)*it)->Style);
		++it;
	}

	Config->SetBool(CONFIG_SECTION_EDIT, CONFIG_SMILIES_REPLACE, SmilesReplace->isChecked());

	it = QListViewItemIterator(ShortcutList);
	while (it.current())
	{
		QListViewItem *item = it.current();
		if (item->isSelectable())
			((TShortCutItem*)item)->Apply();
		++it;
	}

	((TMainWindow*)qApp->mainWidget())->MainActions->Save();
	EditWindowCopy->EditActions->Save();

	it = QListViewItemIterator(ReplaceList);
	while (it.current())
	{
		QListViewItem *item = it.current();
		if (!item->text(0).stripWhiteSpace().isEmpty() && !item->text(1).stripWhiteSpace().isEmpty())
			Replace.append(QString("%1\t%2").arg(item->text(0).stripWhiteSpace()).arg(item->text(1).stripWhiteSpace()));
		++it;
	}
	Config->SetStrList(CONFIG_SECTION_EDIT, CONFIG_REPLACELIST, &Replace);

	Config->SetBool(CONFIG_SECTION_EDIT, CONFIG_SHOWKLUDGES, KludgesInEdit->isChecked());

	Config->SetBool(CONFIG_SECTION_CONFIRM, CONFIG_CONFIRM_DELETE, DelMessConfirm->isChecked());
	Config->SetBool(CONFIG_SECTION_CONFIRM, CONFIG_CONFIRM_QUITSAVE, QuitSaveConfirm->isChecked());
	Config->SetBool(CONFIG_SECTION_CONFIRM, CONFIG_CONFIRM_QUIT, QuitConfirm->isChecked());
	Config->SetBool(CONFIG_SECTION_CONFIRM, CONFIG_CONFIRM_RRQ, RRQConfirm->isChecked());

	Config->FlushIni();

	accept();
}

void TOptions::BrowseFidoconfAction()
{
	QFileDialog* fd = new QFileDialog(this);
	fd->setDir(FidoconfPathEdit->text());
	fd->setFilter(QObject::tr("All Files (*)"));
	fd->setCaption(QObject::tr("Select file..."));
	fd->setIcon(*Config->FindPixmap("icon_load"));
	fd->setMode(QFileDialog::ExistingFile);
	fd->setInfoPreviewEnabled(true);
	if (fd->exec() == QDialog::Accepted)
		FidoconfPathEdit->setText(fd->selectedFile());
	delete fd;
}

void TOptions::FidoconfPathEditChanged(const QString &s)
{
	FidoconfPathEdit->setPaletteForegroundColor(testexists(s) ? Qt::black : Qt::red);
}

void TOptions::ViewFontChanged(const QString &)
{
	QListViewItemIterator it;

	ViewFont = QFont(ViewFontNameCombo->currentText(), ViewFontSizeBox->value());
	if (isShown())
	{
		it = QListViewItemIterator(StringsTable);
		while (it.current())
		{
			((TTagItem*)*it)->SetFont(&ViewFont);
			++it;
		}
		it = QListViewItemIterator(GoldedTable);
		while (it.current())
		{
			((TTagItem*)*it)->SetFont(&ViewFont);
			++it;
		}
		it = QListViewItemIterator(FipsTable);
		while (it.current())
		{
			((TTagItem*)*it)->SetFont(&ViewFont);
			++it;
		}
	}
}

void TOptions::BtnViewForegroundColorAction()
{
	QColor _tmpcolor = QColorDialog::getColor(FColor, this);
	if (_tmpcolor.isValid())
		FColor = QColor(_tmpcolor);
}

void TOptions::BtnViewBackgroundColorAction()
{
	QColor _tmpcolor = QColorDialog::getColor(BColor, this);
	if (_tmpcolor.isValid())
	{
		BColor = QColor(_tmpcolor);
		StringsTable->repaint();
		GoldedTable->repaint();
		FipsTable->repaint();
	}
}

void TOptions::BtnEditForegroundColorAction()
{
	QColor _tmpcolor = QColorDialog::getColor(FEColor, this);
	if (_tmpcolor.isValid())
		FEColor = QColor(_tmpcolor);
}

void TOptions::BtnEditBackgroundColorAction()
{
	QColor _tmpcolor = QColorDialog::getColor(BEColor, this);
	if (_tmpcolor.isValid())
		BEColor = QColor(_tmpcolor);
}

void TOptions::StyleTableAction(QListViewItem *it)
{
	if (it == NULL) return;
	TStyleDialog *Dlg = new TStyleDialog(this);
	Dlg->StyleItem->setFontItalic(((TTagItem*)it)->Style->fontItalic());
	Dlg->StyleItem->setFontWeight(((TTagItem*)it)->Style->fontWeight());
	Dlg->StyleItem->setFontUnderline(((TTagItem*)it)->Style->fontUnderline());
	Dlg->StyleItem->setColor(((TTagItem*)it)->Style->color());
	Dlg->Example->setFont(ViewFont);
	Dlg->Example->setPaletteBackgroundColor(BColor);
	Dlg->Example->setPaletteForegroundColor(FColor);
	if (Dlg->exec() == QDialog::Accepted)
	{
		((TTagItem*)it)->Style->setFontItalic(Dlg->StyleItem->fontItalic());
		((TTagItem*)it)->Style->setFontWeight(Dlg->StyleItem->fontWeight());
		((TTagItem*)it)->Style->setFontUnderline(Dlg->StyleItem->fontUnderline());
		((TTagItem*)it)->Style->setColor(Dlg->StyleItem->color());
		it->repaint();
	}
	delete Dlg;
}

void TOptions::ReplaceListSelectionChanged(QListViewItem *it)
{
	BtnDel->setEnabled(it != NULL);
}

void TOptions::ReplaceListClicked(QListViewItem *it)
{
	if (it != NULL)
	{
		TReplace *Dlg = new TReplace(this);
		Dlg->TextEdit->setText(it->text(0));
		Dlg->ReplaceEdit->setText(it->text(1));
		if (Dlg->exec() == QDialog::Accepted)
		{
			it->setText(0, Dlg->TextEdit->text().stripWhiteSpace());
			it->setText(1, Dlg->ReplaceEdit->text().stripWhiteSpace());
		}
		delete Dlg;
	}
}

void TOptions::BtnAddAction()
{
	TReplace *Dlg = new TReplace(this);
	if (Dlg->exec() == QDialog::Accepted)
		ReplaceList->setSelected(new QListViewItem(ReplaceList, Dlg->TextEdit->text().stripWhiteSpace(), Dlg->ReplaceEdit->text().stripWhiteSpace()), true);
	delete Dlg;
}

void TOptions::BtnDelAction()
{
	ReplaceList->takeItem(ReplaceList->selectedItem());
	ReplaceListSelectionChanged(ReplaceList->selectedItem());
}

void TOptions::ShortcutListCurrentChanged(QListViewItem *it)
{
	if (it->isSelectable())
	{
		QString accel = ((TShortCutItem*)it)->GetAccel();
		EditKeyGroup->setEnabled(true);
		KeyEdit->setEnabled(accel.isEmpty());
		KeyEdit->setText(accel);
	} else {
		EditKeyGroup->setEnabled(false);
		KeyEdit->setText(QString::null);
	}
}

void TOptions::BtnEraseKeyAction()
{
	((TShortCutItem*)ShortcutList->selectedItem())->ClearAccel();
	KeyEdit->setText(QString::null);
	KeyEdit->setEnabled(true);
}

bool TOptions::eventFilter(QObject *o, QEvent *e)
{
	if (o == KeyEdit)
	{
		if (e->type() == QEvent::KeyPress)
		{
			QKeyEvent *event = (QKeyEvent*)e;
			switch (event->key())
			{
				case Qt::Key_Shift:
				case Qt::Key_Control:
				case Qt::Key_Meta:
				case Qt::Key_Alt:
				case Qt::Key_CapsLock:
				case Qt::Key_NumLock:
				case Qt::Key_ScrollLock:
				case Qt::Key_Clear:
				case Qt::Key_Super_L:
				case Qt::Key_Super_R:
				case Qt::Key_Menu:
				case Qt::Key_Hyper_L:
				case Qt::Key_Hyper_R:
				case Qt::Key_unknown:
					break;
				default:
				{
					QKeySequence key(((event->state() & Qt::ShiftButton) ? Qt::SHIFT : 0) + ((event->state() & Qt::MetaButton) ? Qt::META : 0) + ((event->state() & Qt::ControlButton) ? Qt::CTRL : 0) + ((event->state() & Qt::AltButton) ? Qt::ALT : 0) + event->key());
					if (QString(key).stripWhiteSpace().isEmpty())
						break;
					if (((TShortCutItem*)ShortcutList->selectedItem())->SetAccel(key))
					{
						KeyEdit->setText(key);
						KeyEdit->setEnabled(false);
						return true;
					}
				}
			}
		}
		return false;
	} else
		return QDialog::eventFilter(o, e);
}

QString TOptions::TemplateByIdx()
{
	switch (TemplateCombo->currentItem())
	{
		case 0:
			return TPL_MESSAGE_TPL;
		case 1:
			return TPL_REPLY_TPL;
		case 2:
			return TPL_FORWARD_TPL;
		case 3:
			return TPL_UNSUBSCRIBE_TPL;
		case 4:
			return TPL_ARCHIVE_TPL;
		case 5:
			return TPL_SAVETXT_TPL;
		case 6:
			return TPL_SAVEHTM_TPL;
		case 7:
			return TPL_ORIGIN_LST;
		case 8:
			return TPL_KLUDGES_LST;
		default:
			break;
	}
	return QString::null;
}

void TOptions::BtnReloadAction()
{
	QString FName = TemplateByIdx();

	TemplateEdit->setEnabled(false);
	TemplateEdit->clear();
	BtnReload->setEnabled(false);
	BtnSave->setEnabled(false);

	if (FName.isEmpty())
		return;

	QFile f(Config->ConfigPath + FName);
	if (!f.exists())
	{
		if (questionmessage(this, QObject::tr("File not exists. Create?")))
		{
			if (FileOpenFunc(&f, IO_WriteOnly | IO_Truncate))
				FileCloseFunc(&f);
		} else
			return;
	}

	if (FileOpenFunc(&f, IO_ReadOnly))
	{
		QTextStream t(&f);
		TemplateEdit->setText(TextStreamReadFunc(&t));
		TemplateEdit->setModified(false);
		TemplateEdit->setEnabled(true);
		FileCloseFunc(&f);
	}
}

void TOptions::BtnSaveAction()
{
	QString FName = TemplateByIdx();

	if (FName.isEmpty())
		return;

	QFile f(Config->ConfigPath + FName);
	if (FileOpenFunc(&f, IO_WriteOnly | IO_Truncate))
	{
		QTextStream t(&f);
		t << TemplateEdit->text();
		TemplateEdit->setModified(false);
		FileCloseFunc(&f);
	}
}

void TOptions::TemplateActivated(int)
{
	BtnReloadAction();
}

void TOptions::TemplateEditChanged(bool ModFlag)
{
	BtnReload->setEnabled(ModFlag);
	BtnSave->setEnabled(ModFlag);
}

void TOptions::OptionsTabChanged(QWidget*)
{
	if (OptionsTab->currentPageIndex() == 4)
	{
		TemplateEdit->setFont(QFont(EditFontNameCombo->currentText(), EditFontSizeBox->value()));
		TemplateEdit->setPaletteBackgroundColor(BColor);
		TemplateEdit->setPaletteForegroundColor(FColor);
		if (!TemplateCombo->isEnabled())
		{
			TemplateCombo->setEnabled(true);
			BtnReloadAction();
		}
	}
}

void TOptions::BrowseHtmlAction()
{
	QFileDialog* fd = new QFileDialog(this);
	fd->setDir(HtmlPathEdit->text());
	fd->setFilter(QObject::tr("All Files (*)"));
#if defined (Q_OS_WIN)
	fd->addFilter(QObject::tr("Executable Files (*.exe)"));
	fd->setSelectedFilter(1);
#endif
	fd->setCaption(QObject::tr("Select file..."));
	fd->setIcon(*Config->FindPixmap("icon_load"));
	fd->setMode(QFileDialog::ExistingFile);
	fd->setInfoPreviewEnabled(true);
	if (fd->exec() == QDialog::Accepted)
		HtmlPathEdit->setText(fd->selectedFile());
	delete fd;
}

void TOptions::BrowseEMailAction()
{
	QFileDialog* fd = new QFileDialog(this);
	fd->setDir(EMailPathEdit->text());
	fd->setFilter(QObject::tr("All Files (*)"));
#if defined (Q_OS_WIN)
	fd->addFilter(QObject::tr("Executable Files (*.exe)"));
	fd->setSelectedFilter(1);
#endif
	fd->setCaption(QObject::tr("Select file..."));
	fd->setIcon(*Config->FindPixmap("icon_load"));
	fd->setMode(QFileDialog::ExistingFile);
	fd->setInfoPreviewEnabled(true);
	if (fd->exec() == QDialog::Accepted)
		EMailPathEdit->setText(fd->selectedFile());
	delete fd;
}

void TOptions::BrowseMMAction()
{
	QFileDialog* fd = new QFileDialog(this);
	fd->setDir(MMPathEdit->text());
	fd->setCaption(QObject::tr("Select directory..."));
	fd->setIcon(*Config->FindPixmap("icon_load"));
	fd->setMode(QFileDialog::DirectoryOnly);
	fd->setInfoPreviewEnabled(true);
	if (fd->exec() == QDialog::Accepted)
		MMPathEdit->setText(fd->selectedFile());
	delete fd;
}
