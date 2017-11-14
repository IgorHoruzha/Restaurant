#include "Dish.h"


Dish::Dish()
{
	nDishPrice = NULL;
    cDishType = Undefined;
}

Dish::Dish(const wstring& acszDishName, const double& anDishPrice,const DishType& acDishType)
{
	cszDishName = acszDishName;
	nDishPrice = anDishPrice;
	cDishType = acDishType;
}

void Dish::mSetDishName(const wstring &acszDishName)
{
	cszDishName = acszDishName;
}

void Dish::mSetDishPrice(const double &anDishPrice)
{
	nDishPrice = anDishPrice;
}

void Dish::mSetDishType(const DishType &acDishType)
{
	cDishType = acDishType;
}

void Dish::mSetAllFilds(const wstring &acszDishName, const double &anDishPrice, const DishType &acDishType)
{
	mSetDishName(acszDishName);
	mSetDishPrice(anDishPrice);
	mSetDishType(acDishType);
}

const wstring & Dish::mGetDishName() const
{
	return cszDishName;
}

const double & Dish::mGetDishPrice() const
{
	return nDishPrice;
}

const Dish::DishType& Dish::mGetDishType() const
{
	return cDishType;
}

const void Dish::mWriteInFile(ofstream & Desc)
{
	size_t nlength = wcslen(cszDishName.c_str())* sizeof(wchar_t);

	Desc.write(reinterpret_cast<char*>(&nlength), sizeof(nlength));
	wchar_t *szDishName = new wchar_t[nlength+1]{0};

	wcscpy_s(szDishName, nlength,cszDishName.c_str());

	Desc.write(reinterpret_cast<char*>(szDishName),  nlength);
	Desc.write(reinterpret_cast<char*>(&nDishPrice), sizeof(nDishPrice));
	Desc.write(reinterpret_cast<char*>(&cDishType), sizeof(cDishType));

	delete[]szDishName;
}

const wstring Dish::mGetDishDescription()const
{
	wstring description(cszDishName);
	description += L" ";
	description += std::to_wstring(nDishPrice);
	switch (cDishType)
	{
	case HotDish:
		description += L" Hot Dish";
		break;
	case coldDish:
		description += L" Cold Dish";
		break;
	case DessertDish:
		description += L" Dessert Dish";
		break;
	default:
		description += L" Undefined";
		break;
	}
	return description;
}
