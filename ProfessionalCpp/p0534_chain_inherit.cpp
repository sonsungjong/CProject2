import <iostream>;
import <string>;

class Book534
{
public:
	virtual ~Book534() = default;
	virtual std::string getDescription() const
	{
		return "Book";
	}

	virtual int getHeight() const
	{
		return 120;
	}
};

class Paperback534 : public Book534
{
public:
	std::string getDescription() const override
	{
		return "Paperback " + Book534::getDescription();				// �θ��� �޼��带 ȣ��
	}
};

class Romance534 : public Paperback534
{
public:
	std::string getDescription() const override
	{
		return "Romance " + Paperback534::getDescription();					// �θ� �޼��� ȣ��
	}
	int getHeight() const override
	{
		return Paperback534::getHeight() / 2;								// ü������ Book�� ���� ȣ��
	}
};

class Technical534 : public Book534
{
public:
	std::string getDescription() const override
	{
		return "Technical " + Book534::getDescription();
	}
};

int main534()
{
	Romance534 novel;
	Book534 book;

	printf("%s\n",novel.getDescription().c_str());								// Romance Paperback Book
	printf("%s\n",book.getDescription().c_str());								// Book
	printf("%d\n",novel.getHeight());												// 60
	printf("%d\n",book.getHeight());													// 120

	return 0;
}
