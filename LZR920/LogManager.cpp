
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

// 더블버퍼링기반 로그모듈
// 로그를 쌓고 기록한다
// 처음에 폴더 경로들을 추가하고 사용하도록 (폴더 없으면 생성해줘야함)
// 해당 경로를 기준으로 string을 생성하고 설정한 로그 래벨 이하는 큐에 넣는다
// 일반적으로는 즉시 저장하고, 큐가 10개이상 쌓일 경우 버퍼를 교체해서 string에 이어붙이게 한다음 한번에 저장하게 한다
// [시간]내용 // 경로 + 로그래벨

// 동적 경로 설정과 더블버퍼기반 추가 필요

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
	m_exitFlag = true;				// 종료신호
	if (m_logFile.is_open()) {
		m_logFile.close();
	}
}

void CLogManager::setLogFilePath(const std::string& strLogFileName)
{
	std::lock_guard<std::mutex> lock(m_mtxFile);
	m_strLogFileName = strLogFileName;				// 경로를 멤버변수로 저장
}

void CLogManager::createLogFolder(const std::string& strLogFileName)
{
	if (m_strLogFileName != "")
	{
		// 파일 경로에서 디렉터리 부분 추출
		std::filesystem::path filePath(m_strLogFileName);
		std::filesystem::path directory = filePath.parent_path();

		// 디렉터리가 존재하지 않으면 생성
		if (!directory.empty() && !std::filesystem::exists(directory)) {
			try {
				std::filesystem::create_directories(directory);
			}
			catch (const std::exception& e) {
				std::cerr << "폴더 생성 실패: " << e.what() << std::endl;
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
			// 파일 열기
			{
				std::lock_guard<std::mutex> lock(m_mtxFile);				// m_strLogFileName 잠금
				createLogFolder(m_strLogFileName);
				m_logFile.open(m_strLogFileName, std::ios::app);			// 파일 열기
			}

			if (m_logFile.is_open()) {
				strLogMsg.insert(0, "[LogQue:" + std::to_string(m_queLog.size()) + "]");			// 로그큐 사이즈 체크용
				strLogMsg.insert(0, "[" + getDateTimeStampMS() + "]");			// 앞에 시간 붙여서
				m_logFile << strLogMsg << std::endl;							// 파일에 기록
				m_logFile.close();  // 파일 닫기
			}

		}
	}
}

std::string CLogManager::getDateTimeStampMS()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::tm local_tm;
	// gmtime_s -> UTC 시간
	// localtime_s -> 로컬 시간
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
	// gmtime_s -> UTC 시간
	// localtime_s -> 로컬 시간
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
	// gmtime_s -> UTC 시간
	// localtime_s -> 로컬 시간
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

