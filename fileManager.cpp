#include "stdafx.h"
#include<stock.h>
#include "fileManager.h"

FileManager::FileManager()
{
	url = "https://api.abucoins.com/products/ticker";
	fileName = "kursy.json";
}

FileManager::~FileManager()
{
}
std::vector <rawReadRecord> FileManager::getDataFromFile()
{
	std::vector <rawReadRecord> records;
	std::ifstream input;
	input.open(fileName.c_str());

	if (!input.good())
	{
		//std::cout << "\nNie udało się otworzyć pliku z kursami walut.";
		input.close();
	}
	else
	{
		//std::cout << "\nZaczynam wczytywanie kursów walut z pliku.\n";
		std::string tempString;
		double tempDouble;
		std::string curr1, curr2;
		rawReadRecord tempRecord;
		while (!input.eof())
		{
			getline(input, tempString, ':');
			getline(input, tempString, '"');
			getline(input, tempString, '-');
			if ( tempString.find("]") != -1)
			{
				//std::cout << "konczymy";
				break;
			}

			curr1 = tempString;
			getline(input, tempString, '"');
			curr2 = tempString;

			getline(input, tempString, '"');
			getline(input, tempString, ',');
			getline(input, tempString, ':');
			getline(input, tempString, '"');
			
			input >> tempDouble;

			getline(input, tempString, '}');
			
			tempRecord = { curr1, curr2, tempDouble };
			records.push_back(tempRecord);
			//std::cout << "\n" << tempRecord.curr1 << " " << tempRecord.curr2 << " " << tempRecord.price << std::endl;
		}
		input.close();
		//std::cout << "\nZakończono odczyt";
	}
	return records;
}

void FileManager::downloadJSON()
{
	std::wstring wUrl = std::wstring(url.begin(), url.end());
	std::wstring wFileName = std::wstring(fileName.begin(), fileName.end());
	LPCWSTR urlPointer = wUrl.c_str();
	LPCWSTR fileNamePointer = wFileName.c_str();
	HRESULT res;
	res = URLDownloadToFile(
		NULL,
		urlPointer,
		fileNamePointer,
		0,
		NULL
	);
	if (res != S_OK)
	{
		//std::cout << "\nFailed to download JSON.";
	}
	else
	{
		//std::cout << "\nJSON downloaded succesfully";
	}
}