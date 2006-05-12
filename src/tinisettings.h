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

#ifndef _TINISETTINGS_H_
#define _TINISETTINGS_H_

#include <qfile.h>

class TIniSettings;

#include "funchider.h"

#include "utils.h"

#define MAX_INIFILE_RECORD	256

class TIniSettings : public QFile
{
public:
	TIniSettings(QString);
	~TIniSettings();

	QString GetString(QString, QString, QString = QString::null);
	void SetString(QString, QString, QString);

	void RemoveString(QString, QString);

	void Flush();

private:
	QStringList IniLines;

	int32_t Find(QString, QString);
	void AddInternal(QString, QString, QString);
};

#endif /* _TINISETTINGS_H_ */
