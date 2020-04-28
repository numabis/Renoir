// omdbClient.h: .\Librerias\curl-7.67.0-win32-mingw\include
//
#include <string>
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"

class renoirApp : public CWinApp, CFrameWnd
{
public:
	renoirApp();
    bool initalChecks();
    std::string localPath;

public:
	virtual BOOL InitInstance();
    bool localConfig ;
	DECLARE_MESSAGE_MAP()
};

extern renoirApp theApp;