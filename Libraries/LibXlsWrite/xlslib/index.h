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

#ifndef INDEX_H
#define INDEX_H

#include "common/xlsys.h"
#include "common/systype.h"

#include "xlslib/record.h"	// superclass


// #include "common/xls_pshpack2.h"

namespace xlslib_core
{
	/*
	 ******************************
	 * CIndex class declaration
	 ******************************
	 */
#define INDEX_DFLT_RESERVED 0x00000000

#define INDEX_OFFSET_B8FIRSTROW 8
#define INDEX_OFFSET_B8LASTROW  12

	// forward ref
	class CDataStorage;

	class CIndex : public CRecord
	{
		friend class CDataStorage;

	protected:
		CIndex(CDataStorage &datastore,
			   unsigned32_t firstrow,
			   unsigned32_t lastrow);
	private:
		virtual ~CIndex();

	public:
		signed8_t AddDBCellOffset(size_t dbcoffset);
		void SetRows(unsigned32_t firstrow, unsigned32_t lastrow);
		unsigned32_t GetFirstRow(void);
		unsigned32_t GetLastRow(void);
	};
}

// #include "common/xls_poppack.h"

#endif
//INDEX_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: index.h,v $
 * Revision 1.3  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
