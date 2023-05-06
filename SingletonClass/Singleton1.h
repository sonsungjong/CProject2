#pragma once
class Singleton1
{
public:
	static Singleton1& getInstance() {
		static Singleton1 instance;
		return instance;
	}

	void Say();
	void SetValue(int a_value) { m_value = a_value; }
	int GetValue() { return m_value; }

private:
	// 생성자는 외부에서 호출하지 못하게 private으로 지정한다.
	Singleton1() {}
	~Singleton1() {}
	int m_value;

	Singleton1(const Singleton1&) = delete;
	Singleton1& operator=(const Singleton1&) = delete;
	Singleton1(Singleton1&&) = delete;
	Singleton1& operator=(Singleton1&&) = delete;
};

