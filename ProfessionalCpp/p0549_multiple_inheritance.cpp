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
	���߻���� �ͽ��� Ŭ������ ���� �� ����Ѵ�.
	������Ʈ(��ǰ)�� ���߻�Ӻ��� ��������� �����Ѵ�.
	���̾Ƹ�� ��Ӱ����϶��� �ֻ�� Ŭ������ ��� ���� ���� �޼���� �����Ѵ�.
	��ȣ���ذ��� ���ؼ� dynamic_cast�� �ϰų� �������� �������ش�.
*/
class DogBird549 : public Dog549, public Bird549
{
	// ���߻�� �� ��ȣ�� �ذ��� ���� �������̵� (�Ǵ� �ֻ�� Ŭ������ ���� ���� �޼����)
public:
	using Dog549::eat;
};

int main()
{
	DogBird549 confusedAnimal;
	confusedAnimal.bark();
	confusedAnimal.chirp();

	dynamic_cast<Dog549&>(confusedAnimal).eat();				// ��ȣ�� �ذ�
	confusedAnimal.Dog549::eat();										// ��ȣ�� �ذ�
	confusedAnimal.eat();
}