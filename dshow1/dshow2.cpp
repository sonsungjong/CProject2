#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>
#include "VIdeoCapture.h"
#include <tchar.h>
#include <iostream>
#define TEST(x) printf("\nPress enter to test: #x"); getchar(); fflush(stdin); x;
void callback(unsigned char* data, int len, int bpp)
{
	printf("DATA %d %d\n", len, bpp);
}

int main()
{
	VideoCapture* vc = new VideoCapture(callback);
	VideoDevice* devices = vc->GetDevices();
	int num_device = vc->NumDevices();
	for (int i = 0; i < num_device; i++)
	{
		printf("%s\n", devices[i].GetFriendlyName());
	}
	TEST(vc->Select(devices));

	TEST(vc->Start());

	//TEST(vc->Stop());

	printf("Done testing...");
	getchar();


	return 0;
}