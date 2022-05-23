#include <stdarg.h>
#include "Console.h"
#include "Keyboard.h"
#include "Utility.h"
#include "AssemblyUtility.h"
#include "Types.h"

CONSOLEMANAGER gs_stConsoleManager = {0, };
void kInitializeConsole(int iX, int iY)
{
    kmemset(&gs_stConsoleManager, 0, sizeof(gs_stConsoleManager));
    kSetCursor(iX, iY);
}

void kSetCursor(int iX, int iY)
{
    int iLinearValue;
    iLinearValue = iY * CONSOLE_WIDTH + iX;

    // CRTC Controller Address Register(0x3D4)/Data Register(0x3D5)
    kOutPortByte(VGA_PORT_INDEX, VGA_INDEX_UPPERCURSOR);
    kOutPortByte(VGA_PORT_DATA, iLinearValue >> 8);
    kOutPortByte(VGA_PORT_INDEX, VGA_INDEX_LOWERCURSOR);
    kOutPortByte(VGA_PORT_DATA, iLinearValue & 0xFF);

    gs_stConsoleManager.iCurrentPrintOffset = iLinearValue;
}

void kGetCursor(int *piX, int *piY)
{
    *piX = gs_stConsoleManager.iCurrentPrintOffset % CONSOLE_WIDTH;
    *piY = gs_stConsoleManager.iCurrentPrintOffset / CONSOLE_WIDTH;
}

void kprintf(const char* pcFormatString, ...)
{
    va_list ap;
    char vcBuffer[0x400];
    int iNextPrintOffset;
    va_start(ap, pcFormatString);
    kvsprintf(vcBuffer, pcFormatString, ap);
    va_end(ap);

    iNextPrintOffset = kConsolePrintString(vcBuffer);
    kSetCursor(iNextPrintOffset % CONSOLE_WIDTH, iNextPrintOffset / CONSOLE_WIDTH);
}

int kConsolePrintString(const char* pcBuffer)
{
    CHARACTER* pstScreen = (CHARACTER *)CONSOLE_VIDEOMEMORYADDRESS;
    int i, j;
    int iLength = kstrlen(pcBuffer);
    int iPrintOffset = gs_stConsoleManager.iCurrentPrintOffset;
    for (i=0; i<iLength; ++i)
    {
        if(pcBuffer[i] == '\n')
            iPrintOffset += (CONSOLE_WIDTH - (iPrintOffset % CONSOLE_WIDTH));
        else if(pcBuffer[i] == '\t')
            iPrintOffset += (8 - (iPrintOffset % 8));
        else
        {
            pstScreen[iPrintOffset].bCharacter = pcBuffer[i];
            pstScreen[iPrintOffset].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
            iPrintOffset++;
        }

        // maximum of screen is over
        if (iPrintOffset >= (CONSOLE_HEIGHT * CONSOLE_WIDTH))
        {
            // overwrite the first line of screen
            kmemcpy((void *) CONSOLE_VIDEOMEMORYADDRESS,
                (const void *) CONSOLE_VIDEOMEMORYADDRESS + CONSOLE_WIDTH * sizeof(CHARACTER),
                (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH * sizeof(CHARACTER));
            
            for (j=(CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH; j < CONSOLE_WIDTH*CONSOLE_HEIGHT; ++j)
            {
                pstScreen[j].bCharacter = ' ';
                pstScreen[j].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
            }
            iPrintOffset = (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH;
        }    
    }
    return iPrintOffset;
}
void kClearScreen()
{
    CHARACTER* pstScreen = (CHARACTER *)CONSOLE_VIDEOMEMORYADDRESS;
    int i;
    for(i=0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT; ++i)
    {
        pstScreen[i].bCharacter = ' ';
        pstScreen[i].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
    }
    kSetCursor(0, 0);
}

BYTE kGetCh()
{
    KEYDATA stData;
    while(1)
    {
        while(kGetKeyFromKeyQueue(&stData) == FALSE);
        if(stData.bFlags & KEY_FLAGS_DOWN)
            return stData.bASCIICode;
    }
}

void kPrintStringXY(int iX, int iY, const char* pcString)
{
    CHARACTER * pstScreen = (CHARACTER *)CONSOLE_VIDEOMEMORYADDRESS;
    int i;
    
    pstScreen += (iY * 80) + iX;
    for (i=0; pcString[i]; ++i)
    {
        pstScreen[i].bCharacter = pcString[i];
        pstScreen[i].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
    }
}