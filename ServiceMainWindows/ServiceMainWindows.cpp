#include <Windows.h>
#include <winsvc.h>
#include <time.h>
#include <WinBase.h>
#include <stdio.h>

SERVICE_STATUS g_service_status;
SERVICE_STATUS_HANDLE gh_service_status;
UINT8 g_service_running = 0;
HANDLE gh_wait_event;               // ���� ���� ȿ���� ���� ������ �̺�Ʈ ��ü
wchar_t g_service_name[64] = L"SungjongServiceRoutine";

void WINAPI ServiceCtrlHandler(DWORD a_op_code)
{
    switch (a_op_code)
    {
        case SERVICE_CONTROL_PAUSE:             // ���� �������� '�Ͻ�����' ��ư
            g_service_status.dwCurrentState = SERVICE_PAUSED;
            break;
        case SERVICE_CONTROL_CONTINUE:              // ���� �������� '���' ��ư
            g_service_status.dwCurrentState = SERVICE_RUNNING;
            break;
        case SERVICE_CONTROL_STOP:                          // ���� �������� '����' ��ư
            g_service_status.dwWin32ExitCode = 0;
            g_service_status.dwCurrentState = SERVICE_STOPPED;
            g_service_status.dwCheckPoint = 0;
            g_service_status.dwWaitHint = 0;
            SetServiceStatus(gh_service_status, &g_service_status);             // ���� ���¸� ������ ����
            CloseHandle(gh_wait_event);             // �̺�Ʈ ��ü ����
            RemoveDirectory(L"C:\\SungjongServiceRoutine");
            break;
        case SERVICE_CONTROL_INTERROGATE:
            break;
    }
}

void WINAPI ServiceMain(DWORD argc, LPWSTR* argv)
{
    g_service_status.dwServiceType = SERVICE_WIN32;
    g_service_status.dwCurrentState = SERVICE_START_PENDING;
    g_service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_service_status.dwWin32ExitCode = 0;
    g_service_status.dwServiceSpecificExitCode = 0;
    g_service_status.dwCheckPoint = 0;
    g_service_status.dwWaitHint = 0;

    // ���� �����ڿ��� ���� �̸��� SungjongServiceRoutine �̶�� ���
    gh_service_status = RegisterServiceCtrlHandler(g_service_name, ServiceCtrlHandler);
    if (gh_service_status == (SERVICE_STATUS_HANDLE)0) {
        return;
    }

    g_service_status.dwCurrentState = SERVICE_RUNNING;
    g_service_status.dwCheckPoint = 0;
    g_service_status.dwWaitHint = 0;
    if (!SetServiceStatus(gh_service_status, &g_service_status)) {

    }

    CreateDirectory(L"C:\\SungjongServiceRoutine", NULL);
    gh_wait_event = CreateEvent(NULL, TRUE, FALSE, L"SjSrv1");

    g_service_running = 1;
    while (g_service_running) {
        // ���� ��ƾ�� ���ϸ� ���̱� ���ؼ� 1�� ���� �ڵ� �߰�
        WaitForSingleObject(gh_wait_event, 1000);
        // ToDo : ���� ��ƾ���� �ϰ����ϴ� ���� �ۼ�

    }
}

// '-i' �ɼ����� ���񽺸� �߰��ϴ� �Լ�
int InstallService()
{
    wchar_t service_exe_path[MAX_PATH];
    // ���� �۾� ��θ� ��� �� �ڿ� ���α׷� �̸� �߰�
    int len = GetCurrentDirectory(MAX_PATH, service_exe_path);
    int state = 0;
    wcscat_s(service_exe_path, MAX_PATH - len, L"SungjongService.exe");

    // ���� ��Ͽ� ���񽺸� �߰��ϱ� ���� �ڵ��� ��´�
    SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (h_sc_manager) {
        // ���� ��Ͽ��� "Sj Service List Content" ��� ǥ�õȴ�
        // ����������� ���� �̸����� "SungjongServiceRoutine" ���� ǥ�õȴ�
        SC_HANDLE h_service = CreateService(h_sc_manager, g_service_name, L"Sj Service List Content",
            SERVICE_ALL_ACCESS,                                 // desired access
            SERVICE_WIN32_OWN_PROCESS,                  // service type
            SERVICE_DEMAND_START,                   // start type
            SERVICE_ERROR_NORMAL,                   // error control type
            service_exe_path,               // service's binary
            NULL,               // no load ordering group
            NULL,                   // no tag identifier
            NULL,               // no dependencies
            NULL,               // LocalSystem account
            NULL                // no password
        );
        
        // �ڵ� ����
        if (h_service)
        {
            CloseServiceHandle(h_service);
            state = 1;
        }
        CloseServiceHandle(h_sc_manager);
    }
    return state;
}

// '-d' �ɼ����� ���� �����Ҷ� ����� �Լ�
int DeleteService()
{
    // ���� ��Ͽ��� ���ϴ� ���񽺸� �����ϱ� ���� �ڵ��� ��´�
    SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);              // ���� ��� ����
    int state = 0;

    if (h_sc_manager) {
        // �ڽ��� ����� g_service_name �̸��� ���� ��ƾ�� ã�´�
        SC_HANDLE h_service = OpenService(h_sc_manager, g_service_name, SERVICE_ALL_ACCESS);
        if (h_service) {
            // ������ ���񽺸� ã���� ���� ��ƾ�� ����
            if (DeleteService(h_service)) {
                state = 1;
            }
            CloseServiceHandle(h_service);              // ���񽺷�ƾ ����
            CloseServiceHandle(h_sc_manager);           // ���� ��� �ݱ�
        }
    }
    return state;
}

int main(int argc, char* argv[])
{
    // ������ ���� �ʿ�!
    // ���α׷� ���� ���ڰ� �ִ� ���
    if (argc > 1) {
        if (!strcmp(argv[1], "-i")) {
            // ���� ���ڰ� "-i" �� ���� ��ƾ�� ��ġ�Ѵ�
            if (InstallService()) {
                printf("\n\nService Installed!!\n");
            }
            else {
                printf("\n\nInstall Error!!\n");
            }
        }
        else if (strcmp(argv[1], "-d") == 0) {
            // ���� ���ڰ� "-d" �� ���� ��ƾ�� �����Ѵ�
            if (DeleteService()) {
                printf("\n\nService UnInstalled!!\n");
            }
            else {
                printf("\n\nUninstall Error!!\n");
            }
        }
        else {
            // �߸��� ���ڰ� �ԷµǸ� ������ �˷��ش�
            printf("\n\n[Help]: Install    -> %S -i\n", g_service_name);
            printf("                  Uninstall    -> %S -d\n\n", g_service_name);
        }
    }
    else {
        SERVICE_TABLE_ENTRY DispatchTable[] = { {g_service_name, ServiceMain}, {NULL,NULL} };
        StartServiceCtrlDispatcher(DispatchTable);
    }
    return 0;
}
