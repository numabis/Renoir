#pragma once


// Cuadro de di�logo de FolderManager

class FolderManager : public CDialogEx
{
	DECLARE_DYNAMIC(FolderManager)

public:
	FolderManager(CWnd* pParent = NULL);   // Constructor est�ndar
	virtual ~FolderManager();

// Datos del cuadro de di�logo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()
};
