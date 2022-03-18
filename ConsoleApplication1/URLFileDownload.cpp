#include <Windows.h>
#include <iostream>
#include <tchar.h>

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

int main() {
	// URL
	const TCHAR* url = _T("https://search.pstatic.net/sunny/?src=https%3A%2F%2Fi.pinimg.com%2Foriginals%2F9b%2F9e%2F16%2F9b9e165a2fbce83e99effd5c4416c486.jpg&type=sc960_832");
	// 저장할 공간
	const TCHAR* dst = _T("D:\\STUFF\\myfilename.png");

	if (S_OK == URLDownloadToFile(NULL, url, dst, 0, NULL))
	{
		printf("Saved.\n");
		return 0;
	}
	else {
		printf("Failed.\n");
		return -1;
	}
}