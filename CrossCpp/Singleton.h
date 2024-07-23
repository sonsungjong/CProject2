#ifndef SINGLETON_H_
#define SINGLETON_H_

class CSingleton
{
	CSingleton();
	~CSingleton();
	static CSingleton* getInstance()
	{
		static CSingleton instance;
		return &instance;
	}

	friend CSingleton* Single();

public:
	char m_bigSize[10 * 1000 * 1000];

};

inline CSingleton* Single()
{
	return CSingleton::getInstance();
}

#endif			SINGLETON_H_