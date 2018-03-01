#pragma once
std::string upper(std::string word)
{
	for (int i = 0; i < word.length(); i++)
	{
		word[i] = toupper(word[i]);
	}
	return word;
}
bool correctNumber(std::string number)
{
	int dots = 1;
	for (int i = 0; i < number.length(); i++)
	{
		if ((number[i] < 48 || number[i] > 57) && number[i] != 46)
		{
			return false;
		}
		else if (number[i] == 46)
		{
			dots--;
			if (dots < 0)
			{
				return false;
			}
		}
	}
	return true;
}