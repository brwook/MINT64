#ifndef __UTILITY_H__
#define __UTILITY_H__
#include "Types.h"
void kmemset(void *pvDestination, BYTE bData, int iSize);
int kmemcpy(void *pvDestination, const void *pvSource, int iSize);
int kmemcmp(const void *pvDestination, const void *pvSource, int iSize);


#endif
