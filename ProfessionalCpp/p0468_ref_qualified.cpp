import "HeaderUnits.h";

class TextHolder
{
public:
	TextHolder(std::string text) : m_text{ std::move(text) } 
	{}

	const std::string& getText() const
	{
		return m_text;
	}

	//std::string&& getText() &&
	//{
	//	return std::move(m_text);
	//}

private:
	std::string m_text;
};

int main0468()
{
	TextHolder textHolder{ "Hello World!" };
	printf("%s\n", textHolder.getText().c_str());
	printf("%s\n", TextHolder{"Hello World!"}.getText().c_str());
	printf("%s\n", std::move(textHolder).getText().c_str());


	return 0;
}