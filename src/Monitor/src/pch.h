// pch.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

#include <rest_rpc.hpp>
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;
extern std::shared_ptr<rpc_client> g_RPCClient;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <tchar.h>
#include <stdio.h>

#include <string>
#include <set>