#include <iostream>

using namespace std;

struct Tips
{
	int from_year;
	int member_count;
};

struct Tipsware
{
	int from_year;
	int member_count;
	int sales;
};		// 8byte�� ��ġ

struct A{
	int a, b, z;
};

struct B{
	int c, d;
};

int main42() {
	Tips data1;
	data1.from_year = 1992;
	data1.member_count = 8;
	// �ڷ����� 2���� �ڵ嵵 2��

	Tipsware data2;
	data2.from_year = 2002;
	data2.member_count = 6;
	data2.sales = 6000;
	// �ڷ����� 3���� �ڵ嵵 3��

	Tips* p1 = &data1;
	p1->from_year = 1992;
	p1->member_count = 8;

	Tipsware* p2 = &data2;
	p2->from_year = 2002;
	p2->member_count = 6;
	p2->sales = 6000;

	// �̸��� �޶� ������ ������ ���ٰ���
	Tipsware data3;
	Tips* p3 = (Tips*)&data3;
	p3->from_year = 2002;			// Tipsware�� 4byte �޸𸮿� 2002 �Ҵ� (0~4)
	p3->member_count = 6;		// Tipsware�� 4byte �޸𸮿� 6 �Ҵ�		(5~8)
	*(&p3->member_count+1) = 5000;		// Tipsware�� 4byte �޸𸮿� 5000 �Ҵ�		(9~12)

	A dataA;

	B* p_data = (B*)&dataA;		// ����ȯ	(byte�������� ������ �����ϱ� ����)
	p_data->c = 4;		// dataA.a �� 4�� ��
	*(&p_data->c+1) = 5;		// dataA.b �� 5 ��
	p_data->d = 99;		// dataA.b �� 5 ��
	*(&p_data->c+2) = 77;		// dataA.z �� 77 ��
	
	cout << dataA.a << endl;
	cout << dataA.b << endl;
	cout << dataA.z << endl;

	// A >= B* : ����, �Ѿ �κ��� ���X
	// A <= B* : ����, �޸𸮸� ���(�޸� ħ��)
	// �����ͺ��� ����� �ڷ����� �� Ŀ�� ������

	// �׷��Ƿ�, ��ӿ��� �ڽ��� �� ũ�Ƿ� �θ������ͷ� �ڽİ�ü�� �����ϰ� ����ų �� ����
	// ������ : �θ�Ŭ���� �����ͷ� �ڽ�Ŭ������ ����ų �� �ִ�

	return 0;
}

/*
	������ : �θ�Ŭ������ �����ͷ� �ڽŰ� �ڽ�Ŭ�������� ����ų �� �ֱ� ������ �θ�Ŭ���������ͷ� ��ɱ����� ���� �� �� �ִ�.
*/