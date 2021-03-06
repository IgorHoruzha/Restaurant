#pragma once
#include "Header.h"

class Dish;//Dish class prototype

class AppMemu
{
public:
	AppMemu();
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	
	static AppMemu*  pcPtrAppMenu;
	BOOL Cls_OnInitDialog(HWND hwnd);
	void Cls_OnCommand(const int& id, const int& message)const;
	void Cls_OnClose()const;
	void mShowDish(const HWND& hList, const int& index)const;

	//Dish
	static BOOL CALLBACK DishDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL Cls_DishOnInitDialog(const HWND& hwnd);
	void Cls_DishOnCommand(const int& id, const int& message);
	void Cls_DishOnClose()const;	

	const vector<Dish>::iterator mGetDishIteratorByDescription(const wstring&szDishDescription);
	
	const wstring mGetDishDescriptionFromListByIndex(const HWND&hList,const int &index)const;
	void mAddDish();
	void mRemoveDish();

	void mWriteDishesInFile()const;
	void mReadDishesFromFile();
private:
	HWND hWnd, hMenuShowDishList,
		hMemuNameSearch, hMemuPriceSearch;

	//Dish
	HWND hDishhWnd,hDishName,hDishPrice,hDessert,hHot,hCold, hAddDish,
		hProductList,hRemoveDish,hEditDish,hContinueAddDish;

	vector<Dish> cDishes;
};

