// 함수도 메모리공간과 주소가 있다

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
	// f 함수포인터
	int (*f)(int, int) = &sum;
	cout << (*f)(1, 2) << endl;

	// d 함수레퍼런스
	int (&d)(int, int) = sum;
	cout << d(3, 3) << endl;

	// 함수포인터 직접사용 (암시적으로 주소값을 리턴해줘서 역참조가 가능)
	cout << (*sum)(4, 4) << endl;

	// 함수포인터에 대한 배열
	bool (*callbacks[])(string, string) {
		onLogin, onRegister, onUpdate, onDelete
	};

	callbacks[(int)RequestType::Login]("david", "1234");
	callbacks[(int)RequestType::Register]("david", "1234");
	callbacks[(int)RequestType::Update]("david", "1234");
	callbacks[(int)RequestType::Delete]("david", "1234");

	Character char0{ 200, nullptr };
	Character char1{ 200, gameOver };				// 함수포인터 사용

	damaged(&char0);
	damaged(&char0);
	damaged(&char1);
	damaged(&char1);

	return 0;
}