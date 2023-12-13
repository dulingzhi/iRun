
// 2Box.cpp : ����Ӧ�ó��������Ϊ��
//

#include "pch.h"
#include "afxwinappex.h"
#include "2Box.h"
#include "MainFrm.h"
#include "RpcServer.h"

#include <sys/timeb.h>

int bResultCallBack(unsigned int error, const void* data, unsigned int size, void* usr)
{
    if (error || NULL == data || size != sizeof(BOOL) || NULL == usr)
    {
        return -1;
    }

    memcpy(usr, data, size);
    return 0;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // �Ի�������
    enum
    {
        IDD = IDD_ABOUTBOX
    };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV ֧��

    // ʵ��
protected:
    DECLARE_MESSAGE_MAP()

private:
    CString m_strNote;
};

CAboutDlg::CAboutDlg()
    : CDialog(CAboutDlg::IDD)
    , m_strNote(_T(""))
{
    m_strNote += _T("��������:\r\n");
    TCHAR msgNote[512] = {_T('��'), _T('��'), _T('��'), _T('��'), _T('ȫ'), _T('��'), _T('��'), ',', _T('��'), _T('��'), _T('ѧ'), _T('ϰ'), _T('��'), _T('��'), ',',      '\r',     '\n', _T('��'), _T('��'), _T('��'),
                          _T('��'), _T('��'), _T('��'), _T('��'), _T('ҵ'), _T('��'), _T(';'), ',', '\r',     '\n',     _T('��'), _T('��'), _T('��'), _T('��'), _T('��'), _T('��'), '!',  '\r',     '\n'};

    m_strNote += msgNote;
    m_strNote += _T("\r\n\r\n");

    m_strNote += _T("������ص�ַ:\r\n");
    TCHAR msgAddr[512] = {_T('��'), _T('��'), ':', 'h', 't', 't', 'p', 's', ':', '/', '/', 'p', 'a', 'n',  '.',  'b',      'a',      'i', 'd', 'u', '.', 'c',
                          'o',      'm',      '/', 's', '/', '1', 'b', 'p', 'Y', 'J', '4', 'w', 'Z', '\r', '\n', _T('��'), _T('��'), ':', 'a', 'h', 'h', '4'};

    m_strNote += msgAddr;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_TIP, m_strNote);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CMy2BoxApp

BEGIN_MESSAGE_MAP(CMy2BoxApp, CWinApp)
ON_COMMAND(ID_APP_ABOUT, &CMy2BoxApp::OnAppAbout)
ON_COMMAND(ID_FILE_OPEN, &CMy2BoxApp::OnFileOpen)
END_MESSAGE_MAP()

void* CMy2BoxApp::NewFileThread(void* usr)
{
    CString* pstrName = (CString*)usr;
    if (NULL == pstrName)
    {
        return 0;
    }

    ((CMainFrame*)theApp.GetMainWnd())->CreateFileAndInjectDll(*pstrName);

    delete pstrName;
    return 0;
}

// CMy2BoxApp ����

CMy2BoxApp::CMy2BoxApp()
    : CWinApp()
    , m_nLLDataPos(0)
{

    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CMy2BoxApp ����

CMy2BoxApp theApp;

// CMy2BoxApp ��ʼ��

BOOL CMy2BoxApp::InitInstance()
{
    if (FALSE == InitSelfPath())
    {
        AfxMessageBox(_T("����·����ȡʧ��"));
        return FALSE;
    }

    if (FALSE == InitEnv())
    {
        AfxMessageBox(_T("�������ʼ��ʧ��"));
        return FALSE;
    }

    if (CheckInstance())
    {
        return FALSE;
    }

    g_RPCServer = std::make_shared<rpc_server>(10001, std::thread::hardware_concurrency());

    if (!g_RPCServer)
    {
        AfxMessageBox(_T("rpc���ʼ��ʧ��!��������޷���������"));
        return FALSE;
    }

    g_RPCServer->register_handler("2Box.NewProcess", &RpcServer::OnNewProcess);
    g_RPCServer->register_handler("2Box.NewWnd", &RpcServer::OnNewWnd);
    g_RPCServer->register_handler("2Box.EmbedWnd", &RpcServer::OnEmbedWnd);
    g_RPCServer->register_handler("2Box.ActiveWnd", &RpcServer::OnActiveWnd);
    g_RPCServer->register_handler("2Box.GetAllWnd", &RpcServer::OnGetAllWnd);

    // 	if (FALSE == InitResource())
    // 	{
    // 		AfxMessageBox(_T("��Դ�ļ���"));
    // 		return FALSE;
    // 	}

    g_RPCServer->set_conn_timeout_callback([=](int64_t) {
        RpcServer::AcptEvent(0, 1, 0);
    });

    g_RPCServer->async_run();

    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    // ����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
    // �����ؼ��ࡣ
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey(_T("2Box"));
    LoadStdProfileSettings(4);

    // ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
    // ����Ȼ��������ΪӦ�ó���������ڶ���
    CMainFrame* pFrame = new CMainFrame;
    if (!pFrame)
        return FALSE;
    m_pMainWnd = pFrame;
    // ���������ؿ�ܼ�����Դ
    pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

    // Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
    pFrame->ShowWindow(SW_SHOW);
    pFrame->UpdateWindow();
    // �������к�׺ʱ�ŵ��� DragAcceptFiles
    //  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
    return TRUE;
}

CDocument* CMy2BoxApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
    // TODO: �ڴ����ר�ô����/����û���
    CFileFind finder;
    if (finder.FindFile(lpszFileName))
    {
        CString s(lpszFileName);
        StartLoadFileThread(s);
        return (CDocument*)1;
    }
    else
    {
        AfxMessageBox(_T("�ļ������ڣ�"));
        return NULL;
    }

    // return CWinApp::OpenDocumentFile(lpszFileName);
}

BOOL CMy2BoxApp::OnIdle(LONG lCount)
{
    // TODO: �ڴ����ר�ô����/����û���

    if (CWinApp::OnIdle(lCount))
    {
        return TRUE;
    }

    if (lCount == 2)
    {
        ((CMainFrame*)GetMainWnd())->UpdateViewWhenIdle();
        return FALSE;
    }

    return TRUE;
}

// CMy2BoxApp ��Ϣ�������

void CMy2BoxApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

void CMy2BoxApp::OnFileOpen()
{
    // TODO: �ڴ���������������
    CString strFile;

    CFileDialog dlgFile(TRUE);

    if (IDOK == dlgFile.DoModal())
    {
        strFile = dlgFile.GetPathName();
        AddToRecentFileList(strFile);
        OpenDocumentFile(strFile);
    }
}

void CMy2BoxApp::PreDestroyMainWnd()
{
    g_RPCServer.reset();
}

UINT CMy2BoxApp::GetSettingsInt(LPCTSTR lpszEntry, int nDefault)
{
    return GetProfileInt(_T("Settings"), lpszEntry, nDefault);
}

CString CMy2BoxApp::GetSettingsString(LPCTSTR lpszEntry, LPCTSTR lpszDefault /*= NULL*/)
{
    return GetProfileString(_T("Settings"), lpszEntry, lpszDefault);
}

BOOL CMy2BoxApp::WriteSettingsInt(LPCTSTR lpszEntry, int nValue)
{
    return WriteProfileInt(_T("Settings"), lpszEntry, nValue);
}

BOOL CMy2BoxApp::WriteSettingsString(LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
    return WriteProfileString(_T("Settings"), lpszEntry, lpszValue);
}

BOOL CMy2BoxApp::GetNewEnv(std::wstring& strOut) const
{
    _timeb timebuffer;
    _ftime_s(&timebuffer);
    unsigned long long llData = timebuffer.time * 1000 + timebuffer.millitm;
    wchar_t szLLData[21] = {0};

    swprintf_s(szLLData, 21, L"%020I64u", llData);
    strOut = m_strEnv;
    strOut.replace(m_nLLDataPos, 20, szLLData, 20);
    return TRUE;
}

BOOL CMy2BoxApp::InitEnv()
{
    LPWCH pEnv = GetEnvironmentStringsW();
    if (NULL == pEnv)
    {
        return FALSE;
    }

    BOOL bProcRet = ProcUnknownEnvStringsW(pEnv, (const wchar_t*)m_strSelfPathW, 0, 0);
    FreeEnvironmentStringsW(pEnv);
    return bProcRet;
}

BOOL CMy2BoxApp::ProcUnknownEnvStringsW(const void* pszzEnv, const std::wstring& loaderPath, unsigned long long llData, unsigned long lEventNo)
{
    const wchar_t* toParse = (const wchar_t*)pszzEnv;
    if (((const char*)toParse)[1])
    {
        // ����ʲ���unicode�ַ�;
        return FALSE;
    }

    size_t nSize = 0;
    while (1)
    {
        if (0 == toParse[nSize++] && 0 == toParse[nSize++])
        {
            break;
        }
    }
    m_strEnv.append(toParse, nSize - 1);

    m_strEnv += L"BoxMonLibPath=";
    m_strEnv += loaderPath;
    m_strEnv += L'\0';

    wchar_t szName[21] = {0};
    swprintf_s(szName, 21, L"%020I64u", llData);

    m_strEnv += L"BoxLLData=";
    m_nLLDataPos = m_strEnv.size();
    m_strEnv += szName;
    m_strEnv += L'\0';

    m_strEnv += L'\0';
    return TRUE;
}

BOOL CMy2BoxApp::CheckInstance()
{
    if (FALSE == WaitNamedPipe(_T("\\\\.\\pipe\\2Box_{BE6D2570-E5E5-40f0-9A53-CBDDE9C422BD}"), 1))
    {
        return FALSE;
    }

    rpc_client client("127.0.0.1", 10001);
    if (client.connect())
    {
        client.call("2Box.ActiveWnd");
    }
    return TRUE;
}

BOOL CMy2BoxApp::StartLoadFileThread(CString& FileName)
{
    CString* pstrName = new CString(FileName);
    std::thread thr(&NewFileThread, (void*)pstrName);
    thr.detach();
    // if (0 != SRPC_ThreadPoolWork(NULL, &NewFileThread, pstrName))
    // {
    //     delete pstrName;
    //     AfxMessageBox(_T("��������ʧ��"));
    //     return FALSE;
    // }

    return TRUE;
}

BOOL CMy2BoxApp::InitSelfPath()
{
    wchar_t szPathW[MAX_PATH + 1] = {0};
    char szPathA[MAX_PATH + 1] = {0};

    if (0 == GetModuleFileNameW(NULL, szPathW, MAX_PATH) || 0 == GetModuleFileNameA(NULL, szPathA, MAX_PATH))
    {
        return FALSE;
    }

    m_strSelfPathW = szPathW;
    m_strSelfPathW = m_strSelfPathW.Left(m_strSelfPathW.ReverseFind('\\') + 1);

    m_strSelfPathA = szPathA;
    m_strSelfPathA = m_strSelfPathA.Left(m_strSelfPathA.ReverseFind('\\') + 1);
    return TRUE;
}

// BOOL CMy2BoxApp::InitResource()
// {
// 	wchar_t szPathW[MAX_PATH + 2] = {0};
// 	swprintf_s(szPathW,MAX_PATH,L"%s%s",(const wchar_t*)m_strSelfPathW,L"2BoxFileSystem");
//
// 	SHFILEOPSTRUCTW Op = {0};
// 	Op.wFunc = FO_DELETE;
// 	Op.pFrom = szPathW;
// 	Op.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
//
// 	SHFileOperationW(&Op);
//
// 	CreateDirectoryW( szPathW,NULL);
//
// // 	if (FALSE == CreateResource(MAKEINTRESOURCE(IDR_PE_HELPER),m_strSelfPathW + _T("2Box_x64_helper.dll"))
// // 		|| FALSE == CreateResource(MAKEINTRESOURCE(IDR_PE_MONITOR32),m_strSelfPathW + _T("2BoxMonitor32.dll"))
// // 		|| FALSE == CreateResource(MAKEINTRESOURCE(IDR_PE_MONITOR64),m_strSelfPathW + _T("2BoxMonitor64.dll")))
// // 	{
// // 		return FALSE;
// // 	}
//
// 	return TRUE;
// }

// BOOL CMy2BoxApp::CreateResource(LPCTSTR lpResName,const CString& FilePath)
// {
// 	BOOL bRetVal = FALSE;
// 	HRSRC hRes = NULL;
// 	HGLOBAL hResourceLoaded = NULL;
// 	HANDLE hFile = INVALID_HANDLE_VALUE;
//
// 	do
// 	{
// 		hRes = FindResource(AfxGetInstanceHandle(),lpResName,_T("PE"));
// 		if (NULL == hRes)
// 		{
// 			break;
// 		}
//
// 		hResourceLoaded = LoadResource(AfxGetInstanceHandle(), hRes);
// 		if (NULL == hResourceLoaded)
// 		{
// 			break;
// 		}
//
// 		// �õ���Դ�ļ���С ;
// 		DWORD dwSize = SizeofResource(AfxGetInstanceHandle(), hRes);
// 		// ������Դ�Եõ������ڴ��еĵ�ַ ;
// 		LPVOID sRawPE = (LPVOID)LockResource(hResourceLoaded);
//
// 		hFile = CreateFile(FilePath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
// 		if (INVALID_HANDLE_VALUE == hFile)
// 		{
// 			break;
// 		}
//
// 		DWORD  dwWrited = 0;
// 		if (FALSE == WriteFile(hFile,sRawPE,dwSize,&dwWrited,NULL)
// 			|| dwWrited != dwSize)
// 		{
// 			break;
// 		}
//
// 		bRetVal = TRUE;
//
// 	} while (0);
//
// 	if (hFile != INVALID_HANDLE_VALUE)
// 	{
// 		CloseHandle(hFile);
// 	}
//
// 	if (hResourceLoaded)
// 	{
// 		FreeResource(hResourceLoaded);
// 	}
// 	return bRetVal;
// }
