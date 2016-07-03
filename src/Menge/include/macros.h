/*
 * macros.h
 *
 *  Created on: Oct 8, 2012
 *      Author: piccolo
 */

#ifndef MACROS_H_
#define MACROS_H_

#ifdef _WIN32
	#include <hash_map>
	#define HASH_MAP stdext::hash_map
#else
	#include <tr1/unordered_map>
	#define HASH_MAP std::tr1::unordered_map
#endif

#ifndef _MSC_VER

#include <inttypes.h>
#include <stdio.h>

//FIXME: this should be handle automatically by cmake
#define sprintf_s(buffer, buffer_size, stringbuffer, ...) (snprintf(buffer,buffer_size ,stringbuffer, __VA_ARGS__))
#define sscanf_s(buffer, buffer_size, stringbuffer, ...) (snprintf(buffer,buffer_size ,stringbuffer, __VA_ARGS__))

//typedef long long __int64;

#ifndef DWORD
#define WINAPI
//typedef unsigned long DWORD;
typedef uint32_t DWORD;
typedef short WCHAR;
typedef void * HANDLE;
//#define MAX_PATH    PATH_MAX
typedef uint8_t BYTE;
//typedef unsigned char BYTE;
//typedef unsigned short WORD;
typedef uint16_t WORD;
typedef int32_t BOOL;
//typedef unsigned int BOOL;
#endif

#ifdef UNICODE

#define _tcslen     wcslen
#define _tcscpy     wcscpy
#define _tcscpy_s   wcscpy_s
#define _tcsncpy    wcsncpy
#define _tcsncpy_s  wcsncpy_s
#define _tcscat     wcscat
#define _tcscat_s   wcscat_s
#define _tcsupr     wcsupr
#define _tcsupr_s   wcsupr_s
#define _tcslwr     wcslwr
#define _tcslwr_s   wcslwr_s
#define _stprintf_s swprintf_s
#define _stprintf   swprintf
#define _tprintf    wprintf
#define _vstprintf_s    vswprintf_s
#define _vstprintf      vswprintf
#define _tscanf     wscanf
#define TCHAR wchar_t

#else

#define _tcslen     strlen
#define _tcscpy     strcpy
#define _tcscpy_s   strcpy_s
#define _tcsncpy    strncpy
#define _tcsncpy_s  strncpy_s
#define _tcscat     strcat
#define _tcscat_s   strcat_s
#define _tcsupr     strupr
#define _tcsupr_s   strupr_s
#define _tcslwr     strlwr
#define _tcslwr_s   strlwr_s
#define _stprintf_s sprintf_s
#define _stprintf   sprintf
#define _tprintf    printf
#define _vstprintf_s    vsprintf_s
#define _vstprintf      vsprintf
#define _tscanf     scanf
#define TCHAR char

#endif

#else	// _MSC_VER

//#define snprintf sprintf_s

#endif	// _MSC_VER

#endif /* MACROS_H_ */
