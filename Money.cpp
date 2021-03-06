/*Copyright by Michał Cichy*/
//02.2018

#include "stdafx.h"
#include <currency.h>
#include <stock.h>
#include <fileManager.h>
#include <moneyUtils.h>

int main()
{
	std::cout << "Witaj w konwerterze walut! :-) ";
	
	Stock stock;
	FileManager fileManager;
	std::thread updatingThread = std::thread( &Stock::updatingThread, &stock, fileManager);
	//stock.printInfo();

	bool end = false;
	while (end == false)
	{
		stock.userInteraction();
		std::cout << "\nKonczymy? ( 0 / 1 )";
		std::cin >> end;
	}
	stock.terminateUpdatingThread();
	updatingThread.join();
	//stock.printInfo();

	std::cin.sync();
	getchar();
    return 0;
}

