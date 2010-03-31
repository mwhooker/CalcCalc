/***********************************************************************
CalcCalc - a simple calculator!
Copyright (C) 2007 Matthew Hooker

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Also add information on how to contact you by electronic and paper mail. 
**************************************************************************/

#include "windows.h"
#include "utilities.h"
#include <iostream>
using namespace std;
#define MAX_STRING 1024
#define DebugHeader "[calc] "
#define DBG_SPEW
bool gSpewToFile = false;
char* gszLogPath = "";

// ***************************************************************************
// Function:    DebugSpew
// Description: Outputs text to debugger, usage is same as printf ;)
// ***************************************************************************
VOID DebugSpew(PCHAR szFormat, ...)
{
#ifdef DBG_SPEW
    CHAR szOutput[MAX_STRING] = {0};
    va_list vaList;
    va_start( vaList, szFormat );
    vsprintf(szOutput,szFormat, vaList);
    DebugSpewAlways(szOutput);
#endif
}



VOID DebugSpewAlways(PCHAR szFormat, ...)
{
    CHAR szOutput[MAX_STRING] = {0};
    va_list vaList;

    OutputDebugString(DebugHeader);
    va_start( vaList, szFormat );
    vsprintf(szOutput,szFormat, vaList);
    OutputDebugString(szOutput);
    OutputDebugString("\n");

#ifdef DBG_SPEW
    if (gSpewToFile) {

    FILE *fOut = NULL;
    CHAR Filename[MAX_STRING] = {0};
    sprintf(Filename,"%s\\DebugSpew.log",gszLogPath);
    fOut = fopen(Filename,"at");
    if (!fOut) return;
#ifdef DBG_CHARNAME
	CHAR Name[256] = "Unknown";
	PCHARINFO pCharInfo = GetCharInfo();
	if (pCharInfo)
	{
		strcpy(Name,pCharInfo->Name);
	}
    fprintf(fOut,"%s - %s\r\n", Name, szOutput);
#else
    fprintf(fOut,"%s\r\n", szOutput);
#endif
    fclose(fOut);
    }
#endif
}

VOID DebugSpewNoFile(PCHAR szFormat, ...)
{
#ifdef DBG_SPEW
    CHAR szOutput[MAX_STRING] = {0};
    va_list vaList;

    OutputDebugString(DebugHeader);
    va_start( vaList, szFormat );
    vsprintf(szOutput,szFormat, vaList);
    OutputDebugString(szOutput);
    OutputDebugString("\n");
#endif
}

