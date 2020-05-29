#pragma once
#include <vector>

// Cuadro de diálogo de debugWindow

class debugWindow : public CDialogEx
{
	DECLARE_DYNAMIC(debugWindow)

public:
	debugWindow(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~debugWindow();

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DBG };
#endif
    //void writeToConsole(std::string);
    size_t writeToConsole(const char*, size_t pos = 0);
    size_t writeToConsole(std::string, size_t pos = 0);


private:
    //std::vector<std::string> logs;
    std::vector<CString> Clogs;
    CEdit * getConsole(void);
    CEdit* console;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
    virtual BOOL OnInitDialog(void);
    virtual void OnCancel(void);

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
};
