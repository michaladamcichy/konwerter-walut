#include<currency.h>
#include<moneyUtils.h>
#include<fileManager.h>
class Stock
{
private:
	int currenciesNumber;
	std::vector <Currency> currencies;
	std::map <std::string, int> currencyNameToId;
	std::vector<std::string> currencyIdToName;
	double** currIncMatrix;
	std::mutex updateMutex;
	bool endUpdatingThread = false;
	void fillNameToIdDictionary();
public:
	Stock();
	~Stock();
	void prepareCurrencies(
		std::vector <rawReadRecord> updatedRecords);
	void updateCurrencies(
		std::vector <rawReadRecord> updatedCurrencies);
	double findPriceRec(int curr1, int curr2, std::vector<bool>& visited, double price = 1);
	double findPrice(int curr1, int curr2);
	void updatingThread(FileManager fileManager);
	void terminateUpdatingThread();
	void userInteraction();
	void printInfo();
};

