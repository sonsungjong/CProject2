#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <SafeQueue.h>

class CLogManager
{
public:
	static CLogManager& getInstance() {
		static CLogManager instance;
		return instance;
	}

	virtual ~CLogManager();
	CLogManager(const CLogManager&) = delete;
	CLogManager& operator=(const CLogManager&) = delete;
	CLogManager(CLogManager&&) = delete;
	CLogManager& operator=(CLogManager&&) = delete;

	void setLogLevel(const unsigned int level);
	void log(const std::string& _logMsg, const unsigned int _logLevel = 1);
	void setLogFilePath(const std::string& strLogFileName);
	std::string getDateTimeStampMS();
	std::string getDateTimeStamp();
	std::string getDateStamp();

private:
	CLogManager();
	std::string m_strLogFileName;
	std::ofstream m_logFile;
	unsigned int m_logLevel;
	CSafeQueue<std::string> m_queLog;											// �α׸� �޾� �ִ´�
	std::queue<std::string> m_queBufferQueue;								// �α׸� ���� �����Ѵ� -> ����� 100�� �Ѿ�� �ѹ��� �����Ѵ�
	std::atomic_bool m_exitFlag;
	std::mutex m_mtxFile;

	void processLogQueue();				// ť �޽����� ó���ϴ� �Լ�
	void createLogFolder(const std::string& strLogFileName);
};

#endif		LOG_MANAGER_H_