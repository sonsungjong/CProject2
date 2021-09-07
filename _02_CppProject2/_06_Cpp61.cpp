#include <iostream>

using namespace std;

int g_data;			// ��������

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
	static int m_data;		// �ش� Ŭ���� �ȿ����� ��� ������ ��������
public:
	MyDataLst()
	{
		static int data;		// �Լ��ȿ����� ����� �� �ִ� ��������
		mp_head = mp_tail = NULL;
	}
	~MyDataLst()
	{
		DeleteAllNode();
	}

	Node* GetHead() const		// ��������� ���� ���ٲٰ���, ��븸����
	{
		// mp_head = NULL;	�� ���� �Ұ���
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

	cout << "�Է��� �����Ϸ��� 999�� �Է��ϼ���" << endl;
	while (1) {
		cout << "���� �Է��ϼ��� : ";
		cin >> num;

		if (num != 999) {
			my_list.AddNode(num);
		}
		else break;
	}
	
	return 0;
}

/*
	this�����ͷ� ������ ������ �����

	const �� �����Ͻ� �����
	�����Ͽ��� ���� ������ �� ���� ó��
	const int* p : p�� ����Ű�� ������ ���ȭ
	int* const p : �����ͺ��� p�� �ּҰ��� ���ٲ�
	void �Լ�() const {} : const�������

	static������ ��������

	reference &

	���� ������
	(const Ŭ������& �����Ұ�ü��)

*/