#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

int main(){
	int �� = 260, �� = 290, �� = 330, �� = 340, �� = 380, �� = 430, �� = 490, ������ = 510;
	int ª = 90, �� = 120, �� = 200;
	char c;
	while (1) {
		if(_kbhit()){									// Ű���� �Է�Ȯ�� true/false
			c = _getch();								// Ű���� �Է°� ����
			printf("�Է°� : %c\n", c);

			if (c == 'a') {				// ��
				Beep(��, ��);
				Sleep(ª);
			}
			else if (c == 's') {			// ��
				Beep(��, ��);
				Sleep(ª);
			}
			else if (c == 'd') {			// ��
				Beep(��, ��);
				Sleep(ª);
			}
			else if (c == 'f') {			// ��
				Beep(��, ��);
				Sleep(ª);
			}
			else if (c == 'j') {			// ��
				Beep(��, ��);
				Sleep(ª);
			}
			else if (c == 'k') {				// ��
				Beep(��, ��);
				Sleep(ª);
			}
			else if (c == 'l') {				// ��
				Beep(��, ��);
				Sleep(ª);
			}
			else if (c == 59) {				// ������
				Beep(������, ��);
				Sleep(ª);
			}
			else if (c == 27 || c == 'p') {
				break;
			}
			else if (c == '1') {
				Sleep(2000);

				// �ٵ���
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);

				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);

				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);

				Beep(������, ��);
				Sleep(��);
				Beep(������, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(ª);
				Beep(������, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);

				Beep(������, ��);
				Sleep(��);
				Beep(������, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(ª);
				Beep(������, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);

				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);

				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);

				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(��);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(ª);
				Beep(��, ��);
				Sleep(��);
			}
			else {
				printf("����key, �ٽ� �Է��ϼ���\n");
				continue;
			}
		}
	}
	printf("����\n");

	return 0;
}