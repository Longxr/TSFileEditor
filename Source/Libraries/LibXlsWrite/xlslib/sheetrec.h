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

#ifndef SHEETREC_H
#define SHEETREC_H

#include "../common/xlsys.h"
#include "../common/systype.h"
#include "../xlslib/colinfo.h"
#include "../xlslib/row.h"
#include "../libxlswrite_global.h"

// #include "common/xls_pshpack2.h"

namespace xlslib_core
{
	/*
	 ***********************************
	 *  worksheet class declaration
	 ***********************************
	 */
	class cell_t;
	class range;
	typedef std::vector<xlslib_core::range* XLSLIB_DFLT_ALLOCATOR> RangeObj_Vect_t;
	typedef RangeObj_Vect_t::iterator RangeObj_Vect_Itor_t;
	class range_t;
	typedef std::vector<xlslib_core::range_t* XLSLIB_DFLT_ALLOCATOR> Range_Vect_t;

	class expression_node_t;

	typedef enum
	{
		SHEET_INIT,
		SHEET_BOF,
		SHEET_INDEX,
		SHEET_DFLT_ROW_HEIGHT,
		SHEET_COLINFO,
		SHEET_DFLT_COL_WIDTH,
		SHEET_DIMENSION,
		SHEET_ROWBLOCKS,
		SHEET_MERGED,
		SHEET_WINDOW2,
		SHEET_H_LINKS,
		SHEET_VALIDITY_HEADER,
		SHEET_VALIDITY_BODY,
		SHEET_EOF,
		SHEET_FINISH
	} SheetRecordDumpState_t;

	typedef struct rowblocksize_t
	{
		size_t rowandcell_size;
		size_t dbcell_size;
		size_t rows_sofar;
		size_t cells_sofar;

		unsigned32_t first_col;
		unsigned32_t last_col;
		unsigned32_t first_row;
		unsigned32_t last_row;

		// -------------------------------------

		rowblocksize_t() :
			rowandcell_size(0),
			dbcell_size(0),
			rows_sofar(0),
			cells_sofar(0),
			first_col((unsigned32_t)(-1)),
			last_col(0),
			first_row((unsigned32_t)(-1)),
			last_row(0)
		{
		}

		void reset(void)
		{
			rowandcell_size = 0;
			dbcell_size = 0;
			rows_sofar = 0;
			cells_sofar = 0;

			first_col = (unsigned32_t)(-1);
			last_col = 0;
			first_row = (unsigned32_t)(-1);
			last_row = 0;
		}
	} rowblocksize_t;

	typedef std::vector<xlslib_core::rowblocksize_t XLSLIB_DFLT_ALLOCATOR> RBSize_Vect_t;
	typedef RBSize_Vect_t::iterator RBSize_Vect_Itor_t;

	typedef enum
	{
		RB_INIT,
		RB_FIRST_ROW,
		RB_ROWS,
		RB_FIRSTCELL,
		RB_CELLS,
		RB_DBCELL,
		RB_FINISH
	} DumpRowBlocksState_t;

	typedef std::vector<size_t XLSLIB_DFLT_ALLOCATOR> CellOffsets_Vect_t;
	typedef CellOffsets_Vect_t::iterator CellOffsets_Vect_Itor_t;

    enum {
        DVAL_TYPE_ANY = 0x00,
        DVAL_TYPE_INTEGER = 0x01,
        DVAL_TYPE_DECIMAL = 0x02,
        DVAL_TYPE_LIST = 0x03,
        DVAL_TYPE_DATE = 0x04,
        DVAL_TYPE_TIME = 0x05,
        DVAL_TYPE_LENGTH = 0x06,
        DVAL_TYPE_FORMULA = 0x07
    };

    enum {
        DVAL_ERROR_STOP = 0x00,
        DVAL_ERROR_WARNING = 0x10,
        DVAL_ERROR_INFO = 0x20,
    };

    enum {
        DVAL_STRING_LIST_IN_FORMULA = 0x0080,
        DVAL_EMPTY_OK = 0x0100,
        DVAL_STRING_LIST_SUPPRESS_DROPDOWN = 0x0200,
        DVAL_SHOW_PROMPT_IF_SELECTED = 0x040000,
        DVAL_SHOW_ERROR_IF_INVALID   = 0x080000
    };

    enum {
        DVAL_OP_BETWEEN          = 0x000000,
        DVAL_OP_NOT_BETWEEN      = 0x100000,
        DVAL_OP_EQUAL            = 0x200000,
        DVAL_OP_NOT_EQUAL        = 0x300000,
        DVAL_OP_GREATER_THAN     = 0x400000,
        DVAL_OP_LESS_THAN        = 0x500000,
        DVAL_OP_GREATER_OR_EQUAL = 0x600000,
        DVAL_OP_LESS_OR_EQUAL    = 0x700000,
    };

	struct DataValidation
	{
        unsigned32_t				first_row;
        unsigned32_t				last_row;
        unsigned32_t				first_col;
        unsigned32_t				last_col;
		unsigned32_t				options;
		xlslib_strings::u16string	prompt_title;
		xlslib_strings::u16string	prompt_text;
		xlslib_strings::u16string	error_title;
		xlslib_strings::u16string	error_text;
        const formula_t				*cond1;
        const formula_t				*cond2;
	};

	typedef std::vector<xlslib_core::DataValidation * XLSLIB_DFLT_ALLOCATOR> DataValidationList_t;
	typedef DataValidationList_t::iterator DataValidationList_Itor_t;

	struct HyperLink
	{
		unsigned16_t row;
		unsigned16_t col;
		xlslib_strings::u16string url;
		xlslib_strings::u16string mark;
	};
	typedef std::vector<xlslib_core::HyperLink * XLSLIB_DFLT_ALLOCATOR> HyperLinkList_t;
	typedef HyperLinkList_t::iterator HyperLinkList_Itor_t;

	typedef std::vector<xlslib_core::CUnit* XLSLIB_DFLT_ALLOCATOR> ColInfo_t;
	typedef ColInfo_t::iterator ColInfo_Itor_t;

	typedef std::vector<xlslib_core::formula_t * XLSLIB_DFLT_ALLOCATOR> FormulaStackList_t;
	typedef FormulaStackList_t::iterator FormulaStackList_Itor_t;

    class LIBXLSWRITESHARED_EXPORT worksheet : public CBiffSection
	{
		friend class workbook;

	private:
		CGlobalRecords&			m_GlobalRecords;
		SheetRecordDumpState_t m_DumpState;
		CUnit*					m_pCurrentData;

		Range_Vect_t m_MergedRanges;

		Colinfo_Set_t m_Colinfos;
		Colinfo_Set_Itor_t m_Current_Colinfo;

		RowHeight_Vect_t m_RowHeights;
		RowHeight_Vect_Itor_t m_Current_RowHeight;

		unsigned32_t minRow, minCol, maxRow, maxCol;
		unsigned16_t sheetIndex;

		Cell_Set_t m_Cells;
		Cell_Set_Itor_t	m_CurrentCell;              // Init this one in the RowBlocksDump INIT state
		Cell_Set_Itor_t	m_CurrentSizeCell;          // Init this one in the INIT state
		//bool m_CellsSorted;

		RangeObj_Vect_t	m_Ranges;
		RBSize_Vect_t m_RBSizes;
		RBSize_Vect_Itor_t m_Current_RBSize;
		bool m_SizesCalculated;

		DumpRowBlocksState_t m_DumpRBState;
		unsigned32_t m_RowCounter;
		unsigned32_t m_CellCounter;
		size_t m_DBCellOffset;
		size_t m_FirstRowOffset;
		CellOffsets_Vect_t m_CellOffsets;

		Cell_Set_Itor_t	m_Starting_RBCell;

		// cache a bit for speedups
		Cell_Set_Itor_t	cellIterHint;
		cell_t*					cellHint;

		bool defRowsHidden;
		unsigned16_t defRowHeight;
		unsigned16_t defColWidth;

        FormulaStackList_t m_FormulaStacks;

        DataValidationList_t m_DataValidations;
        DataValidationList_Itor_t m_CurrentDval;

		HyperLinkList_t	m_HyperLinks;
		HyperLinkList_Itor_t m_CurrentHlink;

	private:
		worksheet(CGlobalRecords& gRecords, unsigned16_t idx);
		~worksheet();

		//void					GetFirstLastRows(unsigned32_t* first_row, unsigned32_t* last_row);
		size_t					GetNumRowBlocks(rowblocksize_t* rbsize_ref = NULL);
		bool					GetRowBlockSizes(rowblocksize_t& rbsize);
		CUnit*					RowBlocksDump(CDataStorage &datastore, const size_t offset);
		size_t					EstimateNumBiffUnitsNeeded(void);

		void					AddCell(cell_t* pcell);
		CUnit*					DumpData(CDataStorage &datastore, size_t offset, size_t writeLen /*, size_t &Last_BOF_offset*/);

		CUnit*					MakeDataValidationHeader(CDataStorage& datastore, unsigned32_t dval_count);
		CUnit*					MakeDataValidationEntry(CDataStorage& datastore, DataValidation* dval);
		CUnit*					MakeHyperLink(CDataStorage& datastore, HyperLink* link);

	private:
		worksheet(const worksheet& that);
		worksheet& operator=(const worksheet& right);

	public:
		void					MakeActive();   // makes this sheet come up first
		size_t					NumCells() const { return m_Cells.size(); }
		unsigned16_t            GetIndex() const { return sheetIndex; }

		cell_t*					FindCell(unsigned32_t row, unsigned32_t col) const;
		cell_t*					FindCellOrMakeBlank(unsigned32_t row, unsigned32_t col);

		void					GetFirstLastRowsAndColumns(unsigned32_t* first_row, unsigned32_t* last_row, unsigned32_t* first_col, unsigned32_t* last_col); /* [i_a] */

		// Cell operations
		void merge(unsigned32_t first_row, unsigned32_t first_col,
				   unsigned32_t last_row, unsigned32_t last_col);
		void colwidth(unsigned32_t col, unsigned16_t width, xf_t* pxformat = NULL);				// sets column widths to 1/256 x width of "0"
		void rowheight(unsigned32_t row, unsigned16_t heightInTwips, xf_t* pxformat = NULL);	// twips

		void defaultRowHeight(unsigned16_t width, bool hidden = false) { defRowHeight = width; defRowsHidden = hidden; } // sets column widths to 1/256 x width of "0"
		void defaultColwidth(unsigned16_t width) { defColWidth = width; } // in points (Excel uses twips, 1/20th of a point, but xlslib didn't)

        formula_t* formula_data();

		// Ranges
		range* rangegroup(unsigned32_t row1, unsigned32_t col1,
						  unsigned32_t row2, unsigned32_t col2);
		// Cells
		cell_t* blank(unsigned32_t row, unsigned32_t col,
					  xf_t* pxformat = NULL);

		cell_t* label(unsigned32_t row, unsigned32_t col,
					  const std::string& strlabel, xf_t* pxformat = NULL);
		cell_t* label(unsigned32_t row, unsigned32_t col,
					  const xlslib_strings::ustring& strlabel, xf_t* pxformat = NULL);

		cell_t* number(unsigned32_t row, unsigned32_t col,
					   double numval, format_number_t fmtval, xf_t* pxformat);  // Deprecated
		cell_t* number(unsigned32_t row, unsigned32_t col,
					   double numval, xf_t* pxformat = NULL);
		// 536870911 >= numval >= -536870912
		cell_t* number(unsigned32_t row, unsigned32_t col,
					   signed32_t numval, xf_t* pxformat = NULL);
		cell_t* number(unsigned32_t row, unsigned32_t col,
					   unsigned32_t numval, xf_t* pxformat = NULL);

		cell_t* boolean(unsigned32_t row, unsigned32_t col,
						bool boolval, xf_t* pxformat = NULL);

		cell_t* error(unsigned32_t row, unsigned32_t col,
					  errcode_t errorcode, xf_t* pxformat = NULL);

		cell_t* note(unsigned32_t row, unsigned32_t col,
					 const std::string& remark, const std::string& author, xf_t* pxformat = NULL);
		cell_t* note(unsigned32_t row, unsigned32_t col,
					 const xlslib_strings::ustring& remark, const xlslib_strings::ustring& author, xf_t* pxformat = NULL);

		cell_t* formula(unsigned32_t row, unsigned32_t col,
						expression_node_t* expression_root, bool auto_destruct_expression_tree = false,
						xf_t* pxformat = NULL);
		cell_t* formula(unsigned32_t row, unsigned32_t col,
						formula_t *formula, xf_t* pxformat = NULL);

        void validate(const range_t *crange, unsigned32_t options,
                const formula_t *cond1 = NULL, const formula_t *cond2 = NULL,
                const std::string& promptTitle = std::string(), const std::string& promptText = std::string(),
                const std::string& errorTitle = std::string(), const std::string& errorText = std::string());
        void validate(const range_t *crange, unsigned32_t options,
                const formula_t *cond1 = NULL, const formula_t *cond2 = NULL,
                const xlslib_strings::ustring& promptTitle = xlslib_strings::ustring(), const xlslib_strings::ustring& promptText = xlslib_strings::ustring(),
                const xlslib_strings::ustring& errorTitle = xlslib_strings::ustring(), const xlslib_strings::ustring& errorText = xlslib_strings::ustring());

		// define a cell (label, number, etc) - apply proper url (http://blah.blah), possible text mark too (minus the '#')
		void hyperLink(const cell_t *cell, const std::string& url, const std::string& mark = std::string());
		void hyperLink(const cell_t *cell, const xlslib_strings::ustring& url, const xlslib_strings::ustring& mark = xlslib_strings::ustring());
	};

	typedef std::vector<xlslib_core::worksheet*> Sheets_Vector_t;
	typedef Sheets_Vector_t::iterator Sheets_Vector_Itor_t;
}

// #include "common/xls_poppack.h"

#endif
