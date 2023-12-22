#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <cstdio>

void WindowsOff1()
{
    // 20초후 윈도우즈 종료
    system("shutdown -s -f -t 20");
}

void WindowsOff2(int a_second)
{
    char sysoff[128];
    sprintf_s(sysoff, 128, "C:\\Windows\\System32\\shutdown.exe -s -f -t %d", a_second);

    // a_second초후 윈도우즈 종료
    system(sysoff);
}

void WindowsRestart(int a_second)
{
    char sys_restart[128];
    sprintf_s(sys_restart, 128, "C:\\Windows\\System32\\shutdown.exe -r -f -t %d", a_second);

    // 20초후 윈도우즈 재시작
    system(sys_restart);
}

void WindowsOffCancel()
{
    system("C:\\Windows\\System32\\shutdown.exe -a");           // 종료 및 재시작 예약메시지 취소
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

bool isProcessRunning(const TCHAR* a_process_name)
{
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry))
    {
        do {
            if (!_wcsicmp(entry.szExeFile, a_process_name)) {
                exists = true;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return exists;
}

void runProcess(const char* a_program_path, const TCHAR* a_process_name) {
    if (isProcessRunning(a_process_name) == false) {
        system(a_program_path);
    }
    else {
        printf("Process is already running.\n");
    }
}

int main()
{
    runProcess("C:\\exe\\WPF-RJ.exe", _T("WPF-RJ.exe"));
    //Sleep(50);
    //if (ProcessKill1(_T("vmplayer.exe")))
    //{
    //    printf("종료 성공1\n");
    //}
    //else {
    //    printf("종료 안됨1\n");
    //}
    //Sleep(10);
    //WindowsOff2(20);
}
