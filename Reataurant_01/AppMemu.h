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
	void mShowDish(const HWND& hList, const int& index=-1)const;

	//Dish
	static BOOL CALLBACK DishDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL Cls_DishOnInitDialog(const HWND& hwnd);
	void Cls_DishOnCommand(const int& id, const int& message);
	void Cls_DishOnClose()const;	

	void mAddDish();

	void mWriteDishesInFile()const;
	void mReadDishesFromFile();
private:
	HWND hWnd, hMenuShowDishList;

	//Dish
	HWND hDishhWnd,hDishName,hDishPrice,hDessert,hHot,hCold, hAddDish,hProductList;

	vector<Dish> cDishes;
};

