#pragma once
#include "Header.h"

class AppMemu
{
public:
	AppMemu();
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	
	static AppMemu*  pcPtrAppMenu;
	BOOL Cls_OnInitDialog(HWND hwnd);
	void Cls_OnCommand(const int& id, const int& message);
	void Cls_OnClose();
	void mShowDish(HWND hList,int index=-1);
	~AppMemu();
	

	//Dish
	static BOOL CALLBACK DishDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL Cls_DishOnInitDialog(HWND hwnd);
	void Cls_DishOnCommand(const int& id, const int& message);
	void Cls_DishOnClose()const;	

	void mWriteDishesInFile();
	void mReadDishesFromFile();
private:
	HWND hWnd, hMenuShowDishList;

	//Dish
	HWND hDishhWnd,hDishName,hDishPrice,hDessert,hHot,hCold, hAddDish,hProductList;

	vector<Dish> cDishes;


};

