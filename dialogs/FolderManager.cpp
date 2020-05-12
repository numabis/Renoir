// D:\My Projects\Renoir\dialogs\FolderManager.cpp: archivo de implementación
//

#include "stdafx.h"
#include "renoir.h"
#include "D:\My Projects\Renoir\dialogs\FolderManager.h"
#include "afxdialogex.h"


// Cuadro de diálogo de FolderManager

IMPLEMENT_DYNAMIC(FolderManager, CDialogEx)

FolderManager::FolderManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PATHS, pParent)
{

}

FolderManager::~FolderManager()
{
}

void FolderManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FolderManager, CDialogEx)
END_MESSAGE_MAP()


// Controladores de mensajes de FolderManager
