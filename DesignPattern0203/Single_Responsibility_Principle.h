#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
// ���� å�� ��Ģ : SRP�� ���� å���� �и�

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

	// ������ ����(�߰�, ����) �ܿ� ���� ����(���)�̶�� ������ å���� �������� (���� ����� �����Ϸ��� �ش� ����ü�� �����ؾ���)
	void save(const std::string& filename)
	{
		std::ofstream ofs(filename);
		for (auto& e : entries)
		{
			ofs << e << std::endl;
		}
	}
};

// ���� å�� ��Ģ �ؼ� (�����̳� ��� ������ �����Ϸ��� �Ʒ� ����ü�� �����ϸ� �� / �������� �ٸ� ������ ������ ������)
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