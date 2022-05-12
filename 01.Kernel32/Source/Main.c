#include "Types.h"
#include "Page.h"
#include "ModeSwitch.h"

void kPrintString(int iX, int iY, const char * pcString);
BOOL kInitializeKernel64Area();
BOOL kIsMemoryEnough();
void kCopyKernel64ImageTo2Mbyte();
void Main()
{
	DWORD dwEAX, dwEBX, dwECX, dwEDX;
	char vcVendorString[13] = {0, };
	kPrintString(0, 3, "Protected Mode C Language Kernel Started!!!......[Pass]");
	kPrintString(0, 4, "Minimum Memory Size Check........................[    ]" );
	if(kIsMemoryEnough() == FALSE)
	{
		kPrintString(50, 4, "Fail");
		kPrintString(0, 5, "Not Enough Memory~!! MINT64 OS Requires Over");
		while(1);
	}
	kPrintString(50, 4, "Pass");
	kPrintString(0, 5, "IA-32e Kernel Area Initialization................[    ]");
	if(kInitializeKernel64Area() == FALSE)
	{
		kPrintString(45, 5, "Fail");
		kPrintString(0, 6, "Kernal Area Initialization Failed!");
		while(1);
	}
	kPrintString(50, 5, "Pass");

	// create Page Table
	kPrintString(0, 6, "IA-32e Page Tables Initialize....................[    ]");
	kInitializePageTables();
	kPrintString(50, 6, "Pass");

	kReadCPUID(0x00, &dwEAX, (DWORD*)vcVendorString, (DWORD*)(vcVendorString + 8),
			(DWORD*)(vcVendorString + 4));
	kPrintString(0, 7, "Processor Vendor String..........................[            ]");
	kPrintString(50, 7, vcVendorString);

	kReadCPUID(0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	kPrintString(0, 8, "64bit Mode Support Check.........................[    ]");
	if(dwEDX & (1 << 29))
		kPrintString(50, 8, "Pass");
	else
	{
		kPrintString(50, 8, "Fail");
		kPrintString(0, 9, "This proccessor does not support 64bit mode...");
		while(1);
	}

	kPrintString(0, 9, "Copy IA-32e Kernel to 2M address.................[    ]");
	kCopyKernel64ImageTo2Mbyte();
	kPrintString(50, 9, "Pass");

	kPrintString(0, 10, "Switch to IA-32e Mode");
	kSwitchAndExecute64bitKernel();
	while(1);
}

void kPrintString(int iX, int iY, const char * pcString)
{
	CHARACTER *pstScreen = ( CHARACTER *) 0xb8000;
	int i;
	pstScreen += iX + (iY * 80);
	for (i=0; pcString[i]; ++i)
		pstScreen[i].bCharacter = pcString[i];
}

BOOL kInitializeKernel64Area()
{
	DWORD *pdwCurrentAddress = (DWORD *) 0x100000;
	while( (DWORD) pdwCurrentAddress < 0x600000 )
	{
		*pdwCurrentAddress = 0x00000000;
		if(*pdwCurrentAddress != 0)
		{
			return FALSE;
		}
		pdwCurrentAddress++;
	}
	return TRUE;
}
BOOL kIsMemoryEnough()
{
	DWORD *pdwCurrentAddress = (DWORD *) 0x100000;
	while ((DWORD) pdwCurrentAddress < 0x4000000)
	{
		*pdwCurrentAddress = 0x12345678;
		if( *pdwCurrentAddress != 0x12345678)
			return FALSE;
		pdwCurrentAddress += (0x100000 / 4);
	}
	return TRUE;
}

void kCopyKernel64ImageTo2Mbyte()
{
	WORD wKernel32SectorCount, wTotalKernelSectorCount;
	DWORD *pdwSourceAddress, *pdwDestinationAddress;
	int i;

	wTotalKernelSectorCount = *((WORD *)0x7C05);
	wKernel32SectorCount = *((WORD *)0x7C07);
	pdwSourceAddress = (DWORD *) (0x10000 + (wKernel32SectorCount * 0x200));
	pdwDestinationAddress = (DWORD *)0x200000;

	for(i=0; i<512 * (wTotalKernelSectorCount - wKernel32SectorCount) / 4; ++i)
		*pdwDestinationAddress++ = *pdwSourceAddress++;
}
