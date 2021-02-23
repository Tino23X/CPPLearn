#include "learn/SL1.h"
#include "learn/TestClass.h"
#include "RayTrace/RayTraceMain.h"
#include "learn/ContainersTest.h"

void LearnTest()
{
	//TemplateTestClass<int> temp(3);
	//TestArray();
	//TestVector();
	//TestList();
	//TestListForward();
	TestDeque();
}

void RayTraceTest()
{
	RenderStart();
}

int main()
{
	//LearnTest();
	RayTraceTest();

	return 0;
}
