#pragma once
#include "Header.h"

class Dish
{
public:
	enum DishType
	{
		Undefined,HotDish,coldDish, DessertDish
	};

private:
	wstring cszDishName;
	double nDishPrice;
	DishType cDishType = Undefined;
public:
	Dish();
	Dish(const wstring &, const double &, const DishType&);

	void mSetDishName(const wstring &);
	void mSetDishPrice(const double &);
	void mSetDishType(const DishType &);

	void mSetAllFilds(const wstring &, const double &, const DishType &);

	const wstring& mGetDishName()const;
    const double& mGetDishPrice()const;
	const DishType& mGetDishType()const;	

	const wstring mGetDishDescription()const;	

};

