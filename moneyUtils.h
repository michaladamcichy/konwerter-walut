#pragma once
#include "stdafx.h"

struct rawReadRecord
{
	std::string curr1;
	std::string curr2;
	double price;
};

struct compatibleCurrency
{
	int id;
	double price;
};

struct solution
{
	double value;
	int pathLength;
};