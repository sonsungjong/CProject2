#include <iostream>

using namespace std;
// �ְ߼� �������� �������� �� -> +����� -> +������
class Dog
{
public:
	void Sound(){
		cout << "�۸�" << endl;
	}
	void Eat(){
		cout << "���ٱ�" << endl;
	}
};

#define MAX_DOG_COUNT		20

int main51() {
	Dog* p_dog_list[MAX_DOG_COUNT];		// �迭������
	int select;
	int dog_count = 0;

	while (1) {
		cout << endl << "[  �޴�  ]" << endl;
		cout << "1. �߰�" << endl;
		cout << "2. �Ծ�" << endl;
		cout << "3. ¢��" << endl;
		cout << "4. ����" << endl;

		cout << "��� ���� : " << endl;
		cin >> select;

		if (select == 1) {
			p_dog_list[dog_count] = new Dog;		// �� �Ѹ��� �߰�
			dog_count++;
		}
		else if (select == 2) {
			for (int i = 0; i < dog_count; i++) {
				p_dog_list[i]->Eat();
			}
		}
		else if (select == 3) {
			for (int i = 0; i < dog_count; i++) {
				p_dog_list[i]->Sound();
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

	return 0;
}

/*
	������ : �ڽ� ��ü�� �θ� ������
	virtual : �θ� �����ͷ� �ڽ� �޼��带 ����� �� �������̵��� �ڽ� �Լ��� ȣ���ϱ� ���� �θ� �Լ� �տ� ���̴� Ű����
	
*/