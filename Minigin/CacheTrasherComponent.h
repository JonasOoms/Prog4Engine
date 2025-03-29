#pragma once
#include "RenderComponent.h"
#include <map>


struct TestTransform
{
	float matrix[16] =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};

class HeapTestObject
{
public:
	TestTransform local;
	int id;
};

class HeapTestObjectWithPtr
{
public:
	TestTransform* local;
	int id;
};


class CacheTrasherComponent : public RenderComponent
{
public:
	void IntegerHeapTest(int samples);
	void ObjectHeapTest(int samples);
	void ObjectWithPointerHeapTest(int samples);
	virtual void Render() override;
private:
	int m_SamplesIntegers{};
	std::map<int, double> m_FunctionTimingIntegers;
	int m_SamplesObjects{};
	std::map<int, double> m_FunctionTimingObjects;
	int m_SamplesObjectsWithPtrs{};
	std::map<int, double> m_FunctionTimingObjectsWithPtrs;
};

