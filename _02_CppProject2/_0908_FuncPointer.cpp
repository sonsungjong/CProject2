// �Լ��� �޸𸮰����� �ּҰ� �ִ�

#include <iostream>

using namespace std;

int sum(int x, int y)
{
	return x + y;
}

enum struct RequestType
{
	Login,								// 0
	Register,							// 1
	Update,								// 2
	Delete								// 3
};

bool onLogin(string id, string password)
{
	cout << "onLogin" << endl;
	cout << id << endl;
	cout << password << endl;
	return true;
}

bool onRegister(string id, string password)
{
	cout << "onRegister" << endl;
	cout << id << endl;
	cout << password << endl;
	return true;
}

bool onUpdate(string id, string password)
{
	cout << "onUpdate" << endl;
	cout << id << endl;
	cout << password << endl;
	return true;
}

bool onDelete(string id, string password)
{
	cout << "onDelete" << endl;
	cout << id << endl;
	cout << password << endl;
	return true;
}

struct Character
{
	int health;
	void (*dieCallback)();
};

void damaged(Character* character)
{
	character->health -= 100;
	if (character->health <= 0) {
		cout << "died" << endl;
		if(character->dieCallback)
			character->dieCallback();
	}
}

void gameOver() {
	cout << "gameOver" << endl;
}

int main() {
	// f �Լ�������
	int (*f)(int, int) = &sum;
	cout << (*f)(1, 2) << endl;

	// d �Լ����۷���
	int (&d)(int, int) = sum;
	cout << d(3, 3) << endl;

	// �Լ������� ������� (�Ͻ������� �ּҰ��� �������༭ �������� ����)
	cout << (*sum)(4, 4) << endl;

	// �Լ������Ϳ� ���� �迭
	bool (*callbacks[])(string, string) {
		onLogin, onRegister, onUpdate, onDelete
	};

	callbacks[(int)RequestType::Login]("david", "1234");
	callbacks[(int)RequestType::Register]("david", "1234");
	callbacks[(int)RequestType::Update]("david", "1234");
	callbacks[(int)RequestType::Delete]("david", "1234");

	Character char0{ 200, nullptr };
	Character char1{ 200, gameOver };				// �Լ������� ���

	damaged(&char0);
	damaged(&char0);
	damaged(&char1);
	damaged(&char1);

	return 0;
}