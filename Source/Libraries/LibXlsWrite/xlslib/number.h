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

#ifndef NUMBER_H
#define NUMBER_H

#include "common/xlsys.h"
#include "common/systype.h"

#include "xlslib/cell.h"	// superclass


// #include "common/xls_pshpack2.h"

namespace xlslib_core
{
	class number_t : public cell_t
	{
		friend class worksheet;
		//friend class CNumber; // ::CNumber(number_t& blankdef);

	private:
		number_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval,
				 double numval, xf_t* pxfval = NULL);
		// 536870911 >= numval >= -536870912
		number_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval,
				 signed32_t numval, xf_t* pxfval = NULL);
		number_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval,
				 unsigned32_t numval, xf_t* pxfval = NULL);
		virtual ~number_t(){}

		virtual size_t GetSize(void) const {return isDouble ? 18 : 14; }
		virtual CUnit* GetData(CDataStorage &datastore) const;

	private:
		bool isDouble;
		union
		{
			double dblNum;
			signed32_t intNum;
		} num;

	public:
		// idea is by passing either an int or a double, we can get the proper conversion (which might be none)
		double GetNumber(double unused
#ifdef __GNUC__
						 __attribute__((unused))
#endif
						 ) const {return isDouble ? num.dblNum : (double)num.intNum; }
		signed32_t GetNumber(signed32_t unused
#ifdef __GNUC__
							 __attribute__((unused))
#endif
							 ) const {return isDouble ? (signed32_t)num.dblNum : num.intNum; }
		bool GetIsDouble() const { return isDouble; }
		double GetDouble() const {return isDouble ? num.dblNum : (double)num.intNum; }
		signed32_t GetInt() const {return isDouble ? (signed32_t)num.dblNum : num.intNum; }
	};


	// forward ref
	class CDataStorage;

	class CNumber : public CRecord
	{
		friend class CDataStorage;

	protected:
		CNumber(CDataStorage &datastore, const number_t& numdef);
	private:
		virtual ~CNumber();
	};
}


// #include "common/xls_poppack.h"

#endif
