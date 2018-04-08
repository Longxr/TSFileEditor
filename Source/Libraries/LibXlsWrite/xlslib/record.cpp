/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2011 David Hoerl All Rights Reserved.
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

#include "xlslib/record.h"
#include "xlslib/rectypes.h"


using namespace std;
using namespace xlslib_core;

/*
 ******************************
 * CRecord class implementation
 ******************************
 */
CRecord::CRecord(CDataStorage &datastore) :
	CUnit(datastore)
{
	static const unsigned8_t array[] = {0, 0, 0, 0};
	// Initialize (and create) the space for record type
	// and record length
	XL_VERIFY(NO_ERRORS == AddDataArray(array, 4));
}

CRecord::~CRecord()
{
}

signed8_t CRecord::SetRecordType(unsigned16_t rtype)
{
	return SetValueAt16(rtype, 0);
}

unsigned16_t CRecord::GetRecordType() const
{
	unsigned16_t value;

	XL_VERIFY(NO_ERRORS == GetValue16From(&value, 0));

	return value;
}

signed8_t CRecord::SetRecordLength(size_t rlength)
{
	return SetValueAt16((unsigned16_t)rlength, 2);
}

signed8_t CRecord::SetRecordTypeIndexed(unsigned16_t rtype, size_t index)
{
	return SetValueAt16(rtype, index);
}

signed8_t CRecord::SetRecordLengthIndexed(size_t rlength, size_t index)
{
	return SetValueAt16((unsigned16_t)rlength, index+2);
}

size_t CRecord::GetRecordLength() const
{
	unsigned16_t value;

	XL_VERIFY(NO_ERRORS == GetValue16From(&value, 2));

	return value;
}

const unsigned8_t* CRecord::GetRecordDataBuffer() const
{
	return GetBuffer() + RECORD_HEADER_SIZE;
}

/*
 ******************************
 ******************************
 */
size_t CRecord::GetRecordDataSize() const
{
	size_t len = GetDataSize() - RECORD_HEADER_SIZE;

	return len;
}
