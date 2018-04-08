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

#ifndef WORKBOOK_H
#define WORKBOOK_H

#include "../common/xlsys.h"
#include "../common/systype.h"

// The following includes are for instanciated objects
#include "../xlslib/summinfo.h"
#include "../xlslib/docsumminfo.h"
#include "../xlslib/formula_expr.h"
#include "../xlslib/globalrec.h"
#include "../xlslib/sheetrec.h"
#include "../libxlswrite_global.h"

// #include "common/xls_pshpack2.h"

#define XLSLIB_VERSION PACKAGE_VERSION

namespace xlslib_core
{
	typedef enum
	{
		WB_INIT,
		WB_GLOBALRECORDS,
		WB_SHEETS,
		WB_CONTINUE_REC,
		WB_FINISH
	} WorkbookDumpState_t;

	class CSummaryInfo;
	class CDocSummaryInfo;

    class LIBXLSWRITESHARED_EXPORT workbook
	{
	public:
		workbook();
		virtual ~workbook();

		const char*	version() const { return XLSLIB_VERSION; }

		worksheet*	sheet(const std::string& sheetname);
		worksheet*	sheet(const xlslib_strings::ustring& sheetname);
		worksheet*	GetSheet(unsigned16_t sheetnum);

		expression_node_factory_t& GetFormulaFactory(void);

		font_t*		font(unsigned8_t fontnum);          // use as a way to get a font to modify
		font_t*		font(const std::string& name);
		format_t*	format(const std::string& formatstr);
		format_t*	format(const xlslib_strings::ustring& formatstr);

		bool		setColor(unsigned8_t r, unsigned8_t g, unsigned8_t b, unsigned8_t idx);

		xf_t*		xformat(void);
		xf_t*		xformat(font_t* font);
		xf_t*		xformat(format_t *format);
		xf_t*		xformat(font_t* font, format_t *format);

#ifdef HAVE_WORKING_ICONV
		int			iconvInType(const char *inType);
#endif

		bool		property(property_t prop, const std::string& content);

		void		windPosition(unsigned16_t horz, unsigned16_t vert);
		void		windSize(unsigned16_t width, unsigned16_t height);
		void		firstTab(unsigned16_t firstTab);
		void		tabBarWidth(unsigned16_t width);

		int			Dump(const std::string& filename);

	private:
		workbook(const workbook& that);
		workbook& operator=(const workbook& right);

	private:
		CUnit*		DumpData(CDataStorage &datastore);   // oledoc use

	private:
		CGlobalRecords				m_GlobalRecords;
		expression_node_factory_t	m_ExprFactory;

		CSummaryInfo				m_SummaryInfo;
		CDocSummaryInfo				m_DocSummaryInfo;
		Sheets_Vector_t				m_Sheets;
		WorkbookDumpState_t			m_DumpState;
		WorkbookDumpState_t			m_PreviousDumpState;
		unsigned16_t				m_sheetIndex;

		CUnit*						m_pCurrentData;

		// Continue record variables:
		CUnit*						m_pContinueRecord;
		size_t						m_ContinuesRealRecordSize;
		unsigned16_t				m_ContinueIndex;

		// INDEX / DBCELL assistant variables:
		size_t						m_writeLen;
		size_t						m_offset;
		unsigned16_t				m_current_sheet;
	};
}

// #include "common/xls_poppack.h"

#endif
