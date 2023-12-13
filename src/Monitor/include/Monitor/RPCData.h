#pragma once

#include <rest_rpc.hpp>

struct NEW_PROC_DATA
{
    unsigned iPid;
    unsigned long long hNewProcHandleInBox;
    // unsigned long long	hBoxHandleInNewProc;
    // unsigned long long	hHookedEventInNewProc;
    // unsigned long long	llData;
    // char				szLoaderPath[261];

    MSGPACK_DEFINE(iPid, hNewProcHandleInBox);
};

struct EMBED_INFO
{
    unsigned iPid;
    unsigned long long hWnd;
    unsigned dwExStyle;
    unsigned dwStyle;
    wchar_t szTitle[260];

    MSGPACK_DEFINE(iPid, hWnd, dwExStyle, dwStyle, szTitle);
};

struct WND_INFO
{
    unsigned iPid;
    unsigned long long hWnd;

    MSGPACK_DEFINE(iPid, hWnd);
};
