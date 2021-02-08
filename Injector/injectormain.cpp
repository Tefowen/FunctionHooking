#include <iostream>
#include <Windows.h>
#include <string>

int main()
{
	// Dll path
	LPCSTR DllPath = "C:\\Coding\\FunctionHooking\\Release\\Dll.dll";

	// Getting process ID
	HWND windowHandle = FindWindowW(NULL, L"C:\\Coding\\FunctionHooking\\Release\\testprogram.exe");
	DWORD* processID = new DWORD;
	GetWindowThreadProcessId(windowHandle, processID);

	// Open a handle to target process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, *processID);

	// Allocate memory
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1, MEM_COMMIT, PAGE_READWRITE);

	// Write the path to the address of the memory
	WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath, strlen(DllPath) + 1, 0);

	// Create a remote thread to call LoadLibraryA
	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), pDllPath, 0, 0);

	// Wait for execution of loader thread to finish
	WaitForSingleObject(hLoadThread, INFINITE);
	std::cout << "Dll path located at: " << pDllPath << std::endl;
	std::cin.get();

	// Free the memory allocated for the Dll path
	VirtualFreeEx(hProcess, pDllPath, strlen(DllPath) + 1, MEM_RELEASE);

	return 0;
}