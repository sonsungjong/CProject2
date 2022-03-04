#include <stdio.h>

typedef struct MyData
{
	unsigned char age;							// 나이
	unsigned short wage;						// 시급
	unsigned int salary;						// 월급
} MD;

int main2() {
	MD data = {23, 10000, 3000000};

	data.age = 23;
	data.wage = 10000;
	data.salary = 3000000;

	MD temp[3] = { {23, 10000, 3000000},{24, 10000, 3000000},{25, 10000, 3000000} };
	temp[0].age = 23;

	//temp[1].wage = 20000;				// 밑표현과 같음
	//(*(temp+1)).wage = 20000;				// 밑표현과 같음
	(temp+1)->wage = 20000;

	MD* p;
	p = &data;
	p->age = 31;								// (*p).age = 31;

	*(unsigned short*)((unsigned char*)(temp + 1) + 2) = 20000;			// temp[1].wage = 20000;
	// *((unsigned short*)temp + 5) = 20000;

	int total_age = 0, total_wage = 0, total_salary = 0;
	p = temp;
	MD* p_limit = temp + 3;					// 마지막 배열을 1초과한 주소
	//for (int i = 0; i < 3; i++)
	while(p < p_limit)							// i++ 연산을 없앤 while문
	{
		//total_age += temp[i].age;								// 구조체배열의 중복코드
		total_age += p->age;
		//total_wage += (*(temp+i)).wage;
		total_wage += p->wage;
		//total_salary += (temp+i)->salary;
		total_salary += p->salary;						// 포인터로 변경해서 중복연산을 줄임
		p++;
	}
	// Displacement Addressing

	return 0;
}
