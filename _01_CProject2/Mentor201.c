#include <stdio.h>

int main_201()
{
	char good[] = "Good!";
	char* bad = "Bad!";
	printf("%s %s \n", good, bad);

	good[0] = 'H';
	//good = "New Good";			// �迭���� �ʰ��� �����Ͽ���

	bad[0] = 'S';							// ������ ��� ���ڿ��� ������ڰ� �Ҵ� �Ұ��� (�������)
	bad = "New Bad";
	printf("%s %s \n", good, bad);


	// putchar : ���� �Ǵ� ������ ���
	// getchar : Ű����� �Ѱ��� ���ڸ� �Է¹���
	int ch1, ch2;
	ch1 = getchar();
	ch2 = getchar();
	putchar(ch1);
	putchar(ch2);

	// puts, gets
	char cc[30];
	gets(cc);				// ���ڿ��� �Է¹޾� �Ķ���Ϳ� ����
	puts(cc);		// ���ڿ� ��� �� �ڵ��ٹٲ�

	// EOF : End Of File (�Է��� ����Ǿ���)
	// �о���� �����Ͱ� ���� �Լ��� �����ϰų� Ctrl+z ���� �ڷΰ��⸦ �������� ��� �߻�
	int ch;
	while (1)
	{
		ch = getchar();

		if (ch == EOF)				// ��Ʈ��Z (�������� ��Ʈ��D)
		{
			break;
		}
		putchar(ch);
	}

	return 0;
}