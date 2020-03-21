// Darkest Dungeon Cheat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "proc.h"

DWORD pid;
DWORD moduleBaseAddr;
DWORD item1FinalAddr;
std::vector<DWORD> itemOffset1 = { 0x0179E9E0, 0x1B0, 0x118, 0x10, 0x8, 0x0 };
DWORD onekillAddr = 0;
DWORD noHarmAddr = 0;
DWORD noStressAddr = 0;
std::vector<BYTE> beforeOneKill = { 0xF3, 0x0F, 0x5C, 0xC1 };
std::vector<BYTE> afterOneKill = { 0xF3, 0x0F, 0x5C, 0xC0 };
std::vector<BYTE> beforeNoHarm = { 0xF3, 0x0F, 0x5C, 0x45, 0x0C };
std::vector<BYTE> afterNoHarm = { 0xF3, 0x0F, 0x58, 0x45, 0x0C };
//std::vector<BYTE> afterNoHarm = { 0x90, 0x90, 0x90, 0x90, 0x90 };
std::vector<BYTE> beforeNoStress = { 0xF3, 0x0F, 0x58, 0x8E, 0x54, 0x0E, 0x00, 0x00};
std::vector<BYTE> afterNoStress = { 0xF3, 0x0F, 0x5C, 0xC9, 0x90, 0x90, 0x90, 0x90 };
//std::vector<BYTE> afterNoStress = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
BOOL isOneKill = FALSE;
BOOL isNoHarm = FALSE;
BOOL isNoStress = FALSE;
HANDLE UIThread = NULL;
HANDLE hProcess;


DWORD WINAPI UIfunc(LPVOID lpThreadParameter);
void toggleOneKill();
void toggleNoHarm();
void toggleNoStress();
void set9999onFirstItem();

int main()
{
    pid = getPidByName((TCHAR*)(TEXT("Darkest.exe")));
    moduleBaseAddr = getModuleBase(PTCHAR(TEXT("darkest.exe")), pid);
    

    onekillAddr = moduleBaseAddr + 0x1105D8E;
    noHarmAddr = moduleBaseAddr + 0x10D22AD;
    noStressAddr = moduleBaseAddr + 0x10D299A;

    UIThread = CreateThread(NULL, 0, UIfunc, 0, 0, NULL);

    DWORD exitCode;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    
    while (GetExitCodeProcess(hProcess, &exitCode) && exitCode == STILL_ACTIVE)
    {
        if (GetAsyncKeyState(VK_F1) & 1) {
            isOneKill = !isOneKill;
            toggleOneKill();
            ResumeThread(UIThread);
        }

        if (GetAsyncKeyState(VK_F2) & 1)
        {
            isNoHarm = !isNoHarm;
            toggleNoHarm();
            ResumeThread(UIThread);
        }

        if (GetAsyncKeyState(VK_F3) & 1)
        {
            isNoStress = !isNoStress;
            toggleNoStress();
            ResumeThread(UIThread);
        }

        if (GetAsyncKeyState(VK_F4) & 1)
        {
            set9999onFirstItem();
        }


    }


    CloseHandle(UIThread);
    CloseHandle(hProcess);
}

DWORD WINAPI UIfunc(LPVOID lpThreadParameter) {
    while (true)
    {
        system("cls");

        printf("-----------Author: Absolutelycold-----------\n\n");
        printf("Update on github: https://github.com/absolutelycold \n\n");

        if (isOneKill)
        {
            printf("F1: One Kill: > ON <\n\n");
        }
        else
        {
            printf("F1: One Kill: > OFF <\n\n");
        }


        if (isNoHarm)
        {
            printf("F2: No Harm: > ON <\n\n");
        }
        else
        {
            printf("F2: No Harm: > OFF <\n\n");
        }
        
        if (isNoStress)
        {
            printf("F3: No Stress: > ON <\n\n");
        }
        else
        {
            printf("F3: No Stress: > OFF <\n\n");
        }

        printf("F4: set 9999 on the first item\n\n");
        SuspendThread(UIThread);
    }
}

void toggleOneKill() {
    if (isOneKill)
    {
        patchEx(afterOneKill, onekillAddr, pid);
    }
    else
    {
        patchEx(beforeOneKill, onekillAddr, pid);
    }
}

void toggleNoHarm() {
    if (isNoHarm)
    {
        patchEx(afterNoHarm, noHarmAddr, pid);
    }
    else
    {
        patchEx(beforeNoHarm, noHarmAddr, pid);
    }
}

void toggleNoStress() {
    if (isNoStress) {
        patchEx(afterNoStress, noStressAddr, pid);
    }
    else
    {
        patchEx(beforeNoStress, noStressAddr, pid);
    }
}

void set9999onFirstItem() {
    DWORD num = 9999;
    DWORD numberToWrite;
    item1FinalAddr = findFinalAddress(itemOffset1, moduleBaseAddr, pid);
    if (WriteProcessMemory(hProcess, (LPVOID)item1FinalAddr, &num, 4, &numberToWrite) == 0) {
        DWORD lastError = GetLastError();
    }
}
