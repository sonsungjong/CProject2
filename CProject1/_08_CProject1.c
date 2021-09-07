#include <stdio.h>
#include <malloc.h>

typedef struct MyData
{
	unsigned char age;
	unsigned short wage;
	unsigned int salary;
} MD;

int main81() {
	unsigned char* p;

	p = (unsigned char*)malloc(7);		// 7byte �޸𸮸� �Ҵ�

	*p = 23;		// ���� 1byte
	*(unsigned short*)(p + 1) = 10000;		// �ñ� 2byte
	*(unsigned int*)(p + 3) = 3000000;		// ���� 4byte

	free(p);

	MD data;
	data.age = 23;
	*(unsigned char*)&data = 23;
	data.wage = 10000;
	*(unsigned short*)((unsigned char*)&data + 2) = 10000;
	data.salary = 3000000;
	*(unsigned int*)((unsigned char*)&data + 4) = 3000000;

	MD* p2;
	p2 = &data;
	(*p2).age = 31;
	p2->age = 31;

	MD temp[3] = {
		{23,10000,3000000}, {24,10000,3000000},{25,10000,3000000}
	};

	temp[1].wage = 20000;
	(*(temp + 1)).wage = 30000;			// (temp+1)->wage
	*(unsigned short*)((unsigned char*)(temp + 1) + 2) = 20000;
	*((unsigned short*)(temp + 1) + 1) = 20000;
	*((unsigned short*)temp + 5) = 20000;

	return 0;
}

/*
�迭 : ���� ũ���� �����͸� �׷�����
����ü : �ٸ� ũ���� �����͵��� �׷�����

�Լ��� �ڵ��� ��ȭ���� ���������� 
�Ű������� ���� ��ȭ���� ������ ���� ����ü�� ����
*/