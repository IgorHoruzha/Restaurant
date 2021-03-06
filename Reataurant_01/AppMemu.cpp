#include "AppMemu.h"
#include <cmath>
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

class DishNameEquality
{
	const Dish * element;
public:

	DishNameEquality(const Dish& element) {
		this->element = &element;
	}

	bool operator()(const Dish& currentDish) const
	{
		return  element->mGetDishName() == currentDish.mGetDishName();
	}
};


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
	mShowDish(hMenuShowDishList,-1);
	hMemuNameSearch = GetDlgItem(hwnd, IDC_CMNAME);
	hMemuPriceSearch = GetDlgItem(hwnd, IDC_CMPRICE);

	return 0;
}

void AppMemu::Cls_OnCommand(const int& id, const int& message) const
{
	if (id == IDC_ADDDISH)
	{
		Cls_OnClose();
		DialogBox(NULL, MAKEINTRESOURCE(EditDish), NULL, AppMemu::DishDlgProc);
	}
	else
		if (id== IDC_CMNAME||id== IDC_CMPRICE ||id == IDC_CMHOT || id == IDC_CMCOLD || id == IDC_CMDESERT)
		{
			int nDish=0;
			if (IsDlgButtonChecked(hWnd, IDC_CMHOT))
				nDish |= 2;
				if (IsDlgButtonChecked(hWnd, IDC_CMCOLD))
					nDish |= 4;			
					if (IsDlgButtonChecked(hWnd, IDC_CMDESERT))
						nDish |= 8;					
						
			mShowDish(hMenuShowDishList, nDish);
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
	wchar_t *szName = nullptr;
	wchar_t *szPrice = nullptr;

	int nLength = SendMessage(hMemuNameSearch, WM_GETTEXTLENGTH, 0, 0) + 1;
	szName = new wchar_t[nLength];
	GetWindowText(hMemuNameSearch, szName, nLength);

	nLength = SendMessage(hMemuPriceSearch, WM_GETTEXTLENGTH, 0, 0) + 1;
	szPrice = new wchar_t[nLength];
	GetWindowText(hMemuPriceSearch, szPrice, nLength);

	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	for (auto& cEachDish : cDishes)
	{
		


		int a = 0;
		a=pow(2,int(cEachDish.mGetDishType()));	

		if (index == -1)		
			SendMessage(hList, LB_ADDSTRING, 0,
				LPARAM(cEachDish.mGetDishDescription().c_str()));
		else if ((index ? (a&index) : 1) &&
			( szName[0] ? 0 == wcscmp(szName, cEachDish.mGetDishName().c_str()) : 1) &&
			( _wtof(szPrice) ? _wtof(szPrice) == cEachDish.mGetDishPrice() : 1))
			SendMessage(hList, LB_ADDSTRING, 0,
				LPARAM(cEachDish.mGetDishDescription().c_str()));
	
	}

	delete[]szName;
	delete[]szPrice;

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
	hEditDish = GetDlgItem(hwnd, IDC_EDITDISH);
	hContinueAddDish = GetDlgItem(hwnd, IDC_CONTINUEADDDISH);
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
		if (id == IDC_REMOVEDISH || id == IDC_EDITDISH || id == IDC_CONTINUEADDDISH)
		{
			if (id == IDC_CONTINUEADDDISH)
			{
				EnableWindow(hRemoveDish, 0);
				EnableWindow(hEditDish, 0);
				EnableWindow(hContinueAddDish, 0);
				EnableWindow(hAddDish, 1);
				SendMessage(hProductList, LB_SETSEL, 0, -1);
			}
			else
				if (SendMessage(hProductList, LB_GETCURSEL, 0, 0) != -1) {
					mRemoveDish();
					if (id == IDC_EDITDISH)
						mAddDish();
				}
				else
					MessageBox(hDishhWnd, L"Dish to remove is not selected.", L"ERROR", MB_OK);
			EnableWindow(hProductList, 1);
		}
		else
			if (id == IDC_PRODUCTLIST && message == LBN_SELCHANGE)
			{
				if (SendMessage(hProductList, LB_GETCURSEL, 0, 0) != -1)
				{
					EnableWindow(hRemoveDish, 1);
					EnableWindow(hEditDish, 1);
					EnableWindow(hContinueAddDish, 1);
					EnableWindow(hAddDish, 0);
					EnableWindow(hProductList, 0);

					int index = SendMessage(hProductList, LB_GETCURSEL, 0, 0);
					if (index != LB_ERR)
					{
						wstring szSelectedDish(mGetDishDescriptionFromListByIndex(hProductList, index));

						vector<Dish>::iterator it = mGetDishIteratorByDescription(szSelectedDish);

						if (it != cDishes.end()) {
							SetWindowText(hDishName, it->mGetDishName().c_str());

							SetWindowText(hDishPrice, std::to_wstring(it->mGetDishPrice()).c_str());

							switch (it->mGetDishType())
							{
							case 1:
								SendDlgItemMessage(hWnd, Hot, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
								break;
							case 2:
								SendDlgItemMessage(hWnd, Cold, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
								break;
							case 3:
								SendDlgItemMessage(hWnd, Dessert, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
								break;
							default:
								break;
							}
							MessageBox(NULL, szSelectedDish.c_str(), L"DishEdit", MB_OK);
						}
						else {
							MessageBox(NULL, L"Dish not Found.", L"Error", MB_OK);
						}
					}
				}
			}
}



void AppMemu::Cls_DishOnClose() const
{
	mWriteDishesInFile();
	EndDialog(hDishhWnd, 0);
	DialogBox(NULL, MAKEINTRESOURCE(IDD_APPMENU), NULL, AppMemu::DlgProc);
}

const vector<Dish>::iterator AppMemu::mGetDishIteratorByDescription(const wstring & szDishDescription) 
{
	return std::find_if(cDishes.begin(), cDishes.end(), SearchDishThroughThemInfo(szDishDescription));
}

const wstring AppMemu::mGetDishDescriptionFromListByIndex(const HWND & hList, const int & index) const
{
	size_t length = SendMessage(hProductList, LB_GETTEXTLEN, index, 0);
	TCHAR *pBuffer = new TCHAR[length + 1]{ 0 };

	SendMessage(hProductList, LB_GETTEXT, index, LPARAM(pBuffer));
	const wstring szSelectedDish(pBuffer);

	delete[]pBuffer;
	return szSelectedDish;
}

void AppMemu::mAddDish()
{
	wchar_t *szName = nullptr;
	wchar_t *szPrice = nullptr;

	int nLength = SendMessage(hDishName, WM_GETTEXTLENGTH, 0, 0) + 1;
	szName = new wchar_t[nLength];
	GetWindowText(hDishName, szName, nLength);

	nLength = SendMessage(hDishPrice, WM_GETTEXTLENGTH, 0, 0) + 1;
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
	if (!count_if(cDishes.begin(), cDishes.end(), DishNameEquality(cDish)))
	{
		cDishes.push_back(cDish);
		SendMessage(hProductList, LB_ADDSTRING, 0,
			LPARAM(cDish.mGetDishDescription().c_str()));
	}
	else	
		MessageBox(NULL, cDish.mGetDishDescription().c_str(), L"ERROR", MB_OK);
	

	delete[]szName;
	delete[]szPrice;
}

void AppMemu::mRemoveDish()
{
	int index = SendMessage(hProductList, LB_GETCURSEL, 0, 0);
	if (index != LB_ERR)
	{
		const wstring szSelectedDishDescription(mGetDishDescriptionFromListByIndex(hProductList, index));

		SendMessage(hProductList, LB_DELETESTRING, index, 0);
		if (mGetDishIteratorByDescription(szSelectedDishDescription) != cDishes.end()) {
			remove_if(cDishes.begin(), cDishes.end(), SearchDishThroughThemInfo(szSelectedDishDescription));
			cDishes.resize(cDishes.size() - 1);

			MessageBox(NULL, szSelectedDishDescription.c_str(), L"Delete dish", MB_OK);
		}
		else MessageBox(NULL, L"Dish not Found.", L"Error", MB_OK);
		
	}
}

void AppMemu::mWriteDishesInFile()const
{
	ofstream Desc("Dishes.bin", ios_base::out | ios_base::trunc| ios_base::binary);//open binary file for write, delete information in file
	//TODO : Write check for open file
	size_t nCountDish = cDishes.size();
	Desc.write(reinterpret_cast<char*>(&nCountDish), sizeof(nCountDish));
	for (auto cEachDish : cDishes)
		cEachDish.mWriteInFile(Desc);
	Desc.close();
}

void AppMemu::mReadDishesFromFile()
{
	ifstream Desc("Dishes.bin", ios_base::in);//open binary file for write, delete information in file
	if (!Desc.is_open())//file is open
	{
		//TODO : Write error if file not open
	}

	size_t nCountDishes = 0;
	Desc.read(reinterpret_cast<char*>(&nCountDishes), 4);
	

	for (size_t i = 0; i < nCountDishes; i++)
	{
		size_t nDishNameLength = 0;
		Desc.read(reinterpret_cast<char*>(&nDishNameLength), sizeof(nDishNameLength));
		
		wchar_t* szDishName = new wchar_t[nDishNameLength+1]{0};

		Desc.read(reinterpret_cast<char*>(szDishName), nDishNameLength);
		nDishNameLength = 0;

		double nDishPrice = 0;
		Desc.read(reinterpret_cast<char*>(&nDishPrice), sizeof(nDishPrice));

		int nDishType = 0;
		Desc.read(reinterpret_cast<char*>(&nDishType), sizeof(nDishType));

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
			cDishes.push_back(Dish(szDishName, nDishPrice, cDishType));

		delete[]szDishName;
	}
	
	Desc.close();
}
