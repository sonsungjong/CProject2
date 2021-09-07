#include <iostream>

using namespace std;
// 애견샵 동물관리 프로프램 개 -> +고양이 -> +햄스터
// 자료형 추가로 인한 일 2배, 코드 2배
class Dog
{
public:
	void Sound() {
		cout << "멍멍" << endl;
	}
	void Eat() {
		cout << "뼈다귀" << endl;
	}
};

class Cat
{
public:
	void Sound() {
		cout << "야옹" << endl;
	}
	void Eat() {
		cout << "생선" << endl;
	}
};

#define MAX_DOG_COUNT		20
#define MAX_CAT_COUNT		20

int main52() {
	Dog* p_dog_list[MAX_DOG_COUNT];		// 문제
	Cat* p_cat_list[MAX_CAT_COUNT];		// 문제
	int select, type;
	int dog_count = 0, cat_count = 0;

	while (1) {
		cout << endl << "[  메뉴  ]" << endl;
		cout << "1. 추가" << endl;
		cout << "2. 먹어" << endl;
		cout << "3. 짖어" << endl;
		cout << "4. 종료" << endl;

		cout << "기능 선택 : " << endl;
		cin >> select;

		if (select == 1) {
			cout << "0: 개, 1: 고양이 ? ";
			cin >> type;
			if (type == 0) {
				p_dog_list[dog_count] = new Dog;		// 개 한마리 추가
				dog_count++;
			}
			else if (type == 1) {
				p_cat_list[cat_count] = new Cat;		// 고양이 한마리 추가
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
	다형성 : 자식 객체와 부모 포인터
	virtual : 부모 포인터로 자식 메서드를 사용할 때 오버라이딩된 자식 함수를 호출하기 위해 부모 함수 앞에 붙이는 키워드

*/