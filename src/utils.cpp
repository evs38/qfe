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

#include "utils.h"

#if !defined(Q_OS_WIN)
#	define _open	open
#	define _write	write
#	define _close	close
#else
#	define random()	rand()
#	if defined(_MSC_VER)
#		include <io.h>
#	endif
#	include <winsock2.h>
#endif

bool dodebug = false;
extern TConfig *Config;

const TCharSetRecord CharSetRecords[CODEPAGES_COUNT] =
{
	{
		"ISO8859-1",
		"CP437 ISO8859-1",
		"ISO8859-1",
		"LATIN-1 2"
	},
	{
		"ISO8859-2",
		"LATIN-2 ISO8859-2",
		"ISO8859-2",
		"LATIN-2 2"
	},
	{
		"ISO8859-9",
		"LATIN-5 ISO8859-9",
		"ISO8859-9",
		"LATIN-5 2"
	},
	{
		"IBM 850",
		"CP850",
		"IBM 850",
		"CP850 2"
	},
	{
		"IBM 866",
		"CP866 +7 +7_FIDO",
		"IBM 866",
		"CP866 2"
	},
	{
		"UTF-8",
		"UTF-8",
		"utf8",
		"UTF-8 4"
	}
};

const char MonthNames[][4] =
{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

#undef _FAKE_STATIC_TRANSLATIONS_
#ifdef _FAKE_STATIC_TRANSLATIONS_
const char *Static_Translations[] =
{
	/* QColorDialog */
	QT_TRANSLATE_NOOP("QColorDialog", "Hu&e:"),
	QT_TRANSLATE_NOOP("QColorDialog", "&Sat:"),
	QT_TRANSLATE_NOOP("QColorDialog", "&Val:"),
	QT_TRANSLATE_NOOP("QColorDialog", "&Red:"),
	QT_TRANSLATE_NOOP("QColorDialog", "&Green:"),
	QT_TRANSLATE_NOOP("QColorDialog", "Bl&ue:"),
	QT_TRANSLATE_NOOP("QColorDialog", "A&lpha channel:"),
	QT_TRANSLATE_NOOP("QColorDialog", "&Basic colors"),
	QT_TRANSLATE_NOOP("QColorDialog", "&Custom colors"),
	QT_TRANSLATE_NOOP("QColorDialog", "&Define Custom Colors >>"),
	QT_TRANSLATE_NOOP("QColorDialog", "OK"),
	QT_TRANSLATE_NOOP("QColorDialog", "Cancel"),
	QT_TRANSLATE_NOOP("QColorDialog", "&Add to Custom Colors"),
	QT_TRANSLATE_NOOP("QColorDialog", "Select color"),

	/* QFileDialog */
	QT_TRANSLATE_NOOP("QFileDialog", "Copy or Move a File"),
	QT_TRANSLATE_NOOP("QFileDialog", "Read: %1"),
	QT_TRANSLATE_NOOP("QFileDialog", "Write: %1"),
	QT_TRANSLATE_NOOP("QFileDialog", "Cancel"),
	QT_TRANSLATE_NOOP("QFileDialog", "All Files (*)"),
	QT_TRANSLATE_NOOP("QFileDialog", "Name"),
	QT_TRANSLATE_NOOP("QFileDialog", "Size"),
	QT_TRANSLATE_NOOP("QFileDialog", "Type"),
	QT_TRANSLATE_NOOP("QFileDialog", "Date"),
	QT_TRANSLATE_NOOP("QFileDialog", "Attributes"),
	QT_TRANSLATE_NOOP("QFileDialog", "Look &in:"),
	QT_TRANSLATE_NOOP("QFileDialog", "File &name:"),
	QT_TRANSLATE_NOOP("QFileDialog", "File &type:"),
	QT_TRANSLATE_NOOP("QFileDialog", "Back"),
	QT_TRANSLATE_NOOP("QFileDialog", "One directory up"),
	QT_TRANSLATE_NOOP("QFileDialog", "Create New Folder"),
	QT_TRANSLATE_NOOP("QFileDialog", "List View"),
	QT_TRANSLATE_NOOP("QFileDialog", "Detail View"),
	QT_TRANSLATE_NOOP("QFileDialog", "Preview File Info"),
	QT_TRANSLATE_NOOP("QFileDialog", "Preview File Contents"),
	QT_TRANSLATE_NOOP("QFileDialog", "Read-write"),
	QT_TRANSLATE_NOOP("QFileDialog", "Read-only"),
	QT_TRANSLATE_NOOP("QFileDialog", "Write-only"),
	QT_TRANSLATE_NOOP("QFileDialog", "Inaccessible"),
	QT_TRANSLATE_NOOP("QFileDialog", "Symlink to File"),
	QT_TRANSLATE_NOOP("QFileDialog", "Symlink to Directory"),
	QT_TRANSLATE_NOOP("QFileDialog", "Symlink to Special"),
	QT_TRANSLATE_NOOP("QFileDialog", "File"),
	QT_TRANSLATE_NOOP("QFileDialog", "Dir"),
	QT_TRANSLATE_NOOP("QFileDialog", "Special"),
	QT_TRANSLATE_NOOP("QFileDialog", "Open"),
	QT_TRANSLATE_NOOP("QFileDialog", "Save As"),
	QT_TRANSLATE_NOOP("QFileDialog", "&Open"),
	QT_TRANSLATE_NOOP("QFileDialog", "&Save"),
	QT_TRANSLATE_NOOP("QFileDialog", "&Rename"),
	QT_TRANSLATE_NOOP("QFileDialog", "&Delete"),
	QT_TRANSLATE_NOOP("QFileDialog", "R&eload"),
	QT_TRANSLATE_NOOP("QFileDialog", "Sort by &Name"),
	QT_TRANSLATE_NOOP("QFileDialog", "Sort by &Size"),
	QT_TRANSLATE_NOOP("QFileDialog", "Sort by &Date"),
	QT_TRANSLATE_NOOP("QFileDialog", "&Unsorted"),
	QT_TRANSLATE_NOOP("QFileDialog", "Sort"),
	QT_TRANSLATE_NOOP("QFileDialog", "Show &hidden files"),
	QT_TRANSLATE_NOOP("QFileDialog", "the file"),
	QT_TRANSLATE_NOOP("QFileDialog", "the directory"),
	QT_TRANSLATE_NOOP("QFileDialog", "the symlink"),
	QT_TRANSLATE_NOOP("QFileDialog", "Delete %1"),
	QT_TRANSLATE_NOOP("QFileDialog", "<qt>Are you sure you wish to delete %1 \"%2\"?</qt>"),
	QT_TRANSLATE_NOOP("QFileDialog", "&Yes"),
	QT_TRANSLATE_NOOP("QFileDialog", "&No"),
	QT_TRANSLATE_NOOP("QFileDialog", "New Folder"),
	QT_TRANSLATE_NOOP("QFileDialog", "New Folder %1"),
	QT_TRANSLATE_NOOP("QFileDialog", "Find Directory"),
	QT_TRANSLATE_NOOP("QFileDialog", "Directories"),
	QT_TRANSLATE_NOOP("QFileDialog", "Error"),
	QT_TRANSLATE_NOOP("QFileDialog", "%1\nFile not found.\nCheck path and filename."),
	QT_TRANSLATE_NOOP("QFileDialog", "Directory:"),
	QT_TRANSLATE_NOOP("QFileDialog", "&OK")
};
#endif

const uint32_t CRC32_TABLE[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

void errormessage(QWidget *parent, QString Message)
{
	QMessageBox::critical(parent, "QFE - " + QObject::tr("Error"), Message, QObject::tr("OK"));
}

void informationmessage(QWidget *parent, QString Message)
{
	QMessageBox::information(parent, "QFE - " + QObject::tr("Information"), Message, QObject::tr("OK"));
}

bool questionmessage(QWidget *parent, QString Message)
{
	return (QMessageBox::question(parent, "QFE - " + QObject::tr("Question"), Message, QObject::tr("Yes"), QObject::tr("No"), QString::null, 1, 1) == 0) ? true : false;
}

bool extquestionmessage(QWidget *parent, QString Message, QString Param)
{
	bool tmp = false;

	TExtMessage *Wnd = new TExtMessage(parent);
	if (Wnd->SetUp(Message, Param))
		tmp = ((Wnd->exec() == QDialog::Accepted) ? true : false);
	else
		tmp = true;
	delete Wnd;

	return tmp;
}

#if defined(ENABLE_DEBUG)
bool debugmessage(QString Message)
{
	if (dodebug)
	{
		if (Config != NULL)
		{
			QFile debugfile(Config->ConfigPath + DEBUG_LOG_FILE);
			if (FileOpenFunc(&debugfile, IO_WriteOnly | IO_Append))
			{
				QTextStream stream(&debugfile);
				stream << Message << "\n";
				FileCloseFunc(&debugfile);
			} else {
				fprintf(stdout, Message.prepend("Debug file open error. ").append("\n").local8Bit());
				return false;
			}
		} else {
			fprintf(stdout, Message.append("\n").local8Bit());
			return false;
		}
	}
	return true;
}
#endif

QString addr2str1(fidoaddr *_addr, uint8_t *CtrlBuff, bool addzero)
{
	QString ret = QString("%1:%2/%3").arg(_addr->zone).arg(_addr->net).arg(_addr->node);
	ret.append(((_addr->point > 0) || addzero) ? ("." + QString::number(_addr->point)) : QString::null);

	if ((CtrlBuff != NULL) && (_addr->zone == 0) && (_addr->net == 0) && (_addr->node == 0))
	{
		QString msgi = GetKludge(CtrlBuff, "MSGID: ");
		if (!msgi.isEmpty())
			ret = gettoken(msgi.mid(7), 1);
	}

	return ret;
}

QString addr2str2(hs_addr _addr, bool addzero)
{
	return QString("%1:%2/%3").arg(_addr.zone).arg(_addr.net).arg(_addr.node) + (((_addr.point != 0) || addzero) ? ("." + QString::number(_addr.point)) : QString::null);
}

void addr2addr(fidoaddr *_addr1, hs_addr *_addr2)
{
	_addr1->zone = _addr2->zone;
	_addr1->net = _addr2->net;
	_addr1->node = _addr2->node;
	_addr1->point = _addr2->point;
}

void addr2addr(hs_addr *_addr1, fidoaddr *_addr2)
{
	_addr1->zone = _addr2->zone;
	_addr1->net = _addr2->net;
	_addr1->node = _addr2->node;
	_addr1->point = _addr2->point;
}

bool str2addr(QString str, hs_addr *addr)
{
	fidoaddr _addr;

	bool ret = str2addr(str, &_addr);
	addr2addr(addr, &_addr);

	return ret;
}

bool str2addr(QString str, fidoaddr *addr, uint32_t maxlen)
{
	uint32_t i = 0;
	bool ok;

	if (maxlen > 0)
		str.truncate(maxlen);

	while (str.at(i).isDigit())
		i++;
	addr->zone = str.left(i).toUShort(&ok);
	if (ok && (str.at(i) == ':'))
	{
		str = str.mid(i + 1);
		i = 0;
		while (str.at(i).isDigit())
			i++;
		addr->net = str.left(i).toUShort(&ok);
		if (ok && (str.at(i) == '/'))
		{
			str = str.mid(i + 1);
			i = 0;
			while (str.at(i).isDigit())
				i++;
			addr->node = str.left(i).toUShort(&ok);
			if (ok)
			{
				if (str.at(i) == '.')
				{
					str = str.mid(i + 1);
					i = 0;
					while (str.at(i).isDigit())
						i++;
					addr->point = str.left(i).toUShort(&ok);
					if (ok)
						return true;
				} else {
					addr->point = 0;
					return true;
				}
			}
		}
	}

	memset(addr, '\0', sizeof(fidoaddr));
	return false;
}

int32_t compareaddr(fidoaddr *addr1, fidoaddr *addr2)
{
	if (addr1->zone > addr2->zone)
		return 1;
	if (addr1->zone < addr2->zone)
		return -1;

	if (addr1->net > addr2->net)
		return 1;
	if (addr1->net < addr2->net)
		return -1;

	if (addr1->node > addr2->node)
		return 1;
	if (addr1->node < addr2->node)
		return -1;

	if (addr1->point > addr2->point)
		return 1;
	if (addr1->point < addr2->point)
		return -1;

	return 0;
}

QString HandleFlag(QString Buff, uint32_t flag)
{
	QString ret = Buff, kl = GetKludge((uint8_t*)Buff.ascii(), "FLAGS "), nf = flags2str(flag).upper();

	if (!(kl.isEmpty()) && (kl.find(" " + nf) < 0))
		ret.replace("FLAGS ", "FLAGS " + nf + " ");
	else
		ret.append("\nFLAGS " + nf);

	return ret;
}

uint32_t ExtendFlags(uint32_t flags, QString ext)
{
	if (!ext.isEmpty())
	{
		if (ext.find(" PVT") > -1)
			flags |= FLAG_PVT;
		if (ext.find(" HLD") > -1)
			flags |= FLAG_HLD;
		if (ext.find(" CRA") > -1)
			flags |= FLAG_CRASH;
		if (ext.find(" K/S") > -1)
			flags |= FLAG_KILL;
		if (ext.find(" SNT") > -1)
			flags |= FLAG_SENT;
		if (ext.find(" DIR") > -1)
			flags |= FLAG_DIR;
		if (ext.find(" FIL") > -1)
			flags |= FLAG_FILE;
		if (ext.find(" FRQ") > -1)
			flags |= FLAG_FRQ;
		if (ext.find(" RRQ") > -1)
			flags |= FLAG_RRQ;
		if (ext.find(" CFM") > -1)
			flags |= FLAG_CPT;
		if (ext.find(" LOK") > -1)
			flags |= FLAG_LOK;
		if (ext.find(" ENC") > -1)
			flags |= FLAG_ENC;
		if (ext.find(" CMP") > -1)
			flags |= FLAG_CMP;
		if (ext.find(" ESC") > -1)
			flags |= FLAG_ESC;
		if ((ext.find(" IMM") > -1) || (ext.find(" FPU") > -1))
			flags |= FLAG_IMM;
	}

	return flags;
}

QString flags2str(uint32_t flags, QString ext)
{
	QString ret = QString::null;

	flags = ExtendFlags(flags, ext);

	if (flags & FLAG_PVT) ret.append("Pvt ");
	if (flags & FLAG_CRASH) ret.append("Cra ");
	if (flags & FLAG_SENT) ret.append("Snt ");
	if (flags & FLAG_FILE) ret.append("Att ");
	if (flags & FLAG_FWD) ret.append("Fwd ");
	if (flags & FLAG_ORP) ret.append("Orp ");
	if (flags & FLAG_KILL) ret.append("K/s ");
	if (flags & FLAG_LOC) ret.append("Loc ");
	if (flags & FLAG_HLD) ret.append("Hld ");
	if (flags & FLAG_DIR) ret.append("Dir ");
	if (flags & FLAG_FRQ) ret.append("Frq ");
	if (flags & FLAG_RRQ) ret.append("Rrq ");
	if (flags & FLAG_CPT) ret.append("Irq ");
	if (flags & FLAG_ARQ) ret.append("Arq ");
	if (flags & FLAG_URQ) ret.append("Urq ");
	if (flags & FLAG_IMM) ret.append("Imm ");
	if (flags & FLAG_LOK) ret.append("Lok ");
	if (flags & FLAG_ENC) ret.append("Enc ");
	if (flags & FLAG_CMP) ret.append("Cmp ");
	if (flags & FLAG_ESC) ret.append("Esc ");
	if (!ext.isEmpty())
	{
		if (ext.find(" RCV") > -1)
			ret.append("Rcv ");
		if (ext.find(" A/S") > -1)
			ret.append("A/S ");
		if (ext.find(" ZON") > -1)
			ret.append("Zon ");
		if (ext.find(" HUB") > -1)
			ret.append("Hub ");
		if (ext.find(" FAX") > -1)
			ret.append("Fax ");
		if (ext.find(" NPD") > -1)
			ret.append("Npd ");
		if (ext.find(" TFS") > -1)
			ret.append("Tfs ");
		if (ext.find(" KFS") > -1)
			ret.append("Kfs ");
		if (ext.find(" NOD") > -1)
			ret.append("Nod ");
	}
	return ret.stripWhiteSpace();
}

QString flag2desc(uint32_t flag)
{
	if (flag == FLAG_PVT)
		return QObject::tr("Indicates that the message may only be read by its addressee and author.");
	if (flag == FLAG_CRASH)
		return QObject::tr("High-priority mail.");
	if (flag == FLAG_SENT)
		return QObject::tr("Message has been successfully sent.");
	if (flag == FLAG_FILE)
		return QObject::tr("Message has one or more files attached to it.");
	if (flag == FLAG_FWD)
		return QObject::tr("Message is not destined for this system.");
	if (flag == FLAG_ORP)
		return  QObject::tr("Unknown destination.");
	if (flag == FLAG_KILL)
		return QObject::tr("Remove message after it has been sent.");
	if (flag == FLAG_LOC)
		return QObject::tr("Message is created locally.");
	if (flag == FLAG_HLD)
		return QObject::tr("Message should be held for pickup by its destination system.");
	if (flag == FLAG_DIR)
		return QObject::tr("Message must be sent directly to its destination and may not be routed.");
	if (flag == FLAG_FRQ)
		return QObject::tr("Message has one or more file requests in subject field.");
	if (flag == FLAG_RRQ)
		return QObject::tr("Return receipt request.");
	if (flag == FLAG_CPT)
		return QObject::tr("Is Return Receipt.");
	if (flag == FLAG_ARQ)
		return QObject::tr("Audit Request.");
	if (flag == FLAG_URQ)
		return QObject::tr("Update Request.");
	if (flag == FLAG_IMM)
		return QObject::tr("Immediate mail (highest priority).");
	if (flag == FLAG_LOK)
		return QObject::tr("Message is Locked.");
	if (flag == FLAG_ENC)
		return QObject::tr("Message text is Encrypted.");
	if (flag == FLAG_CMP)
		return QObject::tr("Message text is Compressed.");
	if (flag == FLAG_ESC)
		return QObject::tr("Message text is 7-Bit ASCII.");

	return QString::null;
}

void Fts2CRLF(char *buffer)
{
	char *tmp = buffer;

	while ((tmp = strstr(tmp, "\r\n")) != NULL)
	{
		qmemmove((void*)tmp, (void*)(tmp + 1), strlen(tmp + 1) + 1);
		tmp++;
	}

	tmp = buffer;
	while ((tmp = strchr(tmp, '\r')) != NULL)
		*tmp++ = '\n';

	tmp = buffer;
	while (*(tmp + strlen(tmp) - 1) == '\n')
		*(tmp + strlen(tmp) - 1) = '\0';
}

void CRLF2Fts(char *buffer)
{
	char *tmp = buffer;

	while ((tmp = strstr(tmp, "\r\n")) != NULL)
	{
		qmemmove((void*)tmp, (void*)(tmp + 1), strlen(tmp + 1) + 1);
		tmp++;
	}

	tmp = buffer;
	while ((tmp = strchr(tmp, '\n')) != NULL)
		*tmp++ = '\r';
}

QString convert2qhtm(QString buffer)
{
	return buffer.replace('\n', "<br>").replace("  ", "&nbsp;&nbsp;");
}

QString convert2extern(QString buffer)
{
#if defined(Q_OS_WIN)
	QUrl::encode(buffer);
	return buffer;
/*
	QString RetBuff = buffer.replace('%', "%25");
	RetBuff = RetBuff.replace('\n', "%0D").replace(' ', "%20");
	RetBuff = RetBuff.replace('"', "%22").replace('&', "%26");
	RetBuff = RetBuff.replace('\'', "%27").replace(':', "%3A");
	RetBuff = RetBuff.replace(';', "%3B").replace('=', "%3D");
	return RetBuff.replace('?', "%3F");
*/
#else
	return buffer.append("\"").prepend("\"");
#endif
}

bool testexists(QString Name)
{
	bool ret = false;
	QFile File(Name);
	if (File.exists())
		if (FileOpenFunc(&File, IO_ReadOnly))
		{
			ret = true;
			FileCloseFunc(&File);
		}
	return ret;
}

int32_t tokencount(QString str, QString sep)
{
	QString tmp = str.stripWhiteSpace();
	uint32_t j;
	int32_t i, k, ret;

	ret = tmp.isEmpty() ? 0 : 1;

	for (;;)
	{
		i = -1;
		for (j = 0; j < sep.length(); j++)
		{
			k = tmp.find(sep.at(j));
			if (k > -1)
				i = (i > -1) ? QMIN(i, k) : k;
		}

		if (i > -1)
		{
			tmp = tmp.mid(i + 1).stripWhiteSpace();
			if (!tmp.isEmpty())
				ret++;
			else
				break;
		} else
			break;
	}

	return ret;
}

QString gettoken(QString str, int32_t num, QString sep)
{
	QString ret = QString::null, tmp = str.stripWhiteSpace();
	uint32_t j;
	int32_t i, k, cnt = 0;

	for (;;)
	{
		i = -1;
		for (j = 0; j < sep.length(); j++)
		{
			k = tmp.find(sep.at(j));
			if (k > -1)
				i = (i > -1) ? QMIN(i, k) : k;
		}

		if (i > -1)
		{
			if (cnt == (num - 1))
			{
				if (i > 0)
					tmp.truncate(i);
				ret = tmp;
				break;
			}
			tmp = tmp.mid(i + 1).stripWhiteSpace();
			if (!tmp.isEmpty())
				cnt++;
			else
				break;
		} else {
			if (cnt == (num - 1))
				ret = tmp;
			break;
		}
	}

	return ret;
}

void AppendArray(QByteArray *target, QByteArray *source)
{
	uint32_t offset = target->size();
	target->resize(offset + source->size());
	for (uint32_t i = 0; i < source->size(); i++)
		target->at(offset + i) = source->at(i);
}

bool ParseCommand(QObject *parent, QString *val)
{
	QString cmd;
	int32_t i, tc, end, start = val->find("@command ", 0, false);

	if (start > -1)
	{
		end = val->find("@", start + 9, false);
		if (end > -1)
		{
			cmd = val->mid(start + 9, end - (start + 9));
			val->remove(start, (end + 1) - start);
			QProcess proc(parent);
			tc = tokencount(cmd, " \t");
			if (tc > 0)
			{
				for (i = 0; i < tc; i++)
					proc.addArgument(gettoken(cmd, i + 1, " \t"));

				proc.setWorkingDirectory(QDir());
				if (proc.start())
				{
					QTime tim;
					tim.start();
					while (proc.isRunning() && (tim.elapsed() < 10000))
						qApp->processEvents();
					if (proc.isRunning())
					{
						proc.tryTerminate();
						QTimer::singleShot(1000, &proc, SLOT(kill()));
						fprintf(stdout, "Command %s terminated.\n", cmd.ascii());
					} else if (proc.normalExit()) {
						cmd = QString::fromLocal8Bit((const char*)proc.readStdout().data());
						if (!cmd.isEmpty())
							val->insert(start, cmd);
					} else
						fprintf(stdout, "Command %s. Error returned.\n", cmd.ascii());
				} else
					fprintf(stdout, "Can`t start command: %s.\n", cmd.ascii());
			}
			return true;
		}
	}
	return false;
}

bool ValidateFileName(QString fn)
{
	if (Config->FidoConfig->notValidFNChars)
		for (uint32_t i = 0; i < strlen(Config->FidoConfig->notValidFNChars); i++)
			if (fn.find(QChar(*(Config->FidoConfig->notValidFNChars + i))) > -1)
				return false;

	return true;
}

#define VERSION_SEPARATORS	"._- "
int32_t CompareVersions(QString ver1, QString ver2)
{
	int32_t ret, val1, val2, cnt = 2;
	bool ok1, ok2;
	QString tok1, tok2;

	tok1 = gettoken(ver1, 1, VERSION_SEPARATORS);
	tok2 = gettoken(ver2, 1, VERSION_SEPARATORS);

	while (!tok1.isEmpty() && !tok2.isEmpty())
	{
		val1 = tok1.toInt(&ok1);
		val2 = tok2.toInt(&ok2);
		if (ok1 && ok2)
		{
			if (val1 > val2)
				return 1;
			if (val1 < val2)
				return -1;
		} else {
			ret = stricmp(tok1.ascii(), tok2.ascii());
			if (ret != 0)
				return ret;
		}

		tok1 = gettoken(ver1, cnt, VERSION_SEPARATORS);
		tok2 = gettoken(ver2, cnt, VERSION_SEPARATORS);
		cnt++;
	}

	return (tok2.isEmpty() ? (tok1.isEmpty() ? 0 : 1) : -1);
}

QString GetMessageMsgId()
{
	if (Config->FidoConfig->seqDir && (Config->FidoConfig->seqOutrun > 0))
	{
		dword id = GenMsgId(Config->FidoConfig->seqDir, Config->FidoConfig->seqOutrun);
		if (id != 0)
			return QString::number(id, 16).rightJustify(8, '0', true);
	}

	return QString::number(time(NULL), 16).rightJustify(8, '0', true);
}

QString GetRfcMessageMsgId(QString addr)
{
	hs_addr _addr;

	str2addr(addr, &_addr);
	return QString("<%1@p%2.f%3.n%4.z%5.ftn>").arg(QString::number(time(NULL), 10).rightJustify(10, '0', true)).arg(_addr.point).arg(_addr.node).arg(_addr.net).arg(_addr.zone);
}

uint32_t RandomNumber(uint32_t minval, uint32_t maxval)
{
	uint32_t rnd = minval;

	if (minval < maxval)
		for (;;)
		{
			rnd = (uint32_t)random() % (maxval + 1);
			if ((rnd >= minval) && (rnd <= maxval))
				break;
		}

	return rnd;
}

void ExpandMacros(TArea *Area, TMessage *Message, QString *Line, QString _Area)
{
	if (Message != NULL)
	{
		Line->replace("@origarea@", _Area);
		Line->replace("@origareadesc@", Message->Area->Desc);
		Line->replace("@origto@", Config->toUTF((char*)Message->to));
		Line->replace("@origtoname@", gettoken(Config->toUTF((char*)Message->to), 1));
		if (Area->AreaType == AREATYPE_NETMAIL)
			Line->replace("@origtoaddr@", addr2str1(&Message->destaddr));
		Line->replace("@origfrom@", Config->toUTF((char*)Message->from));
		Line->replace("@origfromname@", gettoken(Config->toUTF((char*)Message->from), 1));
		Line->replace("@origfromaddr@", addr2str1(&Message->origaddr, Message->Area->CtlBuff));
		Line->replace("@origsubj@", DecodeMIMELine((char*)Message->subj));
		QDateTime dt;
		dt.setTime_t(Message->dt);
		Line->replace("@origdate@", dt.toString("dd.MM.yyyy"));
		Line->replace("@origtime@", dt.toString("hh:mm:ss"));
		Line->replace("@origattr@", flags2str(Message->attr, GetKludge(Message->Area->CtlBuff, "FLAGS ")));
	}
}

void ExpandMacros2(QWidget *Widget, QString *Line)
{
	Line->replace("@product@", "QFE");
	Line->replace("@platform@", PLATFORM_NAME);
	Line->replace("@version@", QFE_VERSION);
	if (Line->find("@hostname@") > -1)
	{
		char buf[255];

		Line->replace("@hostname@", (gethostname(&buf[0], sizeof(buf)) == 0) ? buf : "localhost");
	}

	while (ParseCommand(Widget, Line))
		qApp->processEvents();
}

QPixmap ScalePixmap(QPixmap inpix, uint32_t w, uint32_t h)
{
	static QPixmap p;

	if (inpix.isNull())
	{
		p.resize(0, 0);
		return p;
	}
	QImage ScaledImg = inpix.convertToImage();

	return QPixmap(ScaledImg.smoothScale(w, h, QImage::ScaleMin));
}

QString FindMatchedFile(QDir dir, QString pattern)
{
#if !defined(Q_OS_WIN)
	QStringList lst = dir.entryList("*.*");

	for (QStringList::Iterator it = lst.begin(); it != lst.end(); ++it)
		if (strcompare(*it, pattern))
			return *it;
#else
	if (testexists(dir.absPath() +  QDir::separator() + pattern))
		return pattern;
#endif
	return QString::null;
}

int32_t LockFile_(QString lockfile, int32_t advisoryLock)
{
	int32_t fh = -1;

	if (advisoryLock > 0)
		while (advisoryLock > 0)
		{
			fh = _open(lockfile.ascii(), O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
			if (fh > 0)
			{
				if (_write(fh, " ", 1) != 1)
				{
					_close(fh);
					fh = -1;
				} else {
#if !defined(Q_OS_WIN)
					struct flock fl;

					fl.l_type = F_WRLCK;
					fl.l_start = 0;
					fl.l_whence = SEEK_SET;
					fl.l_len = 1;
					fl.l_pid = getpid();
					if (fcntl(fh, F_SETLK, &fl) < 0)
					{
						_close(fh);
						fh = -1;
					}
#else
					long offset = tell(fh);
					int32_t r;

					if (offset != -1)
					{
						lseek(fh, 0, SEEK_SET);
						r = _locking(fh, 2, 0);
						lseek(fh, 0, SEEK_SET);
						if (r)
						{
							_close(fh);
							fh = -1;
						}
					}
#endif
				}
			}
			if (fh <= 0)
			{
				advisoryLock--;
				sleep(1);
			} else
				break;
		}
	else
		fh = _open((const char*)lockfile, O_CREAT | O_RDWR | O_EXCL, S_IREAD | S_IWRITE);

	return fh;
}

int32_t UnLockFile_(QString lockfile, int32_t fh, bool RemoveLock)
{
	if (fh > 0)
		_close(fh);
	if (RemoveLock)
		remove(lockfile.ascii());

	return 0;
}

void ReplaceUnicodeChars(QString *Buffer)
{
	QString tmp;
	QRegExp chrx(REGEXP_UNICODE_CHRS, false);

	while (chrx.search(*Buffer) > -1)
	{
		tmp = chrx.cap(0);
		Buffer->replace(tmp, QChar(tmp.mid(2, tmp.length() - (2 + 1)).toUInt()));
	}
}

void KeepKludge(QString Kludge, uint8_t *Buffer, QStringList *List)
{
	QString tmp;

	tmp = GetKludge(Buffer, Kludge);
	if (!tmp.isEmpty())
		List->append(tmp);
}

QString GetKludge(uint8_t *Buffer, QString Token, int32_t cnt)
{
	int32_t i, j = cnt, lp = 0;
	QString StrBuff = (char*)Buffer;

	while (j >= 0)
	{
		i = StrBuff.find(Token, lp);
		if (i < 0)
			break;
		lp = i + Token.length() + 1;
		j--;
	}
	if ((i > -1) && (j == -1))
	{
		j = StrBuff.find("\n", i + Token.length() + 1);
		if (j == -1)
			j = StrBuff.find("\r", i + Token.length() + 1);
		return StrBuff.mid(i, (j > -1) ? (j - i) : (uint32_t)-1);
	}

	return QString::null;
}

uint32_t Calculate_CRC(const uint8_t *Buffer, uint32_t cnt)
{
	uint32_t ret = 0xffffffff;

	if (cnt == 0)
		cnt = strlen((const char*)Buffer);

	for (; cnt-- > 0; Buffer++)
		ret = CRC32_TABLE[((int32_t)ret ^ (*Buffer)) & 0xff] ^ ((ret >> 8) & 0x00ffffff);

	return ret;
}

uint32_t Calculate_Hash(const uint8_t *Buffer, uint32_t cnt)
{
	uint32_t ret = 0, g;

	if (cnt == 0)
		cnt = strlen((const char*)Buffer);

	for (; cnt-- > 0; Buffer++)
	{
		ret = (ret << 4) + *Buffer;
		g = ret & 0xf0000000;
		if (g != 0)
			ret |= (g >> 24) | g;
	}

	return ret & 0x7fffffff;
}

bool Fts2DateTime(uint8_t *Buffer, time_t *val)
{
	QDateTime dt;
	uint8_t i2[10];
	uint32_t i1, i3, i4, i5, i6;

	if (sscanf((const char*)Buffer, "%u %s %u  %u:%u:%u", &i1, &i2[0], &i3, &i4, &i5, &i6) == 6)
	{
		dt.setTime(QTime(i4, i5, i6));

		for (i4 = 1; i4 <= 12; i4++)
			if (stricmp(MonthNames[i4 - 1], (const char*)&i2[0]) == 0)
			{
				i3 += (i3 <= 80) ? 2000 : 1900;
				dt.setDate(QDate(i3, i4, i1));
				break;
			}
	}

	*val = dt.toTime_t();

	return dt.isValid();
}

QString DateTime2Fts(time_t val)
{
	QDateTime dt;

	dt.setTime_t(val);

	return dt.toString("dd ") + MonthNames[dt.date().month() - 1] + dt.toString(" yy  hh:mm:ss");
}

bool Opus2DateTime(uint32_t t, time_t *val)
{
	opusstamp *stamp = (opusstamp*)&t;
	QDateTime dt(QDate(stamp->ts_year + 1980, stamp->ts_month, stamp->ts_day), QTime(stamp->ts_hour, stamp->ts_min, stamp->ts_sec * 2));

	*val = dt.toTime_t();

	return ((stamp->ts_year > 5) ? dt.isValid() : false);
}

void DateTime2Opus(time_t val, uint32_t *t)
{
	opusstamp stamp;
	QDateTime dt;

	dt.setTime_t(val);

	stamp.ts_year = dt.date().year() - 1980;
	stamp.ts_month = dt.date().month();
	stamp.ts_day = dt.date().day();
	stamp.ts_hour = dt.time().hour();
	stamp.ts_min = dt.time().minute();
	stamp.ts_sec = dt.time().second() / 2;

	*t = *((uint32_t*)&stamp);
}

void WrapLines(QStringList *List, QString str, QString Prefix, uint32_t MaxPos)
{
	str.prepend(Prefix);

	while (str.length() > MaxPos)
	{
		int32_t FixWrap = MaxPos;
		for (uint32_t j = (MaxPos - 1); j > (Prefix.length() + 1); j--)
			if (str.at(j).isSpace())
			{
				FixWrap = j;
				break;
			}
		*List << str.left(FixWrap);
		str.remove(0, FixWrap);
		str = str.stripWhiteSpace().prepend(Prefix);
	}
	*List << str;
}

bool isLowerKludge(QString *str)
{
	return (str->startsWith("SEEN-BY: ") || str->startsWith("\001PATH: ") || str->startsWith("\001Via ") || str->startsWith("\001Recd") || str->startsWith("\001Forwarded"));
}

QString GetCharSetForMessage(uint8_t *Buffer, QString DefVal)
{
	uint32_t i;
	QString CHRS = GetKludge(Buffer, "CHRS: ");
	QString Charset = GetKludge(Buffer, "CHARSET: ");

	if (!CHRS.isEmpty())
		CHRS = gettoken(CHRS, 2);
	if (!Charset.isEmpty())
		Charset = gettoken(Charset, 2);

	if (Charset.isEmpty())
	{
		if (!CHRS.isEmpty())
		{
			for (i = 0; i < CODEPAGES_COUNT; i++)
				if (QString(CharSetRecords[i].Aliases).find(CHRS, 0, false) > -1)
					return CharSetRecords[i].CodecValue;
			if ((!strcompare(CHRS, "IBMPC")) && (!strcompare(CHRS, "ASCII")))
				return DefVal;
		}
		return DefVal;
	}

	for (i = 0; i < CODEPAGES_COUNT; i++)
		if (QString(CharSetRecords[i].Aliases).find(Charset, 0, false) > -1)
			return CharSetRecords[i].CodecValue;

	return DefVal;
}
