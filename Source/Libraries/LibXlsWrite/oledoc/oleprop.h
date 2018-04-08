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

#ifndef OLEPROP_H
#define OLEPROP_H

#include <list>
#include <string>

#include "common/xlsys.h"

namespace xlslib_core
{
	class COleProp;

	typedef std::vector<xlslib_core::COleProp* XLSLIB_DFLT_ALLOCATOR> Tree_Level_Vect_t;
	typedef Tree_Level_Vect_t::iterator Tree_Level_Itor_t;

	typedef std::vector<xlslib_core::COleProp* XLSLIB_DFLT_ALLOCATOR> NodeList_t;
	typedef NodeList_t::iterator NodeList_Itor_t;

	/*
	 ******************************
	 * COleProp class declaration
	 ******************************
	 */
	class oleSort;
	class CDataStorage;

	class COleProp
	{
	protected:
		std::string m_sName;
		unsigned16_t m_nNameSize;
		unsigned8_t m_nPropType;
		unsigned8_t m_nNodeColor;

		signed32_t m_nPrevPropIndex;
		signed32_t m_nNextPropIndex;
		signed32_t m_nChildPropIndex;

		signed32_t m_nTSCreatedSeconds;
		signed32_t m_nTSCreatedDays;
		signed32_t m_nTSModifiedSeconds;
		signed32_t m_nTSModifiedDays;

		signed32_t m_nStartBlock;
		size_t m_nSize;

		// The following set of attributes are not part of the definition of
		// an OleDoc's property:
		signed32_t m_nIndex;
		CDataStorage* m_pData;

	private:
		COleProp(const COleProp& that);
		COleProp& operator=(const COleProp& right);

	public:
		Tree_Level_Vect_t m_Child_List;

		COleProp();
		COleProp(signed32_t node_index, const std::string& name, CDataStorage* data = NULL);
		COleProp(signed32_t node_index, const char *name, CDataStorage* data = NULL);
		~COleProp();

		int Init(signed32_t node_index, const std::string& name, CDataStorage* data = NULL);

		int SetName(const std::string& name);
		const std::string& GetName(void) const;

		int SetIndex(signed32_t newindex);
		inline signed32_t GetIndex(void) const {return m_nIndex; }

		int SetSize(size_t size);
		size_t GetSize(void) const;

		int SetType(unsigned8_t newtype);
		unsigned8_t GetType(void) const;

		int SetColor(unsigned8_t newcolor);
		unsigned8_t GetColor(void) const;

		int SetPreviousIndex(signed32_t prev);
		signed32_t GetPreviousIndex(void) const;

		int SetNextIndex(signed32_t next);
		signed32_t GetNextIndex(void) const;

		int SetChildIndex(signed32_t child);
		signed32_t GetChildIndex(void) const;

		int SetStartBlock(signed32_t sb);
		signed32_t GetStartBlock(void) const;

		void SetDataPointer(CDataStorage* pdata);
		CDataStorage* GetDataPointer(void) const;

		void SetCreatedSecs(signed32_t sec1);
		signed32_t GetCreatedSecs(void) const;

		void SetCreatedDays(signed32_t day1);
		signed32_t GetCreatedDays(void) const;

		void SetModifiedSecs(signed32_t sec2);
		signed32_t GetModifiedSecs(void) const;

		void SetModifiedDays(signed32_t day2);
		signed32_t GetModifiedDays(void) const;
	};
}

#endif
