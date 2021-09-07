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
		cout << "멍멍" << endl;
	}
	void Eat()
	{
		cout << "뼈다귀" << endl;
	}
};
// 고양이 추가하면 유지보수 업무 2배 (클래스 복붙시)
// Dog, Cat 따로 관리해야하는 코드가 됨
class Cat
{
public:
	void Sound()
	{
		cout << "야옹" << endl;
	}
	void Eat()
	{
		cout << "생선" << endl;
	}
};

#define MAX_DOG_COUNT		20
#define MAX_CAT_COUNT		20

int main클래스복붙() {
	Dog *p_dog_list[MAX_DOG_COUNT];		// 갯수가 바뀔 수 있으니 포인터로
	Cat *p_cat_list[MAX_CAT_COUNT];		// 갯수가 바뀔 수 있으니 포인터로
	int select, type, dog_count = 0, cat_count = 0;

	while (1) {
		cout << endl << "[ 메뉴 ]" << endl;
		cout << "1. 추가" << endl;
		cout << "2. 먹어" << endl;
		cout << "3. 짖어" << endl;
		cout << "4. 종료" << endl;

		cout << "기능 선택 : ";
		cin >> select;

		if (select == 1) {
			cout << "0: 개, 1: 고양이 ? ";
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