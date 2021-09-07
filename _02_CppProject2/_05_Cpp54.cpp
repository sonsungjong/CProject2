#include <iostream>

using namespace std;
// 애견샵 동물관리 프로프램 개 -> +고양이 -> +햄스터
// 개도 짖고 고양이도 짖음 -> Pet이라는 클래스를 생성
// 테이블 기법으로 통일되지 않는 코드를 관리

class Pet
{
	// 다형성 : Pet으로 모두 가리킬 수 있음 (통합가능)
public:
	// 오버라이딩 전용으로 만들어 놓음
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
		cout << "멍멍" << endl;
	}
	void Eat() {
		cout << "뼈다귀" << endl;
	}

	virtual Pet* CreatePet() {
		return new Dog;
	}

	virtual const char* GetName() {
		return "개";
	}
};

class Cat : public Pet
{
public:
	void Sound() {
		cout << "야옹" << endl;
	}
	void Eat() {
		cout << "생선" << endl;
	}

	virtual Pet* CreatePet() {
		return new Cat;
	}

	virtual const char* GetName() {
		return "고양이";
	}
};

class Mouse : public Pet
{
public:
	void Sound() {
		cout << "찎찎" << endl;
	}
	void Eat() {
		cout << "치즈" << endl;
	}

	virtual Pet* CreatePet() {
		return new Mouse;
	}

	virtual const char* GetName() {
		return "햄스터";
	}
};

//#define MAX_DOG_COUNT		20
//#define MAX_CAT_COUNT		20
#define MAX_PET_COUNT		40
#define PET_TABLE_COUNT	3

int main54() {
	Pet* p_pet_table[PET_TABLE_COUNT] = { new Dog, new Cat, new Mouse };		// 개한마리, 고양이한마리를 미리 만들어놓음

	Pet* p_pet_list[MAX_PET_COUNT];			// Pet으로 통합하여 관리
	//Dog* p_dog_list[MAX_DOG_COUNT];		// 문제
	//Cat* p_cat_list[MAX_CAT_COUNT];		// 문제
	int select, type;
	int pet_count = 0;

	while (1) {
		cout << endl << "[  메뉴  ]" << endl;
		cout << "1. 추가" << endl;
		cout << "2. 먹어" << endl;
		cout << "3. 짖어" << endl;
		cout << "4. 종료" << endl;

		cout << "기능 선택 : " << endl;
		cin >> select;

		if (select == 1) {
			for (int i = 0; i < PET_TABLE_COUNT; i++)
			{
				cout << i << ": " << p_pet_table[i]->GetName() << ", ";
			}
			cout << " ? ";

			cin >> type;

			p_pet_list[pet_count] = p_pet_table[type]->CreatePet();		// 테이블 기법
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
		delete p_pet_table[i];		// 미리 만들어놓는 new를 해제하는 코드
	}

	return 0;
}

/*
	다형성 : 자식 객체와 부모 포인터
	virtual : 부모 포인터로 자식 메서드를 사용할 때 오버라이딩된 자식 함수를 호출하기 위해 부모 함수 앞에 붙이는 키워드
	루틴을 일원화, 하나의 객체로 모두 처리할 수 있음
*/