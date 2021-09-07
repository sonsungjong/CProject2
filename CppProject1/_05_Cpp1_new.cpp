#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

using namespace std;
// 다형성과 테이블기법을 이용한 코드
class Pet
{
// 부모는 Dog도 가리킬 수 있고 Cat도 가리킬 수 있음
// 비워놓고 virtual로 오버라이딩 전용
public:
	virtual void Sound()
	{

	}
	virtual void Eat()
	{

	}
	// 테이블 기법
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
		cout << "멍멍" << endl;
	}
	void Eat()
	{
		cout << "뼈다귀" << endl;
	}

	Pet* CreatePet()
	{
		return new _Dog;
	}
	const char* GetName()
	{
		return "개";
	}
};
// 다형성을 이용
class _Cat : public Pet
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

	Pet* CreatePet()
	{
		return new _Cat;
	}
	const char* GetName()
	{
		return "고양이";
	}
};

// 다형성을 이용해서 변화에 의해 반영되는 코드를 최소화
class _Mouse : public Pet
{
public:
	void Sound()
	{
		cout << "찎찎" << endl;
	}
	void Eat()
	{
		cout << "치즈" << endl;
	}

	Pet* CreatePet()
	{
		return new _Mouse;
	}
	const char* GetName()
	{
		return "쥐";
	}
};

#define MAX_PET_COUNT		60
#define PET_TABLE_COUNT		3

int main다형성2() {
	// 다형성: 부모클래스 포인터로 자식을 가리키게 작성
	Pet* p_pet_list[MAX_PET_COUNT];		// 통합해서 관리 (다형성)
	int select, type, pet_count = 0;

	// 테이블 기법
	Pet* p_pet_table[PET_TABLE_COUNT] = { new _Dog, new _Cat, new _Mouse };		// 0번, 1번, 2번

	while (1) {
		cout << endl << "[ 메뉴 ]" << endl;
		cout << "1. 추가" << endl;
		cout << "2. 먹어" << endl;
		cout << "3. 짖어" << endl;
		cout << "4. 종료" << endl;

		cout << "기능 선택 : ";
		cin >> select;

		if (select == 1) {
			// 테이블 기법
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
			// 중복 없어짐
		}
		else if (select == 3) {
			for (int i = 0; i < pet_count; i++)
			{
				p_pet_list[i]->Sound();
			}
			// 중복 없어짐
		}
		else if (select == 4) {
			break;
		}
	}

	for (int i = 0; i < pet_count; i++)
	{
		delete p_pet_list[i];
	}
	// 중복 없어짐

	// 테이블 동적메모리 해제
	for (int i = 0; i < PET_TABLE_COUNT; i++)
	{
		delete p_pet_table[i];
	}
	return 0;
}

// 변경될 수 있는 부분이 줄어들어 스트레스 감소