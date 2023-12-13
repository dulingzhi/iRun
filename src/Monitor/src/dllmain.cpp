// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "pch.h"
#include "InitialData.h"
#include "HookMgr.h"
#include <detours.h>

std::shared_ptr<rpc_client> g_RPCClient;

int Init()
{
    g_RPCClient = std::make_shared<rpc_client>("127.0.0.1", 10001);
    if (!g_RPCClient->connect())
    {
        TerminateProcess(GetCurrentProcess(), 0);
        return -1;
    }

    g_RPCClient->set_error_callback([](asio::error_code e) {
        std::cout << e << std::endl;
        TerminateProcess(GetCurrentProcess(), 0);
    });

    return 0;
}

void InitHook()
{
    //	SetUnhandledExceptionFilter(MyFilter);

    if (FALSE == g_pData->Init())
    {
        TerminateProcess(GetCurrentProcess(), 0);
        return;
    }

    CHookMgr hookMgr;

    if (FALSE == hookMgr.Init())
    {
        TerminateProcess(GetCurrentProcess(), 0);
        return;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }

    if (DLL_PROCESS_ATTACH == ul_reason_for_call)
    {
        DetourRestoreAfterWith();
        DisableThreadLibraryCalls(hModule);
        Init();
        InitHook();
    }

    return TRUE;
}