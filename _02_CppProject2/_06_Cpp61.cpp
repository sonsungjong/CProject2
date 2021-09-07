#include <iostream>

using namespace std;

int g_data;			// 전역변수

struct Node
{
	int num;
	Node* p_next;
};

class MyDataLst
{
protected:
	Node* mp_head;
	Node* mp_tail;
	static int m_data;		// 해당 클래스 안에서만 사용 가능한 전역변수
public:
	MyDataLst()
	{
		static int data;		// 함수안에서만 사용할 수 있는 전역변수
		mp_head = mp_tail = NULL;
	}
	~MyDataLst()
	{
		DeleteAllNode();
	}

	Node* GetHead() const		// 멤버변수의 값을 못바꾸게함, 사용만가능
	{
		// mp_head = NULL;	값 변경 불가능
		return mp_head;
	}

	void DeleteAllNode()
	{
		while (mp_head)
		{
			mp_tail = mp_head;
			mp_head = mp_head->p_next;
			delete mp_tail;
		}
		mp_tail = NULL;
	}

	__inline void AddNode(int a_num)		// MyDataLst* const this;
	{
		if (mp_head == NULL) {
			mp_tail = mp_head = new Node;
		}
		else {
			mp_tail->p_next = new Node;
			mp_tail = mp_tail->p_next;
		}
		mp_tail->num = a_num;
		mp_tail->p_next = NULL;
	}
};

int MyDataLst::m_data = 0;

int main61() {
	MyDataLst my_list;
	int num = 0;

	cout << "입력을 종료하려면 999를 입력하세요" << endl;
	while (1) {
		cout << "값을 입력하세요 : ";
		cin >> num;

		if (num != 999) {
			my_list.AddNode(num);
		}
		else break;
	}
	
	return 0;
}

/*
	this포인터로 포인터 문법이 사라짐

	const 는 컴파일시 사라짐
	컴파일에서 값을 변경할 수 없게 처리
	const int* p : p가 가리키는 변수를 상수화
	int* const p : 포인터변수 p의 주소값을 못바꿈
	void 함수() const {} : const멤버변수

	static변수와 전역변수

	reference &

	복사 생성자
	(const 클래스명& 복사할객체명)

*/