#include <iostream>

struct A {
	int a;
	int b;
};
struct B {
	int c;		// A�� ��ġ, ��ġ ��ġ
	int d;		// A�� ��ġ, ��ġ ��ġ
	int m;	// �� Ŀ�� ��
};

struct Tips {
	int from_year;
	int member_count;
	// ����������������
};
// ����� ����
struct Tipsware {
	int from_year;
	int member_count;
	int sales;
	// ������������������������
};

class Taps 
{
public:
	int m_from_year;
	int m_member_count;
};

class Tapsware : public Taps
{
public:
	int m_sales;
	// �����͸� ����Ҷ� ����� �θ������͸� ���(������)
	// Taps* �� ���
};

int main������() 
{
	A data = {2, 3};
	A temp = {4 ,5};
	// ��� ���� ����
	data.a = temp.a;
	data.b = temp.b;
	// ��� ���� ����
	memcpy(&data, &temp, sizeof(A));
	// ��� ���� ����
	data = temp;		// ���� �����ϸ� ������ �߻��� �� �־� ��������ڸ� ���
							// (�����޸� �Ҵ�������)
	
	// ��ü�� �����ɶ� �����ڰ� ȣ��ȴ�.

	// ������ ������ �ϳ��� �����ͷ� �������� ����ü�� ������ �� ����
	B* p0 = (B*)&data;		// ����ȯ
	p0->c = 34;		// A����ü�� data��ü�� a���� ���� �ٲ����
	std::cout << data.a << std::endl;		// ������ ������

	Tips data1;
	data1.from_year = 1992;
	data1.member_count = 9;

	Tipsware data2;
	data2.from_year = 2002;
	data2.member_count = 6;
	data2.sales = 8000;

	Tips* p1 = &data1;
	Tipsware* p2 = &data2;
	p1->from_year = 1992;
	p1->member_count = 5;

	p2->from_year = 2002;
	p2->member_count = 7;
	p2->sales = 8888;

	Tips* p3 = (Tips*)&data2;
	p3->from_year = 3003;
	p3->member_count = 600;
	*(&p3->member_count + 1) = 500;	// Tipsware 3��° ������ �־����
	
	return 0;
}


// = �����Ҵ� �޸𸮰� ���� ��� ���Կ����ڷ� ���
// = �����Ҵ� �޸𸮰� ���� ��� ��������ڷ� ����

// ������
// 
// ����� �����Ҷ� �ڷ����� �ִ��� ���� ���� ���� ���������� �����ϴ� => ������ ����
// �Ѱ��� �ڷ����� �Ἥ ���α׷���
// �ϳ��� ���°� ���������� Ŀ��

// ������ : �θ� �����ͷ� �ڽ�Ŭ������ ����ų �� �ִ�