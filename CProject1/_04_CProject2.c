#include <stdio.h>
// �������� ���� ���� ����
int main4ȸ2��°() {
	const short a = 3;			// 2byte
	void* p;		// 4byte		(��ɱ�� -> �ڷ������ : �Ǽ�ã���ַ����ϴٰ� �����ϴµ� �����ʰԵ�)

	p = &a;
	*(short*)p = 44;
	// p = (short*)((char*)p + 1);		// �ּ� 1��ŭ �ű�

	printf("%hd\n", a);

	return 0;
}