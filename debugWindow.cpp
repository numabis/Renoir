// debugWindow.cpp: archivo de implementación
//

#include "stdafx.h"
#include "renoir.h"
#include "debugWindow.h"
#include "afxdialogex.h"
#include "sources\definitions.h"

#define MAXLOG  1000

// Cuadro de diálogo de debugWindow

IMPLEMENT_DYNAMIC(debugWindow, CDialogEx)

debugWindow::debugWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

debugWindow::~debugWindow()
{
}

void debugWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL debugWindow::OnInitDialog()
{
    console = (CEdit *)GetDlgItem(IDC_EDIT_LOG);
    return true;
}

BEGIN_MESSAGE_MAP(debugWindow, CDialogEx)
    ON_BN_CLICKED(IDOK, &debugWindow::OnBnClickedOk)
END_MESSAGE_MAP()


// Controladores de mensajes de debugWindow

//void debugWindow::writeToConsole(std::string _text)
size_t debugWindow::writeToConsole(const wchar_t *_text, size_t _pos)
{
    time_t ltime = time(NULL);
    struct tm tim;
    localtime_s(&tim, &ltime);
    CString newLog;
    size_t sz = Clogs.size();
    int pos = sz;
    if (_pos != 0)
        pos = _pos;

    newLog.Format(L"D%02d-%02d:%02d:%02d [%03d] %s\r\n", tim.tm_mday, tim.tm_hour, tim.tm_sec, tim.tm_min, pos, _text);
    //    std::string newLog = "" + _text + NEWLINE;
    CString logText;
    if (_pos != 0)
        Clogs.at(_pos) = newLog;
    else
    {
        Clogs.push_back(newLog);
        _pos = sz;
    }
    //logs.push_back(_text);
    sz -= 2;
    int maxRange = sz - MAXLOG;
    if (maxRange < 0) maxRange = 0;
    for (int i = sz; i >= maxRange; i--)
    {
        //logText.Append(CString(logs.at(i).c_str()));
        newLog.Append(Clogs.at(i));
    }
    console->SetWindowText(newLog);
    return _pos;
}
size_t debugWindow::writeToConsole(std::string _text, size_t _pos)
{
    time_t ltime = time(NULL);
    struct tm tim;
    localtime_s(&tim, &ltime);
    CString newLog;
    size_t sz = Clogs.size();
    int pos = sz;
    if (_pos != 0)
        pos = _pos;

    newLog.Format(L"D%02d-%02d:%02d:%02d [%03d] ", tim.tm_mday, tim.tm_hour, tim.tm_sec, tim.tm_min, pos);
    newLog.Append(CString(_text.c_str()));
    newLog.Append(L"\r\n");
    //    std::string newLog = "" + _text + NEWLINE;
    CString logText;
    if (_pos != 0)
        Clogs.at(_pos) = newLog;
    else
    {
        Clogs.push_back(newLog);
        _pos = sz;
    }
    //logs.push_back(_text);
    sz -= 2;
    int maxRange = sz - MAXLOG;
    if (maxRange < 0) maxRange = 0;
    for (int i = sz; i >= maxRange; i--)
    {
        //logText.Append(CString(logs.at(i).c_str()));
        newLog.Append(Clogs.at(i));
    }
    console->SetWindowText(newLog);
    return _pos;
}

CEdit * debugWindow::getConsole(void)
{
    return console;
}

void debugWindow::OnBnClickedOk()
{
    CDialogEx::OnOK();
}
