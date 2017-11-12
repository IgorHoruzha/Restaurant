#include "AppMemu.h"

class SearchDishThroughThemInfo
{
	const wstring*  description;
public:

	SearchDishThroughThemInfo(const wstring& description) {
		this->description = &description;
	}

	bool operator()(const Dish& currentDish) const
	{
		return  *description == currentDish.mGetDishDescription();
	}
};


/*
wofstream and  dish Exemplar
Write Dish in file
wofstream Exemplar
*/
wofstream& operator<<(wofstream& Desc, const Dish&  cDish)
{
	Desc << cDish.mGetDishName() << endl;
	Desc << cDish.mGetDishPrice() <<" ";
	Desc << cDish.mGetDishType() ;
	return  Desc;
}



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
}


BOOL AppMemu::Cls_OnInitDialog(HWND hwnd)
{
	hWnd = hwnd;
	hMenuShowDishList = GetDlgItem(hwnd, IDC_CMSHOWDISH);
	if (!cDishes.size())
		mReadDishesFromFile();
	mShowDish(hMenuShowDishList);

	return 0;
}

void AppMemu::Cls_OnCommand(const int& id, const int& message) const
{
	if (id == IDC_ADDDISH)
	{
		Cls_OnClose();
		DialogBox(NULL, MAKEINTRESOURCE(EditDish), NULL, AppMemu::DishDlgProc);
	}
}

void AppMemu::Cls_OnClose() const
{
	mWriteDishesInFile();
	EndDialog(hWnd, 0);
}

void AppMemu::mShowDish(const HWND& hList, const int& index)const
{

	//TODO: Show each category by index

	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	for (auto& cEachDish : cDishes)
	{
		if (index == -1)
		{
			SendMessage(hList, LB_ADDSTRING, 0,
				LPARAM(cEachDish.mGetDishDescription().c_str()));
		}
	}
}

BOOL AppMemu::Cls_DishOnInitDialog(const HWND& hwnd)
{
	hDishhWnd = hwnd;

	hDishName = GetDlgItem(hwnd, DishName);
	hDishPrice = GetDlgItem(hwnd, DishPrice);
	hDessert = GetDlgItem(hwnd, Dessert);
	hHot = GetDlgItem(hwnd, Hot);
	hCold = GetDlgItem(hwnd, Cold);
	hAddDish = GetDlgItem(hwnd, AddDish);
	hProductList = GetDlgItem(hwnd, IDC_PRODUCTLIST);
	hRemoveDish = GetDlgItem(hwnd, IDC_REMOVEDISH);
	//TODO: Remove default value setter
	SetWindowText(hDishName, L"DishName");
	SetWindowText(hDishPrice, L"123.25");
	SendDlgItemMessage(hwnd, Hot, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
	mShowDish(hProductList, -1);
	return 0;
}


void AppMemu::Cls_DishOnCommand(const int& id, const int& message)
{
	if (id == AddDish)
	{
		mAddDish();
	}
	else
		if (id == IDC_REMOVEDISH)
			{
		
			if(SendMessage(hProductList, LB_GETCURSEL, 0, 0)!=-1)
				mRemoveDish();
			else
				MessageBox(hDishhWnd, L"Dish to remove is not selected.", L"ERROR", MB_OK);
		}		
}

void AppMemu::Cls_DishOnClose() const
{
	mWriteDishesInFile();
	EndDialog(hDishhWnd, 0);
	DialogBox(NULL, MAKEINTRESOURCE(IDD_APPMENU), NULL, AppMemu::DlgProc);	
}

void AppMemu::mAddDish()
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
	};

	Dish cDish(szName, _wtof(szPrice), Type);
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

	delete[]szName;
	delete[]szPrice;
}

void AppMemu::mRemoveDish()
{
	int index = SendMessage(hProductList, LB_GETCURSEL, 0, 0);
	if (index != LB_ERR)
	{
		int length = SendMessage(hProductList, LB_GETTEXTLEN, index, 0);
		TCHAR *pBuffer = new TCHAR[length + 1]{ 0 };

		SendMessage(hProductList, LB_GETTEXT, index, LPARAM(pBuffer));
		wstring szSelectedDish(pBuffer);

		delete[]pBuffer;

		vector<Dish>::iterator it= std::find_if(cDishes.begin(), cDishes.end(), SearchDishThroughThemInfo(szSelectedDish));		
		
		if (it!= cDishes.end()) {
			remove_if(cDishes.begin(), cDishes.end(), SearchDishThroughThemInfo(szSelectedDish));
			cDishes.resize(cDishes.size() - 1);

			SendMessage(hProductList, LB_DELETESTRING, index, 0);

			MessageBox(NULL, szSelectedDish.c_str(), L"Delete dish", MB_OK);
		}
		else {
			MessageBox(NULL, L"Dish not Found.", L"Error", MB_OK);
		}
	}
}

void AppMemu::mWriteDishesInFile()const
{
	wofstream Desc("Dishes.bin", ios_base::out | ios_base::trunc | ios_base::binary);//open binary file for write, delete information in file
	//TODO : Write check for open file
	Desc << cDishes.size() ;

	for (const auto& cEachDish : cDishes)
		Desc << cEachDish;
	Desc.close();
}

void AppMemu::mReadDishesFromFile()
{
	wifstream Desc("Dishes.bin", ios_base::in | ios_base::binary);//open binary file for write, delete information in file
	if (!Desc.is_open())//file is open
	{
		//TODO : Write error if file not open
	}
	size_t nCountDishes = 0;

	Desc >> nCountDishes;
	for (size_t i = 0; i < nCountDishes; i++)
	{
		wstring szDishName;
		getline(Desc, szDishName);

		double szDishPrice=0;
		Desc >> szDishPrice;

		int nDishType = 0;
		Desc >> nDishType;

		Dish::DishType cDishType = Dish::Undefined;
		switch (nDishType)
		{
		case 1:
			cDishType = Dish::HotDish;
			break;
		case 2:
			cDishType = Dish::coldDish;
			break;
		case 3:
			cDishType = Dish::DessertDish;
			break;
		default:
			break;
		}
		cDishes.push_back(Dish(szDishName, szDishPrice, cDishType));
	}
	Desc.close();
}
