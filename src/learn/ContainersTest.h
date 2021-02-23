#pragma once
#include <iostream>
#include <array>
#include <ctime>
#include <list>
#include <forward_list>
#include <deque>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::array;

#define ASIZE 500000

long GetATargetLong()
{
	long target = 0;
	cout << "Input Target number (0 ~ " << RAND_MAX << ")；";
	cin >> target;
	return target;
}

string GetATargetString()
{
	long target = 0;
	char buf[10];
	cout << "Input Target number (0 ~ " << RAND_MAX << ")；";
	cin >> target;
	snprintf(buf, 10, "%d", target);
	return string(buf);
}

int CompareLongs(const void* a, const void* b)
{
	return (*(long*)a - *(long*)b);
}

int CompareStrings(const void* a, const void* b)
{
	if (*(string*)a > * (string*)b)
		return 1;
	else if (*(string*)a < *(string*)b)
		return -1;
	else
		return 0;

}

void TestArray()
{
	cout << "Test Array" << endl;

	srand((unsigned int)(time(NULL)));

	array<long, ASIZE>* parr;
	parr = new array<long, ASIZE>();
	array<long, ASIZE>& arr = *parr;


	clock_t timeStart = clock();
	for (long i = 0; i < ASIZE; ++i)
	{
		arr[i] = rand();
	}

	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "Array Size::" << arr.size() << endl;
	cout << "Array front::" << arr.front() << endl;
	cout << "Array back::" << arr.back() << endl;
	//实际Array开始的地址
	cout << "Array data::" << arr.data() << endl;

	long target = GetATargetLong();
	timeStart = clock();
	qsort(arr.data(), ASIZE, sizeof(long), CompareLongs);

	long* pItem = (long*)bsearch(&target, (arr.data()), ASIZE, sizeof(long), CompareLongs);
	cout << "qsort + bsearch Use Time::" << (clock() - timeStart) << endl;

	if (pItem != nullptr)
		cout << "Found:" << *pItem << endl;
	else
		cout << "Not Found:" << target << endl;

	delete parr;
}

void TestVector()
{
	cout << "Test Vector" << endl;

	cout << "Set Vector size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	vector<string> vectorT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		vectorT.push_back(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "Array Size::" << vectorT.size() << endl;
	cout << "Array MaxSize::" << vectorT.max_size() << endl;
	cout << "Array front::" << vectorT.front() << endl;
	cout << "Array back::" << vectorT.back() << endl;
	//实际Vector分配的起始位置
	cout << "Array data::" << vectorT.data() << endl;
	//实际已经分配的资源Size
	cout << "Array capacity::" << vectorT.capacity() << endl;

	string target = GetATargetString();
	{
		timeStart = clock();
		auto pItme = find(vectorT.begin(), vectorT.end(), target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != vectorT.end())
			cout << "Found " << target << endl;
		else
			cout << "Not Found" << endl;
	}

	{
		timeStart = clock();
		sort(vectorT.begin(), vectorT.end());
		cout << "Sort Time::" << clock() - timeStart << endl;
		timeStart = clock();
		string* pItem = (string*)bsearch(&target, vectorT.data(), vectorT.size(), sizeof(string), CompareStrings);
		cout << "bsearch Time::" << clock() - timeStart << endl;

		if (pItem != NULL)
			cout << "Found " << *pItem << endl;
		else
			cout << "Not Found" << endl;
	}

	vectorT.clear();
}

void TestList()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	list<string> listT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		listT.push_back(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "Array Size::" << listT.size() << endl;
	cout << "Array MaxSize::" << listT.max_size() << endl;
	//第一个元素
	cout << "Array front::" << listT.front() << endl;
	//后一个元素
	cout << "Array back::" << listT.back() << endl;

	string target = GetATargetString();
	{
		timeStart = clock();
		auto pItme = find(listT.begin(), listT.end(), target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != listT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}


	{
		timeStart = clock();
		listT.sort();
		cout << "Sort Time::" << clock() - timeStart << endl;
		timeStart = clock();
	}

	listT.clear();
}

void TestListForward()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	forward_list<string> listT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		listT.push_front(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "Array MaxSize::" << listT.max_size() << endl;
	//第一个元素
	cout << "Array front::" << listT.front() << endl;

	string target = GetATargetString();
	{
		timeStart = clock();
		auto pItme = find(listT.begin(), listT.end(), target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != listT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	{
		timeStart = clock();
		listT.sort();
		cout << "Sort Time::" << clock() - timeStart << endl;
		timeStart = clock();
	}

	listT.clear();
}

void TestDeque()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	deque<string> dequeT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		dequeT.push_front(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "Array MaxSize::" << dequeT.max_size() << endl;
	cout << "Array Size::" << dequeT.size() << endl;
	//第一个元素
	cout << "Array front::" << dequeT.front() << endl;
	cout << "Array back::" << dequeT.back() << endl;

	string target = GetATargetString();
	{
		timeStart = clock();
		auto pItme = find(dequeT.begin(), dequeT.end(), target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != dequeT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	{
		timeStart = clock();
		sort(dequeT.begin(), dequeT.end());
		cout << "Sort Time::" << clock() - timeStart << endl;
		timeStart = clock();
	}

	dequeT.clear();
}