#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <cstdio>

void WindowsOff1()
{
    // 20���� �������� ����
    system("shutdown -s -f -t 20");
}

void WindowsOff2(int a_second)
{
    char sysoff[128];
    sprintf_s(sysoff, 128, "C:\\Windows\\System32\\shutdown.exe -s -f -t %d", a_second);

    // a_second���� �������� ����
    system(sysoff);
}

void WindowsRestart(int a_second)
{
    char sys_restart[128];
    sprintf_s(sys_restart, 128, "C:\\Windows\\System32\\shutdown.exe -r -f -t %d", a_second);

    // 20���� �������� �����
    system(sys_restart);
}

void WindowsOffCancel()
{
    system("C:\\Windows\\System32\\shutdown.exe -a");           // ���� �� ����� ����޽��� ���
}

BOOL ProcessKill1(const TCHAR* a_process_name)
{
    HANDLE sj_handle_first = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    DWORD_PTR dwsma = GetLastError();
    HANDLE sj_handle_second;
    DWORD dwExitCode = 0;

    PROCESSENTRY32 procEntry = { 0 };
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    Process32First(sj_handle_first, &procEntry);

    while (1)
    {
        if (!_tcscmp(procEntry.szExeFile, a_process_name))
        {
            sj_handle_second = OpenProcess(PROCESS_ALL_ACCESS, 0, procEntry.th32ProcessID);
            if (GetExitCodeProcess(sj_handle_second, &dwExitCode))
            {
                if (TerminateProcess(sj_handle_second, dwExitCode))
                {
                    return TRUE;
                }
            }
        }
        if (!Process32Next(sj_handle_first, &procEntry))
        {
            return FALSE;
        }
    }
    return FALSE;
}

int main()
{
    if (ProcessKill1(_T("vmplayer.exe")))
    {
        printf("���� ����1\n");
    }
    else {
        printf("���� �ȵ�1\n");
    }
    //Sleep(10);
    //WindowsOff2(20);
}