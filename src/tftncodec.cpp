/***************************************************************************
 *   Copyright (C) 2005 by Alexander Shiyan                                *
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

#include "tftncodec.h"

#warning "Encode functions is untested !!!"

extern TConfig *Config;

const uint8_t Base64EncMap[64] =
{
  0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
  0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
  0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
  0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
  0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
  0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
  0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2B, 0x2F
};

const uint8_t Base64DecMap[128] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
  0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
  0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
  0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
  0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
  0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
  0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const uint8_t UUEncMap[64] =
{
  0x60, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F
};

const uint8_t UUDecMap[128] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/*
const uint8_t XXEncMap[64] =
{
  0x2B, 0x2D, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
  0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44,
  0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C,
  0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54,
  0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x61, 0x62,
  0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A,
  0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72,
  0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A
};
*/

QString EncodeQuotedPrintable(QString Str, QString ChrCodec)
{
	QString ret = QString::null;
	QTextCodec *qpcodec = QTextCodec::codecForName(ChrCodec);
	if ((qpcodec != NULL) && !Str.isEmpty())
	{
		QByteArray raw = qpcodec->fromUnicode(Str);
		uint8_t *cur = (uint8_t*)raw.data();

		for (uint32_t i = 0; i < raw.size(); i++)
		{
			if (((*cur > 32) && (*cur < 127)) && (QString("\n\r?=").find(*cur, 0) < 0))
				ret += QChar(*cur);
			else
				ret += "=" + QString::number((uint32_t)*cur, 0, 16).rightJustify(2, '0', true).upper();
			cur++;
		}

		ret = ret.prepend("=?" + ChrCodec.lower() + "?Q?").append("?=");
	} else
		return Str;

	return ret;
}

QString DecodeQuotedPrintable(QString Str, QString ChrCodec)
{
	QByteArray ret(0);
	QTextCodec *qpcodec = QTextCodec::codecForName(ChrCodec);

	if ((qpcodec != NULL) && !Str.isEmpty())
	{
		int32_t h1, h2;
		uint16_t *raw = (uint16_t *)Str.ucs2();
		uint8_t cur;

		for (uint32_t i = 0; i < Str.length(); i++)
		{
			cur = (uint8_t)*(raw + i);
			if (cur == '=')
			{
				if (i < (Str.length() - 2))
				{
					h1 = QString("0123456789ABCDEF").find(Str.at(i + 1), 0, false);
					h2 = QString("0123456789ABCDEF").find(Str.at(i + 2), 0, false);
					if ((h1 > -1) && (h2 > -1))
					{
						//FIXME: Qt4 allow use append() here?
						ret.resize(ret.size() + 1);
						ret.at(ret.size() - 1) = (uint8_t)(h1 * 16 + h2);
					}
					i += 2;
				}
			} else if (cur == '?')
			{
				break;
			} else if (((cur > 32) && (cur < 127)) && (QString("\n\r").find(cur) < 0))
			{
				ret.resize(ret.size() + 1);
				ret.at(ret.size() - 1) = cur;
			}
		}
	} else
		return Str;

	return qpcodec->toUnicode(ret.data(), ret.size());
}

bool EncodeUUCode(QString *Str, QByteArray *Arr)
{
	uint32_t Cnt = 0, LineCnt = 0, LineLimit = MAX_ENCODE_UUELENGTH;
	*Str = QString::null;

	if (Arr->size() == 0)
		return false;

	while (Cnt < Arr->size())
	{
		if (LineCnt == 0)
		{
			LineLimit = QMIN(Arr->size() - Cnt, MAX_ENCODE_UUELENGTH);
			Str->append(UUEncMap[(uint8_t)LineLimit]);
		}

		Str->append(UUEncMap[(uint8_t)(Arr->at(Cnt) >> 2)]);
		if ((Cnt + 1) < Arr->size())
		{
			Str->append(UUEncMap[(uint8_t)(((Arr->at(Cnt) & 0x03) << 4) | (Arr->at(Cnt + 1) >> 4))]);
			if ((Cnt + 2) < Arr->size())
			{
				Str->append(UUEncMap[(uint8_t)(((Arr->at(Cnt + 1) & 0x0F) << 2) | (Arr->at(Cnt + 2) >> 6))]);
				Str->append(UUEncMap[(uint8_t)(Arr->at(Cnt + 2) & 0x3F)]);
			} else
				Str->append(UUEncMap[(uint8_t)((Arr->at(Cnt + 1) & 0x0F) << 2)]);
		} else
			Str->append(UUEncMap[(uint8_t)((Arr->at(Cnt) & 0x03) << 4)]);

		Cnt += 3;
		LineCnt += 4;
		if (LineCnt >= LineLimit)
		{
			Str->append('\n');
			LineCnt = 0;
		}
	}
	if (LineCnt)
		Str->append('\n');
	Str->append(UUEncMap[0]);
	Str->append('\n');

	return !Str->isEmpty();
}

bool DecodeUUCode(QString Str, QByteArray *Arr)
{
	QString tmp = Str.stripWhiteSpace();
	int32_t LineLen;
	uint32_t i, Count = Str.length(), Cnt = 0, OutPtr = 0;
	uint8_t t[4];
	uint16_t *raw = (uint16_t *)tmp.ucs2();

	Arr->resize(0);

	while (Cnt < Count)
	{
		if ((Cnt < Count) && (*(raw + Cnt) <= ' '))
		{
			Cnt++;
			continue;
		}

		if ((*(raw + Cnt) < 0x80) && (Cnt < Count))
			LineLen = UUDecMap[*(raw + Cnt)];
		else
			break;
		Cnt++;

		if ((LineLen == 0) || (LineLen == 0xFF))
			break;

		OutPtr = Arr->size();
		Arr->resize(Arr->size() + LineLen);

		while (LineLen > 0)
		{
			for (i = 0; i < 4; i++)
				t[i] = 0xFF;

			for (i = 0; (i < 4) && (Cnt < Count); i++)
			{
				if (*(raw + Cnt) < 0x80)
					t[i] = UUDecMap[*(raw + Cnt)];
				else
					return false;
				Cnt++;
			}

			if (t[0] != 0xFF)
			{
				if (t[1] != 0xFF)
				{
					Arr->at(OutPtr) = (uint8_t)((t[0] << 2) | (t[1] >> 4));
					OutPtr++;
					if (LineLen >= 2)
					{
						if (t[2] != 0xFF)
						{
							Arr->at(OutPtr) = (uint8_t)(((t[1] & 0x0F) << 4) | (t[2] >> 2));
							OutPtr++;
							if (LineLen >= 3)
							{
								if (t[3] != 0xFF)
									Arr->at(OutPtr) = (uint8_t)(((t[2] & 0x03) << 6) | t[3]);
								else {
									Arr->at(OutPtr) = (uint8_t)((t[2] & 0x03) << 6);
									break;
								}
							}
						} else {
							Arr->at(OutPtr) = (uint8_t)((t[1] & 0x0F) << 4);
							break;
						}
					}
				} else {
					Arr->at(OutPtr) = (uint8_t)(t[0] << 2);
					break;
				}
			} else
				break;
			LineLen -= 3;
			OutPtr++;
		}
	}

	return (Arr->size() > 0);
}

bool EncodeBase64(QString *Str, QByteArray *Arr)
{
	uint32_t Cnt = 0, LineCnt = 0;
	*Str = QString::null;

	if (Arr->size() == 0)
		return false;

	while (Cnt < Arr->size())
	{
		Str->append(Base64EncMap[(uint8_t)(Arr->at(Cnt) >> 2)]);
		if ((Cnt + 1) < Arr->size())
		{
			Str->append(Base64EncMap[(uint8_t)(((Arr->at(Cnt) & 0x03) << 4) | (Arr->at(Cnt + 1) >> 4))]);
			if ((Cnt + 2) < Arr->size())
			{
				Str->append(Base64EncMap[(uint8_t)(((Arr->at(Cnt + 1) & 0x0F) << 2) | (Arr->at(Cnt + 2) >> 6))]);
				Str->append(Base64EncMap[(uint8_t)(Arr->at(Cnt + 2) & 0x3F)]);
			} else {
				Str->append(Base64EncMap[(uint8_t)((Arr->at(Cnt + 1) & 0x0F) << 2)]);
				Str->append('=');
			}
		} else {
			Str->append(Base64EncMap[(uint8_t)((Arr->at(Cnt) & 0x03) << 4)]);
			Str->append("==");
		}
		Cnt += 3;
		LineCnt += 4;
		if (LineCnt >= MAX_ENCODE_B64LENGTH)
		{
			Str->append('\n');
			LineCnt = 0;
		}
	}
	if (LineCnt > 0)
		Str->append('\n');

	return !Str->isEmpty();
}

bool DecodeBase64(QString Str, QByteArray *Arr)
{
	QString tmp = Str.stripWhiteSpace();
	uint32_t Count = Str.length(), Cnt = 0;
	uint8_t t[4];
	uint16_t *raw = (uint16_t*)tmp.ucs2();

	Arr->resize(0);

	while (Cnt < Count)
	{
		if ((Cnt < Count) && (*(raw + Cnt) == '\n'))
		{
			Cnt++;
			continue;
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			t[i] = 0xFF;

			switch (*(raw + Cnt + i))
			{
				case '=':
				case ' ':
				case '\n':
				case '\t':
					break;
				default:
					if ((*(raw + Cnt + i) < 0x80) && ((Cnt + i) < Count))
					{
						t[i] = Base64DecMap[*(raw + Cnt + i)];
						continue;
					}
					break;
			}
			break;
		}
		Cnt += 4;

		if (t[0] != 0xFF)
		{
			Arr->resize(Arr->size() + 1);
			if (t[1] != 0xFF)
			{
				Arr->at(Arr->size() - 1) = (uint8_t)((t[0] << 2) | (t[1] >> 4));
				Arr->resize(Arr->size() + 1);
				if (t[2] != 0xFF)
				{
					Arr->at(Arr->size() - 1) = (uint8_t)(((t[1] & 0x0F) << 4) | (t[2] >> 2));
					Arr->resize(Arr->size() + 1);
					if (t[3] != 0xFF)
						Arr->at(Arr->size() - 1) = (uint8_t)(((t[2] & 0x03) << 6) | t[3]);
					else {
						Arr->at(Arr->size() - 1) = (uint8_t)((t[2] & 0x03) << 6);
						break;
					}
				} else {
					Arr->at(Arr->size() - 1) = (uint8_t)((t[1] & 0x0F) << 4);
					break;
				}
			} else {
				Arr->at(Arr->size() - 1) = (uint8_t)(t[0] << 2);
				break;
			}
		} else
			break;
	}

	return (Arr->size() > 0);
}

QString DecodeMIMELine(char *charval)
{
	QChar enc;
	bool found;
	int32_t i, i1, j, k = -1;
	QString ret = Config->toUTF(charval), dec, chrs;

	for (;;) {
		found = false;
		if (!ret.isEmpty() && (ret.at(0) == '?'))
			i = 0;
		else
			i = ret.find("=?", 0);
		if (i > -1)
		{
			i1 = ret.find('?', 0);
			j = ret.find('?', i + 2);
			if (j > 0)
			{
				chrs = QString(ret.data()).mid(i1 + 1, j - (i1 + 1));
				enc = ret.at(j + 1).upper();
				if (ret.at(j + 2) == '?')
				{
					k = ret.find('?', j + 3);
					if (k < 0)
						k = ret.length();
					dec = QString(ret.data()).mid(j + 3, k - (j + 3));
					switch (enc)
					{
						case 'B':
						{
							QByteArray arr(0);
							QTextCodec *qpcodec = QTextCodec::codecForName(chrs);
							if (qpcodec != NULL)
								if (DecodeBase64(dec, &arr))
								{
									found = true;
									dec = qpcodec->toUnicode(arr.data(), arr.size());
								}
							break;
						}
						case 'Q':
							found = true;
							dec = DecodeQuotedPrintable(dec, chrs);
							break;
						case 'U':
						{
							QByteArray arr(0);
							QTextCodec *qpcodec = QTextCodec::codecForName(chrs);
							if (qpcodec != NULL)
								if (DecodeUUCode(dec, &arr))
								{
									found = true;
									dec = qpcodec->toUnicode(arr.data(), arr.size());
								}
							break;
						}
			    			default:
							break;
					}
				}
				if (found)
					return ret.replace(i, (k + 2) - i, dec);
			}
		}
		if (!found)
			break;
	};
	return ret;
}

TFTNCoDecPart::TFTNCoDecPart(QByteArray *Arr, uint32_t PartNum, uint32_t PartCount, QString FName, QString _ID, uint32_t _Perms, uint32_t _type)
{
	Array = new QByteArray();
	AppendArray(Array, Arr);
	PartString = QString::null;
	Section = PartNum;
	AllCount = PartCount;
	FileName = FName;
	ID = _ID;
	Permissions = _Perms;
	Type = _type;
}

TFTNCoDecPart::TFTNCoDecPart(uint32_t PartNum, uint32_t PartCount, QString FName, QString _ID, uint32_t _Perms, uint32_t _type)
{
	Array = NULL;
	PartString = QString::null;
	Section = PartNum;
	AllCount = PartCount;
	FileName = FName;
	ID = _ID;
	Permissions = _Perms;
	Type = _type;
}

TFTNCoDecPart::~TFTNCoDecPart()
{
	if (Array != NULL)
		delete Array;
}

TFTNCoDec::TFTNCoDec()
{
	setAutoDelete(true);
}

void TFTNCoDec::DecodeMessage(TMessage *Msg)
{
	uint32_t s1, s2, pn = 0, pc = 0, pt = CODERTYPE_NONE, pm = 0644, perms;
	QString fn = QString::null, id = QString::null, split;
	bool r1, r2, ret = false;
	int32_t pnum = 0, pcnt = 0;
	float uver = 0.0;
	char fname[256], *Txt, *TxtPtr, *Ctl, *LastEnd, *Section = NULL, *Begin = NULL;
	QByteArray *Arr = new QByteArray();

	if (Msg->Read())
	{
		Txt = qstrdup((const char*)Msg->Area->TxtBuff);
		Ctl = qstrdup((const char*)Msg->Area->CtlBuff);

		TxtPtr = Txt;
		for (;;)
		{
			Section = strstr(TxtPtr, "section ");
			while (Section) /* Find keyword "section" */
			{
				if (sscanf(Section + 8, "%d of %d of file %s", &pnum, &pcnt, &fname[0]) == 3)
				{
					pn = pnum;
					pc = pcnt;
					fn = fname;
					break;
				} else if (sscanf(Section + 8, "%d of %d of File %s", &pnum, &pcnt, &fname[0]) == 3)
				{
					pn = pnum;
					pc = pcnt;
					fn = fname;
					break;
				} else if (sscanf(Section + 8, "%d of uuencode %f of file %s", &pnum, &uver, &fname[0]) == 3)
				{
					pn = pnum;
					pc = pnum;
					fn = fname;
					pt = CODERTYPE_UU;
					break;
				}
				Section = strstr(Section + 8, "section ");
			}
			if (Section == NULL)  /* Find keyword "begin" */
			{
				Section = strstr(TxtPtr, "begin ");
				while (Section)
				{
					Section += 6;
					pt = CODERTYPE_UU;
					if (sscanf(Section, "base64 %o %s", &perms, &fname[0]) == 2)
					{
						Section += 7;
						pt = CODERTYPE_B64;
					}
					if (sscanf(Section, "%o %s", &perms, &fname[0]) == 2)
					{
						fn = fname;
						pm = perms;

						split = GetKludge((uint8_t*)Ctl, "SPLIT: ");
						if (!split.isEmpty())
						{
							pn = split.mid(45, 2).toUInt();
							pc = split.mid(48, 2).toUInt();
							split.truncate(44);
							id = split.mid(7).stripWhiteSpace();;
						} else {
							pn = 1;
							pc = 1;
						}

						Begin = Section;
						break;
					}
					pt = CODERTYPE_NONE;
					Section = strstr(Section + 1, "begin ");
				}
				if (Section == NULL)  /* Find kludge "SPLIT" */
				{
					split = GetKludge((uint8_t*)Ctl, "SPLIT: ");
					if (!split.isEmpty())
					{
						pn = split.mid(45, 2).toUInt();
						pc = split.mid(48, 2).toUInt();
						split.truncate(44);
						id = split.mid(7).stripWhiteSpace();;
						Begin = TxtPtr;
					}
				}
			} else {
				if (pn == 1)
				{
					Begin = strstr(Section, "begin ");
					if (Begin)
						pt = (sscanf(Begin + 6, "base64 %o %s", &perms, &fname[0]) == 2) ? CODERTYPE_B64 : CODERTYPE_UU;
				} else
					Begin = Section;
			}

			if (Begin && (pn > 0) && (pn <= 99) && (pc > 0) && (pc <= 99))
			{
				while (*Begin && (*Begin != '\n'))
					Begin++;
				while (*Begin && (*Begin == '\n'))
					Begin++;

				LastEnd = Begin + 1;

				Section = strstr(Begin, "sum ");
				if (Section)
				{
					*Section = '\0';
					LastEnd = Section + 4;
				}
				Section = strstr(Begin, "\nend");
				if (Section)
				{
					*Section = '\0';
					if (Section > LastEnd)
						LastEnd = Section + 4;
				}
				Section = strstr(Begin, "\n--- ");
				if (Section)
				{
					*Section = '\0';
					if (Section > LastEnd)
						LastEnd = Section + 4;
				}

				if (Begin != NULL)
				{
					if (pt == CODERTYPE_NONE)
					{
						r1 = DecodeUUCode(QString(Begin), Arr);
						s1 = Arr->size();
						r2 = DecodeBase64(QString(Begin), Arr);
						s2 = Arr->size();
						pt = r1 ? (r2 ? ((s1 > s2) ? CODERTYPE_UU : CODERTYPE_B64) : CODERTYPE_UU) : CODERTYPE_B64;
					}
					ret = (pt == CODERTYPE_UU) ? DecodeUUCode(QString(Begin), Arr) : DecodeBase64(QString(Begin), Arr);

					if (ret && fn.isEmpty())
						if (sscanf((char*)Msg->subj, "[%u/%u] %s", &s1, &s2, &fname[0]) == 3)
							fn = fname;

					if (ret)
						append(new TFTNCoDecPart(Arr, pn, pc, fn, id, pm, pt));

					TxtPtr = LastEnd;
					Section = NULL;
					Begin = NULL;
					fn = QString::null;
					id = QString::null;

					if ((TxtPtr + 20) >= (Txt + strlen((char*)Msg->Area->TxtBuff)))
						break;
				} else
					break;
			} else
				break;
		}

		delete Txt;
		delete Ctl;
	}

	delete Arr;
}

int32_t TFTNCoDec::EncodeFile(QString fname, uint32_t type, uint32_t scnt)
{
	uint32_t i, j = 0;
	TFTNCoDecPart *Part = NULL;

	clear();

	QFile enc(fname);
	if (FileOpenFunc(&enc, IO_ReadOnly))
	{
		QString out;
		QStringList outlst;
		QByteArray Arr(enc.size());

		for (i = 0; i < enc.size(); i++)
			Arr.at(i) = (uint8_t)enc.getch();

		switch (type)
		{
			case CODERTYPE_B64:
				EncodeBase64(&out, &Arr);
				break;
			default:
				EncodeUUCode(&out, &Arr);
				break;
		}

		outlst.split("\n", out);

		if (outlst.count())
		{
			if (outlst.count() > 5)
				while ((outlst.count() % scnt) < 5)
					scnt++;

			if (((outlst.count() / scnt) + 1) <= 99)
			{
#if !defined(Q_OS_WIN)
				uint32_t perms = 0644;
				struct stat stat_buf;
				if (stat(fname.ascii(), &stat_buf) == 0)
					perms = stat_buf.st_mode;
#else
				const uint32_t perms = 0644;
#endif

				for (i = 0; i < outlst.count(); i++)
				{
					if (j == 0)
					{
						Part = new TFTNCoDecPart((i / scnt) + 1, (outlst.count() / scnt) + 1, fname, QString::null, perms, type);
						append(Part);
						Part->PartString = QString("section %1 of %2 of file %3 -=={QFE encode}==-\n").arg((i / scnt) + 1).arg((outlst.count() / scnt) + 1).arg(fname);
						if (count() == 1)
							Part->PartString.append(QString("begin %1 %2 %3\n").arg((type == 2) ? "base64" : QString::null).arg(perms).arg(fname));
					}
					if (!outlst[i].isEmpty())
						Part->PartString.append(outlst[i] + "\n");
					if (j >= (scnt - 1))
						j = 0;
					else
						j++;
				}

				Part->PartString.append("end\n");

				FileCloseFunc(&enc);
			} else
				return ERROR_ENCODE_MAXSECTIONS;
		} else
			return ERROR_ENCODE_FILEOPEN;
	} else
		return ERROR_ENCODE_FILEOPEN;

	return count();
}

bool TFTNCoDec::MakeFile(QByteArray *Arr, QString *FileName)
{
	QString tmpName = QString::null, tmpID = QString::null;
	int32_t probed = -1;
	uint32_t i, j, tmpType = 0, tmpSections = 0;
	TFTNCoDecPart *Part;

	Arr->resize(0);
	*FileName = QString::null;

	if (count() > 0)
	{
		for (i = 0; i < count(); i++)
		{
			Part = at(i);
			if (Part->Section == 1)
			{
				probed = i;
				tmpSections = Part->AllCount;
				tmpName = Part->FileName;
				tmpID = Part->ID;
				tmpType = Part->Type;
				AppendArray(Arr, Part->Array);
				remove(probed);
				break;
			}
		}
		if (probed > -1)
		{
			for (j = 2; j <= tmpSections; j++)
			{
				probed = -1;
				for (i = 0; i < count(); i++)
				{
					Part = at(i);
					if (Part->Section == j)
					{
						if ((((tmpName == Part->FileName) && !tmpName.isEmpty()) || ((tmpID == Part->ID) && !tmpID.isEmpty())) && (tmpType == Part->Type))
						{
							if (tmpName.isEmpty() && !Part->FileName.isEmpty())
								tmpName = Part->FileName;
							probed = i;
							tmpSections = QMAX(tmpSections, Part->AllCount);
							AppendArray(Arr, Part->Array);
							break;
						}
					}
				}
				if (probed == -1)
				{
					Arr->resize(0);
					break;
				}
			}
			if (!tmpName.isEmpty())
				*FileName = tmpName;
			return (Arr->size() > 0);
		}
	}

	return false;
}
