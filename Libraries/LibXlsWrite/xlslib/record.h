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

#ifndef RECORD_H
#define RECORD_H

#include "../common/xlsys.h"
#include "../common/systype.h"

#include "../xlslib/unit.h"	// superclass


// #include "common/xls_pshpack2.h"

namespace xlslib_core
{
	// forward ref
	class CDataStorage;

	/*
	 ******************************
	 * CRecord class declaration
	 ******************************
	 */
	class CRecord : public CUnit
	{
		friend class CDataStorage;

	protected:
		//unsigned16_t m_Num;
		//unsigned32_t streamPos;

	protected:
		CRecord(CDataStorage &datastore);
		virtual ~CRecord();

	public:
		signed8_t SetRecordType(unsigned16_t rtype);
		unsigned16_t GetRecordType() const;

		signed8_t SetRecordLength(size_t len);
		size_t GetRecordLength() const;

		signed8_t SetRecordTypeIndexed(unsigned16_t rtype, size_t index);
		signed8_t SetRecordLengthIndexed(size_t len, size_t index);

		const unsigned8_t* GetRecordDataBuffer() const;
		size_t GetRecordDataSize() const;

		//void SetStreamPos(size_t pos) { streamPos = pos; };
		//size_t GetStreamPos() { return streamPos; };
	};
}

// #include "common/xls_poppack.h"

#endif
