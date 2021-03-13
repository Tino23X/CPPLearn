#pragma once
#include <cmath>
#include <string>
#include <iostream>

using namespace std;
class Topic2 {
public:
	int reverse(int x) {

		string maxStr = "2147483647";

		int sign = 0;
		if (x >= 0)
			sign = 1;
		else
			sign = -1;

		x = abs(x);

		string str = to_string(x);
		std::reverse(str.begin(), str.end());

		cout << str << endl;


		if (str.length() > 10)
			return 0;
		else if (str.length() == 10 && str[0] != '0')
		{
			for (int i = 0; i < 10; ++i)
			{
				if (i == 9 && sign < 0 && str[i] == '9')
				{
					return 0;
				}
				else if (str[i] > maxStr[i])
					return 0;
				else if (str[i] < maxStr[i])
					break;
			}
		}

		size_t pos;
		int resault = std::stoi(str, &pos);
		return resault * sign;
	}
};

void Topic2Exe()
{
		

	int x = -2147483412;
	Topic2 topic2;
	cout << topic2.reverse(x) << endl;
}