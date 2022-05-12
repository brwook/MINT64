#include "PIC.h"
#include "AssemblyUtility.h"

void kInitializePIC()
{
	// Master PIC - ICW1, IC4 = 1
	kOutPortByte(PIC_MASTER_PORT1, 0x11);

	// Master PIC - ICW2, Interrupt Vector(0x20)
	kOutPortByte(PIC_MASTER_PORT2, PIC_IRQSTARTVECTOR);

	// Master PIC - ICW3, 
	kOutPortByte(PIC_MASTER_PORT2, 0x4);

	// Master PIC - ICW4, uPM bit = 1
	kOutPortByte(PIC_MASTER_PORT2, 0x1);

	// Slave PIC - ICW1, IC4 = 1
	kOutPortByte(PIC_SLAVE_PORT1, 0x11);

	// Slave PIC - ICW2, Interrupt Vector(0x28)
	kOutPortByte(PIC_SLAVE_PORT2, PIC_IRQSTARTVECTOR + 8);

	// Slave PIC - ICW3, 
	kOutPortByte(PIC_SLAVE_PORT2, 0x2);

	// Slave PIC - ICW4, uPM bit = 1
	kOutPortByte(PIC_SLAVE_PORT2, 0x1);
}

void kMaskPICInterrupt(WORD wIRQBitmask)
{
	// Master PIC - OCW1, IRQ0 ~ IRQ7
	kOutPortByte(PIC_MASTER_PORT2, (BYTE) wIRQBitmask);

	// Slave PIC - OCW1, IRQ8 ~ IRQ15
	kOutPortByte(PIC_SLAVE_PORT2, (BYTE) (wIRQBitmask >> 8));
}

// send EOI from Processor to PIC
// 		Master PIC - only Master PIC
//		Slave  PIC - both Master and Slave PIC
void kSendEOIToPIC(int iIRQNumber)
{
	// Master PIC - OCW2, EOI bit = 1
	kOutPortByte(PIC_MASTER_PORT1, 0x20);

	if (iIRQNumber >= 8)
	{
		kOutPortByte(PIC_SLAVE_PORT1, 0x20);
	}
}


