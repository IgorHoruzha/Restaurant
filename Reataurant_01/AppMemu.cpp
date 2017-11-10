#include "AppMemu.h"


AppMemu*  AppMemu::pcPtrAppMenu = nullptr;
AppMemu::AppMemu()
{
	pcPtrAppMenu = this;
	hWnd = NULL;
}

BOOL CALLBACK AppMemu::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		pcPtrAppMenu->Cls_OnClose();
		return TRUE;

	case WM_INITDIALOG:
		pcPtrAppMenu->Cls_OnInitDialog(hWnd);
		return TRUE;

	case WM_COMMAND:
		pcPtrAppMenu->Cls_OnCommand(LOWORD(wParam), HIWORD(wParam));

		return TRUE;
	}
	return FALSE;
}

BOOL AppMemu::DishDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		pcPtrAppMenu->Cls_DishOnClose();
		return TRUE;

	case WM_INITDIALOG:
		pcPtrAppMenu->Cls_DishOnInitDialog(hWnd);
		return TRUE;

	case WM_COMMAND:
		pcPtrAppMenu->Cls_DishOnCommand(LOWORD(wParam), HIWORD(wParam));

		return TRUE;
	}
	return FALSE;
	return 0;
}


BOOL AppMemu::Cls_OnInitDialog(HWND hwnd)
{
	hWnd = hwnd;
	hMenuShowDishList = GetDlgItem(hwnd, IDC_CMSHOWDISH);
	//cDishes.clear();
	if(!cDishes.size())
	mReadDishesFromFile();
	mShowDish(hMenuShowDishList);
	
	return 0;
}

void AppMemu::Cls_OnCommand(const int& id, const int& message)
{
	if (id == IDC_ADDDISH)
	{
		Cls_OnClose();
		DialogBox(NULL, MAKEINTRESOURCE(EditDish), NULL, AppMemu::DishDlgProc);
	}
}

AppMemu::~AppMemu()
{
}
void AppMemu::Cls_OnClose()
{
	EndDialog(hWnd, 0);
}

void AppMemu::mShowDish(HWND hList,int index)
{//TODO: Show each category by index

	SendMessage(hMenuShowDishList, LB_RESETCONTENT, 0, 0);
	for (auto& cEachDish : cDishes)
	{
		if (index == -1)
		{			
			SendMessage(hList, LB_ADDSTRING, 0,
					LPARAM(cEachDish.mGetDishDescription().c_str()));
		}
	}
}

BOOL AppMemu::Cls_DishOnInitDialog(HWND hwnd)
{
	hDishhWnd = hwnd;

	hDishName = GetDlgItem(hwnd, DishName);
	hDishPrice = GetDlgItem(hwnd, DishPrice);
	hDessert = GetDlgItem(hwnd, Dessert);
	hHot = GetDlgItem(hwnd, Hot);
	hCold = GetDlgItem(hwnd, Cold);
	hAddDish = GetDlgItem(hwnd, AddDish);
	hProductList = GetDlgItem(hwnd, IDC_PRODUCTLIST);

	SetWindowText(hDishName, L"DishName");
	SetWindowText(hDishPrice, L"123.25");
	SendDlgItemMessage(hwnd, Hot, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
	return 0;
}

void AppMemu::Cls_DishOnCommand(const int& id, const int& message)
{
	if (id == AddDish)
	{
		wchar_t *szName = nullptr;
		wchar_t *szPrice = nullptr;

		int nLength = SendMessage(hDishName, WM_GETTEXTLENGTH, 0, 0) + 2;
		szName = new wchar_t[nLength];
		GetWindowText(hDishName, szName, nLength);

		nLength = SendMessage(hDishPrice, WM_GETTEXTLENGTH, 0, 0) + 2;
		szPrice = new wchar_t[nLength];
		GetWindowText(hDishPrice, szPrice, nLength);

		Dish::DishType Type;
		if (IsDlgButtonChecked(hDishhWnd, Dessert))
			Type = Dish::DessertDish;
		else
			if (IsDlgButtonChecked(hDishhWnd, Hot))
				Type = Dish::HotDish;
			else
				if (IsDlgButtonChecked(hDishhWnd, Cold))
					Type = Dish::coldDish;
				else
					Type = Dish::Undefined;

		Dish cDish(szName, _wtof(szPrice), Type);

		class DishNameEquality
		{
			const Dish  element;
		public:

			DishNameEquality(const Dish& element) :element(element) {
			}

			bool operator()(const Dish& currentDish) const
			{
				return  element.mGetDishName() == currentDish.mGetDishName();
			}

			~DishNameEquality() {
			}
		};

		if (!count_if(cDishes.begin(), cDishes.end(), DishNameEquality(cDish)))
		{


			cDishes.push_back(cDish);
			SendMessage(hProductList, LB_ADDSTRING, 0,
				LPARAM(cDish.mGetDishDescription().c_str()));
		}
		else
		{
			MessageBox(NULL, cDish.mGetDishDescription().c_str(), L"ERROR", MB_OK);
		}
		mWriteDishesInFile();
		delete[]szName;
		delete[]szPrice;
	}
}

void AppMemu::Cls_DishOnClose() const
{
	EndDialog(hDishhWnd, 0);
	DialogBox(NULL, MAKEINTRESOURCE(IDD_APPMENU), NULL, AppMemu::DlgProc);
}

wofstream& operator<<(wofstream& Desc, const Dish&  cDish)
{
	Desc << cDish.mGetDishName().c_str() << endl;
	Desc << cDish.mGetDishPrice() << endl;
	Desc << cDish.mGetDishType() << endl;
	return  Desc;
}

void AppMemu::mWriteDishesInFile()
{


	

		wofstream Desc("Dishes.bin", ios_base::out | ios_base::trunc | ios_base::binary);//open binary file for write, delete information in file
	
		Desc << cDishes.size() << endl;
		for (auto& cEachDish : cDishes)
		{		
			Desc << cEachDish;
		}
		Desc.close();

	
	return;

}

void AppMemu::mReadDishesFromFile()
{
	wfstream Desc("Dishes.bin", ios_base::in | ios_base::binary);//open binary file for write, delete information in file
	if (!Desc.is_open())//file is open
	{
		throw exception("Dishes.bin");//throw file name for  show error
	}
	int nCountDishes = 0;

	Desc >> nCountDishes;
	for (int i = 0; i < nCountDishes; i++)
	{
		int nDishNameSize = 0;
		wstring szDishName;
		Desc >> szDishName;
		double szDishPrice;
		Desc >> szDishPrice;

		int nDishType = 0;
		Desc >> nDishType;

		Dish::DishType cDishType = Dish::Undefined;
		switch (nDishType)
		{
		case 0:
			cDishType = Dish::coldDish;
			break;
		case 1:
			cDishType = Dish::HotDish;
			break;
		case 2:
			cDishType = Dish::DessertDish;
			break;
		default:
			break;
		}

		cDishes.push_back(Dish(szDishName, szDishPrice, cDishType));

	}

	Desc.close();
	

}
