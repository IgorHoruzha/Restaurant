#include <windows.h>
#include "resource.h"
#include "Dish.h"
#include "Header.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst,
	LPSTR lpszCmdLine, int nCmdShow)
{	
	AppMemu Exemplar;

	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_APPMENU), NULL, AppMemu::DlgProc);
}
