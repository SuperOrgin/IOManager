#include "BaseMultipleIOManager.h"

DataInfo::DataInfo()
{
	nRemainderCount = 1;
	nUsingCount = 0;
	pData = NULL;
	nPosition = -1;
}

BaseMultipleIOManager::BaseMultipleIOManager()
{
}

BaseMultipleIOManager::~BaseMultipleIOManager()
{
	map<string, DataInfo*>::iterator iter = mapDate.begin();
	for (; iter != mapDate.end(); iter++)
	{
		if (iter->second != NULL)
		{
			Delete(iter->second);
		}
	}
	vector<DataInfo*>::iterator iterDelete = vDelete.begin();

	for (; iterDelete != vDelete.end(); iterDelete++)
	{
		if (*iterDelete != NULL)
		{
			Delete(*iterDelete);
		}
	}

	vInfo.clear();
	mapDate.clear();
	vDelete.clear();
}

bool BaseMultipleIOManager::Release(string strKey)
{
	nMutex.lock();
	map<string, DataInfo*> ::iterator iter = mapDate.find(strKey);
	if (iter == mapDate.end())
	{
		return false;
	}
	DataInfo* pInfo = iter->second;
	pInfo->nUsingCount--;
	bool bResult = UnLoad(pInfo);
	nMutex.unlock();
	return bResult;
}

bool BaseMultipleIOManager::Add(string strKey)
{
	nMutex.lock();
	map<string, DataInfo*> ::iterator iter = mapDate.find(strKey);
	if (iter != mapDate.end())
	{
		iter->second->nRemainderCount++;
	}
	else
	{
		mapDate[strKey] = new DataInfo();
	}
	nMutex.unlock();
	return true;
}

bool BaseMultipleIOManager::UnLoad(DataInfo* pInfo)
{
	//Check undelete list
	vector<DataInfo*>::iterator iter = vDelete.begin();
	for (; iter != vDelete.end();)
	{
		DataInfo* pInfo = *iter;
		if (pInfo->nPosition == -1 && pInfo->nPosition == 0)
		{
			Delete(pInfo->pData);
			iter = vDelete.erase(iter);
		}
		else if (pInfo->nPosition != -1) // it's return vInfo again
		{
			iter = vDelete.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	//nRemainderCount is 0 & nUsingCount = 0 delete
	//nPosition  is -1 & nUsingCount = 0 delete
	if (pInfo->nRemainderCount == 0 && pInfo->nUsingCount == 0 ||
		pInfo->nPosition == -1 && pInfo->nUsingCount == 0)
	{
		Delete(pInfo->pData);
		return true;
	}
	else if (pInfo->nPosition == -1 && pInfo->nUsingCount != 0) //Need out the vInfo but not finish used.
	{
		vDelete.push_back(pInfo);
	}
	return false;

}

//a simple BubbleSort for vector
void BaseMultipleIOManager::BubbleSort(int nNow, BubbleDirection type)
{
	if (type == Left)
	{
		if (nNow == 0)
		{
			return;
		}

		int nNowTemp = nNow;
		while (nNowTemp > 0)
		{
			//more than Left then exchange
			if (vInfo[nNowTemp]->nRemainderCount > vInfo[nNowTemp - 1]->nRemainderCount)
			{
				DataInfo* pTemp = vInfo[nNowTemp];
				vInfo[nNowTemp] = vInfo[nNowTemp - 1];
				vInfo[nNowTemp - 1] = pTemp;
				nNowTemp--;
			}
			else
			{
				break;
			}
		}
		//make everyone position is new
		for (int i = nNowTemp; i <= nNow; i++)
		{
			vInfo[i]->nPosition = i;
		}
	}
	else if (type == Right)
	{
		int nLimit = vInfo.size();
		int nNowTemp = nNow;
		while (nNowTemp < nLimit)
		{
			//Less than Right is exchange
			if (vInfo[nNowTemp]->nRemainderCount < vInfo[nNowTemp + 1]->nRemainderCount)
			{
				DataInfo* pTemp = vInfo[nNowTemp + 1];
				vInfo[nNowTemp + 1] = vInfo[nNowTemp];
				vInfo[nNowTemp] = pTemp;
				nNowTemp++;
			}
			else
			{
				break;
			}
		}
		for (int i = nNow; i <= nNowTemp; i++)
		{
			vInfo[i]->nPosition = i;
		}
	}
}

void BaseMultipleIOManager::InsertSort()
{
	int now = vInfo.size() - 1;

	//at first is the first
	if (vInfo.size() == 1)
	{
		vInfo[0]->nPosition = 0;
		return;
	}

	BubbleSort(now, Left);
};

bool BaseMultipleIOManager::Updata(DataInfo* pInfo)
{
	//keep the status in energy conservation
	pInfo->nRemainderCount--;
	pInfo->nUsingCount++;
	int nNow = pInfo->nPosition;
	//If nNow is -1 instance of not in vInfo ,then need more process
	if (nNow == -1)
	{
		// If object nRemainderCount no more then do nothing 
		if (pInfo->nRemainderCount == 0)
		{
			return false;
		}

		// If vInfo has not memory & object nRemainderCount at leaset one time
		// then try make a space for this object
		if (!Insert(pInfo))
		{
			return false;
		}
	}
	else
	{
		BubbleSort(nNow, Right);
	}
	return true;
}

bool BaseMultipleIOManager::Insert(DataInfo* pInfo)
{
	//If vInfo has memory & object nRemainderCount at leaset one time
	if (vInfo.size() < 256)
	{
		vInfo.push_back(pInfo);
		InsertSort();
	}
	else if (vInfo[255]->nRemainderCount < pInfo->nRemainderCount)
	{
		vInfo[255]->nPosition = -1;
		UnLoad(vInfo[255]);
		vInfo[255] = pInfo;
	}
	return true;
};

void BaseMultipleIOManager::Delete(DataInfo*& pInfo)
{
	Delete((void*)pInfo->pData);
	delete pInfo;
	pInfo = NULL;
}
