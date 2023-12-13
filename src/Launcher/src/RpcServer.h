#pragma once
#include "2Box.h"
#include <Monitor/RPCData.h>

namespace RpcServer
{
int AcptEvent(void* instance, int type, void* usr);

// int ConnErr(void* instance,void* usr);

// int HookCompleted( const void* data_in, unsigned int size_in, COMPLETIONPROC proc,void* usr);

int OnNewProcess(rpc_conn conn, DWORD pid);

int OnNewWnd(rpc_conn conn, const EMBED_INFO& embed_info);

int OnEmbedWnd(rpc_conn conn, const EMBED_INFO& embed_info);

int OnActiveWnd(rpc_conn conn);

std::vector<WND_INFO> OnGetAllWnd(rpc_conn conn);

// int IsProcessInBox(const void* data_in, unsigned int size_in, COMPLETIONPROC proc,void* usr);

// int InitAllRemoteDlls(const void* data_in, unsigned int size_in, COMPLETIONPROC proc,void* usr);
}; // namespace RpcServer
