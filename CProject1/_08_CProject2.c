#include <stdio.h>

typedef struct MyData
{
	unsigned char age;							// ����
	unsigned short wage;						// �ñ�
	unsigned int salary;						// ����
} MD;

int main2() {
	MD data = {23, 10000, 3000000};

	data.age = 23;
	data.wage = 10000;
	data.salary = 3000000;

	MD temp[3] = { {23, 10000, 3000000},{24, 10000, 3000000},{25, 10000, 3000000} };
	temp[0].age = 23;

	//temp[1].wage = 20000;				// ��ǥ���� ����
	//(*(temp+1)).wage = 20000;				// ��ǥ���� ����
	(temp+1)->wage = 20000;

	MD* p;
	p = &data;
	p->age = 31;								// (*p).age = 31;

	*(unsigned short*)((unsigned char*)(temp + 1) + 2) = 20000;			// temp[1].wage = 20000;
	// *((unsigned short*)temp + 5) = 20000;

	int total_age = 0, total_wage = 0, total_salary = 0;
	p = temp;
	MD* p_limit = temp + 3;					// ������ �迭�� 1�ʰ��� �ּ�
	//for (int i = 0; i < 3; i++)
	while(p < p_limit)							// i++ ������ ���� while��
	{
		//total_age += temp[i].age;								// ����ü�迭�� �ߺ��ڵ�
		total_age += p->age;
		//total_wage += (*(temp+i)).wage;
		total_wage += p->wage;
		//total_salary += (temp+i)->salary;
		total_salary += p->salary;						// �����ͷ� �����ؼ� �ߺ������� ����
		p++;
	}
	// Displacement Addressing

	return 0;
}
