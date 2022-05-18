#include "Utility.h"
#include "AssemblyUtility.h"

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

BOOL kSetInterruptFlag(BOOL bEnableInterrupt)
{
	QWORD qwRFLAGS;
	qwRFLAGS = kReadRFLAGS();
	if(bEnableInterrupt == TRUE)
		kEnableInterrupt();
	
	else
		kDisableInterrupt();
	
	// Check previous IF bit of RFLAGS and return status
	if (qwRFLAGS & 0x0200)
		return TRUE;

	return FALSE;
}