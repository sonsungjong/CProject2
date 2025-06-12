#include <iostream>

class Rectangle
{
protected:
	int m_width;
	int m_height;

public:
	Rectangle(const int width, const int height)
		: m_width{width}, m_height{height}
	{ }

	int getWidth() const { return m_width; }
	virtual void setWidth(const int width) { this->m_width = width; }
	int getHeight() const { return m_height; }
	virtual void setHeight(const int height) { m_height = height; }

	int calculateArea() const { return m_width * m_height; }
};

class Square : public Rectangle
{
public:
	Square(int size) : Rectangle(size, size) {}
	void setWidth(const int width) override
	{
		this->m_width = m_height = width;
	}
	void setHeight(const int height) override {
		this->m_height = m_width = height;
	}
};

struct RectangleFactory
{
	//static Rectangle createRectangle(int w, int h);
	//static Rectangle createSquare(int size);
};

void process(Rectangle& r)
{
	int w = r.getWidth();
	r.setHeight(10);

	std::cout << "expected area = " << (w * 10)
		<< ", got " << r.calculateArea() << std::endl;
}

int main()
{
	Rectangle r{ 5,5 };
	process(r);

	Square s{ 5 };
	process(s);

	(void)getchar();
	return 0;
}