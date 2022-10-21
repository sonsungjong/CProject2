#include <stdio.h>

int main_201()
{
	char good[] = "Good!";
	char* bad = "Bad!";
	printf("%s %s \n", good, bad);

	good[0] = 'H';
	//good = "New Good";			// 배열길이 초과로 컴파일에러

	bad[0] = 'S';							// 포인터 기반 문자열은 개별요솟값 할당 불가능 (변경없음)
	bad = "New Bad";
	printf("%s %s \n", good, bad);


	// putchar : 문자 또는 정수를 출력
	// getchar : 키보드로 한개의 문자를 입력받음
	int ch1, ch2;
	ch1 = getchar();
	ch2 = getchar();
	putchar(ch1);
	putchar(ch2);

	// puts, gets
	char cc[30];
	gets(cc);				// 문자열을 입력받아 파라미터에 저장
	puts(cc);		// 문자열 출력 후 자동줄바꿈

	// EOF : End Of File (입력이 종료되었음)
	// 읽어들일 데이터가 없어 함수가 실패하거나 Ctrl+z 같은 뒤로가기를 실행했을 경우 발생
	int ch;
	while (1)
	{
		ch = getchar();

		if (ch == EOF)				// 컨트롤Z (리눅스는 컨트롤D)
		{
			break;
		}
		putchar(ch);
	}

	return 0;
}