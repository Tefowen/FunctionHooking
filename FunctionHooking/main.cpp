#include <Windows.h>
#include <iostream>
#include <stdlib.h>

// Function to hook
int __stdcall sum(int x, int y)
{
	int result = x;
	for (int i = 0; i < y; i++) result++;
	return result;
}

int main()
{
	while (true)
	{
		int x = rand() % 10 + 1;
		int y = rand() % 10 + 1;
		std::cout << "testprogram.exe: " << x << " + " << y << " = " << sum(x, y) << std::endl;
		Sleep(1000);
	}
	0;
}
