#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "Types.h"

/* GDT */
// basic macro
#define GDT_TYPE_CODE				0x0A
#define GDT_TYPE_DATA				0x02
#define GDT_TYPE_TSS				0x09
#define GDT_FLAGS_LOWER_S			0x10
#define GDT_FLAGS_LOWER_DPL0		0x00
#define GDT_FLAGS_LOWER_DPL1		0x20
#define GDT_FLAGS_LOWER_DPL2		0x40
#define GDT_FLAGS_LOWER_DPL3		0x60
#define GDT_FLAGS_LOWER_P			0x80
#define GDT_FLAGS_UPPER_L			0x20
#define GDT_FLAGS_UPPER_DB			0x40
#define GDT_FLAGS_UPPER_G			0x80

// real used macro
#define GDT_FLAGS_LOWER_KERNELCODE 	( GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_KERNELDATA 	( GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_TSS			( GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_USERCODE	( GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_USERDATA	( GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_UPPER_CODE		( GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L )
#define GDT_FLAGS_UPPER_DATA		( GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L )
#define GDT_FLAGS_UPPER_TSS			( GDT_FLAGS_UPPER_G )

// segment descriptor offset
#define GDT_KERNELCODESEGMENT 	0x08
#define GDT_KERNELDATASEGMENT	0x10
#define GDT_TSSSEGMENT			0x18

// macros about GDT
#define GDTR_STARTADDRESS		0x142000
#define GDT_MAXENTRY8COUNT		3
#define GDT_MAXENTRY16COUNT		1
#define GDT_TABLESIZE			( (sizeof(GDTENTRY8) * GDT_MAXENTRY8COUNT ) + (sizeof(GDTENTRY16) * GDT_MAXENTRY16COUNT) )
#define TSS_SEGMENTSIZE			( sizeof(TSSSEGMENT) )


/* IDT */
// basic macro
#define IDT_TYPE_INTERRUPT		0x0E
#define IDT_TYPE_TRAP			0x0F
#define IDT_FLAGS_DPL0			0x00
#define IDT_FLAGS_DPL1			0x20
#define IDT_FLAGS_DPL2			0x40
#define IDT_FLAGS_DPL3			0x60
#define IDT_FLAGS_P				0x80
#define IDT_FLAGS_IST0			0
#define IDT_FLAGS_IST1			1

// real used macro
#define	IDT_FLAGS_KERNEL		( IDT_FLAGS_DPL0 | IDT_FLAGS_P )
#define IDT_FLAGS_USER			( IDT_FLAGS_DPL3 | IDT_FLAGS_P )

// macros about IDT
#define IDT_MAXENTRYCOUNT 		100
#define IDTR_STARTADDRESS		( GDTR_STARTADDRESS + sizeof(GDTR) + GDT_TABLESIZE + TSS_SEGMENTSIZE )
#define IDT_TABLESIZE			( IDT_MAXENTRYCOUNT * sizeof(IDTENTRY) )
#define IST_STARTADDRESS		0x700000
#define IST_SIZE				0x100000

#pragma pack (push, 1)
typedef struct kGDTRstruct
{
	WORD wLimit;
	QWORD qwBaseAddress;
	WORD wPading;
	DWORD dwPading;
} GDTR, IDTR;

typedef struct kGDTEntry8struct
{
	WORD wLowerLimit;
	WORD wLowerBaseAddress;
	BYTE bUpperBaseAddress1;
	BYTE bTypeAndLowerFlag;			// 4: Type, 1: S, 2: DPL, 1: P 
	BYTE bUpperLimitAndUpperFlag;	// 4: Segment Limit, 1: AVL, L, D/B, G
	BYTE bUpperBaseAddress2;
} GDTENTRY8;

typedef struct kGDTEntry16struct
{
	WORD wLowerLimit;
	WORD wLowerBaseAddress;
	BYTE bMiddleBaseAddress1;
	BYTE bTypeAndLowerFlag;			// 4: Type, 1: 0, 2: DPL, 1: P 
	BYTE bUpperLimitAndUpperFlag;	// 4: Segment Limit, 1: AVL, 0, 0, G
	BYTE bMiddleBaseAddress2;
	DWORD dwUpperBaseAddress;
	DWORD dwReserved;
} GDTENTRY16;

typedef struct kTSSstruct
{
	DWORD dwReserved1;
	QWORD qwRsp[3];
	QWORD qwReserved2;
	QWORD qwIST[7];
	QWORD qwReserved3;
	WORD wReserved;
	WORD wIOMapBaseAddress;
} TSSSEGMENT;

typedef struct kIDTEntryStruct
{
	WORD wLowerBaseAddress;
	WORD wSegmentSelector;
	BYTE bIST;						// 3: IST, 5: 0
	BYTE bTypeAndFlags;				// 4: Type, 1: 0, 2: DPL, 1: P
	WORD wMiddleAddress;
	DWORD dwUpperBaseAddress;
	DWORD dwReserved;
} IDTENTRY;
#pragma pack (pop)

void kInitializeGDTTableAndTSS();
void kSetGDTEntry8(GDTENTRY8 *pstEntry, DWORD dwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType);
void kSetGDTEntry16(GDTENTRY16 *pstEntry, QWORD qwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType);
void kInitializeTSSSegment(TSSSEGMENT *pstTSS);
void kInitializeIDTTables();
void kSetIDTEntry(IDTENTRY *pstEntry, void *pvHandler, WORD wSelector, BYTE bIST, BYTE bFlags, BYTE bType);
void kDummyHandler();
#endif

