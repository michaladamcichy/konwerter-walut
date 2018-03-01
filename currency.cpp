#include "stdafx.h"
#include "currency.h"

Currency::Currency()
{
	id = 0;
}
Currency::Currency(int id_value)
{
	id = id_value;
}
Currency::~Currency()
{

}
void Currency::create(int id_value)
{
	id = id_value;
}
void Currency::addCompatible(compatibleCurrency currency)
{
	compatibles.push_back(currency);
}
void Currency::clearCompatibles()
{
	compatibles.clear();
}
int Currency::getId()
{
	return id;
}
std::vector <compatibleCurrency> Currency::getCompatibles()
{
	return compatibles;
}
