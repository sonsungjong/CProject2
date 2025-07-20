
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <filesystem>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>
#include <ctime>
#include <mutex>
#include <cstdio>
#include "LogManager.h"

// ������۸���� �α׸��
// �α׸� �װ� ����Ѵ�
// ó���� ���� ��ε��� �߰��ϰ� ����ϵ��� (���� ������ �����������)
// �ش� ��θ� �������� string�� �����ϰ� ������ �α� ���� ���ϴ� ť�� �ִ´�
// �Ϲ������δ� ��� �����ϰ�, ť�� 10���̻� ���� ��� ���۸� ��ü�ؼ� string�� �̾���̰� �Ѵ��� �ѹ��� �����ϰ� �Ѵ�
// [�ð�]���� // ��� + �α׷���

// ���� ��� ������ ������۱�� �߰� �ʿ�

CLogManager::CLogManager()
	: m_logLevel(1U)
	, m_exitFlag(false)
{
	setLogFilePath("./log/log.log");

	std::thread th(&CLogManager::processLogQueue, this);
	th.detach();
}

CLogManager::~CLogManager()
{
	m_exitFlag = true;				// �����ȣ
	if (m_logFile.is_open()) {
		m_logFile.close();
	}
}

void CLogManager::setLogFilePath(const std::string& strLogFileName)
{
	std::lock_guard<std::mutex> lock(m_mtxFile);
	m_strLogFileName = strLogFileName;				// ��θ� ��������� ����
}

void CLogManager::createLogFolder(const std::string& strLogFileName)
{
	if (m_strLogFileName != "")
	{
		// ���� ��ο��� ���͸� �κ� ����
		std::filesystem::path filePath(m_strLogFileName);
		std::filesystem::path directory = filePath.parent_path();

		// ���͸��� �������� ������ ����
		if (!directory.empty() && !std::filesystem::exists(directory)) {
			try {
				std::filesystem::create_directories(directory);
			}
			catch (const std::exception& e) {
				std::cerr << "���� ���� ����: " << e.what() << std::endl;
				return;
			}
		}
	}
}

void CLogManager::processLogQueue()
{
	while (m_exitFlag.load() == false) {
		std::string strLogMsg = m_queLog.wait_pop();
		if (m_exitFlag.load() == true) {
			break;
		}
		else {
			// ���� ����
			{
				std::lock_guard<std::mutex> lock(m_mtxFile);				// m_strLogFileName ���
				createLogFolder(m_strLogFileName);
				m_logFile.open(m_strLogFileName, std::ios::app);			// ���� ����
			}

			if (m_logFile.is_open()) {
				strLogMsg.insert(0, "[LogQue:" + std::to_string(m_queLog.size()) + "]");			// �α�ť ������ üũ��
				strLogMsg.insert(0, "[" + getDateTimeStampMS() + "]");			// �տ� �ð� �ٿ���
				m_logFile << strLogMsg << std::endl;							// ���Ͽ� ���
				m_logFile.close();  // ���� �ݱ�
			}

		}
	}
}

std::string CLogManager::getDateTimeStampMS()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::tm local_tm;
	// gmtime_s -> UTC �ð�
	// localtime_s -> ���� �ð�
#ifdef _WIN32
	localtime_s(&local_tm, &in_time_t);
#else
	localtime_r(&in_time_t, &local_tm);
#endif

	auto duration = now.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

	char buffer[100]{};
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);

	char result[128]{};
	snprintf(result, sizeof(result), "%s.%03d", buffer, static_cast<int>(millis.count()));

	return std::string(result);
}

std::string CLogManager::getDateTimeStamp()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::tm local_tm;
	// gmtime_s -> UTC �ð�
	// localtime_s -> ���� �ð�
#ifdef _WIN32
	localtime_s(&local_tm, &in_time_t);
#else
	localtime_r(&in_time_t, &local_tm);
#endif

	char buffer[128]{};
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);

	return std::string(buffer);
}

std::string CLogManager::getDateStamp()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::tm local_tm;
	// gmtime_s -> UTC �ð�
	// localtime_s -> ���� �ð�
#ifdef _WIN32
	localtime_s(&local_tm, &in_time_t);
#else
	localtime_r(&in_time_t, &local_tm);
#endif

	char buffer[128]{};
	strftime(buffer, sizeof(buffer), "%Y-%m-%d", &local_tm);

	return std::string(buffer);
}

void CLogManager::setLogLevel(const unsigned int level)
{
	m_logLevel = level;
}

void CLogManager::log(const std::string& _logMsg, const unsigned int _logLevel)
{
	if (_logLevel <= m_logLevel && _logLevel > 0) {
		m_queLog.push(_logMsg);
	}
}

