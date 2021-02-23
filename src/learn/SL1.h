#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

void sl2_1()
{
	int ia[6] = { 27,220,32,48,100,30 };

	//vector container
	//allocator 分配器
	vector<int, allocator<int>> vi(ia, ia + 6);

	//vi.end,vi.begin iterator
	//count_if 算法
	//not1 function adapter
	//bind2nd function adapter
	//less function object
	cout << count_if(vi.begin(), vi.end(), not1(bind2nd(less<int>(), 40)));
}

//迭代器法
void sl2_2()
{
	int ia[6] = { 27,220,32,48,100,30 };

	vector<int, allocator<int>> vi(ia, ia + 6);

	for (vector<int>::iterator i = vi.begin(); i != vi.end(); ++i)
	{
		cout << *i << endl;
	}
}

//CPP11 for loop
void sl2_3()
{
	int ia[6] = { 27,220,32,48,100,30 };

	vector<int, allocator<int>> vi(ia, ia + 6);

	for (auto i : vi)
	{
		cout << i << endl;
		i = 1;
	}

	cout << "---------" << endl;

	for (auto i : vi)
	{
		cout << i << endl;
	}

	cout << "---------Reference---------" << endl;
	for (auto& i : vi)
	{
		cout << i << endl;
		i = 1;
	}

	cout << "---------" << endl;

	for (auto i : vi)
	{
		cout << i << endl;
	}
}
