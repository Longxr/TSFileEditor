/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2013 David Hoerl All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY David Hoerl ''AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL David Hoerl OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef __BCPLUSPLUS__
#include <memory.h>
// string.h needed for memcpy(). RLN 111215
// This may be applicable to other compilers as well.
#endif

#include "xlslib/record.h"
#include "xlslib/unit.h"
#include "xlslib/globalrec.h"
#include "xlslib/rectypes.h"
#include "xlslib/datast.h"

using namespace xlslib_core;
using namespace xlslib_strings;

/*
 *********************************************************************************
 *  CUnit class implementation
 *********************************************************************************
 */

// Default constructor
CUnit::CUnit(CDataStorage &datastore) :
	m_Store(datastore),
	m_Index(INVALID_STORE_INDEX),
	m_Backpatching_Level(0),
	m_AlreadyContinued(false)
{
	datastore.Push(this);
}

CUnit::CUnit(const CUnit& orig) :
	m_Store(orig.m_Store),
	m_Index(INVALID_STORE_INDEX),
	m_Backpatching_Level(0)
{
	XL_ASSERT(m_Index == INVALID_STORE_INDEX);
	if (orig.m_Index != INVALID_STORE_INDEX) {
		m_Index = m_Store.RequestIndex(orig.GetDataSize());
		if (m_Index != INVALID_STORE_INDEX)	{
			XL_ASSERT(m_Index >= 0);
			XL_ASSERT(m_Store[m_Index].GetSize() >= orig.GetDataSize());
			memcpy(m_Store[m_Index].GetBuffer(), orig.GetBuffer(), orig.GetDataSize());
		} else {
			// TODO: mark error. Should we throw an exception from this constructor?
		}
	}
}

CUnit& CUnit::operator=(const CUnit& right)
{
	if(this == &right) {
		return *this;
	}

	size_t len = right.GetDataSize();
	if (m_Index == INVALID_STORE_INDEX && right.m_Index != INVALID_STORE_INDEX)	{
		m_Index = m_Store.RequestIndex(len);
		if (m_Index == INVALID_STORE_INDEX)	{
			// TODO: mark error.
		}
	} else
	if (right.m_Index != INVALID_STORE_INDEX)	{
		signed8_t ret = m_Store[m_Index].Resize(len);
		if (ret != NO_ERRORS) {
			// TODO mark error
		}
		XL_ASSERT(ret == NO_ERRORS);
	}
	XL_ASSERT(right.m_Index != INVALID_STORE_INDEX && m_Index != INVALID_STORE_INDEX);
	if (right.m_Index != INVALID_STORE_INDEX && m_Index != INVALID_STORE_INDEX)	{
		XL_ASSERT(m_Store[m_Index].GetSize() >= len);
		memcpy(m_Store[m_Index].GetBuffer(), right.GetBuffer(), len);
		m_Store[m_Index].SetDataSize(len);
	}

	return *this;
}

// Default destructor
CUnit::~CUnit()
{
	ResetDataStorage();
}

void CUnit::ResetDataStorage(void)
{
	if (m_Index != INVALID_STORE_INDEX)	{
		XL_ASSERT(m_Index >= 0 ? !m_Store[m_Index].IsSticky() : 1);
		XL_ASSERT(m_Index < 0 ? m_Store[m_Index].IsSticky() : 1);
		if (m_Index >= 0) {
			m_Store[m_Index].Reset();
		}
	}
	m_Index = INVALID_STORE_INDEX;
}

const size_t CUnit::DefaultInflateSize = 10;

signed8_t CUnit::SetValueAt8(unsigned8_t newval, unsigned32_t index)
{
	signed8_t errcode = NO_ERRORS;

	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	size_t datasize = m_Store[m_Index].GetDataSize();

	if(data != NULL) {
		XL_ASSERT(m_Store[m_Index].GetSize() >= datasize);
		if (index < datasize) {
			data[index] = newval;
		} else {
			errcode = ERR_INVALID_INDEX;
		}
	} else {
		errcode = ERR_DATASTORAGE_EMPTY;
	}

	return errcode;
}

signed8_t CUnit::AddValue16(unsigned16_t newval)
{
	signed8_t errcode = NO_ERRORS;

	if(AddValue8(BYTE_0(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_1(newval))) { errcode = GENERAL_ERROR; }

	return errcode;
}

signed8_t CUnit::AddValue32(unsigned32_t newval)
{
	signed8_t errcode = NO_ERRORS;

	if(AddValue8(BYTE_0(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_1(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_2(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_3(newval))) { errcode = GENERAL_ERROR; }

	return errcode;
}

signed8_t CUnit::AddValue64(unsigned64_t newval)
{
	signed8_t errcode = NO_ERRORS;

	if(AddValue8(BYTE_0(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_1(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_2(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_3(newval))) { errcode = GENERAL_ERROR; }

	if(AddValue8(BYTE_4(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_5(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_6(newval))) { errcode = GENERAL_ERROR; }
	if(AddValue8(BYTE_7(newval))) { errcode = GENERAL_ERROR; }

	return errcode;
}

unsigned64_t CUnit::EncodeFP2I64(double newval)
{
//#include "common/xls_pshpack1.h"

	union
	{
		double f;
		unsigned64_t i;
		unsigned8_t b[8];
	} v;

//#include "common/xls_poppack.h"

	XL_ASSERT(sizeof(v.f) == sizeof(v.i));
	XL_ASSERT(sizeof(v.f) == sizeof(v.b));
	XL_ASSERT(sizeof(v) == sizeof(v.f));

	v.f = newval;

	return v.i;
}

signed8_t CUnit::AddValue64FP(double newval)
{
	unsigned64_t i = EncodeFP2I64(newval);

	return AddValue64(i);
}

signed8_t CUnit::SetValueAt16(unsigned16_t newval, unsigned32_t index)
{
	signed8_t errcode = NO_ERRORS;

	if(SetValueAt8(BYTE_0(newval), index  )) {errcode = GENERAL_ERROR; }
	if(SetValueAt8(BYTE_1(newval), index+1)) { errcode = GENERAL_ERROR; }

	return errcode;
}

signed8_t CUnit::SetValueAt32(unsigned32_t newval, unsigned32_t index)
{
	signed8_t errcode = NO_ERRORS;

	if(SetValueAt8(BYTE_0(newval), index  )) {errcode = GENERAL_ERROR; }
	if(SetValueAt8(BYTE_1(newval), index+1)) { errcode = GENERAL_ERROR; }
	if(SetValueAt8(BYTE_2(newval), index+2)) { errcode = GENERAL_ERROR; }
	if(SetValueAt8(BYTE_3(newval), index+3)) { errcode = GENERAL_ERROR; }

	return errcode;
}

signed8_t CUnit::GetValue16From(unsigned16_t* val, unsigned32_t index) const
{
	signed8_t errcode = NO_ERRORS;

	*val = (unsigned16_t)(operator[](index) +
						  operator[](index+1)*0x0100U);

	return errcode;
}

signed8_t CUnit::GetValue32From(unsigned32_t* val, unsigned32_t index) const
{
	signed8_t errcode = NO_ERRORS;
	// Yikes! this was signed16_t - DFH
	*val = (unsigned32_t)(operator[](index)*0x00000001U +
						  operator[](index+1)*0x00000100U +
						  operator[](index+2)*0x00010000U +
						  operator[](index+3)*0x01000000U); // Yikes again, it was
	return errcode;
}

signed8_t CUnit::GetValue8From(unsigned8_t* dst, unsigned32_t index) const
{
	signed8_t errcode = NO_ERRORS;

	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	size_t datasize = m_Store[m_Index].GetDataSize();

	if(dst != NULL) {
		XL_ASSERT(m_Store[m_Index].GetSize() >= datasize);
		if (index < datasize) {
			*dst = data[index];
		} else {
			errcode = ERR_INVALID_INDEX;
		}
	} else {
		errcode = ERR_DATASTORAGE_EMPTY;
	}

	return errcode;
}

signed8_t CUnit::AddDataArray(const unsigned8_t* newdata, size_t size)
{
	signed8_t errcode = NO_ERRORS;

	if (m_Index == INVALID_STORE_INDEX)	{
		m_Index = m_Store.RequestIndex(size);
		if (m_Index == INVALID_STORE_INDEX)	{
			return GENERAL_ERROR;
		}
	}

	XL_ASSERT(GetSize() >= GetDataSize());
	size_t spaceleft = GetSize() - GetDataSize();

	if(spaceleft < size) { // allocate more space if new to-be-added array won't fit
		errcode = Inflate(size + GetDataSize());
		if (errcode != NO_ERRORS) {
			return errcode;
		}
	}

	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	size_t datasize = m_Store[m_Index].GetDataSize();

	if(newdata != NULL) {
		// TODO: memmove() (not memcpy() as can be called from Append() <-- this += *this; fringe case where memcpy() would possibly fail!
		for(size_t i=0; i<size; i++) {
			XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
			data[datasize++] = newdata[i];
		}

		m_Store[m_Index].SetDataSize(datasize);
	} else {
		//No data to add. Do nothing
		if (size != 0) {
			return GENERAL_ERROR; // [i_a] at least report this very suspicious situation
		}
	}

	return errcode;
}

signed8_t CUnit::AddFixedDataArray(const unsigned8_t value, size_t size)
{
	signed8_t errcode = NO_ERRORS;

	if (m_Index == INVALID_STORE_INDEX)	{
		m_Index = m_Store.RequestIndex(size);
		if (m_Index == INVALID_STORE_INDEX)	{
			return GENERAL_ERROR;
		}
	}

	XL_ASSERT(GetSize() >= GetDataSize());
	size_t spaceleft = GetSize() - GetDataSize();

	if(spaceleft < size) { // allocate more space if new to-be-added array won't fit
		errcode = Inflate(size + GetDataSize());
		if (errcode != NO_ERRORS) {
			return errcode;
		}
	}

	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	size_t datasize = m_Store[m_Index].GetDataSize();

	// The following can be a memset
	for(size_t i=0; i<size; i++) {
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = value;
	}

	m_Store[m_Index].SetDataSize(datasize);

	return errcode;
}

//    signed8_t AddDataArray (const unsigned16_t* newdata, size_t size);
//    signed8_t AddFixedDataArray (const unsigned16_t value, size_t size);

signed8_t CUnit::SetArrayAt(const unsigned8_t* newdata, size_t size, unsigned32_t index)
{
	signed8_t errcode = NO_ERRORS;
	size_t space = GetSize();

	if(space < size + index) { // allocate more space if new to-be-added array won't fit
		errcode = Inflate(size + index);
		if (errcode != NO_ERRORS) {
			return errcode;
		}
	}

	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	//size_t datasize = m_Store[m_Index].GetDataSize();

	if (newdata != NULL) {
		for (size_t i=0; i<size; i++) {
			XL_ASSERT(m_Store[m_Index].GetSize() > index);
			data[index++] = newdata[i];
		}
	}

	return errcode;
}

signed8_t CUnit::AddValue8(unsigned8_t newdata)
{
	XL_ASSERT(GetSize() >= GetDataSize());
	if(GetDataSize() >= GetSize()) {
		signed8_t errcode = Inflate(GetDataSize() + 1);
		if (errcode != NO_ERRORS) {
			return errcode;
		}
	}

	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	size_t datasize = m_Store[m_Index].GetDataSize();

	data[datasize++] = newdata;

	m_Store[m_Index].SetDataSize(datasize);

	return NO_ERRORS;
}

signed8_t CUnit::AddUnicodeString(CGlobalRecords& gRecords, const std::string& str, XlsUnicodeStringFormat_t fmt)
{
	std::string::const_iterator	cBegin, cEnd;
	signed8_t errcode = NO_ERRORS;
	size_t strSize = 0;
	size_t strLen;
	size_t spaceleft;
	bool isASCII = CGlobalRecords::IsASCII(str);

	if (!isASCII) {
		u16string s16;

		XL_ASSERTS("Should never happen!");

		gRecords.char2str16(str, s16);
		return AddUnicodeString(s16, fmt);
	}

	strLen = str.length();

	switch (fmt) {
	case LEN2_FLAGS_UNICODE:			// RECTYPE_FORMAT, RECTYPE_LABEL -- 'regular'
		strSize = 2;
		strSize += 1;   // flags byte
		break;

	case LEN2_NOFLAGS_PADDING_UNICODE:	// RECTYPE_NOTE (RECTYPE_TXO)
		strSize = 2;
		strSize += (strLen % 1);    // padding byte
		break;

	case LEN1_FLAGS_UNICODE:			// RECTYPE_BOUNDSHEET
		strSize = 1;
		strSize += 1;   // flags byte
		break;

	case NOLEN_FLAGS_UNICODE:			// RECTYPE_NAME
		strSize = 0;
		strSize += 1;   // flags byte
		break;

	default:
		XL_ASSERTS("should never go here!");
		break;
	}
	strSize += strLen;

	XL_ASSERT(GetSize() >= GetDataSize());
	spaceleft = GetSize() - GetDataSize();
	if(spaceleft < strSize)	{ // allocate more space if new to-be-added array won't fit
		errcode = Inflate(strSize + GetDataSize());
		if (errcode != NO_ERRORS) {
			return errcode;
		}
	}

	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	size_t datasize = m_Store[m_Index].GetDataSize();
	XL_ASSERT(data);
	//XL_ASSERT(datasize > strSize);

	switch (fmt) {
	case LEN2_FLAGS_UNICODE: // RECTYPE_FORMAT, RECTYPE_LABEL -- 'regular'
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = strLen & 0xFF;
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = (strLen >> 8) & 0xFF;
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = 0x00; // ASCII
		break;

	case LEN2_NOFLAGS_PADDING_UNICODE: // RECTYPE_NOTE (RECTYPE_TXO)
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = strLen & 0xFF;
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = (strLen >> 8) & 0xFF;
		// the string is padded to be word-aligned with NUL bytes /preceding/ the text:
		if (strLen % 1) {
			XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
			data[datasize++] = 0x00; // padding
		}
		break;

	case LEN1_FLAGS_UNICODE: // RECTYPE_BOUNDSHEET
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = strLen & 0xFF;
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = 0x00; // ASCII
		break;

	case NOLEN_FLAGS_UNICODE: // RECTYPE_NAME
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = 0x00; // ASCII
		break;

	default:
		XL_ASSERTS("should never go here!");
		break;
	}

	cBegin	 = str.begin();
	cEnd = str.end();

	while(cBegin != cEnd) {
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = (unsigned8_t)*cBegin++;
	}

	m_Store[m_Index].SetDataSize(datasize);

	return errcode;
}

signed8_t CUnit::AddUnicodeString(const u16string& str16, XlsUnicodeStringFormat_t fmt)
{
	u16string::const_iterator cBegin, cEnd;
	signed8_t errcode = NO_ERRORS;
	size_t spaceleft;
	size_t strLen;
	bool isASCII;
	size_t strSize = UnicodeStringLength(str16, strLen, isASCII, fmt /* = LEN2_FLAGS_UNICODE */ );

	XL_ASSERT(GetSize() >= GetDataSize());
	spaceleft = GetSize() - GetDataSize();
	if(spaceleft < strSize)	{ // allocate more space if new to-be-added array won't fit
		errcode = Inflate(strSize + GetDataSize());
		if (errcode != NO_ERRORS) {
			return errcode;
		}
	}

	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	size_t datasize = m_Store[m_Index].GetDataSize();
	XL_ASSERT(data);
	//XL_ASSERT(datasize > strSize);

	switch (fmt) {
	case LEN2_FLAGS_UNICODE: // RECTYPE_FORMAT, RECTYPE_LABEL -- 'regular'
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = strLen & 0xFF;
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = (strLen >> 8) & 0xFF;
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = (isASCII ? 0x00 : 0x01); // ASCII or UTF-16
		break;

	case LEN2_NOFLAGS_PADDING_UNICODE: // RECTYPE_NOTE (RECTYPE_TXO)
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = strLen & 0xFF;
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = (strLen >> 8) & 0xFF;
		// the string is padded to be word-aligned with NUL bytes /preceding/ the text:
		if (isASCII && (strLen % 1)) {
			XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
			data[datasize++] = 0x00; // padding
		}
		break;

	case LEN1_FLAGS_UNICODE: // RECTYPE_BOUNDSHEET
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = strLen & 0xFF;
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = (isASCII ? 0x00 : 0x01); // ASCII or UTF-16
		break;

	case NOLEN_FLAGS_UNICODE: // RECTYPE_NAME
		XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
		data[datasize++] = (isASCII ? 0x00 : 0x01); // ASCII or UTF-16
		break;

	default:
		XL_ASSERTS("should never go here!");
		break;
	}

	cBegin	= str16.begin();
	cEnd	= str16.end();

	if (isASCII) {
		while(cBegin != cEnd) {
			unsigned16_t c = *cBegin++;

			XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
			data[datasize++] = static_cast<unsigned8_t>(c);
		}
	} else {
		while(cBegin != cEnd) {
			unsigned16_t c = *cBegin++;

			XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
			data[datasize++] = c & 0xFF;
			XL_ASSERT(m_Store[m_Index].GetSize() > datasize);
			data[datasize++] = (c >> 8) & 0xFF;
		}
	}
	m_Store[m_Index].SetDataSize(datasize);

	return errcode;
}

size_t CUnit::UnicodeStringLength(const u16string& str16, size_t& strLen, bool& isASCII, XlsUnicodeStringFormat_t fmt /* = LEN2_FLAGS_UNICODE */ )
{
	strLen = str16.length();
	isASCII = CGlobalRecords::IsASCII(str16);
	size_t strSize = strLen;

	switch (fmt) {
	case LEN2_FLAGS_UNICODE:			// RECTYPE_FORMAT, RECTYPE_LABEL -- 'regular'
		strSize += 2;
		strSize += 1;   // flags byte
		if (!isASCII) {
			strSize += strLen;  // UTF16 takes 2 bytes per char
		}
		break;

	case LEN2_NOFLAGS_PADDING_UNICODE:	// RECTYPE_NOTE (RECTYPE_TXO)
		strSize += 2;
		if (isASCII) {
			strSize += (strLen % 1);    // padding byte
		} else { // if (!isASCII)
			strSize += strLen;  // UTF16 takes 2 bytes per char
		}
		break;

	case LEN1_FLAGS_UNICODE:			// RECTYPE_BOUNDSHEET
		strSize += 1;
		strSize += 1;   // flags byte
		if (!isASCII) {
			strSize += strLen;  // UTF16 takes 2 bytes per char
		}
		break;

	case NOLEN_FLAGS_UNICODE:			// RECTYPE_NAME
		//strSize = 0;
		strSize += 1;   // flags byte
		if (!isASCII) {
			strSize += strLen;  // UTF16 takes 2 bytes per char
		}
		break;

	default:
		XL_ASSERTS("should never go here!");
		break;
	}

	return strSize;
}

signed8_t CUnit::Inflate(size_t newsize)
{
	signed8_t errcode = NO_ERRORS;

	if (m_Index == INVALID_STORE_INDEX)	{
		XL_ASSERT(newsize > 0);
		m_Index = m_Store.RequestIndex(newsize);
		if (m_Index == INVALID_STORE_INDEX)	{
			return GENERAL_ERROR;
		}
	} else {
		XL_ASSERT(newsize > 0);
#if 0
		{
			size_t oldlen = m_Store[m_Index].GetSize();
			if (oldlen < 64) {
				increase = CUnit::DefaultInflateSize;
			} else {
				// bigger units grow faster: save on the number of realloc redimension operations...
				increase = oldlen / 2;
			}
		}
#endif
		XL_ASSERT(m_Index != INVALID_STORE_INDEX);
		errcode = m_Store[m_Index].Resize(newsize);
	}

	return errcode;
}

unsigned8_t& CUnit::operator[](const size_t index) const
{
	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	unsigned8_t *data = m_Store[m_Index].GetBuffer();
	//size_t datasize = m_Store[m_Index].GetDataSize();

	XL_ASSERT(index < GetSize());       // DFH: need to read ahead when setting bits in 32bit words
	XL_ASSERT(index < GetDataSize());   // [i_a]
	//if(index >= datasize) printf("ERROR: Short read!! \n");

	return data[index];
}

CUnit& CUnit::operator +=(const CUnit& from)
{
	XL_VERIFY(NO_ERRORS == Append(from));

	return *this;
}

CUnit& CUnit::operator +=(unsigned8_t from)
{
	XL_VERIFY(NO_ERRORS == AddValue8(from));

	return *this;
}

signed8_t CUnit::Init(const unsigned8_t* data, const size_t size, const unsigned32_t datasz)
{
	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	signed8_t ret = m_Store[m_Index].Init(data, size, datasz);
	return ret;
}

signed8_t CUnit::Append(const CUnit& newunit)
{
	XL_ASSERT(GetSize() >= GetDataSize());
	size_t spaceleft = GetSize() - GetDataSize();
	if(spaceleft < newunit.GetDataSize()) { // allocate more space if new to-be-added array won't fit
		signed8_t errcode = Inflate(GetDataSize() + newunit.GetDataSize());
		if (errcode != NO_ERRORS) {
			return errcode;
		}
	}

	return AddDataArray(newunit.GetBuffer(), newunit.GetDataSize());
}

signed8_t CUnit::InitFill(unsigned8_t data, size_t size)
{
	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	return m_Store[m_Index].InitWithValue(data, size);
}

size_t CUnit::GetSize(void) const
{
	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	return m_Store[m_Index].GetSize();
}

size_t CUnit::GetDataSize(void) const
{
	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	return m_Store[m_Index].GetDataSize();
}

const unsigned8_t* CUnit::GetBuffer(void) const
{
	XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	return m_Store[m_Index].GetBuffer();
}
