/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2009 David Hoerl All Rights Reserved.
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

#ifndef FONT_H
#define FONT_H

#include "../common/xlsys.h"
#include "../common/systype.h"

#include "../xlslib/colors.h"	// need enum values


// #include "common/xls_pshpack2.h"

namespace xlslib_core
{
	// Bold style most used values
	// (it can be a number of some range. See Documentation for details):
#define FONT_BOLDNESS_BOLD			700
#define FONT_BOLDNESS_HALF			550
#define FONT_BOLDNESS_NORMAL		400
#define FONT_BOLDNESS_DOUBLE		800

	typedef enum
	{
		BOLDNESS_BOLD = 0,
		BOLDNESS_HALF,
		BOLDNESS_NORMAL,
		BOLDNESS_DOUBLE
	} boldness_option_t;


	// Super/subscript field option values
#define FONT_SCRIPT_NONE			0x0000
#define FONT_SCRIPT_SUPER			0x0001
#define FONT_SCRIPT_SUB				0x0002

	typedef enum
	{
		SCRIPT_NONE = 0,
		SCRIPT_SUPER,
		SCRIPT_SUB
	} script_option_t;

	// Underline field option values:
#define FONT_UNDERLINE_NONE			0x00
#define FONT_UNDERLINE_SINGLE		0x01
#define FONT_UNDERLINE_DOUBLE		0x02
#define FONT_UNDERLINE_SINGLEACC	0x21
#define FONT_UNDERLINE_DOUBLEACC	0x22

	typedef enum
	{
		UNDERLINE_NONE = 0,
		UNDERLINE_SINGLE,
		UNDERLINE_DOUBLE,
		UNDERLINE_SINGLEACC,
		UNDERLINE_DOUBLEACC
	} underline_option_t;



	// The following are default values used when the font's
	// constructor is called without args:
#define FONT_DFLT_FAMILY			0x00
// NONE (don't know, don't care)
#define FONT_DFLT_CHARSET			0x01
// 0 == ANSI Latin, 1 == System Default (this was 0x00 before 12/2008)
#define FONT_DFLT_HEIGHT			0x00c8
#define FONT_DFLT_ATTRIBUTES		0x0000
#define FONT_DFLT_PALETTE			0x7fff
// See Palette record - this is a special flag meaning the window color
#define FONT_DFLT_FONTNAME			std::string("Verdana")
// Was Arial before 12/2008

#define FONT_RESERVED				0x00

	// The font-record field offsets:
#define FONT_OFFSET_HEIGHT           4
#define FONT_OFFSET_ATTRIBUTES       6
#define FONT_OFFSET_PALETTE          8
#define FONT_OFFSET_BOLDSTYLE		10
#define FONT_OFFSET_SCRIPT			12
#define FONT_OFFSET_UNDERLINE		14
#define FONT_OFFSET_FAMILY			15
#define FONT_OFFSET_CHARSET			16
#define FONT_OFFSET_NAMELENGTH		18
#define FONT_OFFSET_NAME			19


	// The attribute bit or-masks:
#define FONT_ATTR_BOLD				0x0001
// documented as 'reserved' in the Microsoft Excel 2003 documentation!
#define FONT_ATTR_ITALIC			0x0002
#define FONT_ATTR_UNDERLINED		0x0004
// documented as 'reserved' in the Microsoft Excel 2003 documentation!
#define FONT_ATTR_STRIKEOUT			0x0008
#define FONT_ATTR_OUTLINEMACH		0x0010
#define FONT_ATTR_SHADOWMACH		0x0020
#define FONT_ATTR_CONDENSED			0x00c0
// documented as 'reserved' in the Microsoft Excel 2003 documentation!
#define FONT_ATTR_EXTENDED			0x0080
// documented as 'reserved' in the Microsoft Excel 2003 documentation!
#define FONT_ATTR_UNUSED			0xff00
// documented as 'reserved' in the Microsoft Excel 2003 documentation!

	/*
	 ******************************
	 * CFont class declaration
	 ******************************
	 */

	typedef struct
	{
		std::string name;
		unsigned16_t index;
		unsigned16_t height;
		boldness_option_t boldstyle;
		underline_option_t underline;
		script_option_t script;
		color_name_t color;
		unsigned16_t attributes;
		unsigned8_t family;
		unsigned8_t charset;
	} font_init_t;

	class CGlobalRecords;

	class font_i
	{
	public:
		font_i() { }
		virtual ~font_i() { }

		virtual void fontname(const std::string& fntname) = 0;
		virtual void fontheight(unsigned16_t fntheight) = 0;
		virtual void fontbold(boldness_option_t fntboldness) = 0;
		virtual void fontunderline(underline_option_t fntunderline) = 0;
		virtual void fontscript(script_option_t fntscript) = 0;
		virtual void fontcolor(color_name_t fntcolor) = 0;
		virtual void fontcolor(unsigned8_t fntcolor) = 0;
		virtual void fontitalic(bool italic) = 0;
		virtual void fontstrikeout(bool so) = 0;
		virtual void fontoutline(bool ol) = 0;
		virtual void fontshadow(bool sh) = 0;
	};

	class font_t
	{
		friend class CFont;
		friend class CGlobalRecords;
		friend class workbook;

	private:
		font_t(CGlobalRecords& gRecords);
		font_t(const font_t& right);
		font_t(CGlobalRecords& gRecords,
			   unsigned16_t index, const std::string& name,
			   unsigned16_t height, boldness_option_t boldstyle,
			   underline_option_t underline, script_option_t script,
			   color_name_t color, unsigned16_t attributes,
			   unsigned8_t family, unsigned8_t charset);
		virtual ~font_t() {}
		/* MSVC2005: C4512: 'xlslib_core::font_t' : assignment operator could not be generated */
		font_t &operator =(const font_t &src);

		/* FONT Index wrappers*/
		void      SetIndex(unsigned16_t fntidx);

	public:
		static font_t* fontDup(const font_t* orig)
		{
			font_t*	font = new font_t(*orig);
			return font;
		}

		void MarkUsed();
		void UnMarkUsed();
		unsigned32_t Usage() const;
		unsigned16_t GetIndex(void) const;

		/* FONT Index wrappers*/
		void   SetName(const std::string& fntname);
		const std::string& GetName(void) const;

		/* FONT height wrappers*/
		void         SetHeight(unsigned16_t fntheight);
		unsigned16_t GetHeight(void) const;

		/* FONT boldstyle wrappers*/
		void SetBoldStyle(boldness_option_t fntboldness);
		unsigned16_t GetBoldStyle(void) const;

		/* FONT underline wrappers*/
		void        SetUnderlineStyle(underline_option_t fntunderline);
		unsigned8_t GetUnderlineStyle(void) const;

		/* FONT script wrappers*/
		void         SetScriptStyle(script_option_t fntscript);
		unsigned16_t GetScriptStyle(void) const;

		/* FONT script wrappers*/
		void  SetColor(color_name_t fntcolor);
		void  SetColor(unsigned8_t fntcolor);
		unsigned16_t GetColorIdx(void) const;


		void SetItalic(bool italic);
		bool GetItalic() const;

		void SetStrikeout(bool so);
		bool GetStrikeout() const;

		// Mac only (old Mac???)
		void SetOutline(bool ol);
		bool GetOutline() const;
		void SetShadow(bool sh);
		bool GetShadow() const;

		// Miscellaneous: for super users
		void        SetFamily(unsigned8_t fam);
		unsigned8_t GetFamily(void) const;

		void         SetCharset(unsigned8_t chrset);
		unsigned8_t  GetCharset(void) const;

		//void operator=(font_t& right);

#if 0 // [i_a] xls C i/f & C++ facade export these?
	private:
#else
	public:
#endif
		/* FONT  attributes wrappers */
#if defined(DEPRECATED)
		/* [i_a] can cause reserved/illegal attribute bit combo's to be set; use SetOutline(), etc. instead. */
		void SetAttributes(unsigned16_t attr);
#endif
		unsigned16_t GetAttributes(void) const;

	public:
		CGlobalRecords& GetGlobalRecords(void) const { return m_GlobalRecords; }

	private:
		CGlobalRecords&   m_GlobalRecords;
		std::string	name;
		unsigned16_t index;

		unsigned16_t height;
		unsigned16_t boldstyle;
		unsigned16_t script;
		unsigned16_t attributes;
		unsigned16_t color;             // must handle 0x7FFF special font flag
		unsigned8_t	underline;
		unsigned8_t family;
		unsigned8_t charset;

		unsigned32_t m_usage_counter;

		static const unsigned16_t BOLD_OPTION_TABLE[];
		static const unsigned16_t SCRIPT_OPTION_TABLE[];
		static const unsigned8_t UNDERLINE_OPTION_TABLE[];
		static const unsigned8_t COLOR_OPTION_TABLE[];

		void _SetBoldStyle(unsigned16_t fntboldness);
	};

	typedef std::vector<xlslib_core::font_t* XLSLIB_DFLT_ALLOCATOR> Font_Vect_t;
	typedef Font_Vect_t::iterator Font_Vect_Itor_t;


	// forward ref
	class CDataStorage;

	class CFont : public CRecord
	{
		friend class CDataStorage;

	protected:
		CFont(CDataStorage &datastore,
			  const font_t* fontdef);
	private:
		virtual ~CFont();
	};
}

// #include "common/xls_poppack.h"

#endif
