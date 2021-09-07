#include <iostream>
//#include <malloc.h> C++은 new 연산자를 사용
// 빠진내용 + 보충이필요한내용
using namespace std;
// this포인터로 포인터의 사용을 간단히 하고자함

struct Node
{
	int num;
	Node* p_next;		// C++은 struct 안써도됨
};		// 객체화할때 앞에 struct 쓸 필요 없으니 typedef 안함

class MyDataList {
private:
	Node* mp_head;
	Node* mp_tail;

public:
	MyDataList()
	{
		mp_head = mp_tail = NULL;
	}

	~MyDataList()
	{
		DeleteAllNode();
	}

	void DeleteAllNode()
	{
		while (mp_head) {
			mp_tail = mp_head;
			mp_head = mp_head->p_next;
			//free(p_tail);		// malloc 해제
			delete(mp_tail);		// new연산자 해제
		}
		mp_tail = NULL;
	}

	void AddNodeCpp(int a_num)	// 각 함수 아규먼트에 MyDataList * const this; 포인터가 숨겨져 들어가있음
	{
		if (mp_head == NULL) {
			//*ap_tail = *ap_head = (Node*)malloc(sizeof(Node));
			//*ap_tail = *ap_head = new Node;
			mp_tail = mp_head = new Node;
		}
		else {
			//(*ap_tail)->p_next = (Node*)malloc(sizeof(Node));
			//(*ap_tail)->p_next = new Node;
			mp_tail->p_next = new Node;
			//(*ap_tail) = (*ap_tail)->p_next;
			mp_tail = mp_tail->p_next;
		}
		//(*ap_tail)->num = a_num;
		mp_tail->num = a_num;
		//(*ap_tail)->p_next = NULL;
		mp_tail->p_next = NULL;
	}
};

int mainThis() {
	MyDataList my_list;
	//Node* p_head = NULL;
	//Node* p_tail = NULL;
	int num = 0;

	cout << "입력을 종료하고 싶다면 999를 입력하세요." << endl;
	while (1) {
		cout << "값을 입력하세요 : ";
		cin >> num;

		if (num != 999) {
			my_list.AddNodeCpp(num);
		}
		else break;
	}

	/* 함수로 만듦
	while (p_head) {
		p_tail = p_head;
		p_head = p_head->p_next;
		//free(p_tail);		// malloc 해제
		delete(p_tail);		// new연산자 해제
	}
	p_tail = NULL;
	*/
	return 0;
}
