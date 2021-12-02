#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

int main(){
	int 도 = 260, 레 = 290, 미 = 330, 파 = 340, 솔 = 380, 라 = 430, 시 = 490, 높은도 = 510;
	int 짧 = 90, 긴 = 120, 뒤 = 200;
	char c;
	while (1) {
		if(_kbhit()){									// 키보드 입력확인 true/false
			c = _getch();								// 키보드 입력값 저장
			printf("입력값 : %c\n", c);

			if (c == 'a') {				// 도
				Beep(도, 뒤);
				Sleep(짧);
			}
			else if (c == 's') {			// 레
				Beep(레, 뒤);
				Sleep(짧);
			}
			else if (c == 'd') {			// 미
				Beep(미, 뒤);
				Sleep(짧);
			}
			else if (c == 'f') {			// 파
				Beep(파, 뒤);
				Sleep(짧);
			}
			else if (c == 'j') {			// 솔
				Beep(솔, 뒤);
				Sleep(짧);
			}
			else if (c == 'k') {				// 라
				Beep(라, 뒤);
				Sleep(짧);
			}
			else if (c == 'l') {				// 시
				Beep(시, 뒤);
				Sleep(짧);
			}
			else if (c == 59) {				// 높은도
				Beep(높은도, 뒤);
				Sleep(짧);
			}
			else if (c == 27 || c == 'p') {
				break;
			}
			else if (c == '1') {
				Sleep(2000);

				// 바둑이
				Beep(도, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(긴);
				Beep(미, 뒤);
				Sleep(긴);

				Beep(레, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(긴);
				Beep(파, 뒤);
				Sleep(긴);

				Beep(미, 뒤);
				Sleep(짧);
				Beep(솔, 뒤);
				Sleep(짧);
				Beep(솔, 뒤);
				Sleep(긴);
				Beep(솔, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(긴);
				Beep(레, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(레, 뒤);
				Sleep(짧);
				Beep(도, 뒤);
				Sleep(긴);

				Beep(높은도, 뒤);
				Sleep(긴);
				Beep(높은도, 뒤);
				Sleep(긴);
				Beep(솔, 뒤);
				Sleep(긴);
				Beep(솔, 뒤);
				Sleep(긴);
				Beep(라, 뒤);
				Sleep(짧);
				Beep(높은도, 뒤);
				Sleep(짧);
				Beep(시, 뒤);
				Sleep(짧);
				Beep(라, 뒤);
				Sleep(짧);
				Beep(솔, 뒤);
				Sleep(긴);

				Beep(높은도, 뒤);
				Sleep(긴);
				Beep(높은도, 뒤);
				Sleep(긴);
				Beep(솔, 뒤);
				Sleep(긴);
				Beep(솔, 뒤);
				Sleep(긴);
				Beep(라, 뒤);
				Sleep(짧);
				Beep(높은도, 뒤);
				Sleep(짧);
				Beep(시, 뒤);
				Sleep(짧);
				Beep(라, 뒤);
				Sleep(짧);
				Beep(솔, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(레, 뒤);
				Sleep(짧);

				Beep(도, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(긴);
				Beep(미, 뒤);
				Sleep(긴);

				Beep(레, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(긴);
				Beep(파, 뒤);
				Sleep(긴);

				Beep(미, 뒤);
				Sleep(짧);
				Beep(솔, 뒤);
				Sleep(짧);
				Beep(솔, 뒤);
				Sleep(긴);
				Beep(솔, 뒤);
				Sleep(긴);
				Beep(미, 뒤);
				Sleep(긴);
				Beep(레, 뒤);
				Sleep(짧);
				Beep(파, 뒤);
				Sleep(짧);
				Beep(미, 뒤);
				Sleep(짧);
				Beep(레, 뒤);
				Sleep(짧);
				Beep(도, 뒤);
				Sleep(긴);
			}
			else {
				printf("없는key, 다시 입력하세요\n");
				continue;
			}
		}
	}
	printf("종료\n");

	return 0;
}