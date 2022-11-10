#include <iostream>
#include <string>
using namespace std;

// 디폴트값을 갖고있는 클래스 틀
template<typename T=int, int capT=100>
class Array2
{
	T* buf;
	int size;
	int capacity;

public:
	explicit Array2(int cap = capT) : buf(0), size(0), capacity(cap)
	{
		buf = new T[capacity];
	}
	~Array2() { delete[] buf; }

	void Add(T data)
	{
		buf[size++] = data;
	}
	T operator[](int idx) const
	{
		return buf[idx];
	}

	int GetSize() const
	{
		return size;
	}
};

template<typename T>
class ObjectInfo
{
	T data;
public:
	ObjectInfo(const T& d) : data(d) {}

	void Print()
	{
		cout << "타입 : " << typeid(data).name() << endl;
		cout << "크기 : " << sizeof(data) << endl;
		cout << "값 : " << data << endl;
		cout << endl;
	}
};

// string에 대한 특수화 클래스템플릿
template<>
class ObjectInfo<string>
{
	string data;
public:
	ObjectInfo(const string& d) : data(d) {}

	void Print()
	{
		// cout << "타입 : " << typeid(data).name() << endl;
		cout << "타입 : " << "string" << endl;
		// cout << "크기 : " << sizeof(data) << endl;
		cout << "문자열 길이 : " << data.size() << endl;
		cout << "값 : " << data << endl;
		cout << endl;
	}
};

int main109()
{
	Array2<> iarr;			// 디폴트값 int와 100 사용
	iarr.Add(10);
	iarr.Add(20);
	iarr.Add(30);
	for (int i = 0; i < iarr.GetSize(); ++i)
	{
		cout << iarr[i] << " ";
	}
	cout << endl;

	Array2<double> darr;				// 디폴트값 100 사용
	darr.Add(10.12);
	darr.Add(20.12);
	darr.Add(30.12);

	for (int i = 0; i < darr.GetSize(); ++i)
	{
		cout << darr[i] << " ";
	}
	cout << endl;

	Array2<string, 10> sarr;
	sarr.Add("abc");
	sarr.Add("ABC");
	sarr.Add("Hello!");

	for (int i = 0; i < sarr.GetSize(); ++i)
	{
		cout << sarr[i] << " ";
	}
	cout << endl;

	ObjectInfo<int> d1(10);
	d1.Print();

	ObjectInfo<double> d2(5.5);
	d2.Print();

	ObjectInfo<string> d3("Hello!");
	d3.Print();

	return 0;
}