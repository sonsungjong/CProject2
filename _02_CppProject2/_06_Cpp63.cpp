#include <iostream>

using namespace std;

void App(void (*afp_user)());
void MyUser();
// ������ ���� -> �Լ������͸� �迭�� ������ �� �ְԵ�
void OnLButtonDown(int wParam, int lParam);
void OnLButtonUp(int wParam, int lParam);
void OnMouseMove(int wParam, int lParam);

//#define OnLButtonDown			0
//#define OnLButtonUp				1
//#define OnMouseMove				2

// Stream
int main63() {
	cout.flags(ios::left);		// ���� ����
	cout.width(8);				// 8ĭ Ȯ��
	cout << "hello";
	cout.flags(ios::right);		// ������ ����
	cout.width(5);				// 5ĭ Ȯ��
	cout << "world" << endl;

	void (*p_msg_proc[3])(int, int) = { OnLButtonDown, OnLButtonUp, OnMouseMove};
	int msg_id;

	(*p_msg_proc[msg_id])(2, 3);

	App(MyUser);

	return 0;
}

/*
	Stream : ���⼺�� �ִ� ������ (�����)
	�������� �帧, �̵��ϴ� ������
	iostream Ŭ������ �����ؼ� ������� ����

	�ܼ� : 50�Ⱓ ��ɾ OS�� ���Ͻ�Ű�� ���� ����� ��¥�Լ� malloc, new
	OS API : OS�������� ��ɾ� heapalloc (windows)
	
	������ ���� �ý���(�����)

	API : OS ���� �Լ�
	SDK : ����ȯ��
	IDE : ���հ���ȯ��

	H : �ڵ�
	�Լ������� : ����ǰ ������ ���� C�����, ����ϴ� ������ �Լ��� �Ű������� �ѱ� (�Լ����� �̸� ���س��� �� ����, OS������ ���� <-> ����ڰ� ����� ���� �� ����)

	C#�̳� python���� APIȣ��?
	�ü��C (�ַܼ� ����?) -> API -> malloc?
	32��Ʈ�� 64��Ʈ ũ�⺯��Ǵ� �ڷ���
	
	windows : �޽������� (��ȣ����)
	�ٸ�OS : ���Ǳ���

	WM_ : ������޽���
	WM_LBUTTONDOWN : ���ʹ�ư ����
	WM_LBUTTONUP : ���ʹ�ư ������

	wParam : word
	lParam : long

	ON : �޽��� ����� ó���ϴ� �Լ��� ���̴� ���ξ�
	Event : On/Off (������)
	Message : On/Off + �ΰ�����

	����ó�� (���� + �׷���)
	GDI : �׷��� ����̽� �������̽� (32��Ʈ�� ���� ���׷���)
	DC : �Ű������� ���̱� ���ؼ� �� �ȹٲ�� �׸���Ӽ��� ����ü�� ���� ��	(Pen + Brush + Font + ...)	 , Device Context
	Pen : DC����ü �ȿ� �ִ� ���Ӽ� ����ü
	Brush : DC����ü �ȿ� �ִ� ��Ӽ� ����ü (ä���)

	HDC, HPen, HBrush, HFont

*/

void App(void (*afp_user)())			// �Լ�������
{
	// Init();		// ����ǰ

	if(afp_user != NULL)
	(*afp_user)();			// ���߿� ����ڰ� ���� �Լ��� �Ű����� �������� ���� : �Լ����� �̸� ���س����� �������� �ȵǰ�, ���س����� ����ڰ� �ش� ����� �߰��� �� ����

	// Delete();			// ����ǰ
}

void MyUser()
{

}

void OnLButtonDown(int wParam, int lParam) 
{

}
void OnLButtonUp(int wParam, int lParam)
{

}
void OnMouseMove(int wParam, int lParam) 
{

}