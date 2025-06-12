#include <iostream>
#include <string>
#include <vector>

// Open Closed Principle : 확장에는 열려있고 변경에는 닫혀있어야 한다

enum class Color { red, green, blue };
enum class Size { small, medium, large };

struct Product
{
	std::string name;
	Color color;
	Size size;
};

// 나쁜 예시 (함수 계속 만들어야함)
class ProductFilter
{
	typedef std::vector<Product*> Items;
public:
	Items by_color(Items items, Color color)
	{
		Items result;
		for (auto& i : items)
		{
			if (i->color == color) 
			{
				result.push_back(i);
			}
		}
		return result;
	}

	Items by_size(Items items, const Size size)
	{
		Items result;
		for (auto& i : items)
		{
			if (i->size == size)
			{
				result.push_back(i);
			}
		}

		return result;
	}

	Items by_size_and_color(Items items, const Size size, const Color color)
	{
		Items result;
		for (auto& i : items)
		{
			if (i->size == size && i->color == color)
			{
				result.push_back(i);
			}
		}
		
		return result;
	}
};

template <typename T> struct AndSpecification;

// 조건을 객체 Specification로 추상화한다
template <typename T> struct Specification
{
	virtual bool is_satisfied(T* item) const = 0;

	virtual ~Specification() = default;
};

template <typename T> AndSpecification<T> operator&&(const Specification<T>& first, const Specification<T>& second)
{
	return { first, second };
}

template <typename T> struct Filter
{
	virtual std::vector<T*> filter(std::vector<T*> items, Specification<T>& spec) = 0;
};

struct BetterFilter : Filter<Product>
{
	std::vector<Product*> filter(std::vector<Product*> items, Specification<Product>& spec) override
	{
		std::vector<Product*> result;
		for (auto& p : items)
		{
			if (spec.is_satisfied(p))
			{
				result.push_back(p);
			}
		}

		return result;
	}
};

struct ColorSpecification : public Specification<Product>
{
	Color color;

	ColorSpecification(Color color) : color(color) {}
	bool is_satisfied(Product* item) const override
	{
		return item->color == color;
	}
};

struct SizeSpecification : public Specification<Product>
{
	Size size;

	explicit SizeSpecification(const Size size) : size{size}
	{

	}

	bool is_satisfied(Product* item) const override
	{
		return item->size == size;
	}
};

// NameSpecification을 추가하고 기존 필터 BetterFilter는 변경 없이 그대로 사용가능
class NameSpecification : public Specification<Product>
{
public:
	std::string name;
	NameSpecification(const std::string& _name) : name(_name)
	{
	}

	bool is_satisfied(Product* item) const override
	{
		return item->name == name;
	}
};

template<typename T> struct AndSpecification : public Specification<T>
{
	const Specification<T>& first;
	const Specification<T>& second;

	AndSpecification(const Specification<T>& first, const Specification<T>& second)
		: first(first), second(second)
	{
	}

	bool is_satisfied(T* item) const override
	{
		return first.is_satisfied(item) && second.is_satisfied(item);
	}
};

int main()
{
	Product apple{ "Apple", Color::green, Size::small };
	Product tree{ "Tree", Color::green, Size::large };
	Product house{ "House", Color::blue, Size::large };

	std::vector<Product*> all{ &apple, &tree, &house };

	//std::vector<Product*> items{ &apple, &tree, &house };
	//ProductFilter pf;
	//auto green_things = pf.by_color(items, Color::green);
	//for (auto& item : green_things)
	//{
	//	printf("%s is green\n", item->name.c_str());
	//}

	BetterFilter bf;
	ColorSpecification green(Color::green);
	auto green_things = bf.filter(all, green);
	for (auto& x : green_things)
	{
		printf("%s is green\n", x->name.c_str());
	}

	SizeSpecification large(Size::large);
	AndSpecification<Product> green_and_large(green, large);

	AndSpecification<Product> spec = green && large;
	for (auto& x : bf.filter(all, spec))
	{
		printf("%s is green and large\n", x->name.c_str());
	}

	// 이름찾기용 NameSpecification 추가
	NameSpecification name("House");
	for (auto& x : bf.filter(all, name))
	{
		printf("%s is house\n", x->name.c_str());
	}

	(void)getchar();
	return 0;
}