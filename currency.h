#pragma once
#include<moneyUtils.h>
class Currency
{
private:
	int id;
	std::string name;
	std::vector <compatibleCurrency> compatibles;
public:
	Currency();
	Currency(int id_value);
	~Currency();
	void create(int id_value);
	void addCompatible(compatibleCurrency currency);
	void clearCompatibles();
	int getId();
	std::vector <compatibleCurrency> getCompatibles();
};