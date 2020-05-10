
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


// Construcción de renoirApp

renoirApp::renoirApp()
{
	// admite Administrador de reinicio
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	// TODO: agregar aquí el código de construcción,
	// Colocar toda la inicialización importante en InitInstance
}

bool renoirApp::initalChecks()
{
//    FILE* file = NULL;
//    char configPath[PATHSIZE];
//    //char configLocalPath[PATHSIZE];
//
//#if defined(_DEBUG) || defined(_DEMO)
//    sprintf(configPath, "%s", DBGCONFIGFILE);
//#else
//    sprintf(configPath, "%s", CONFIGFILE);
//#endif
//
//    //BUTIL::exLog::getInstance().setLogLevel(BUTIL::exLog::exLOG_XML);
//    wchar_t * cmdLine = GetCommandLine();
//
//    // Configuración.
//    file = fopen(configPath, "r");
//    if (file != NULL)
//        fclose(file);
//    else
//    {
//        std::ostringstream msg;
//        msg << "Missing " << configPath;
//        MBOX(msg.str(), "Error", MB_ICONERROR | MB_OK);
//        return false;
//    }

    return true;
}

// El único objeto renoirApp

renoirApp theApp;


// Inicialización de renoirApp

BOOL renoirApp::InitInstance()
{
    if (initalChecks() == false)
        return false;
	// Windows XP requiere InitCommonControlsEx() si un manifiesto de
	// aplicación especifica el uso de ComCtl32.dll versión 6 o posterior para habilitar
	// estilos visuales. De lo contrario, se generará un error al crear ventanas.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Establecer para incluir todas las clases de control comunes que desee utilizar
	// en la aplicación.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Crear el administrador de shell, en caso de que el cuadro de diálogo contenga
	// alguna vista de árbol de shell o controles de vista de lista de shell.
	CShellManager *pShellManager = new CShellManager;

	// Inicialización estándar
	// Si no utiliza estas características y desea reducir el tamaño
	// del archivo ejecutable final, debe quitar
	// las rutinas de inicialización específicas que no necesite
	// Cambie la clave del Registro en la que se almacena la configuración
	// TODO: debe modificar esta cadena para que contenga información correcta
	// como el nombre de su compañía u organización
	SetRegistryKey(_T("NumaCorp"));

	omdbClientDlg dlg;

    m_pMainWnd = &dlg;

    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Aceptar
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Cancelar
	}

	// Eliminar el administrador de shell creado anteriormente.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Dado que el cuadro de diálogo se ha cerrado, devolver FALSE para salir
	//  de la aplicación en vez de iniciar el suministro de mensajes de dicha aplicación.
	return FALSE;
}
