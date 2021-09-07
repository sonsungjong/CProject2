#include <iostream>

using namespace std;
// �ְ߼� �������� �������� �� -> +����� -> +�ܽ���
// �ڷ��� �߰��� ���� �� 2��, �ڵ� 2��
class Dog
{
public:
	void Sound() {
		cout << "�۸�" << endl;
	}
	void Eat() {
		cout << "���ٱ�" << endl;
	}
};

class Cat
{
public:
	void Sound() {
		cout << "�߿�" << endl;
	}
	void Eat() {
		cout << "����" << endl;
	}
};

#define MAX_DOG_COUNT		20
#define MAX_CAT_COUNT		20

int main52() {
	Dog* p_dog_list[MAX_DOG_COUNT];		// ����
	Cat* p_cat_list[MAX_CAT_COUNT];		// ����
	int select, type;
	int dog_count = 0, cat_count = 0;

	while (1) {
		cout << endl << "[  �޴�  ]" << endl;
		cout << "1. �߰�" << endl;
		cout << "2. �Ծ�" << endl;
		cout << "3. ¢��" << endl;
		cout << "4. ����" << endl;

		cout << "��� ���� : " << endl;
		cin >> select;

		if (select == 1) {
			cout << "0: ��, 1: ����� ? ";
			cin >> type;
			if (type == 0) {
				p_dog_list[dog_count] = new Dog;		// �� �Ѹ��� �߰�
				dog_count++;
			}
			else if (type == 1) {
				p_cat_list[cat_count] = new Cat;		// ����� �Ѹ��� �߰�
				cat_count++;
			}
		}
		else if (select == 2) {
			for (int i = 0; i < dog_count; i++) {
				p_dog_list[i]->Eat();
			}
			for (int i = 0; i < cat_count; i++) {
				p_cat_list[i]->Eat();
			}
		}
		else if (select == 3) {
			for (int i = 0; i < dog_count; i++) {
				p_dog_list[i]->Sound();
			}
			for (int i = 0; i < cat_count; i++) {
				p_cat_list[i]->Sound();
			}
		}
		else if (select == 4) {
			break;
		}
	}

	for (int i = 0; i < dog_count; i++)
	{
		delete p_dog_list[i];
	}
	for (int i = 0; i < cat_count; i++)
	{
		delete p_cat_list[i];
	}
	return 0;
}

/*
	������ : �ڽ� ��ü�� �θ� ������
	virtual : �θ� �����ͷ� �ڽ� �޼��带 ����� �� �������̵��� �ڽ� �Լ��� ȣ���ϱ� ���� �θ� �Լ� �տ� ���̴� Ű����

*/