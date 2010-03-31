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
#include "Windows.h"
#include "utilities.h"
#include "CalcHandle.h"

#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include "winable.h"
//HWND CalcWnd;
extern HWND gCalcWnd;
using namespace std;

CalcHandle* CalcHandle::pInstance_ = 0;
bool CalcHandle::destroyed_ = false;

#pragma warning(disable:4244)

#define AVERAGE(x, y) (x+y)/2







BOOL CALLBACK EnumCalcChildByCIDProc(      
    HWND hwnd,
    LPARAM lParam
) {

	PFINDWND ans = (PFINDWND)lParam;
	unsigned long ControlID = ans->opcode;
	
	if (GetDlgCtrlID(hwnd) == ControlID)
	{
		ans->resultWnd = hwnd;
		
	}
	return 1;
}

CalcInterface::CalcInterface() : stringstream() {
	//setf(ios::fixed);
	
	//setprecision(3);
	//unsetf(ios::showpoint);
}


CalcInterface::~CalcInterface() {
	//SendMessage(hCalcWnd_, WM_CLOSE,0, 0);
//	DebugSpew("~CalcHanlde()");
//	DestroyWindow(hCalcWnd_);
	
};






char *CalcInterface::GetAnswer(char *szResult) {
	FINDWND temp;
	temp.opcode = 0x193;
	EnumChildWindows(CalcHandle::Instance().getCalcWnd(), EnumCalcChildByCIDProc, (LPARAM)&temp);

	char currentText[32]; SendMessage(
		temp.resultWnd, WM_GETTEXT, 
		sizeof(currentText), (LPARAM)(void*)currentText);
	DebugSpew("Found box %s with CID 0x%x", currentText, temp.opcode);
	strncpy(szResult,currentText,32);
		

	DebugSpew("CalcInterface::GetAnswer(%s)",szResult);
	return szResult;
}


bool CalcInterface::isError() {
	char szResult[32] = {0};
	if (strstr( GetAnswer(szResult), "Cannot"))
		return true;
	else
		return false;
}

/*
0  0x7C
1  0x7D
2  0x7E
3  0x7F
4  0x80
5  0x81
6  0x82
7  0x83
8  0x84
9  0x85
+  0x5C
-  0x5D
/  0x5A
*  0x5B
=  0x70
*/

HWND CalcInterface::GetWndByName(char name) {
	FINDWND w;
	switch (name) {
		case '0':
			w.opcode = 0x7C;
			break;
		case '1':
			w.opcode = 0x7D;
			break;
		case '2':
			w.opcode = 0x7E;
			break;
		case '3':
			w.opcode = 0x7F;
			break;
		case '4':
			w.opcode = 0x80;
			break;
		case '5':
			w.opcode = 0x81;
			break;
		case '6':
			w.opcode = 0x82;
			break;
		case '7':
			w.opcode = 0x83;
			break;
		case '8':
			w.opcode = 0x84;
			break;
		case '9':
			w.opcode = 0x85;
			break;
		case '+':
			w.opcode = 0x5C;
			break;
		case '-':
			w.opcode = 0x5D;
			break;
		case '*':
			w.opcode = 0x5B;
			break;
		case '/':
			w.opcode = 0x5A;
			break;
		case '=':
			w.opcode = 0x70;
			break;
		case 'C':
			w.opcode = 0x51;
			break;
		case '.':
			w.opcode = 0x5D;
			break;
		default:
			DebugSpew("CalcInterface::GetWndByName(%c) - error, no window found", name);
			return 0;
	}
	//DebugSpew("CalcInterface::GetWndByName(%c) 0x%x", name, w.opcode);
	EnumChildWindows(CalcHandle::Instance().getCalcWnd(), EnumCalcChildByCIDProc, (LPARAM)&w);
	return w.resultWnd;
}


void CalcInterface::MoveMouse(RECT &area) {

	
	POINT point;
	point.x = AVERAGE(area.left, area.right);
	point.y = AVERAGE(area.top, area.bottom);
	
	DebugSpew("CalcInterface::MoveMouse(%d, %d)", point.x, point.y);

	INPUT Input[1];
	memset(Input, 0, sizeof(INPUT));

	Input[0].type = INPUT_MOUSE;
	Input[0].mi.dx = MulDiv(point.x, 65536, GetSystemMetrics(SM_CXSCREEN));
	Input[0].mi.dy = MulDiv(point.y, 65536, GetSystemMetrics(SM_CYSCREEN));
	Input[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;

	SendInput(1, Input, sizeof(INPUT));

	Input[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, Input, sizeof(INPUT));
	Sleep(100);	

	
}


void CalcInterface::DoClick(char name) {
	DebugSpew("CalcInterface::DoClick(%c)", name);

	HWND clicker = GetWndByName(name);
	if (clicker) {

		WINDOWINFO info;
		info.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(clicker, &info);
		MoveMouse(info.rcWindow);
	}
}

char *CalcInterface::Execute(char* szResult) {
	ShowWindow(CalcHandle::Instance().getCalcWnd(), SW_SHOWNORMAL);
	
	//preamble
	BlockInput(true);
	POINT mousePoint;
	GetCursorPos(&mousePoint);
	BringWindowToTop(CalcHandle::Instance().getCalcWnd());
	

	//if (isError())
		Clear();


	INPUT Input[1];
	memset(Input, 0, sizeof(INPUT));
	Input[0].type = INPUT_KEYBOARD;
	Input[0].ki.wVk = VK_F6;
	SendInput(1, Input, sizeof(INPUT));
	Input[0].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, Input, sizeof(INPUT));

	DebugSpew("CommandProcessor::Execute(%s)", str().c_str());
	for (size_t i = 0; i< str().length(); ++i)
		DoClick(str().at(i));


	//postamble
	BlockInput(false);
	SetCursorPos(mousePoint.x, mousePoint.y);
	BringWindowToTop(gCalcWnd);

	return GetAnswer(szResult);
	
}

void CalcInterface::Clear() {
	DoClick('C');
}


BOOL CALLBACK FindCalcProc(      
    HWND hwnd,
    LPARAM lParam
) 
{
	int *res = (int*)lParam;
	char szName[128] = {0};
	GetWindowText(hwnd, szName, 128);
	
	if (!stricmp(szName,"calculator"))
	{
		//DebugSpew("FindCalcProc(found calc: %s)", szName);
		CalcHandle::Instance().setCalcWnd(hwnd);
		*res = 1;
	}
	return 1;
}


HWND &CalcHandle::getCalcWnd() {
	int result = -1;
	EnumDesktopWindows(0, FindCalcProc, (LPARAM)&result);
	if (result == 1) {
		return hCalcWnd_;
	} else {
		ShellExecuteA(gCalcWnd, 0,"calc.exe",0,"C:\\windows\\system32\\",0);
		Sleep(1000);
		return getCalcWnd();
	}
}

