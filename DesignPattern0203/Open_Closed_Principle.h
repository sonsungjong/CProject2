#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

enum class Color{red, green, blue};
enum class Size {small, medium, large};

struct STProduct
{
	std::string name;
	Color color;
	Size size;
};

// 새로운 필터링이 추가될때마다 기존 코드를 수정해야한다
struct STProductFilter
{
	typedef std::vector<STProduct*> vecItems;

	vecItems by_color(vecItems items, Color color)
	{
		vecItems result;
		for (auto& i : items)
		{
			if (i->color == color) {
				result.push_back(i);
			}
		}
		return result;
	}

	vecItems by_size(vecItems items, Size size)
	{
		vecItems result;
		for (auto& i : items)
		{
			if (i->size == size) {
				result.push_back(i);
			}
		}
		return result;
	}

	vecItems by_size_and_color(vecItems items, Size size, Color color)
	{
		vecItems result;
		for (auto& i : items)
		{
			if (i->size == size && i->color == color) {
				result.push_back(i);
			}
		}
		return result;
	}
};

template <typename T> struct AndSpecification;

// 인터페이스 일반화 (기존 코드를 수정하지 않고 새로운 클래스를 추가, OCP 원칙)
template<typename T> struct  Specification
{
	virtual ~Specification() = default;
	// 필터링 조건을 추상화, 실제 조건은 구현부에서 결정
	virtual bool is_satisfied(T* item) const = 0;
};

template<typename T> AndSpecification<T> operator&&(const Specification<T>& first, const Specification<T>& second)
{
	return AndSpecification<T>(const_cast<Specification<T>&>(first), const_cast<Specification<T>&>(second));
}

template <typename T> struct Filter
{
	virtual vector<T*> filter(vector<T*> items, Specification<T>& spec) = 0;
};

struct BetterFilter : Filter<STProduct>
{
	// 원본 대상과 조건필터링 클래스를 받아서 조건클래스의 is_satisfied 메서드를 만족할때만 push
	vector<STProduct*> filter(vector<STProduct*> items, Specification<STProduct>& spec) override
	{
		vector<STProduct*> result;
		for (auto& item : items)
		{
			if (spec.is_satisfied(item))
			{
				result.push_back(item);
			}
		}
		return result;				// 벡터를 리턴
	}
};

// 클래스 생성자를 통해 필터링 처리
struct ColorSpecification : Specification<STProduct>
{
	Color color;
	ColorSpecification(Color color) : color(color) {}

	bool is_satisfied(STProduct* item) const override
	{
		return item->color == color;
	}
};

// 클래스 생성자를 통해 필터링 처리
struct SizeSpecification : Specification<STProduct>
{
	Size size;
	explicit SizeSpecification(const Size size) : size{size} {}

	bool is_satisfied(STProduct* item) const override
	{
		return item->size == size;
	}
};

// 조건 클래스 2개를 생성자에서 받음
template <typename T> struct AndSpecification : Specification<T>
{
	Specification<T>& first;
	Specification<T>& second;

	AndSpecification(Specification<T>& first, Specification<T>& second)
		: first(first), second(second)
	{}

	bool is_satisfied(T* item) const override
	{
		return first.is_satisfied(item) && second.is_satisfied(item);
	}
};

void main0204()
{
	STProduct apple{ "Apple", Color::red, Size::small };
	STProduct tree{ "Tree", Color::green, Size::large };
	STProduct house{ "House", Color::blue, Size::large };

	vector<STProduct*> items{ &apple, &tree, &house };

	//STProductFilter pf;
	//auto green_things = pf.by_color(items, Color::green);
	//for (auto& item : green_things)
	//{
	//	cout << item->name << " is green\n";
	//}
	BetterFilter bf;
	ColorSpecification green(Color::green);
	auto green_things = bf.filter(items, green);

	for (auto& item : green_things)
	{
		cout << item->name << " is green\n";
	}
	SizeSpecification large(Size::large);
	AndSpecification<STProduct> green_and_large(green, large);

	auto spec = green && large;
	for (auto& item : bf.filter(items, spec)) {
		cout << item->name << " is green and large\n";
	}
}