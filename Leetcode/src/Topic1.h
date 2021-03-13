#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Topic1 {
public:
	vector<int> twoSum(vector<int>& nums, int target) {

		vector<int> vec;

		for (int i = 0; i < nums.size(); ++i)
		{
			int resault = target - nums[i];

			for (int j = i + 1; j < nums.size(); ++j)
			{

				if (resault == nums[j])
				{

					vec.push_back(i);
					vec.push_back(j);
					return vec;
				}
			}
		}

		return vec;
	}
};

void Topic1Exe()
{

	Topic1 topic1;
	vector<int> nums;
	nums.push_back(2);
	nums.push_back(7);
	nums.push_back(11);
	nums.push_back(15);

	vector<int> tmpVec = topic1.twoSum(nums, 9);

	cout << tmpVec[0] << "--" << tmpVec[1] << endl;

}