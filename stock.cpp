#include "stdafx.h"
#include<currency.h>
#include<stock.h>
#include<inputUtils.h>


Stock::Stock()
{
	fillNameToIdDictionary();
	currenciesNumber = currencyNameToId.size();
	Currency tempCurrency;
	for (int i = 0; i < currenciesNumber; i++)
	{
		tempCurrency.create(i);
		currencies.push_back(tempCurrency);
	}
	currIncMatrix = new double*[currenciesNumber];
	for (int i = 0; i < currenciesNumber; i++)
	{
		currIncMatrix[i] = new double [currenciesNumber] {0};
	}
}

Stock::~Stock()
{
	delete[] currIncMatrix;
}

void Stock::prepareCurrencies(
	std::vector <rawReadRecord> updatedRecords)
{

	int curr1, curr2;
	double price1, price2;

	updateMutex.lock();
	for (int i=0; i < currenciesNumber; i++)
	{
		currencies[i].clearCompatibles();
	}
	for (const rawReadRecord& record : updatedRecords)
	{
		curr1 = currencyNameToId[record.curr1];
		curr2 = currencyNameToId[record.curr2];
		price1 = record.price;
		price2 = double(1)/record.price;
		currencies[curr1].addCompatible({ curr2, price1 });
		currencies[curr2].addCompatible({ curr1, price2 });
		currIncMatrix[curr1][curr2] = price1;
		currIncMatrix[curr2][curr1] = price2;
	}
	updateMutex.unlock();
}

void Stock::updateCurrencies(
	std::vector <rawReadRecord> updatedRecords)
{
	int curr1, curr2;
	double price1, price2;
	for (const rawReadRecord& record : updatedRecords)
	{
		curr1 = currencyNameToId[record.curr1];
		curr2 = currencyNameToId[record.curr2];
		price1 = record.price;
		price2 = double(1) / record.price;
		currencies[curr1].addCompatible({ curr2, price1 });
		currencies[curr2].addCompatible({ curr1, price2 });
	}
}
double Stock::findPrice(int curr1, int curr2)
{
	std::vector<bool> visited(currenciesNumber, 0);
	return findPriceRec(curr1, curr2, visited);
}
double Stock::findPriceRec(int curr1, int curr2, std::vector<bool>& visited, double price)
{
	if ( currIncMatrix[curr1][curr2] > 0)
	{
		return price*currIncMatrix[curr1][curr2];
	}
	else
	{
		visited[curr1] = true;
		double res;
		for (const compatibleCurrency& neighbour : currencies[curr1].getCompatibles())
		{
			if ( !visited[neighbour.id] )
			{
				res = findPriceRec(neighbour.id, curr2, visited, price*neighbour.price);
				if (res != -1)
				{
					return res;
				}
			}
		}
		return -1;
	}
}

void Stock::updatingThread(FileManager fileManager)
{
	while (!endUpdatingThread)
	{
		fileManager.downloadJSON();
		prepareCurrencies(fileManager.getDataFromFile());
	}
}

void Stock::terminateUpdatingThread()
{
	endUpdatingThread = true;
}

void Stock::userInteraction()
{
	std::string curr1, curr2;
	std::string priceString;
	double amount;
	std::vector<std::string>::iterator searcher;
	
	std::cout << "\nPodaj walute pocz. --> ";
	std::cin >> curr1;
	curr1 = upper(curr1);
	while (
		std::find(
			currencyIdToName.begin(),
			currencyIdToName.end(),
			curr1) == currencyIdToName.end()
		)
	{
		std::cout << "\nNie mamy takiej waluty.\nPodaj walute pocz. --> ";
		std::cin >> curr1;
		curr1 = upper(curr1);
	}
	std::cout << "\nPodaj walute docelowa. --> ";
	std::cin >> curr2;
	curr2 = upper(curr2);
	while (
		std::find(
			currencyIdToName.begin(),
			currencyIdToName.end(),
			curr2) == currencyIdToName.end()
		)
	{
		std::cout << "\nNie mamy takiej waluty.\nPodaj walute docelowa --> ";
		std::cin >> curr2;
		curr2 = upper(curr2);
	}
	std::cout << "\nPodaj kwote " << curr1 << " --> ";
	std::cin >> priceString;
	while (!correctNumber(priceString))
	{
		std::cout << "\nNieprawidlowy format liczby.\n";
		std::cout << "\nPodaj kwote " << curr1 << " --> ";
		std::cin >> priceString;
	}
	amount = atof(priceString.c_str());
	updateMutex.lock();
	double queryResult = findPrice( currencyNameToId[curr1], currencyNameToId[curr2]);
	updateMutex.unlock();
	if (queryResult < 0)
	{
		std::cout << "\nNiestety nie jestem w stanie tego przekonwertowac.\n";
	}
	else
	{
		std::cout <<"\n"<<amount<<" "<<curr1<<" to "<< queryResult / amount<<" "<<curr2<<"\n";
	}
}

void Stock::printInfo()
{
	std::vector <compatibleCurrency> compatibles;
	for (int i=0; i < currenciesNumber; i++)
	{
		std::cout << "\n" << currencyIdToName[currencies[i].getId()] << "  Sasiedzi: ";
		compatibles = currencies[i].getCompatibles();
		for (const compatibleCurrency& compCurr : compatibles)
		{
			std::cout << currencyIdToName[compCurr.id] << " ";
		}
	}
	std::cout << "\nMacierz incydencji:\n" << std::endl;
	for (int y = 0; y < currenciesNumber; y++)
	{
		for (int x = 0; x < currenciesNumber; x++)
		{
			std::cout<<currIncMatrix[x][y]<<" ";
		}
		std::cout << std::endl;
	}
}
void Stock::fillNameToIdDictionary()
{
	int count = 0;
	currencyNameToId["PLN"] = count++;
	currencyNameToId["USD"] = count++;
	currencyNameToId["EUR"] = count++;
	currencyNameToId["ADA"] = count++;
	currencyNameToId["ARK"] = count++;
	currencyNameToId["BCH"] = count++;
	currencyNameToId["BTC"] = count++;
	currencyNameToId["BTG"] = count++;
	currencyNameToId["DASH"] = count++;
	currencyNameToId["EOS"] = count++;
	currencyNameToId["ETC"] = count++;
	currencyNameToId["ETH"] = count++;
	currencyNameToId["GNT"] = count++;
	currencyNameToId["HSR"] = count++;
	currencyNameToId["LSK"] = count++;
	currencyNameToId["LTC"] = count++;
	currencyNameToId["QTUM"] = count++;
	currencyNameToId["REP"] = count++;
	currencyNameToId["SC"] = count++;
	currencyNameToId["STRAT"] = count++;
	currencyNameToId["TRX"] = count++;
	currencyNameToId["XEM"] = count++;
	currencyNameToId["XMR"] = count++;
	currencyNameToId["XRP"] = count++;
	currencyNameToId["ZEC"] = count++;

	currencyIdToName.push_back("PLN");
	currencyIdToName.push_back("USD");
	currencyIdToName.push_back("EUR");
	currencyIdToName.push_back("ADA");
	currencyIdToName.push_back("ARK");
	currencyIdToName.push_back("BCH");
	currencyIdToName.push_back("BTC");
	currencyIdToName.push_back("BTG");
	currencyIdToName.push_back("DASH");
	currencyIdToName.push_back("EOS");
	currencyIdToName.push_back("ETC");
	currencyIdToName.push_back("ETH");
	currencyIdToName.push_back("GNT");
	currencyIdToName.push_back("HSR");
	currencyIdToName.push_back("LSK");
	currencyIdToName.push_back("LTC");
	currencyIdToName.push_back("QTUM");
	currencyIdToName.push_back("REP");
	currencyIdToName.push_back("SC");
	currencyIdToName.push_back("STRAT");
	currencyIdToName.push_back("TRX");
	currencyIdToName.push_back("XEM");
	currencyIdToName.push_back("XMR");
	currencyIdToName.push_back("XRP");
	currencyIdToName.push_back("ZEC");
}