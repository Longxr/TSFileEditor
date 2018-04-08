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

#include "xlslib/index.h"
#include "xlslib/datast.h"
#include "xlslib/rectypes.h"

using namespace std;
using namespace xlslib_core;

/*
 ******************************
 * CIndex class implementation
 ******************************
 */
CIndex::CIndex(CDataStorage &datastore, unsigned32_t firstrow, unsigned32_t lastrow) :
	CRecord(datastore)
{
	m_Backpatching_Level = 2;

	SetRecordType(RECTYPE_INDEX);

	AddValue32(INDEX_DFLT_RESERVED);

	AddValue32(firstrow);
	AddValue32(lastrow+1);

//	AddValue32(INDEX_DFLT_RESERVED); (now written when outputting sheet)

	// NOTE: This record is created with an empty array. It should work if the rest
	// of the record is not completed later, since the record's size reflects the
	// lack of such array
	SetRecordLength(GetDataSize()-RECORD_HEADER_SIZE);
}

CIndex::~CIndex()
{
}

signed8_t CIndex::AddDBCellOffset(size_t dboffset)
{
	signed8_t errcode = AddValue32((unsigned32_t)dboffset);

	if (errcode != NO_ERRORS) {
		return errcode;
	}

	return SetRecordLength(GetDataSize()-RECORD_HEADER_SIZE); // Update record's length
}

void CIndex::SetRows(unsigned32_t firstrow, unsigned32_t lastrow)
{
	XL_VERIFY(NO_ERRORS == SetValueAt32((unsigned32_t)firstrow, INDEX_OFFSET_B8FIRSTROW)); // [i_a]
	XL_VERIFY(NO_ERRORS == SetValueAt32((unsigned32_t)(lastrow+1), INDEX_OFFSET_B8LASTROW)); // [i_a]
}

unsigned32_t CIndex::GetFirstRow(void)
{
	unsigned32_t firstrow;

	XL_VERIFY(NO_ERRORS == GetValue32From(&firstrow, INDEX_OFFSET_B8FIRSTROW)); // [i_a]

	return firstrow;
}

unsigned32_t CIndex::GetLastRow(void)
{
	unsigned32_t lastrow;

	XL_VERIFY(NO_ERRORS == GetValue32From(&lastrow, INDEX_OFFSET_B8LASTROW)); // [i_a]

	return lastrow;
}
