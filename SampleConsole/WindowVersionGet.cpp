#include <Windows.h>
#include <iostream>

// 레지스트리의 DisplayVersion 키를 확인해서 윈도우 버전 판별
std::string GetWindowsDisplayVersionFromRegistry()
{
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;
	char version[256] = { 0 };

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		RegQueryValueExA(hKey, "DisplayVersion", NULL, &dwType, (LPBYTE)&version, &dwSize);
		RegCloseKey(hKey);
	}

	return std::string(version);
}

void main_windowversionget()
{
	std::string win_version = "";
	std::string win_architecture = "";

	std::string displayVersion = GetWindowsDisplayVersionFromRegistry();
	if (displayVersion == "10.0")
	{
		win_version = "Windows 10";
		std::cout << win_version << std::endl;
	}
	else if (displayVersion == "6.3")
	{
		win_version = "Windows 8.1";
		std::cout << win_version << std::endl;
	}
	else if (displayVersion == "6.2")
	{
		win_version = "Windows 8";
		std::cout << win_version << std::endl;
	}
	else if (displayVersion == "6.1")
	{
		win_version = "Windows 7";
		std::cout << win_version << std::endl;
	}
	else if (displayVersion == "6.0")
	{
		win_version = "Windows Vista";
		std::cout << win_version << std::endl;
	}
	else if (displayVersion == "5.2")
	{
		win_version = "Windows XP 64-Bit Edition";
		std::cout << win_version << std::endl;
	}
	else if (displayVersion == "5.1")
	{
		win_version = "Windows XP";
		std::cout << win_version << std::endl;
	}
	else if (displayVersion >= "10.1")
	{
		win_version = "Windows 11";
		std::cout << win_version << std::endl;
	}
	else
	{
		std::cout << "Older or unrecognized version of Windows" << std::endl;
	}

	// 시스템 아키텍처 알아내기
	SYSTEM_INFO sys_info;
	GetNativeSystemInfo(&sys_info);

	switch (sys_info.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		win_architecture = "x64";
		std::cout << win_architecture << std::endl;			// AMD or Intel
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		win_architecture = "ARM";
		std::cout << win_architecture << std::endl;
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		win_architecture = "Intel Itanium-based";
		std::cout << win_architecture << std::endl;
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		win_architecture = "x86";
		std::cout << win_architecture << std::endl;
		break;
	default:
		win_architecture = "Unknown system architecture.";
		std::cout << win_architecture << std::endl;
		break;
	}
}