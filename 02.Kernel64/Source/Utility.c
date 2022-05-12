#include "Utility.h"
void kmemset(void *pvDestination, BYTE bData, int iSize)
{
	int i;
	for (i=0; i<iSize; ++i)
	{
		((char *)pvDestination)[i] = bData;
	}
}

int kmemcpy(void *pvDestination, const void *pvSource, int iSize)
{
	int i;
	for (i=0; i<iSize; ++i)
	{
		((char *)pvDestination)[i] = ((char *)pvSource)[i];
	}
	return iSize;
}

int kmemcmp(const void *pvDestination, const void *pvSource, int iSize)
{
	int i;
	char cTemp;
	for (i=0; i<iSize; ++i)
	{
		cTemp = ((char *)pvDestination)[i] - ((char *)pvSource)[i];
		if (cTemp != 0)
		{
			return (int)cTemp;
		}
	}
	return 0;
}

