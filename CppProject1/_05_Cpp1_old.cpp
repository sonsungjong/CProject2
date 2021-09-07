#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

using namespace std;

class Dog
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
};
// ����� �߰��ϸ� �������� ���� 2�� (Ŭ���� ���ٽ�)
// Dog, Cat ���� �����ؾ��ϴ� �ڵ尡 ��
class Cat
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
};

#define MAX_DOG_COUNT		20
#define MAX_CAT_COUNT		20

int mainŬ��������() {
	Dog *p_dog_list[MAX_DOG_COUNT];		// ������ �ٲ� �� ������ �����ͷ�
	Cat *p_cat_list[MAX_CAT_COUNT];		// ������ �ٲ� �� ������ �����ͷ�
	int select, type, dog_count = 0, cat_count = 0;

	while (1) {
		cout << endl << "[ �޴� ]" << endl;
		cout << "1. �߰�" << endl;
		cout << "2. �Ծ�" << endl;
		cout << "3. ¢��" << endl;
		cout << "4. ����" << endl;

		cout << "��� ���� : ";
		cin >> select;

		if (select == 1) {
			cout << "0: ��, 1: ����� ? ";
			cin >> type;
			if (type) {
				p_cat_list[cat_count] = new Cat;
				cat_count++;
			}
			else if (type == 0) {
				p_dog_list[dog_count] = new Dog;
				dog_count++;
			}
		}
		else if (select == 2) {
			for (int i = 0; i < dog_count; i++)
			{
				p_dog_list[i]->Eat();
			}
			for (int i = 0; i < cat_count; i++)
			{
				p_cat_list[i]->Eat();
			}
		}
		else if (select == 3) {
			for (int i = 0; i < dog_count; i++)
			{
				p_dog_list[i]->Sound();
			}
			for (int i = 0; i < cat_count; i++)
			{
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