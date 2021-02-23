#pragma once
#include <iostream>

using namespace std;

template<class T>
class TemplateTestClass
{
public:
	//���麯�����ܱ�ʵ����
	//virtual void Count() const = 0;
	TemplateTestClass(T tt);
	~TemplateTestClass();

private:
	T t;

};


template<class T>
inline TemplateTestClass<T>::TemplateTestClass(T tt) :t(tt)
{
	cout << "Init::" << t << endl;
}

template<class T>
TemplateTestClass<T>::~TemplateTestClass()
{
	cout << "dctor" << endl;
}
