/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2009-2013 David Hoerl All Rights Reserved.
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

#ifndef XLSTYPES_H
#define XLSTYPES_H

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

// ALL C++ users
#if defined(__cplusplus)

#include <list>
#include <vector>
#include <algorithm>
#include <string>

#endif

// Setup our typedefs now - would like to get them from systype.h
// if we have this one we will have all the others too
#if defined(uint8_t) || defined(HAVE_STDINT_H)
typedef uint8_t				unsigned8_t;
typedef uint16_t			unsigned16_t;
typedef uint32_t			unsigned32_t;

typedef int8_t				signed8_t;
typedef int16_t				signed16_t;
typedef int32_t				signed32_t;

// no systype.h
#else

typedef unsigned char		unsigned8_t;
typedef unsigned short int	unsigned16_t;
typedef unsigned int		unsigned32_t;

typedef char				signed8_t;
typedef short int			signed16_t;
typedef int					signed32_t;

 // uint8_t
#endif

// Windows
#if defined(_MSC_VER) && defined(WIN32)

typedef unsigned __int64	unsigned64_t;

// not windows
#else

#if defined(_UINT64_T)

typedef uint64_t			unsigned64_t;

#else

typedef unsigned long long	unsigned64_t;

// _UINT64_T
#endif

// defined(_MSC_VER) && defined(WIN32)
#endif 

#if defined(__cplusplus)

namespace xlslib_strings
{

// MAC Framework
#if defined(__FRAMEWORK__)

#include "xlconfig.h"

typedef unsigned16_t unichar_t;
typedef std::basic_string<unsigned16_t> ustring;
typedef ustring wstring;
typedef std::basic_string<unsigned16_t> u16string;
typedef std::string string;
typedef unsigned16_t xchar16_t;

// Not Framework
#else										

// Windows
#if defined(_MSC_VER) && defined(WIN32)

// every Visual Studio version before 2010 needs this, as 2010 introduced its own version of u16string
#if _MSC_VER < 1600 
typedef std::basic_string<unsigned16_t> u16string;
typedef unsigned16_t xchar16_t;
#else
typedef std::u16string u16string; 
typedef char16_t xchar16_t;
#endif

#else

// Clang on the Mac needs this
#if __cplusplus >= 201103L
typedef std::basic_string<char16_t> u16string;
typedef char16_t xchar16_t;
#else
typedef std::basic_string<unsigned16_t> u16string;
typedef unsigned16_t xchar16_t;
#endif

// defined(_MSC_VER) && defined(WIN32)
#endif

typedef wchar_t unichar_t;
typedef std::wstring ustring;
typedef ustring wstring;
typedef std::string string;

// defined(__FRAMEWORK__)
#endif

}

// defined(__cplusplus)
#endif	

 // XLSTYPES_H
#endif  
