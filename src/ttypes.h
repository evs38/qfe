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

#ifndef _TTYPES_H_
#define _TTYPES_H_

#if defined(__x86_64__) || defined (__alpha__)
#	define PLATFORM_64BIT
#else
#	define PLATFORM_32BIT
#endif

#if defined(PLATFORM_64BIT)
#	define PLATF_U	uint64_t
#	define PLATF_S	int64_t
#else
#	define PLATF_U	uint32_t
#	define PLATF_S	int32_t
#endif

#if defined(Q_OS_MACX) || defined(__FreeBSD__)
#	include <machine/endian.h>
#elif defined(Q_OS_WIN)
#	include <sys/param.h>
#else
#	include <endian.h>
#endif

#ifndef __BYTE_ORDER
#	define __BYTE_ORDER	BYTE_ORDER
#endif
#ifndef __BIG_ENDIAN
#	define __BIG_ENDIAN	BIG_ENDIAN
#endif
#ifndef __LITTLE_ENDIAN
#	define __LITTLE_ENDIAN	LITTLE_ENDIAN
#endif

#include <stdint.h>

#if __BYTE_ORDER == __BIG_ENDIAN
#	if !defined(__BIG_ENDIAN__)
#		define __BIG_ENDIAN__
#	endif
#	define _TO_BO_U16_(x)	((uint16_t)(((x & 0xff00) >> 8) | ((x & 0x00ff) << 8)))
#	define _TO_BO_U32_(x)	((uint32_t)(((x & 0xff000000) >> 24) | ((x & 0x00ff0000) >> 8) | ((x & 0x0000ff00) << 8) | ((x & 0x000000ff) << 24)))
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#	if !defined(__LITTLE_ENDIAN__)
#		define __LITTLE_ENDIAN__
#	endif
#	define _TO_BO_U16_(x)	((uint16_t)x)
#	define _TO_BO_U32_(x)	((uint32_t)x)
#elif __BYTE_ORDER == __PDP_ENDIAN
#	if !defined(__PDP_ENDIAN__)
#		define __PDP_ENDIAN__
#	endif
#	define _TO_BO_U16_(x)	((uint16_t)x)
#	define _TO_BO_U32_(x)	((uint32_t)(((x & 0xffff0000) >> 16) | ((x & 0x0000ffff) << 16)))
#else
#	error "Unknown endianness."
#endif

#if defined(_MSC_VER)
#	warning "FIXME: What true pack values for MSVC?"
#	define __PACKED_ATTR_PREF__
#	define __PACKED_ATTR_SUFF__
#else
#	define __PACKED_ATTR_PREF__
#	define __PACKED_ATTR_SUFF__	__attribute__((packed))
#endif

#define sizeofrec(a,b)			sizeof(((a*)0)->b)

__PACKED_ATTR_PREF__ struct bo16_t {
	uint16_t unordered;
	inline operator uint32_t() const
	{
		return (uint32_t)_TO_BO_U16_(unordered);
	}
	inline bo16_t& operator=(const uint32_t &o)
	{
		unordered = _TO_BO_U16_((uint16_t)o);
		return *this;
	}
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct bo32_t {
	uint32_t unordered;
	inline operator uint32_t() const
	{
		return _TO_BO_U32_(unordered);
	}
	inline bo32_t& operator=(const uint32_t &o)
	{
		unordered = _TO_BO_U32_(o);
		return *this;
	}
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct fidoaddr
{
	bo16_t zone;
	bo16_t net;
	bo16_t node;
	bo16_t point;
} __PACKED_ATTR_SUFF__;

__PACKED_ATTR_PREF__ struct opusstamp {
	unsigned ts_day  :5;
	unsigned ts_month:4;
	unsigned ts_year :7;
	unsigned ts_sec  :5;
	unsigned ts_min  :6;
	unsigned ts_hour :5;
} __PACKED_ATTR_SUFF__;

enum Area_Type
{
	AREATYPE_UNKNOWN,
	AREATYPE_NETMAIL,
	AREATYPE_BADMAIL,
	AREATYPE_DUPEMAIL,
	AREATYPE_CARBON,
	AREATYPE_LOCALMAIL,
	AREATYPE_ECHOMAIL
};

enum Base_Type
{
	BASETYPE_UNKNOWN,
	BASETYPE_MSG,
	BASETYPE_JAM,
	BASETYPE_SQUISH,
	BASETYPE_HUDSON,	/* Not implemented */
	BASETYPE_FIPS,		/* Not implemented */
	BASETYPE_SQLITE,	/* Not implemented */
	BASETYPE_PASSTROUGH
};

enum Config_Type
{
	CONFTYPE_UNKNOWN,
	CONFTYPE_FIDOCONFIG,
	CONFTYPE_FASTECHO,	/* Not implemented */
	CONFTYPE_PARTOSS,	/* Not implemented */
	CONFTYPE_SQUISH,	/* Not implemented */
	CONFTYPE_AREASBBS	/* Not implemented */
};

#endif /* _TTYPES_H_ */
