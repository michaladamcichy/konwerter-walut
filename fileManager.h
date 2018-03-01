#pragma once
#include<stdafx.h>
#include<moneyUtils.h>

class FileManager
{
private:
	std::string url;
	std::string fileName;
	std::vector <rawReadRecord> rawRecords;
public:
	FileManager();
	~FileManager();

	void downloadJSON();
	std::vector <rawReadRecord> getDataFromFile();
};
