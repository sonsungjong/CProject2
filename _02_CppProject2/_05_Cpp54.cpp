#include <iostream>

using namespace std;
// �ְ߼� �������� �������� �� -> +����� -> +�ܽ���
// ���� ¢�� ����̵� ¢�� -> Pet�̶�� Ŭ������ ����
// ���̺� ������� ���ϵ��� �ʴ� �ڵ带 ����

class Pet
{
	// ������ : Pet���� ��� ����ų �� ���� (���հ���)
public:
	// �������̵� �������� ����� ����
	virtual void Sound() {}
	virtual void Eat() {}

	virtual Pet* CreatePet() {
		return NULL;
	}
	virtual const char* GetName() {
		return NULL;
	}
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

	virtual Pet* CreatePet() {
		return new Dog;
	}

	virtual const char* GetName() {
		return "��";
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

	virtual Pet* CreatePet() {
		return new Cat;
	}

	virtual const char* GetName() {
		return "�����";
	}
};

class Mouse : public Pet
{
public:
	void Sound() {
		cout << "����" << endl;
	}
	void Eat() {
		cout << "ġ��" << endl;
	}

	virtual Pet* CreatePet() {
		return new Mouse;
	}

	virtual const char* GetName() {
		return "�ܽ���";
	}
};

//#define MAX_DOG_COUNT		20
//#define MAX_CAT_COUNT		20
#define MAX_PET_COUNT		40
#define PET_TABLE_COUNT	3

int main54() {
	Pet* p_pet_table[PET_TABLE_COUNT] = { new Dog, new Cat, new Mouse };		// ���Ѹ���, ������Ѹ����� �̸� ��������

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
			for (int i = 0; i < PET_TABLE_COUNT; i++)
			{
				cout << i << ": " << p_pet_table[i]->GetName() << ", ";
			}
			cout << " ? ";

			cin >> type;

			p_pet_list[pet_count] = p_pet_table[type]->CreatePet();		// ���̺� ���
			pet_count++;
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

	for (int i = 0; i < PET_TABLE_COUNT; i++)
	{
		delete p_pet_table[i];		// �̸� �������� new�� �����ϴ� �ڵ�
	}

	return 0;
}

/*
	������ : �ڽ� ��ü�� �θ� ������
	virtual : �θ� �����ͷ� �ڽ� �޼��带 ����� �� �������̵��� �ڽ� �Լ��� ȣ���ϱ� ���� �θ� �Լ� �տ� ���̴� Ű����
	��ƾ�� �Ͽ�ȭ, �ϳ��� ��ü�� ��� ó���� �� ����
*/