#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

using namespace std;
// �������� ���̺����� �̿��� �ڵ�
class Pet
{
// �θ�� Dog�� ����ų �� �ְ� Cat�� ����ų �� ����
// ������� virtual�� �������̵� ����
public:
	virtual void Sound()
	{

	}
	virtual void Eat()
	{

	}
	// ���̺� ���
	virtual Pet* CreatePet()
	{
		return NULL;
	}
	virtual const char* GetName()
	{
		return NULL;
	}
};

class _Dog : public Pet
{
public:
	void Sound()
	{
		cout << "�۸�" << endl;
	}
	void Eat()
	{
		cout << "���ٱ�" << endl;
	}

	Pet* CreatePet()
	{
		return new _Dog;
	}
	const char* GetName()
	{
		return "��";
	}
};
// �������� �̿�
class _Cat : public Pet
{
public:
	void Sound()
	{
		cout << "�߿�" << endl;
	}
	void Eat()
	{
		cout << "����" << endl;
	}

	Pet* CreatePet()
	{
		return new _Cat;
	}
	const char* GetName()
	{
		return "�����";
	}
};

// �������� �̿��ؼ� ��ȭ�� ���� �ݿ��Ǵ� �ڵ带 �ּ�ȭ
class _Mouse : public Pet
{
public:
	void Sound()
	{
		cout << "����" << endl;
	}
	void Eat()
	{
		cout << "ġ��" << endl;
	}

	Pet* CreatePet()
	{
		return new _Mouse;
	}
	const char* GetName()
	{
		return "��";
	}
};

#define MAX_PET_COUNT		60
#define PET_TABLE_COUNT		3

int main������2() {
	// ������: �θ�Ŭ���� �����ͷ� �ڽ��� ����Ű�� �ۼ�
	Pet* p_pet_list[MAX_PET_COUNT];		// �����ؼ� ���� (������)
	int select, type, pet_count = 0;

	// ���̺� ���
	Pet* p_pet_table[PET_TABLE_COUNT] = { new _Dog, new _Cat, new _Mouse };		// 0��, 1��, 2��

	while (1) {
		cout << endl << "[ �޴� ]" << endl;
		cout << "1. �߰�" << endl;
		cout << "2. �Ծ�" << endl;
		cout << "3. ¢��" << endl;
		cout << "4. ����" << endl;

		cout << "��� ���� : ";
		cin >> select;

		if (select == 1) {
			// ���̺� ���
			for (int i = 0; i < PET_TABLE_COUNT; i++) {
				cout << i << ": " << p_pet_table[i]->GetName() << ", ";
			}
			cout << " ? ";

			cin >> type;
			p_pet_list[pet_count] = p_pet_table[type]->CreatePet();
			pet_count++;
			/*
			if (type == 1) {
				p_pet_list[pet_count] = new _Cat;
				pet_count++;
			}
			else if (type == 0) {
				p_pet_list[pet_count] = new _Dog;
				pet_count++;
			}
			*/
		}
		else if (select == 2) {
			for (int i = 0; i < pet_count; i++)
			{
				p_pet_list[i]->Eat();
			}
			// �ߺ� ������
		}
		else if (select == 3) {
			for (int i = 0; i < pet_count; i++)
			{
				p_pet_list[i]->Sound();
			}
			// �ߺ� ������
		}
		else if (select == 4) {
			break;
		}
	}

	for (int i = 0; i < pet_count; i++)
	{
		delete p_pet_list[i];
	}
	// �ߺ� ������

	// ���̺� �����޸� ����
	for (int i = 0; i < PET_TABLE_COUNT; i++)
	{
		delete p_pet_table[i];
	}
	return 0;
}

// ����� �� �ִ� �κ��� �پ��� ��Ʈ���� ����