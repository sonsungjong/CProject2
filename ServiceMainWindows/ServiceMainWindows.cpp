#include <Windows.h>
#include <winsvc.h>
#include <time.h>
#include <WinBase.h>
#include <stdio.h>

SERVICE_STATUS g_service_status;
SERVICE_STATUS_HANDLE gh_service_status;
UINT8 g_service_running = 0;
HANDLE gh_wait_event;               // 수행 지연 효과를 위해 생성할 이벤트 객체
wchar_t g_service_name[64] = L"SungjongServiceRoutine";

void WINAPI ServiceCtrlHandler(DWORD a_op_code)
{
    switch (a_op_code)
    {
        case SERVICE_CONTROL_PAUSE:             // 서비스 관리에서 '일시중지' 버튼
            g_service_status.dwCurrentState = SERVICE_PAUSED;
            break;
        case SERVICE_CONTROL_CONTINUE:              // 서비스 관리에서 '계속' 버튼
            g_service_status.dwCurrentState = SERVICE_RUNNING;
            break;
        case SERVICE_CONTROL_STOP:                          // 서비스 관리에서 '중지' 버튼
            g_service_status.dwWin32ExitCode = 0;
            g_service_status.dwCurrentState = SERVICE_STOPPED;
            g_service_status.dwCheckPoint = 0;
            g_service_status.dwWaitHint = 0;
            SetServiceStatus(gh_service_status, &g_service_status);             // 서비스 상태를 중지로 변경
            CloseHandle(gh_wait_event);             // 이벤트 객체 제거
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

    // 서비스 관리자에게 서비스 이름을 SungjongServiceRoutine 이라고 등록
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
        // 서비스 루틴의 부하를 줄이기 위해서 1초 지연 코드 추가
        WaitForSingleObject(gh_wait_event, 1000);
        // ToDo : 서비스 루틴에서 하고자하는 일을 작성

    }
}

// '-i' 옵션으로 서비스를 추가하는 함수
int InstallService()
{
    wchar_t service_exe_path[MAX_PATH];
    // 현재 작업 경로를 얻고 그 뒤에 프로그램 이름 추가
    int len = GetCurrentDirectory(MAX_PATH, service_exe_path);
    int state = 0;
    wcscat_s(service_exe_path, MAX_PATH - len, L"SungjongService.exe");

    // 서비스 목록에 서비스를 추가하기 위해 핸들을 얻는다
    SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (h_sc_manager) {
        // 서비스 목록에는 "Sj Service List Content" 라고 표시된다
        // 등록정보에서 서비스 이름으로 "SungjongServiceRoutine" 으로 표시된다
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
        
        // 핸들 정리
        if (h_service)
        {
            CloseServiceHandle(h_service);
            state = 1;
        }
        CloseServiceHandle(h_sc_manager);
    }
    return state;
}

// '-d' 옵션으로 서비스 제거할때 사용할 함수
int DeleteService()
{
    // 서비스 목록에서 원하는 서비스를 제거하기 위해 핸들을 얻는다
    SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);              // 서비스 목록 열기
    int state = 0;

    if (h_sc_manager) {
        // 자신이 등록한 g_service_name 이름의 서비스 루틴을 찾는다
        SC_HANDLE h_service = OpenService(h_sc_manager, g_service_name, SERVICE_ALL_ACCESS);
        if (h_service) {
            // 지정한 서비스를 찾으면 서비스 루틴을 제거
            if (DeleteService(h_service)) {
                state = 1;
            }
            CloseServiceHandle(h_service);              // 서비스루틴 제거
            CloseServiceHandle(h_sc_manager);           // 서비스 목록 닫기
        }
    }
    return state;
}

int main(int argc, char* argv[])
{
    // 관리자 권한 필요!
    // 프로그램 실행 인자가 있는 경우
    if (argc > 1) {
        if (!strcmp(argv[1], "-i")) {
            // 실행 인자가 "-i" 면 서비스 루틴을 설치한다
            if (InstallService()) {
                printf("\n\nService Installed!!\n");
            }
            else {
                printf("\n\nInstall Error!!\n");
            }
        }
        else if (strcmp(argv[1], "-d") == 0) {
            // 실행 인자가 "-d" 면 서비스 루틴을 제거한다
            if (DeleteService()) {
                printf("\n\nService UnInstalled!!\n");
            }
            else {
                printf("\n\nUninstall Error!!\n");
            }
        }
        else {
            // 잘못된 인자가 입력되면 사용법을 알려준다
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
