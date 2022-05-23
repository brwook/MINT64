#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "AssemblyUtility.h"
#include "PIC.h"
#include "Console.h"
#include "ConsoleShell.h"
#include "Utility.h"

void Main()
{
	int iCursorX, iCursorY;
	kInitializeConsole(0, 10);
	kprintf("Switch to IA-32e Mode Success!\n");
	kprintf("IA-32e C Language Kernel Start...................[Pass]\n");
	kprintf("Initialize Console...............................[Pass]\n");

	kGetCursor(&iCursorX, &iCursorY);
	kprintf("GDT Initialize And Switch For IA32-e Mode........[    ]");
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_STARTADDRESS);
	kSetCursor(50, iCursorY++);
	kprintf("Pass\n");

	kprintf("TSS Segment Load.................................[    ]");
	kLoadTR(GDT_TSSSEGMENT);
	kSetCursor(50, iCursorY++);
	kprintf("Pass\n");

	kprintf("IDT Initialize...................................[    ]");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_STARTADDRESS);
	kSetCursor(50, iCursorY++);
	kprintf("Pass\n");

	kprintf("Total RAM Size Check.............................[    ]");
	kCheckTotalRAMSize();
	kSetCursor(50, iCursorY++);
	kprintf("Pass], Size = %d MB\n", kGetTotalRAMSize());

	kprintf("Keyboard Activate And Queue Intialize............[    ]");
	if(kInitializeKeyboard() == TRUE)
	{
		kSetCursor(50, iCursorY++);
		kprintf("Pass\n");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else
	{
		kSetCursor(50, iCursorY++);
		kprintf("Fail\n");
		while(1);
	}

	kprintf("PIC Controller And Interrupt Initialize..........[    ]");
	kInitializePIC();
	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kSetCursor(50, iCursorY++);
	kprintf("Pass\n");
	
	kStartConsoleShell();
}