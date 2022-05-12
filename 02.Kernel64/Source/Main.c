#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "AssemblyUtility.h"
#include "PIC.h"

void kPrintString(int iX, int iY, const char *pcString);
void Main()
{
	char vcTemp[2] = {0, };
	BYTE bFlags;
	BYTE bTemp;
	int i = 0;
	

	kPrintString(0, 10, "Switch to IA-32e Mode Success!");
	kPrintString(0, 11, "IA-32e C Language Kernel Start...................[Pass]");
	kPrintString(0, 12, "GDT Initialize And Switch For IA32-e Mode........[Pass]");
	
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_STARTADDRESS);
	kPrintString(50, 12, "Pass");

	kPrintString(0, 13, "TSS Segment Load.................................[    ]");
	kLoadTR(GDT_TSSSEGMENT);
	kPrintString(50, 13, "Pass");

	kPrintString(0, 14, "IDT Initialize...................................[    ]");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_STARTADDRESS);
	kPrintString(50, 14, "Pass");

	kPrintString(0, 15, "Keyboard Activate................................[    ]");
	if (kActivateKeyboard() == TRUE)
	{
		kPrintString(50, 15, "Pass");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else
	{
		kPrintString(50, 15, "Fail");
		while(1);
	}

	kPrintString(0, 16, "PIC Controller And Interrupt Initialize..........[    ]");
	kInitializePIC();
	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kPrintString(50, 16, "Pass");
	
	while (1)
	{
		if (kIsOutputBufferFull() == TRUE)
		{
			bTemp = kGetKeyboardScanCode();
			if (kConvertScanCodeToASCIICode(bTemp, &(vcTemp[0]), &bFlags) == TRUE)
			{
				if (bFlags & KEY_FLAGS_DOWN)
				{
					kPrintString(i++, 17, vcTemp);
					if (vcTemp[0] == '0')
					{
						bTemp = bTemp / 0;
					}
				}
			}
		}
	}
}

void kPrintString(int iX, int iY, const char *pcString)
{
    CHARACTER *pstScreen = ( CHARACTER *) 0xb8000;
    int i;
    pstScreen += iX + (iY * 80);
    for (i=0; pcString[i]; ++i)
        pstScreen[i].bCharacter = pcString[i];
}


