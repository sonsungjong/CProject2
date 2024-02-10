import <iostream>;
import <string>;

class Animal549
{
public:
	virtual void eat() = 0;
};

class Dog549 : Animal549
{
public:
	virtual void bark()
	{
		printf("Woof!\n");
	}
	void eat() override
	{
		printf("The dog ate.\n");
	}
};

class Bird549 : Animal549
{
public:
	virtual void chirp()
	{
		printf("Chirp!\n");
	}

	void eat() override
	{
		printf("The bird ate.\n");
	}
};

/*
	다중상속은 믹스인 클래스를 만들 때 사용한다.
	컴포넌트(부품)은 다중상속보단 멤버변수로 선언한다.
	다이아몬드 상속관계일때는 최상단 클래스는 모두 순수 가상 메서드로 구현한다.
	모호성해결을 위해선 dynamic_cast를 하거나 스코프를 지정해준다.
*/
class DogBird549 : public Dog549, public Bird549
{
	// 다중상속 시 모호성 해결을 위해 오버라이딩 (또는 최상단 클래스가 순수 가상 메서드로)
public:
	using Dog549::eat;
};

int main()
{
	DogBird549 confusedAnimal;
	confusedAnimal.bark();
	confusedAnimal.chirp();

	dynamic_cast<Dog549&>(confusedAnimal).eat();				// 모호성 해결
	confusedAnimal.Dog549::eat();										// 모호성 해결
	confusedAnimal.eat();
}