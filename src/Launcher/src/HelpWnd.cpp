// HelpWnd.cpp : 实现文件
//

#include "pch.h"
#include "2Box.h"
#include "HelpWnd.h"

// CHelpWnd

IMPLEMENT_DYNAMIC(CHelpWnd, CWnd)

CHelpWnd::CHelpWnd()
    : m_bShowText(TRUE)
    , m_strNote(_T(""))
{
    m_font.CreatePointFont(11 * 9, _T("微软雅黑"));
    TCHAR pt = L' ';
    CString strTemp;

    m_strNote += _T("2Box\r\n免责声明:\r\n");
    m_strNote += _T("本软件完全免费仅供学习交流\r\n请勿用于任何商业用途否则后果自负!\r\n");
    m_strNote += _T("\r\n\r\n");
    m_strNote += _T("免费更新及下载地址见\"帮助\"--\"说明...\"\r\n\r\n");
    m_strNote += _T("使用说明:\r\n");
    strTemp = _T("1、关闭所有正在运行的*需要多开的程序*");
    while (strTemp.GetLength() < 26)
    {
        strTemp.AppendChar(pt);
    }
    strTemp += L"\r\n";
    m_strNote += strTemp;

    strTemp = _T("2、在*本程序*上方菜单栏点击\"文件\"-\"打开...\"");
    while (strTemp.GetLength() < 0)
    {
        strTemp.AppendChar(pt);
    }
    strTemp += L"\r\n";
    m_strNote += strTemp;

    strTemp = _T("3、在弹出的对话框中选择需要多开的程序");
    while (strTemp.GetLength() < 25)
    {
        strTemp.AppendChar(pt);
    }
    strTemp += L"\r\n";
    m_strNote += strTemp;

    strTemp = _T("4、重复第2-3步骤即可");
    while (strTemp.GetLength() < 45)
    {
        strTemp.AppendChar(pt);
    }
    strTemp += L"\r\n";
    m_strNote += strTemp;

    m_strNote += _T("\r\n******可选功能******\r\n");
    m_strNote += _T("在*打开的程序*的[标题栏]右键，或[任务栏图标](Shift +)右键\r\n");
    m_strNote += _T("在弹出的系统菜单中,可选择：\r\n");

    // m_strNote += _T("a、关闭所有可疑句柄(*仅*在重复第3步*多开失败时*使用！该功能可能导致程序不稳定！)\r\n");

    strTemp = _T("a、加入box管理(可以标签页方式管理多开的程序窗口，将标签页拖出即可复原)");
    while (strTemp.GetLength() < 53)
    {
        strTemp.AppendChar(pt);
    }
    strTemp += L"\r\n";
    m_strNote += strTemp;
}

CHelpWnd::~CHelpWnd() {}

BEGIN_MESSAGE_MAP(CHelpWnd, CWnd)
ON_WM_PAINT()
END_MESSAGE_MAP()

// CHelpWnd 消息处理程序

void CHelpWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if (m_bShowText)
    {
        CRect rc;
        GetClientRect(&rc);
        rc.DeflateRect(10, 10);
        HGDIOBJ old = dc.SelectObject(m_font);
        dc.DrawText(m_strNote, &rc, DT_CENTER | DT_VCENTER);
        dc.SelectObject(old);
    }
    CWnd::OnPaint();
}
