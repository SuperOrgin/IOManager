#pragma once
#include "BaseMultipleIOManager.h"
#include <mutex>

int** num = new int*[10];
int sum = 0;
int oo = 0;
class TestManager :	public BaseMultipleIOManager
{
public:
	TestManager() 
	{
	}

	~TestManager()
	{

	}
	int* Getdata(string strFileName)
	{
		int* pData = (int*)Load(strFileName);
		return pData;
	}

	virtual void* Load(string strFileName)
	{
		std::map<string, DataInfo*> ::iterator iter = mapDate.find(strFileName);
		if (iter == mapDate.end())
		{
			return NULL;
		}

		DataInfo* pTemp = iter->second;
		if (pTemp == NULL)
		{
			return NULL;
		}
		int* pdata = NULL;

		nMutex.lock();

		if (pTemp->pData == NULL)
		{
			pdata = num[oo++];
			if (pdata == NULL)
			{
				return NULL;
			}
		}
		
		if (pTemp->pData == NULL)
		{
			pTemp->pData =(void*) pdata;
		}
		Updata(pTemp);
		nMutex.unlock();
		pdata = (int*)pTemp->pData;

		return (void*)pdata;
	}

	virtual void Delete(void* pData)
	{
		delete (int*)pData;
	}
};

