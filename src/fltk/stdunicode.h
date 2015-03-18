#ifndef __STDUNICODE_H__
#define __STDUNICODE_H__
////////////////////////////////////////////////////////////////////////////////
//
//  stdunicode
//  ==========================================================================
//  (C)Copyright 2013, Raphael Kim (rage.kim@gmail.com)
//
////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
    #include <windows.h>
    #include <tchar.h>
#else
    #include "tchar.h"
#endif

char*    convertW2M(const wchar_t* src);
wchar_t* convertM2W(const char* src);

#endif /// of __STDUNICODE_H__
