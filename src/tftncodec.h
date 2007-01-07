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

#ifndef _TFTNCODEC_H_
#define _TFTNCODEC_H_

#include <qfile.h>
#include <qlist.h>
#include <qstring.h>

#include "funchider.h"

#include "tarea.h"
#include "tconfig.h"
#include "utils.h"

#define CODERTYPE_NONE			0
#define CODERTYPE_UU			1
#define CODERTYPE_B64			2
#define CODERTYPE_XX			3

#define ERROR_ENCODE_FILEOPEN		-1
#define ERROR_ENCODE_MAXSECTIONS	-2

#define MAX_ENCODE_UUELENGTH		45
#define MAX_ENCODE_B64LENGTH		72

/* This functions are untested & may contain bugs */
QString EncodeQuotedPrintable(QString, QString = "");
QString DecodeQuotedPrintable(QString, QString = "");

bool EncodeUUCode(QString*, QByteArray*);
bool DecodeUUCode(QString, QByteArray*);

bool EncodeBase64(QString*, QByteArray*);
bool DecodeBase64(QString, QByteArray*);

QString DecodeMIMELine(char*);

class TFTNCoDecPart
{
public:
	TFTNCoDecPart(QByteArray*, uint32_t, uint32_t, QString, QString, uint32_t, uint32_t);
	TFTNCoDecPart(uint32_t, uint32_t, QString, QString, uint32_t, uint32_t);
	~TFTNCoDecPart();

	QByteArray *Array;
	QString PartString;
	uint32_t Section;
	uint32_t AllCount;
	uint32_t Permissions;
	QString FileName;
	QString ID;
	uint32_t Type;
};

class TFTNCoDec: public QList<TFTNCoDecPart>
{
public:
	TFTNCoDec();

	void DecodeMessage(TMessage*);
	int32_t EncodeFile(QString, uint32_t, uint32_t);
	bool MakeFile(QByteArray*, QString*);
};

#endif /* _TFTNCODEC_H_ */
