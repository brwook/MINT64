#include "ConsoleShell.h"
#include "Console.h"
#include "Keyboard.h"
#include "Utility.h"

SHELLCOMMANDENTRY gs_vstCommandTable[] =
{
    {"help", "Show Help", kHelp},
    {"cls", "Clear Screen", kCls},
    {"totalram", "Show Total RAM Size", kShowTotalRamSize},
    {"strtod", "String to Decimal/Hex Convert", kStringToDecimalHexTest},
    {"shutdown", "Shutdown And Reboot OS", kShutdown},
};

// Main Loop
void kStartConsoleShell()
{
    char vcCommandBuffer[CONSOLESHELL_MAXCOMMANDBUFCOUNT];
    int iCommandBufferIndex = 0;
    BYTE bKey;
    int iCursorX, iCursorY;
    kprintf(CONSOLESHELL_PROMPTMESSAGE);
    while(1)
    {
        bKey = kGetCh();
        if(bKey == KEY_BACKSPACE)
        {
            if (iCommandBufferIndex > 0)
            {
                kGetCursor(&iCursorX, &iCursorY);
                kPrintStringXY(iCursorX - 1, iCursorY, " ");
                kSetCursor(iCursorX - 1, iCursorY);
                iCommandBufferIndex--;
            }
        }
        else if (bKey == KEY_ENTER)
        {
            kprintf("\n");
            if(iCommandBufferIndex > 0)
            {
                vcCommandBuffer[iCommandBufferIndex] = '\0';
                kExecuteCommand(vcCommandBuffer);
            }
            kprintf("%s", CONSOLESHELL_PROMPTMESSAGE);
            kmemset(vcCommandBuffer, '\0', CONSOLESHELL_MAXCOMMANDBUFCOUNT);
            iCommandBufferIndex = 0;
        }
        else if ((bKey == KEY_LSHIFT) || (bKey == KEY_RSHIFT) ||
            (bKey == KEY_CAPSLOCK) || (bKey == KEY_NUMLOCK) ||
            (bKey == KEY_SCROLLLOCK) || (bKey == KEY_CTRL) || (bKey == KEY_LALT))
        {
            ;
        }
        else
        {
            if (bKey == KEY_TAB)
            {
                bKey = ' ';
            }
            if (iCommandBufferIndex < CONSOLESHELL_MAXCOMMANDBUFCOUNT)
            {
                vcCommandBuffer[iCommandBufferIndex++] = bKey;
                kprintf("%c", bKey);
            }
        }
        
    }
}

void kExecuteCommand(const char* pcCommandBuffer)
{
    int i, iSpaceIndex;
    int iCommandBufferLength, iCommandLength;
    int iCount;

    iCommandBufferLength = kstrlen(pcCommandBuffer);
    for(iSpaceIndex = 0; iSpaceIndex < iCommandBufferLength; iSpaceIndex++)
    {
        if(pcCommandBuffer[iSpaceIndex] == ' ')
            break;
    }
    iCount = sizeof(gs_vstCommandTable) / sizeof(SHELLCOMMANDENTRY);
    for(i=0; i<iCount; ++i)
    {
        iCommandLength = kstrlen(gs_vstCommandTable[i].pcCommand);
        if(iCommandLength == iSpaceIndex && kmemcmp(gs_vstCommandTable[i].pcCommand, pcCommandBuffer, iSpaceIndex) == 0)
        {
            gs_vstCommandTable[i].pfFunction(pcCommandBuffer + iSpaceIndex + 1);
            break;
        }
    }
    if (i >= iCount)
    {
        kprintf("'%s' is not found.\n", pcCommandBuffer);
    }
}

void kInitializeParameter(PARAMETERLIST* pstList, const char* pcParameter)
{
    pstList->pcBuffer = pcParameter;
    pstList->iLength = kstrlen(pcParameter);
    pstList->iCurrentPosition = 0;
}

int kGetNextParameter(PARAMETERLIST *pstList, char *pcParameter)
{
    int i;
    int iLength;
    if (pstList->iLength <= pstList->iCurrentPosition)
        return 0;
    
    for (i = pstList->iCurrentPosition; i<pstList->iLength; ++i)
    {
        if(pstList->pcBuffer[i] == ' ')
            break;
    }

    kmemcpy(pcParameter, pstList->pcBuffer + pstList->iCurrentPosition, i);
    iLength = i - pstList->iCurrentPosition;
    pcParameter[iLength] = '\0';
    pstList->iCurrentPosition += iLength + 1;
    return iLength;
}

void kHelp(const char* pcParameterBuffer)
{
    int i, iCount, iCursorX, iCursorY;
    int iLength, iMaxCommandLength = 0;

    kprintf("=======================================================\n");
    kprintf("                   MINT64 Shell Help                   \n");
    kprintf("=======================================================\n");
    iCount = sizeof(gs_vstCommandTable) / sizeof(SHELLCOMMANDENTRY);
    for (i=0; i<iCount; ++i)
    {
        iLength = kstrlen(gs_vstCommandTable[i].pcCommand);
        if (iLength > iMaxCommandLength)
            iMaxCommandLength = iLength;
    }

    for (i=0; i<iCount; ++i)
    {
        kprintf("%s", gs_vstCommandTable[i].pcCommand);
        kGetCursor(&iCursorX, &iCursorY);
        kSetCursor(iMaxCommandLength, iCursorY);
        kprintf(" - %s\n", gs_vstCommandTable[i].pcHelp);
    }
}
void kCls(const char* pcParameterBuffer)
{
    kClearScreen();
    kSetCursor(0, 1);
}

void kShowTotalRamSize(const char* pcParameterBuffer)
{
    kprintf("Total RAM Size = %dMB\n", kGetTotalRAMSize());
}

void kStringToDecimalHexTest(const char* pcParameterBuffer)
{
    char vcParameter[100];
    int iLength;
    PARAMETERLIST stList;
    int iCount = 0;
    long lValue;

    kInitializeParameter(&stList, pcParameterBuffer);
    while(1)
    {
        iLength = kGetNextParameter(&stList, vcParameter);
        if(iLength == 0)
            break;
        
        kprintf("Param %d = '%s', Length = %d, ", iCount + 1, vcParameter, iLength);
        if(kmemcmp(vcParameter, "0x", 2) == 0)
        {
            lValue = katoi(vcParameter + 2, 16);
            kprintf("Hex Value = %q\n", lValue);
        }
        else
        {
            lValue = katoi(vcParameter, 10);
            kprintf("Decimal Value = %d\n", lValue);
        }
        iCount++;
    }
}
void kShutdown(const char* pcParameterBuffer)
{
    kprintf("System Shutdown Start...\n");
    kprintf("Press Any Key to Reboot PC...\n");
    kGetCh();
    kReboot();
}