#include <Windows.h>
#include <TlHelp32.h>
#include <cstdlib>
#include <iostream>
#include <tchar.h>
#include <string>

class SystemOnOff
{
public:
    void WindowsOff1()
    {
        // 20초 후 윈도우즈 종료
        system("shutdown -s -f -t 20");
    }

    void WindowsOff2(int a_second)
    {
        char sysoff[128] = { 0 };
        sprintf_s(sysoff, 128, "C:\\Windows\\System32\\shutdown.exe -s -f -t %d", a_second);

        // a_second초 후 윈도우즈 종료
        system(sysoff);
    }

    void WindowsRestart(int a_second)
    {
        char sys_restart[128] = { 0 };
        sprintf_s(sys_restart, 128, "C:\\Windows\\System32\\shutdown.exe -r -f -t %d", a_second);

        // a_second초 후 윈도우즈 재시작
        system(sys_restart);
    }

    void WindowsOffCancel()
    {
        // 종료 및 재시작 예약메시지 취소
        system("C:\\Windows\\System32\\shutdown.exe -a");
    }

    // 프로세스 이름을 넣으면 해당 프로세스명을 찾아 종료시키는 함수
    bool ProcessKill1(const TCHAR* a_process_name)
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
                        return true;
                    }
                }
            }
            if (!Process32Next(sj_handle_first, &procEntry))
            {
                return false;
            }
        }
        return false;
    }

    // 프로세스명을 넣으면 해당 프로세스가 이미 실행중인지 알려주는 함수
    bool isProcessRunning(const TCHAR* a_process_name)
    {
        bool exists = false;
        PROCESSENTRY32 entry = { 0 };
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        if (Process32First(snapshot, &entry))
        {
            do {
                if (!_tcsicmp(entry.szExeFile, a_process_name))
                {
                    exists = true;
                    break;
                }
            } while (Process32Next(snapshot, &entry));
        }

        CloseHandle(snapshot);
        return exists;
    }

    // exe 실행 함수
    void runProcess(const char* a_program_fullpath)
    {
        const char* last_back_slash = strrchr(a_program_fullpath, '\\');
        const char* last_forward_slash = strrchr(a_program_fullpath, '/');
        const char* last_slash = 0;
        char filename[256] = { 0 };

        // 어떤 슬래시를 썻는지 찾기
        if (last_back_slash != NULL && last_forward_slash != NULL) {
            if (last_back_slash > last_forward_slash) {
                last_slash = last_back_slash;
            }
            else {
                last_slash = last_forward_slash;
            }
        }
        else if(last_back_slash != NULL){
            last_slash = last_back_slash;
        }
        else if (last_forward_slash != NULL) {
            last_slash = last_forward_slash;
        }

        // 마지막 슬래시 위치에서부터 뒤로 자르기
        if (last_slash != NULL) {
            strncpy_s(filename, last_slash + 1, 255);
        }
        else {
            strncpy_s(filename, a_program_fullpath, 255);
        }
        filename[255] = '\0';

        // char*를 wchar_t*로 변환
        wchar_t w_filename[256] = { 0 };
        MultiByteToWideChar(CP_ACP, 0, filename, -1, w_filename, 256);

        // 해당 exe가 이미 실행중인지 체크
        if (isProcessRunning(w_filename) == false)
        {
            // 프로세스 실행
            system(a_program_fullpath);
        }
        else {
            // 이미 실행중이면
            printf("Process is already running.\n");
        }
    }

private:

};

int main()
{
    SystemOnOff* sysController = new SystemOnOff;
    sysController->runProcess("C:\\exe\\WPF-RJ.exe");
    //Sleep(50);
    //if (sysController->ProcessKill1(_T("vmplayer.exe")))
    //{
    //    printf("종료 성공1\n");
    //}
    //else {
    //    printf("종료 안됨1\n");
    //}
    //Sleep(10);
    //sysController->WindowsOff2(20);

    delete sysController;
}
