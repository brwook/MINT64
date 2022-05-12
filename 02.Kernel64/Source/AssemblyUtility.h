#ifndef __ASSMEBLYUTILITY_H__
#define __ASSMEBLYUTILITY_H__
#include "Types.h"
BYTE kInPortByte(WORD wPort);
void kOutPortByte(WORD wPort, BYTE bData);
void kLoadGDTR(QWORD qwGDTRAddress);
void kLoadTR(WORD wTSSSegmentOffset);
void kLoadIDTR(QWORD qwIDTRAddress);
void kEnableInterrupt();
void kDisableInterrupt();
QWORD kReadRFLAGS();

#endif
