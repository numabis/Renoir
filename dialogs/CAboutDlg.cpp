#include "stdafx.h"
#include "afxdialogex.h"

#include "dialogs\CAboutDlg.h"
#include "definitions.h"
#include "renoir.h"
#include "utils\Convert.h"
#include "utils\exLog.h"
#include "utils\util.h"
#include "version.h"

#include <mmsystem.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
