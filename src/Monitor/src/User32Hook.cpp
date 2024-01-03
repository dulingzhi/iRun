#include "pch.h"
#include "User32Hook.h"
#include "TrampolineFunc.h"
#include "Monitor/RPCData.h"

TRAMPOLINE(LRESULT(WINAPI*)(CONST MSG*), DispatchMessageA);
TRAMPOLINE(LRESULT(WINAPI*)(CONST MSG*), DispatchMessageW);

TRAMPOLINE(HWND(WINAPI*)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID), CreateWindowExA);
TRAMPOLINE(HWND(WINAPI*)(DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID), CreateWindowExW);

TRAMPOLINE(HWND(WINAPI*)(LPCSTR, LPCSTR), FindWindowA);
TRAMPOLINE(HWND(WINAPI*)(LPCWSTR, LPCWSTR), FindWindowW);

TRAMPOLINE(HWND(WINAPI*)(HWND, HWND, LPCSTR, LPCSTR), FindWindowExA);
TRAMPOLINE(HWND(WINAPI*)(HWND, HWND, LPCWSTR, LPCWSTR), FindWindowExW);

TRAMPOLINE(BOOL(WINAPI*)(WNDENUMPROC, LPARAM), EnumWindows);

TRAMPOLINE(BOOL(WINAPI*)(HWND, WNDENUMPROC, LPARAM), EnumChildWindows);

TRAMPOLINE(HWND(WINAPI*)(HWND, UINT), GetWindow);

TRAMPOLINE(HWND(WINAPI*)(HWND), GetTopWindow);

int EmbedInfoCallBack(unsigned int error, const void* data, unsigned int size, void* usr)
{
    if (error || NULL == data || size != sizeof(EMBED_INFO) || NULL == usr)
    {
        return -1;
    }

    memcpy(usr, data, size);
    return 0;
}

CUser32Hook::CUser32Hook(void) {}

CUser32Hook::~CUser32Hook(void) {}

BOOL CUser32Hook::Init()
{
    // CBaseHook::InitFile(L"user32",FALSE);

    HOOK(CUser32Hook, DispatchMessageA);
    HOOK(CUser32Hook, DispatchMessageW);

    HOOK(CUser32Hook, CreateWindowExA);
    HOOK(CUser32Hook, CreateWindowExW);

    HOOK(CUser32Hook, FindWindowA);
    HOOK(CUser32Hook, FindWindowW);

    HOOK(CUser32Hook, FindWindowExA);
    HOOK(CUser32Hook, FindWindowExW);

    HOOK(CUser32Hook, EnumWindows);
    HOOK(CUser32Hook, EnumChildWindows);
    HOOK(CUser32Hook, GetWindow);
    HOOK(CUser32Hook, GetTopWindow);

    // CBaseHook::UninitFile();
    return TRUE;
}

LRESULT
WINAPI CUser32Hook::DispatchMessageA(__in CONST MSG* lpMsg)
{
    if (lpMsg && WM_SYSCOMMAND == lpMsg->message)
    {
        if (0xEFFF == lpMsg->wParam && IsWindowVisible(lpMsg->hwnd))
        {
            EMBED_INFO embed_info = {0};
            embed_info.iPid = GetCurrentProcessId();
            embed_info.hWnd = (unsigned long long)lpMsg->hwnd;

            GetWindowTextW(lpMsg->hwnd, embed_info.szTitle, 260);

            g_Veigar->syncCall("iRun.Server", 100, "iRun.EmbedWnd", embed_info);
        }
    }

    return TrueDispatchMessageA.Call()(lpMsg);
}

LRESULT
WINAPI CUser32Hook::DispatchMessageW(__in CONST MSG* lpMsg)
{
    if (lpMsg && WM_SYSCOMMAND == lpMsg->message)
    {
        if (0xEFFF == lpMsg->wParam && IsWindowVisible(lpMsg->hwnd))
        {
            EMBED_INFO embed_info = {0};
            embed_info.iPid = GetCurrentProcessId();
            embed_info.hWnd = (unsigned long long)lpMsg->hwnd;

            GetWindowTextW(lpMsg->hwnd, embed_info.szTitle, 260);

            g_Veigar->syncCall("iRun.Server", 100, "iRun.EmbedWnd", embed_info);
        }
    }

    return TrueDispatchMessageW.Call()(lpMsg);
}

HWND WINAPI CUser32Hook::CreateWindowExA(__in DWORD dwExStyle,
                                         __in_opt LPCSTR lpClassName,
                                         __in_opt LPCSTR lpWindowName,
                                         __in DWORD dwStyle,
                                         __in int X,
                                         __in int Y,
                                         __in int nWidth,
                                         __in int nHeight,
                                         __in_opt HWND hWndParent,
                                         __in_opt HMENU hMenu,
                                         __in_opt HINSTANCE hInstance,
                                         __in_opt LPVOID lpParam)
{
    HWND hWndRet = TrueCreateWindowExA.Call()(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    if (NULL == hWndRet)
    {
        return hWndRet;
    }

    if (hWndParent || (dwExStyle & WS_EX_TOOLWINDOW))
    {
        return hWndRet;
    }

    HMENU hSysMenu = GetSystemMenu(hWndRet, FALSE);
    if (NULL == hSysMenu)
    {
        return hWndRet;
    }

    if (FALSE == AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL))
    {
        return hWndRet;
    }

    if (FALSE == AppendMenu(hSysMenu, MF_STRING, 0xEFFF, _T("扔进盒子")))
    {
        return hWndRet;
    }

    EMBED_INFO embed_info = {0};
    embed_info.iPid = GetCurrentProcessId();
    embed_info.hWnd = (unsigned long long)hWndRet;
    embed_info.dwExStyle = dwExStyle;
    embed_info.dwStyle = dwStyle;

    g_Veigar->syncCall("iRun.Server", 100, "iRun.NewWnd", embed_info);

    return hWndRet;
}

HWND WINAPI CUser32Hook::CreateWindowExW(__in DWORD dwExStyle,
                                         __in_opt LPCWSTR lpClassName,
                                         __in_opt LPCWSTR lpWindowName,
                                         __in DWORD dwStyle,
                                         __in int X,
                                         __in int Y,
                                         __in int nWidth,
                                         __in int nHeight,
                                         __in_opt HWND hWndParent,
                                         __in_opt HMENU hMenu,
                                         __in_opt HINSTANCE hInstance,
                                         __in_opt LPVOID lpParam)
{
    HWND hWndRet = TrueCreateWindowExW.Call()(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    if (NULL == hWndRet)
    {
        return hWndRet;
    }

    if (hWndParent || (dwExStyle & WS_EX_TOOLWINDOW))
    {
        return hWndRet;
    }

    HMENU hSysMenu = GetSystemMenu(hWndRet, FALSE);
    if (NULL == hSysMenu)
    {
        return hWndRet;
    }

    if (FALSE == AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL))
    {
        return hWndRet;
    }

    if (FALSE == AppendMenu(hSysMenu, MF_STRING, 0xEFFF, _T("扔进盒子")))
    {
        return hWndRet;
    }

    EMBED_INFO embed_info = {0};
    embed_info.iPid = GetCurrentProcessId();
    embed_info.hWnd = (unsigned long long)hWndRet;
    embed_info.dwExStyle = dwExStyle;
    embed_info.dwStyle = dwStyle;

    g_Veigar->syncCall("iRun.Server", 100, "iRun.NewWnd", embed_info);
    return hWndRet;
}

HWND WINAPI CUser32Hook::FindWindowA(__in_opt LPCSTR lpClassName, __in_opt LPCSTR lpWindowName)
{
    HWND hRet = TrueFindWindowA.Call()(lpClassName, lpWindowName);
    if (NULL == hRet)
    {
        return hRet;
    }

    auto res = g_Veigar->syncCall("iRun.Server", 100, "iRun.GetAllWnd");
    if (!res.isSuccess())
    {
        return NULL;
    }

    auto windows = res.obj.get().as<std::vector<WND_INFO>>();

    DWORD dwPid = GetCurrentProcessId();

    for (auto& item : windows)
    {
        if (item.iPid != dwPid && item.hWnd == (unsigned long long)hRet)
        {
            hRet = NULL;
            break;
        }
    }

    return hRet;
}

HWND WINAPI CUser32Hook::FindWindowW(__in_opt LPCWSTR lpClassName, __in_opt LPCWSTR lpWindowName)
{
    HWND hRet = TrueFindWindowW.Call()(lpClassName, lpWindowName);
    if (NULL == hRet)
    {
        return hRet;
    }

    auto res = g_Veigar->syncCall("iRun.Server", 100, "iRun.GetAllWnd");
    if (!res.isSuccess())
    {
        return NULL;
    }

    auto windows = res.obj.get().as<std::vector<WND_INFO>>();
    DWORD dwPid = GetCurrentProcessId();

    for (auto& item : windows)
    {
        if (item.iPid != dwPid && item.hWnd == (unsigned long long)hRet)
        {
            hRet = NULL;
            break;
        }
    }

    return hRet;
}

HWND WINAPI CUser32Hook::FindWindowExA(__in_opt HWND hWndParent, __in_opt HWND hWndChildAfter, __in_opt LPCSTR lpszClass, __in_opt LPCSTR lpszWindow)
{
    HWND hRet = TrueFindWindowExA.Call()(hWndParent, hWndChildAfter, lpszClass, lpszWindow);
    if (NULL == hRet)
    {
        return hRet;
    }

    auto res = g_Veigar->syncCall("iRun.Server", 100, "iRun.GetAllWnd");
    if (!res.isSuccess())
    {
        return NULL;
    }

    auto windows = res.obj.get().as<std::vector<WND_INFO>>();
    DWORD dwPid = GetCurrentProcessId();

    for (auto& item : windows)
    {
        if (item.iPid != dwPid && item.hWnd == (unsigned long long)hRet)
        {
            hRet = NULL;
            break;
        }
    }

    return hRet;
}

HWND WINAPI CUser32Hook::FindWindowExW(__in_opt HWND hWndParent, __in_opt HWND hWndChildAfter, __in_opt LPCWSTR lpszClass, __in_opt LPCWSTR lpszWindow)
{
    HWND hRet = TrueFindWindowExW.Call()(hWndParent, hWndChildAfter, lpszClass, lpszWindow);
    if (NULL == hRet)
    {
        return hRet;
    }

    auto res = g_Veigar->syncCall("iRun.Server", 100, "iRun.GetAllWnd");
    if (!res.isSuccess())
    {
        return NULL;
    }

    auto windows = res.obj.get().as<std::vector<WND_INFO>>();
    DWORD dwPid = GetCurrentProcessId();

    for (auto& item : windows)
    {
        if (item.iPid != dwPid && item.hWnd == (unsigned long long)hRet)
        {
            hRet = NULL;
            break;
        }
    }

    return hRet;
}

struct MyEnumWndParams
{
    WNDENUMPROC lpEnumFunc;
    LPARAM lParam;
    std::vector<WND_INFO> windows;
};

BOOL CALLBACK MyEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    static DWORD dwPid = GetCurrentProcessId();
    auto p = (MyEnumWndParams*)lParam;
    if (!p || !p->lpEnumFunc)
    {
        return FALSE;
    }

    for (auto& item : p->windows)
    {
        if (item.iPid != dwPid && item.hWnd == (unsigned long long)hwnd)
        {
            return TRUE;
        }
    }

    return p->lpEnumFunc(hwnd, p->lParam);
}

BOOL WINAPI CUser32Hook::EnumWindows(__in WNDENUMPROC lpEnumFunc, __in LPARAM lParam)
{
    MyEnumWndParams params{};
    params.lParam = lParam;
    params.lpEnumFunc = lpEnumFunc;
    
    auto res = g_Veigar->syncCall("iRun.Server", 100, "iRun.GetAllWnd");
    if (res.isSuccess())
    {
        params.windows = res.obj.get().as<std::vector<WND_INFO>>();
    }

    return TrueEnumWindows.Call()(&MyEnumWindowsProc, (LPARAM)&params);
}

BOOL WINAPI CUser32Hook::EnumChildWindows(__in_opt HWND hWndParent, __in WNDENUMPROC lpEnumFunc, __in LPARAM lParam)
{
    if (hWndParent)
    {
        return TrueEnumChildWindows.Call()(hWndParent, lpEnumFunc, lParam);
    }

    MyEnumWndParams params{};
    params.lParam = lParam;
    params.lpEnumFunc = lpEnumFunc;

    auto res = g_Veigar->syncCall("iRun.Server", 100, "iRun.GetAllWnd");
    if (res.isSuccess())
    {
        params.windows = res.obj.get().as<std::vector<WND_INFO>>();
    }
    return TrueEnumChildWindows.Call()(hWndParent, &MyEnumWindowsProc, (LPARAM)&params);
}

HWND WINAPI CUser32Hook::GetWindow(__in HWND hWnd, __in UINT uCmd)
{
    HWND hRet = TrueGetWindow.Call()(hWnd, uCmd);

    if (NULL == hRet)
    {
        return hRet;
    }

    auto res = g_Veigar->syncCall("iRun.Server", 100, "iRun.GetAllWnd");
    if (!res.isSuccess())
    {
        return NULL;
    }

    auto windows = res.obj.get().as<std::vector<WND_INFO>>();

    DWORD dwPid = GetCurrentProcessId();

    for (auto& item : windows)
    {
        if (item.iPid != dwPid && item.hWnd == (unsigned long long)hRet)
        {
            hRet = NULL;
            break;
        }
    }

    return hRet;
}

HWND WINAPI CUser32Hook::GetTopWindow(__in_opt HWND hWnd)
{
    if (hWnd)
    {
        return TrueGetTopWindow.Call()(hWnd);
    }

    HWND hRet = TrueGetTopWindow.Call()(hWnd);
    if (NULL == hRet)
    {
        return hRet;
    }

    auto res = g_Veigar->syncCall("iRun.Server", 100, "iRun.GetAllWnd");
    if (!res.isSuccess())
    {
        return NULL;
    }

    auto windows = res.obj.get().as<std::vector<WND_INFO>>();
    DWORD dwPid = GetCurrentProcessId();

    for (auto& item : windows)
    {
        if (item.iPid != dwPid && item.hWnd == (unsigned long long)hRet)
        {
            hRet = NULL;
            break;
        }
    }

    return hRet;
}
