#include <windows.h>
#include "resource.h"
#include "Dish.h"
#include "Header.h"

//HINSTANCE hInst;
//Dish *cDish;
//BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
//bool lpfnGetDishType(HWND& hWnd, Dish::DishType &acDishType);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst,
	LPSTR lpszCmdLine, int nCmdShow)
{	
	AppMemu Exemplar;

	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_APPMENU), NULL, AppMemu::DlgProc);
}

//BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//
//	switch (message)
//	{
//	case WM_CLOSE:
//		EndDialog(hWnd, 0);
//		return TRUE;
//
//	case WM_INITDIALOG:
//
//		return TRUE;
//
//	case WM_COMMAND:
//	{
//
//		if (LOWORD(wParam) == AddDish)
//		{
//			wstring cszDishName;
//			int nDishPrice= NULL;
//			Dish::DishType cDishType = Dish::Undefined;
//			if (!lpfnGetDishType(hWnd, cDishType))
//			{
//				MessageBox(hWnd, L"Dish type not selected", TEXT("Radio Button"), MB_ICONWARNING);
//				return false;
//			}
//
//		//	cDish = new Dish();
//		}
//
//	}
//	return TRUE;
//	}
//	return FALSE;
//}

//bool lpfnGetDishType(HWND& hWnd,Dish::DishType &acDishType)
//{
//	if (SendDlgItemMessage(hWnd, Dessert, BM_GETCHECK, 0, 0) == BST_CHECKED)
//	{
//		acDishType = Dish::DessertDish;
//		return true;
//	}
//	else if (SendDlgItemMessage(hWnd, Hot, BM_GETCHECK, 0, 0) == BST_CHECKED)
//	{
//		acDishType = Dish::HotDish;
//		return true;
//	}
//	else if (SendDlgItemMessage(hWnd, Cold, BM_GETCHECK, 0, 0) == BST_CHECKED)
//	{
//		acDishType = Dish::coldDish;
//		return true;
//	}
//	return false;
//}