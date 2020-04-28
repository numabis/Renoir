#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream
#include "xml.h"
#include "log.h"
#include "renoir.h"

#define _REFRESH_UI_EVENT_	1000		// Timer de actualización del UI
#define _REFRESH_UI_TIMER	1000		// Tiempo de actualización del UI em ms

class CAboutDlg : public CDialogEx
{

public:

    CAboutDlg();
    enum { IDD = IDD_ABOUTBOX };

protected:

    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};
