#pragma once
#include "2Box.h"
#include <Monitor/RPCData.h>

namespace RpcServer
{
int AcptEvent(void* instance, int type, void* usr);

// int ConnErr(void* instance,void* usr);

// int HookCompleted( const void* data_in, unsigned int size_in, COMPLETIONPROC proc,void* usr);

int OnNewProcess(DWORD pid);

int OnNewWnd(const EMBED_INFO& embed_info);

int OnEmbedWnd(const EMBED_INFO& embed_info);

int OnActiveWnd();

std::vector<WND_INFO> OnGetAllWnd();

int OnConnect();

// int IsProcessInBox(const void* data_in, unsigned int size_in, COMPLETIONPROC proc,void* usr);

// int InitAllRemoteDlls(const void* data_in, unsigned int size_in, COMPLETIONPROC proc,void* usr);
}; // namespace RpcServer
