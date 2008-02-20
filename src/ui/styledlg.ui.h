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

void TStyleDialog::init()
{
	FirstTimeInit = false;

	setIcon(*Config->FindPixmap("icon_style"));

	BtnFontColor->setIconSet(QIconSet(*Config->FindPixmap("icon_color")));
	BtnOk->setIconSet(QIconSet(*Config->FindPixmap("icon_ok")));
	BtnCancel->setIconSet(QIconSet(*Config->FindPixmap("icon_cancel")));

	Config->GetPos(this);

	StyleItem = new QStyleSheetItem(Example->styleSheet(), "tag");
}

void TStyleDialog::showEvent(QShowEvent *)
{
	if (FirstTimeInit)
		return;
	FirstTimeInit = true;

	CheckBoxBold->setChecked(StyleItem->fontWeight() == QFont::Bold);
	CheckBoxItalic->setChecked(StyleItem->fontItalic());
	CheckBoxUnderline->setChecked(StyleItem->fontUnderline());
	//CheckBoxInverse...
	RepaintExample();
}

void TStyleDialog::RepaintExample()
{
	Example->clear();
	Example->setText(QObject::tr("Highlighted").prepend("<p align=\"center\">" + QObject::tr("Example") + "<br><tag>").append("</tag><br>" + QObject::tr("Text") + "</p>"));
}

void TStyleDialog::closeEvent(QCloseEvent *e)
{
	Config->SetPos(this);
	e->accept();
}

void TStyleDialog::CheckBoxItalicAction()
{
	StyleItem->setFontItalic(CheckBoxItalic->isOn());
	RepaintExample();
}

void TStyleDialog::CheckBoxBoldAction()
{
	StyleItem->setFontWeight(CheckBoxBold->isOn() ? QFont::Bold : QFont::Normal);
	RepaintExample();
}

void TStyleDialog::CheckBoxUnderlineAction()
{
	StyleItem->setFontUnderline(CheckBoxUnderline->isOn());
	RepaintExample();
}

void TStyleDialog::CheckBoxInverseAction()
{
	//CheckBoxInverse...
	RepaintExample();
}

void TStyleDialog::BtnFontColorAction()
{
	QColor _tmpcolor = QColorDialog::getColor(StyleItem->color(), this);
	if (_tmpcolor.isValid())
	{
		StyleItem->setColor(_tmpcolor);
		RepaintExample();
	}
}
