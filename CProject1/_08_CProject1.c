#include <stdio.h>
#include <malloc.h>

// 나이 1바이트
// 시급 2바이트
// 월급 4바이트
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

typedef struct MyData
{
	// 작은 자료형부터 선언해야 데이터 낭비가 적어짐!
	unsigned char age;
	unsigned short wage;
	unsigned int salary;
} MD;

// 자료형 혹은 변수의 갯수가 변화할 수 있는 경우 구조체로 대처

int main81() {
	UINT8* p;
	p = (UINT8*)malloc(7);		// 7byte 메모리를 할당
	*p = 23;		// 나이 1byte
	*(UINT16*)(p + 1) = 10000;		// 시급 2byte
	*(UINT32*)(p + 3) = 3000000;		// 월급 4byte
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
배열 : 같은 크기의 데이터를 그룹지음
구조체 : 다른 크기의 데이터들을 그룹지음

함수는 코드의 변화에는 대응하지만 
매개변수에 대한 변화에는 감당을 못해 구조체로 제공
*/