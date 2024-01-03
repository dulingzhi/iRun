#include "pch.h"
#include "InitialData.h"
#include "HookMgr.h"
#include <detours.h>

std::shared_ptr<veigar::Veigar> g_Veigar;

int Init()
{
    g_Veigar = std::make_shared<veigar::Veigar>();
    auto res = g_Veigar->syncCall("iRun.Server", 1000, "iRun.Connect");

    if (!res.isSuccess())
    {
        MessageBox(NULL, "Connect to iRun.Server failed.", res.errorMessage.c_str(), MB_OK);
        TerminateProcess(GetCurrentProcess(), 0);
        return -1;
    }

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

DWORD WINAPI InitializeDll()
{
    Init();
    InitHook();
    return 0;
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
        InitializeDll();
    }

    return TRUE;
}
