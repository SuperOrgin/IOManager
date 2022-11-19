#include "TestManager.h"

void main()
{
	for (int i = 0; i < 10; i++)
	{
		num[i] = new int[3];
		for (int j = 0; j < 3; j++)
		{
			num[i][j] = sum++;
		}
	}

	TestManager* pManager = new TestManager();
	pManager->Add("aaa");
	pManager->Getdata("aaa");
	pManager->Release("aaa");
	pManager->Getdata("aa");
}