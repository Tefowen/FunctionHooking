#include "pch.h"

DWORD AddressOfSum = 0;
// Template of original function
typedef int(*sum)(int x, int y);

int __declspec(noinline) __stdcall HookSum(int x, int y)
{
	// Manipulate arguments
	x += 500;
	y += 500;

	std::cout << "HACKHACKHACK\n";

	// Call original function
	sum originalSum = (sum)AddressOfSum;
	return originalSum(x, y);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Use signature scanning to find the function to hook
		SigScan Scanner;

		AddressOfSum = Scanner.FindPattern((char*)"testprogram.exe",
			(char*)"\x85\xD2\x8D\x04\x11",
			(char*)"xxxxx");

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		// Hook the function
		DetourAttach(&(LPVOID&)AddressOfSum, &HookSum);

		DetourTransactionCommit();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		// Unhook from program
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		// Hook the function
		DetourDetach(&(LPVOID&)AddressOfSum, &HookSum);

		DetourTransactionCommit();
	}
	return TRUE;
}
