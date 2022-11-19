#pragma once
#include <vector>
#include <map>
#include <cstring>
#include <mutex>
using namespace std;

//Suit : one object need IO many Time.
//How to use :
//	Create a new Class inheritance The Class BaseMultipleIOManager
//  And reproduction 2 virtual function : Load & Delete , It's core For suits difference type of data.
//  A Simple Sample Like TestManager

enum BubbleDirection
{
	Left = 0,
	Right = 1
};

struct DataInfo
{
	DataInfo();

	// will to do
	int nRemainderCount;

	// using count
	int nUsingCount;

	// Unknown-Type Pointer
	void* pData;

	// Position in vector (-1 represents not in vector)
	int nPosition;
};

class BaseMultipleIOManager
{
public:
	BaseMultipleIOManager();

	~BaseMultipleIOManager();

	bool Release(string strKey);

	bool Add(string strKey);

	virtual void* Load(string strFileName) = 0;

	virtual void Delete(void* pData) = 0;

	bool UnLoad(DataInfo* pInfo);
protected:
	//a simple BubbleSort for vector ,maybe HeapSort is better.
	void BubbleSort(int nNow, BubbleDirection type);

	//use this function after inserted a new object to vInfo.
	//It can make the last object find a suit position in vInfo
	void InsertSort();

	//use this function when IO a new object.
	bool Updata(DataInfo* pInfo);

	//insert a new object to vInfo
	bool Insert(DataInfo* pInfo);

	//delete pInfo
	void Delete(DataInfo*& pInfo);
protected:
	// MapForDate
	map<string, DataInfo*> mapDate;

	// store object out vector but still using , waiting for delete
	vector<DataInfo*> vDelete;

	// DataInfo vector , using BubbleSort now.it's can use HeapSort in the feature
	vector<DataInfo*> vInfo;

	mutex nMutex;
};

