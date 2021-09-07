#include <iostream>

using namespace std;
// �ְ߼� �������� �������� �� -> +����� -> +�ܽ���
// ���� ¢�� ����̵� ¢�� -> Pet�̶�� Ŭ������ ����

class Pet
{
// ������ : Pet���� ��� ����ų �� ���� (���հ���)
public:
	// �������̵� �������� ����� ����
	virtual void Sound() {}
	virtual void Eat() {}
};

class Dog : public Pet
{
public:
	void Sound() {
		cout << "�۸�" << endl;
	}
	void Eat() {
		cout << "���ٱ�" << endl;
	}
};

class Cat : public Pet
{
public:
	void Sound() {
		cout << "�߿�" << endl;
	}
	void Eat() {
		cout << "����" << endl;
	}
};

//#define MAX_DOG_COUNT		20
//#define MAX_CAT_COUNT		20
#define MAX_PET_COUNT		40

int main53() {
	Pet* p_pet_list[MAX_PET_COUNT];			// Pet���� �����Ͽ� ����
	//Dog* p_dog_list[MAX_DOG_COUNT];		// ����
	//Cat* p_cat_list[MAX_CAT_COUNT];		// ����
	int select, type;
	int pet_count = 0;

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
				p_pet_list[pet_count] = new Dog;		// ���� ����Ŵ
				pet_count++;
			}
			else if (type == 1) {
				p_pet_list[pet_count] = new Cat;		// ����̸� ����Ŵ
				pet_count++;
			}
			// �׸��� �߰��ǵ� Ư�� �κи� �ٲ��ָ� �ǵ��� �ڵ尡 ���ϵ�
		}
		else if (select == 2) {
			for (int i = 0; i < pet_count; i++)
			{
				p_pet_list[i]->Eat();
			}
		}
		else if (select == 3) {
			for (int i = 0; i < pet_count; i++)
			{
				p_pet_list[i]->Sound();
			}
		}
		else if (select == 4) {
			break;
		}
	}

	for (int i = 0; i < pet_count; i++)
	{
		delete p_pet_list[i];
	}
	
	return 0;
}

/*
	������ : �ڽ� ��ü�� �θ� ������
	virtual : �θ� �����ͷ� �ڽ� �޼��带 ����� �� �������̵��� �ڽ� �Լ��� ȣ���ϱ� ���� �θ� �Լ� �տ� ���̴� Ű����
	��ƾ�� �Ͽ�ȭ, �ϳ��� ��ü�� ��� ó���� �� ����
*/