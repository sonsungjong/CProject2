#include <Windows.h>
#include <iostream>
#include <conio.h>

// ���� �ʿ�
int mainSleepScreen() {
	SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);

	(void)_getch();

	SetThreadExecutionState(ES_CONTINUOUS);

	return 0;
}