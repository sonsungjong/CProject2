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
        // 20�� �� �������� ����
        system("shutdown -s -f -t 20");
    }

    void WindowsOff2(int a_second)
    {
        char sysoff[128] = { 0 };
        sprintf_s(sysoff, 128, "C:\\Windows\\System32\\shutdown.exe -s -f -t %d", a_second);

        // a_second�� �� �������� ����
        system(sysoff);
    }

    void WindowsRestart(int a_second)
    {
        char sys_restart[128] = { 0 };
        sprintf_s(sys_restart, 128, "C:\\Windows\\System32\\shutdown.exe -r -f -t %d", a_second);

        // a_second�� �� �������� �����
        system(sys_restart);
    }

    void WindowsOffCancel()
    {
        // ���� �� ����� ����޽��� ���
        system("C:\\Windows\\System32\\shutdown.exe -a");
    }

    // ���μ��� �̸��� ������ �ش� ���μ������� ã�� �����Ű�� �Լ�
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

    // ���μ������� ������ �ش� ���μ����� �̹� ���������� �˷��ִ� �Լ�
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

    // exe ���� �Լ�
    void runProcess(const char* a_program_fullpath)
    {
        const char* last_back_slash = strrchr(a_program_fullpath, '\\');
        const char* last_forward_slash = strrchr(a_program_fullpath, '/');
        const char* last_slash = 0;
        char filename[256] = { 0 };

        // � �����ø� ������ ã��
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

        // ������ ������ ��ġ�������� �ڷ� �ڸ���
        if (last_slash != NULL) {
            strncpy_s(filename, last_slash + 1, 255);
        }
        else {
            strncpy_s(filename, a_program_fullpath, 255);
        }
        filename[255] = '\0';

        // char*�� wchar_t*�� ��ȯ
        wchar_t w_filename[256] = { 0 };
        MultiByteToWideChar(CP_ACP, 0, filename, -1, w_filename, 256);

        // �ش� exe�� �̹� ���������� üũ
        if (isProcessRunning(w_filename) == false)
        {
            // ���μ��� ����
            system(a_program_fullpath);
        }
        else {
            // �̹� �������̸�
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
    //    printf("���� ����1\n");
    //}
    //else {
    //    printf("���� �ȵ�1\n");
    //}
    //Sleep(10);
    //sysController->WindowsOff2(20);

    delete sysController;
}
