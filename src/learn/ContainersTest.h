#pragma once
#include <iostream>
#include <array>
#include <ctime>
#include <list>
#include <forward_list>
#include <deque>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

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

void TestStack()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	stack<string> stackT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		stackT.push(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "Array Top Item::" << stackT.top() << endl;
	cout << "Array Size::" << stackT.size() << endl;
	cout << "Pop::" << endl;
	stackT.pop();
	cout << "Array Top Item::" << stackT.top() << endl;
	cout << "Array Size::" << stackT.size() << endl;

	stackT.empty();
}
	
void TestQueue()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	queue<string> queueT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		queueT.push(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "Array front Item::" << queueT.front() << endl;
	cout << "Array back Item::" << queueT.back() << endl;
	cout << "Array Size::" << queueT.size() << endl;
	cout << "Pop::" << endl;
	queueT.pop();
	cout << "Array front Item::" << queueT.front() << endl;
	cout << "Array back Item::" << queueT.back() << endl;
	cout << "Array Size::" << queueT.size() << endl;

	queueT.empty();
}
	

void TestMultiset()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	multiset<string> multisetT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		multisetT.insert(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "MaxSize::" << multisetT.max_size() << endl;
	cout << "Size::" << multisetT.size() << endl;

	string target = GetATargetString();
	{
		timeStart = clock();
		auto pItme = find(multisetT.begin(), multisetT.end(), target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != multisetT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	{
		timeStart = clock();
		auto pItme = multisetT.find(target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != multisetT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	multisetT.clear();
}

void TestMultimap()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	multimap<long, string> multimapT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		multimapT.insert(pair<long, string>(i, buf));
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "MaxSize::" << multimapT.max_size() << endl;
	cout << "Size::" << multimapT.size() << endl;

	long target = GetATargetLong();
	{
		timeStart = clock();
		auto pItme = multimapT.find(target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != multimapT.end())
		{
			cout << "Found  first item::" << pItme->first << endl;
			cout << "second item::" << pItme->second << endl;
		}
		else
			cout << "Not Found" << endl;
	}

	multimapT.clear();
}

void TestUnorderedMultiset()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	unordered_multiset<string> unorderedMultisetT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		unorderedMultisetT.insert(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "MaxSize::" << unorderedMultisetT.max_size() << endl;
	cout << "Size::" << unorderedMultisetT.size() << endl;
	cout << "load factor::" << unorderedMultisetT.load_factor() << endl;
	cout << "max load factor::" << unorderedMultisetT.max_load_factor() << endl;
	cout << "bucket count::" << unorderedMultisetT.bucket_count() << endl;
	cout << "max bucket count::" << unorderedMultisetT.max_bucket_count() << endl;

	for (int i = 0; i < 20; ++i)
	{
		cout << "bucket " << i << "size::" << unorderedMultisetT.bucket_size(i) << endl;
	}

	string target = GetATargetString();
	{
		timeStart = clock();
		auto pItme = find(unorderedMultisetT.begin(), unorderedMultisetT.end(), target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != unorderedMultisetT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	{
		timeStart = clock();
		auto pItme = unorderedMultisetT.find(target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != unorderedMultisetT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	unorderedMultisetT.clear();
}

void TestUnorderedMultimap()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	unordered_multimap<long, string> unorderedMultimapT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		unorderedMultimapT.insert(pair<long, string>(i, buf));
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "MaxSize::" << unorderedMultimapT.max_size() << endl;
	cout << "Size::" << unorderedMultimapT.size() << endl;

	long target = GetATargetLong();
	{
		timeStart = clock();
		auto pItme = unorderedMultimapT.find(target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != unorderedMultimapT.end())
		{
			cout << "Found  first item::" << pItme->first << endl;
			cout << "second item::" << pItme->second << endl;
		}
		else
			cout << "Not Found" << endl;
	}

	unorderedMultimapT.clear();
}

void TestSet()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	set<string> multisetT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		multisetT.insert(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "MaxSize::" << multisetT.max_size() << endl;
	cout << "Size::" << multisetT.size() << endl;

	string target = GetATargetString();
	{
		timeStart = clock();
		auto pItme = find(multisetT.begin(), multisetT.end(), target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != multisetT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	{
		timeStart = clock();
		auto pItme = multisetT.find(target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != multisetT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	multisetT.clear();
}

void TestMap()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	map<long, string> mapT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		mapT[i] = string(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "MaxSize::" << mapT.max_size() << endl;
	cout << "Size::" << mapT.size() << endl;

	long target = GetATargetLong();
	{
		timeStart = clock();
		auto pItme = mapT.find(target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != mapT.end())
		{
			cout << "Found  first item::" << pItme->first << endl;
			cout << "second item::" << pItme->second << endl;
		}
		else
			cout << "Not Found" << endl;
	}

	mapT.clear();
}

void TestUnorderedSet()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	unordered_set<string> unorderedsetT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		unorderedsetT.insert(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "MaxSize::" << unorderedsetT.max_size() << endl;
	cout << "Size::" << unorderedsetT.size() << endl;
	cout << "load factor::" << unorderedsetT.load_factor() << endl;
	cout << "max load factor::" << unorderedsetT.max_load_factor() << endl;
	cout << "bucket count::" << unorderedsetT.bucket_count() << endl;
	cout << "max bucket count::" << unorderedsetT.max_bucket_count() << endl;

	for (int i = 0; i < 20; ++i)
	{
		cout << "bucket " << i << "size::" << unorderedsetT.bucket_size(i) << endl;
	}

	string target = GetATargetString();
	{
		timeStart = clock();
		auto pItme = find(unorderedsetT.begin(), unorderedsetT.end(), target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != unorderedsetT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	{
		timeStart = clock();
		auto pItme = unorderedsetT.find(target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != unorderedsetT.end())
			cout << "Found " << *pItme << endl;
		else
			cout << "Not Found" << endl;
	}

	unorderedsetT.clear();
}

void TestUnorderedMap()
{
	cout << "Test List" << endl;

	cout << "Set List size ::" << endl;
	long sizeCount;
	cin >> sizeCount;

	srand((unsigned int)(time(NULL)));

	unordered_map<long, string> unorderedMapT;
	clock_t timeStart = clock();
	char buf[10];
	for (long i = 0; i < sizeCount; ++i)
	{
		snprintf(buf, 10, "%d", rand());
		unorderedMapT[i] = string(buf);
	}
	cout << "Time::" << (clock() - timeStart) << endl;
	cout << "MaxSize::" << unorderedMapT.max_size() << endl;
	cout << "Size::" << unorderedMapT.size() << endl;

	long target = GetATargetLong();
	{
		timeStart = clock();
		auto pItme = unorderedMapT.find(target);
		cout << "find Time::" << clock() - timeStart << endl;

		if (pItme != unorderedMapT.end())
		{
			cout << "Found  first item::" << pItme->first << endl;
			cout << "second item::" << pItme->second << endl;
		}
		else
			cout << "Not Found" << endl;
	}

	unorderedMapT.clear();
}