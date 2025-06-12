#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <charconv>
//#include <boost/lexical_cast.hpp>

// Single Responsebility Principle (SRP)

// 여행 기록 클래스 (일기 데이터 관리)
class Journal
{
public:
    std::string m_title;
    std::vector<std::string> m_entries;

    explicit Journal(const std::string& title)
        : m_title{ title }
    {
    }

    void add(const std::string& entry)
    {
        static int count = 1;

        m_entries.push_back(std::to_string(count) + ": " + entry);
        count++;
    }

    // void save : 파일에 저장하는 기능은 Single Responsebility Principle 로 분리
};


// 여행 기록에 대한 내용을 파일에 저장하기 위한 save
class PersistenceManager
{
public:
    void save(const Journal& j, const std::string& filename)
    {
        std::ofstream ofs(filename);
        for (auto& s : j.m_entries)
        {
            ofs << s << std::endl;
        }
    }
};

int main3()
{
    Journal journal{ "Dear Diary" };
    journal.add("I ate a bug");
    journal.add("I cried today");

    // 파일 저장을 위한 것은 Journal 클래스 안에 같이 두지 않고 유지보수성을 위해 분리한다 (일기데이터 관리 / 파일 저장 기능)
    PersistenceManager pm;
    pm.save(journal, "diary.txt");
}

