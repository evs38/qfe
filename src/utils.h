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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <qdir.h>
#include <qurl.h>
#include <qfile.h>
#include <qimage.h>
#include <qtimer.h>
#include <qregexp.h>
#include <qstring.h>
#include <qwidget.h>
#include <qprocess.h>
#include <qmessagebox.h>
#include <qapplication.h>

#include "config.h"
#include "funchider.h"

#include "tarea.h"
#include "ttypes.h"
#include "tconfig.h"
#include "tmessage.h"
#include "tftncodec.h"
#include "extmessage.h"
#include "tfidoconfig.h"

#define CHARSETREC_LENGTH	32

struct TCharSetRecord
{
	char Name[CHARSETREC_LENGTH];
	char Aliases[CHARSETREC_LENGTH];
	char CodecValue[CHARSETREC_LENGTH];
	char CHRS[CHARSETREC_LENGTH];
};

#define CODEPAGES_COUNT		6

extern const TCharSetRecord CharSetRecords[CODEPAGES_COUNT];

extern bool dodebug;

void errormessage(QWidget*, QString);
void informationmessage(QWidget*, QString);
bool questionmessage(QWidget*, QString);
bool extquestionmessage(QWidget*, QString, QString);

#if defined(ENABLE_DEBUG)
	bool debugmessage(QString);
#else
	inline bool _debugmessage() { return true; }
#	define debugmessage(x)	_debugmessage(/*x*/)
#endif

QString addr2str1(fidoaddr*, uint8_t* = NULL, bool = false);
QString addr2str2(hs_addr, bool = false);

void addr2addr(fidoaddr*, hs_addr*);
void addr2addr(hs_addr*, fidoaddr*);

bool str2addr(QString, hs_addr*);
bool str2addr(QString, fidoaddr*, uint32_t = 0);

int32_t compareaddr(fidoaddr*, fidoaddr*);

QString HandleFlag(QString, uint32_t);
uint32_t ExtendFlags(uint32_t, QString);

QString flags2str(uint32_t, QString = QString::null);
QString flag2desc(uint32_t);

void Fts2CRLF(char*);
void CRLF2Fts(char*);

QString convert2qhtm(QString);
QString convert2extern(QString);

bool testexists(QString);

inline bool strcompare(QString str1, QString str2)
{
	/* Use qstricmp for avoid crash if str is NULL */
	return (qstricmp(str1.ascii(), str2.ascii()) == 0);
}

PLATF_S tokencount(QString, QString = " ");
QString gettoken(QString, PLATF_S, QString = " ");

void AppendArray(QByteArray*, QByteArray*);

bool ParseCommand(QObject*, QString*);

bool ValidateFileName(QString);

int CompareVersions(QString, QString);

QString GetMessageMsgId();
QString GetRfcMessageMsgId(QString);

uint32_t RandomNumber(uint32_t, uint32_t);

void ExpandMacros(TArea*, TMessage*, QString*, QString);
void ExpandMacros2(QWidget*, QString*);

QPixmap ScalePixmap(QPixmap, uint32_t, uint32_t);

QString FindMatchedFile(QDir, QString);

int32_t LockFile_(QString, int32_t);
int32_t UnLockFile_(QString, int32_t, bool = true);

void ReplaceUnicodeChars(QString*);

void KeepKludge(QString, uint8_t*, QStringList*);
QString GetKludge(uint8_t*, QString, int32_t = 0);

uint32_t Calculate_CRC(const uint8_t*, uint32_t = 0);
uint32_t Calculate_Hash(const uint8_t*, uint32_t = 0);

bool Fts2DateTime(uint8_t*, time_t*);
QString DateTime2Fts(time_t);

bool Opus2DateTime(uint32_t, time_t*);
void DateTime2Opus(time_t, uint32_t*);

void WrapLines(QStringList*, QString, QString, uint32_t);

bool isLowerKludge(QString*);

QString GetCharSetForMessage(uint8_t*, QString);

#endif /* _UTILS_H_ */
