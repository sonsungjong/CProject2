#pragma once
class Singleton1
{
public:
	static Singleton1& GetInstance1() {
		static Singleton1* single = new Singleton1;
		return *single;
	}

	void Say();
	void SetValue(int a_value) { m_value = a_value; }
	int GetValue() { return m_value; }

private:
	// �����ڴ� �ܺο��� ȣ������ ���ϰ� private���� �����Ѵ�.
	Singleton1() { m_value = 0; }
	int m_value;

	Singleton1(const Singleton1&) = delete;
	Singleton1& operator=(const Singleton1&) = delete;
	Singleton1(Singleton1&&) = delete;
	Singleton1& operator=(Singleton1&&) = delete;
};
