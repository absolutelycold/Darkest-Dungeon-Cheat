#include "proc.h"

DWORD getPidByName(TCHAR* processName)
{

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap == INVALID_HANDLE_VALUE)
	{
		printf("Get Snap Handle Error: %d\n", GetLastError());
		return -1;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnap, &pe)) {

		do
		{
			if (_tcscmp(processName, pe.szExeFile) == 0)
			{
				CloseHandle(hSnap);
				//printf("PID: %d\n", pe.th32ProcessID);
				return pe.th32ProcessID;
			}
		} while (Process32Next(hSnap, &pe));

	}



	return 0;
}

DWORD getModuleBase(TCHAR* moduleName, DWORD pid)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

	if (hSnap == INVALID_HANDLE_VALUE)
	{
		printf("Create Module Snap Error: %d\n", GetLastError());
		return -1;
	}

	MODULEENTRY32 me;
	me.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnap, &me)) {
		do
		{
			if (_tcscmp(moduleName, me.szModule) == 0)
			{
				//printf("Module Base: %x\n", me.modBaseAddr);
				CloseHandle(hSnap);
				return DWORD(me.modBaseAddr);
			}
		} while (Module32Next(hSnap, &me));
	}

	return 0;
}

DWORD findFinalAddress(std::vector<DWORD> offset, DWORD baseAddr, DWORD pid)
{

	DWORD addr = baseAddr;
	HANDLE hProcess = NULL;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	for (int i = 0; i < (offset.size() - 1); i++)
	{
		ReadProcessMemory(hProcess, LPCVOID(addr + offset[i]), &addr, sizeof(DWORD), NULL);
	}

	addr += offset[offset.size() - 1];

	CloseHandle(hProcess);

	return addr;
}

BOOL patchEx(std::vector<BYTE> patchValue, DWORD addr, DWORD pid)
{
	DWORD sizeToPatch = patchValue.size();
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	DWORD preProtect;

	VirtualProtectEx(hProcess, (LPVOID)addr, sizeToPatch, PAGE_READWRITE, &preProtect);
	for (int i = 0; i < sizeToPatch; i++)
	{
		WriteProcessMemory(hProcess, LPVOID(addr + i), &(patchValue[i]), 1, NULL);
	}
	VirtualProtectEx(hProcess, (LPVOID)addr, sizeToPatch, preProtect, &preProtect);

	CloseHandle(hProcess);

	return 0;
}
