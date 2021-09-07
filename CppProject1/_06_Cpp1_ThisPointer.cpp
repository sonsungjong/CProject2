#include <iostream>
//#include <malloc.h> C++�� new �����ڸ� ���
// �������� + �������ʿ��ѳ���
using namespace std;
// this�����ͷ� �������� ����� ������ �ϰ�����

struct Node
{
	int num;
	Node* p_next;		// C++�� struct �Ƚᵵ��
};		// ��üȭ�Ҷ� �տ� struct �� �ʿ� ������ typedef ����

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
			//free(p_tail);		// malloc ����
			delete(mp_tail);		// new������ ����
		}
		mp_tail = NULL;
	}

	void AddNodeCpp(int a_num)	// �� �Լ� �ƱԸ�Ʈ�� MyDataList * const this; �����Ͱ� ������ ������
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

	cout << "�Է��� �����ϰ� �ʹٸ� 999�� �Է��ϼ���." << endl;
	while (1) {
		cout << "���� �Է��ϼ��� : ";
		cin >> num;

		if (num != 999) {
			my_list.AddNodeCpp(num);
		}
		else break;
	}

	/* �Լ��� ����
	while (p_head) {
		p_tail = p_head;
		p_head = p_head->p_next;
		//free(p_tail);		// malloc ����
		delete(p_tail);		// new������ ����
	}
	p_tail = NULL;
	*/
	return 0;
}
