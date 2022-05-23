#ifndef __UTILITY_H__
#define __UTILITY_H__
#include "Types.h"
#include <stdarg.h>

void kmemset(void *pvDestination, BYTE bData, int iSize);
int kmemcpy(void *pvDestination, const void *pvSource, int iSize);
int kmemcmp(const void *pvDestination, const void *pvSource, int iSize);
BOOL kSetInterruptFlag(BOOL bEnableInterrupt);
int kstrlen(const char* pcBuffer);
void kCheckTotalRAMSize();
QWORD kGetTotalRAMSize();
void kReverseString(char *pcBuffer);
long katoi(const char *pcBuffer, int iRadix);
QWORD kHexStringToQword(const char *pcBuffer);
long kDecimalStringToLong(const char *pcBuffer);
int kitoa(long lValue, char *pcBuffer, int iRadix);
int kHexToString(QWORD qwValue, char* pcBuffer);
int kDecimalToString(long lValue, char* pcBuffer);
int ksprintf(char *pcBuffer, const char *pcFormatString, ...);
int kvsprintf(char *pcBuffer, const char* pcFormatString, va_list ap);
#endif
