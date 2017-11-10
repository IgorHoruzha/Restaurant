#pragma once
#include "Header.h"
class DlgProc
{
public:
	DlgProc();
	static BOOL CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static DlgProc *ptr;
	~DlgProc();

};

