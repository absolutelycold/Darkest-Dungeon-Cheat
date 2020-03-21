#pragma once

#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <vector>
#include <iostream>

DWORD getPidByName(TCHAR* processName);
DWORD getModuleBase(TCHAR* moduleName, DWORD pid);
DWORD findFinalAddress(std::vector<DWORD> offset, DWORD baseAddr, DWORD pid);
BOOL patchEx(std::vector<BYTE> patchValue, DWORD addr, DWORD pid);
