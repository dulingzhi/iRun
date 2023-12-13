#include "pch.h"
#include "RpcServer.h"
#include "DataMgr.h"
#include "MainFrm.h"

namespace RpcServer
{
int AcptEvent(void* instance, int type, void* usr)
{
    if (1 == type)
    {
        theApp.GetMainWnd()->PostMessage(CMainFrame::WM_GAME_UPDATE);
    }
    return 0;
}

// 	int ConnErr(void* instance,void* usr)
// 	{
// 		theApp.GetMainWnd()->PostMessage(CMainFrame::WM_HELPER_EXCP);
// 		return 0;
// 	}

// 	int HookCompleted( rpc_conn conn )
// 	{
// 		int iPid = 0;
// 		if (data_in && size_in == sizeof(int))
// 		{
// 			memcpy(&iPid,data_in,size_in);
// 		}
// 		else
// 		{
// 			return -1;
// 		}
//
// 		DataPtr p = theDataMgr.FindProcessData(iPid);
// 		if (NULL == p.get())
// 		{
// 			return -1;
// 		}
//
// 		NEW_PROC_DATA& data = p->GetProcData();
//
// 		if (proc)
// 		{
// 			proc(0,&data,sizeof(NEW_PROC_DATA),usr);
// 		}
//
// 		return 0;
// 	}

int OnNewProcess(rpc_conn conn, DWORD iPid)
{
    NEW_PROC_DATA new_proc_data = {0};
    new_proc_data.iPid = iPid;
    new_proc_data.hNewProcHandleInBox = (unsigned long long)OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, FALSE, new_proc_data.iPid);

    DataPtr pData = std::make_shared<CProcessData>(new_proc_data);

    theDataMgr.AddProcessData(pData);

    return 0;
}

int OnNewWnd(rpc_conn conn, const EMBED_INFO& embed_info)
{
    theApp.GetMainWnd()->SendMessage(CMainFrame::WM_GAME_NEWWND, (WPARAM)&embed_info);
    return 0;
}

int OnEmbedWnd(rpc_conn conn, const EMBED_INFO& embed_info)
{
    theApp.GetMainWnd()->SendMessage(CMainFrame::WM_GAME_ATTACH, (WPARAM)&embed_info);
    return 0;
}

int OnActiveWnd(rpc_conn conn)
{
    CWnd* pMainWnd = theApp.GetMainWnd();
    if (NULL == pMainWnd)
    {
        return 0;
    }

    if (pMainWnd->GetSafeHwnd())
    {
        // pMainWnd->ShowWindow(SW_SHOWMINIMIZED);
        pMainWnd->ShowWindow(SW_RESTORE);
        pMainWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        pMainWnd->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        pMainWnd->SetForegroundWindow();
    }
    return 0;
}

std::vector<WND_INFO> OnGetAllWnd(rpc_conn conn)
{
    std::vector<WndDataPtr> vecAllWnd;
    theDataMgr.GetAllWnd(vecAllWnd);
    size_t count = vecAllWnd.size();

    std::vector<WND_INFO> result;
    for (size_t i = 0; i < count; ++i)
    {
        auto& ptr = vecAllWnd[i]->GetWndData();
        result.push_back({ptr.iPid, ptr.hWnd});
    }

    result.push_back({GetCurrentProcessId(), (unsigned long long)theApp.GetMainWnd()->GetSafeHwnd()});

    return result;
}

// 	int IsProcessInBox( rpc_conn conn )
// 	{
// 		int iPid = 0;
// 		if (data_in && size_in == sizeof(int))
// 		{
// 			memcpy(&iPid,data_in,size_in);
// 		}
// 		else
// 		{
// 			return -1;
// 		}
//
// 		DataPtr p = theDataMgr.FindProcessData(iPid);
// 		if (NULL == p.get())
// 		{
// 			return -1;
// 		}
//
// 		BOOL b = TRUE;
// 		if (proc)
// 		{
// 			proc(0,&b,sizeof(BOOL),usr);
// 		}
//
// 		return 0;
// 	}

// 	int InitAllRemoteDlls( rpc_conn conn )
// 	{
// 		int iPid = 0;
// 		if (data_in && size_in == sizeof(int))
// 		{
// 			memcpy(&iPid,data_in,size_in);
// 		}
// 		else
// 		{
// 			return -1;
// 		}
//
// 		if (FALSE == theApp.InitAllDllsByCreateRemoteThread(iPid))
// 		{
// 			return -1;
// 		}
//
// 		if (proc)
// 		{
// 			BOOL bRet = TRUE;
// 			proc(0,&bRet,sizeof(BOOL),usr);
// 		}
// 		return 0;
// 	}
} // namespace RpcServer