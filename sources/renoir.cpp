
// renoir.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "renoir.h"
#include "variables.h"
#include "convert.h"
#include "Util.h"
#include "omdbClientDlg.h"


BEGIN_MESSAGE_MAP(renoirApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

renoirApp::renoirApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

renoirApp theApp;

BOOL renoirApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;

	SetRegistryKey(_T("NumaCorp"));

	omdbClientDlg dlg;

    m_pMainWnd = &dlg;

    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}

	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
    
	return FALSE;
}
