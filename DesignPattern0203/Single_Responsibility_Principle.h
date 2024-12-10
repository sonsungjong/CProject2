#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
// 단일 책임 원칙 : SRP에 따라 책임을 분리

struct STJournal
{
	std::string title;
	std::vector<std::string> entries;

	STJournal(const std::string& _title) : title(_title) {}

	void add_entry(const std::string& entry)
	{
		static int count = 1;
		entries.push_back(std::to_string(count++) + ": " + entry);
	}

	// 데이터 관리(추가, 수정) 외에 파일 저장(출력)이라는 별개의 책임을 갖고있음 (저장 기능을 수정하려면 해당 구조체를 수정해야함)
	void save(const std::string& filename)
	{
		std::ofstream ofs(filename);
		for (auto& e : entries)
		{
			ofs << e << std::endl;
		}
	}
};

// 단일 책임 원칙 준수 (저장이나 출력 형식을 변경하려면 아래 구조체만 수정하면 됨 / 저장기능을 다른 곳에서 재사용이 가능함)
struct STPersistenceManager
{
	static void save(const STJournal& j, const std::string& filename)
	{
		std::ofstream ofs(filename);
		for (auto& e : j.entries)
		{
			ofs << e << std::endl;
		}
	}
};

void main0203()
{
	STJournal stJournal{ "Dear Diary" };
	stJournal.add_entry("I ate a bug");
	stJournal.add_entry("I cried today");

	//stJournal.save("diary.txt");

	STPersistenceManager pm;
	pm.save(stJournal, "diary.txt");
}