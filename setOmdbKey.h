#pragma once


// Cuadro de diálogo de setOmdbKey

class setOmdbKey : public CDialogEx
{
	DECLARE_DYNAMIC(setOmdbKey)

public:
    setOmdbKey();
	//setOmdbKey(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~setOmdbKey();

// Datos del cuadro de diálogo
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETAPIKEY };
//#endif
    enum dialogLines { LINE1, LINE2, LINE3, LINEMAX };
    short idcLines[LINEMAX] = { IDC_STATIC1, IDC_STATIC2, IDC_STATIC3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

    void setTexts(std::string[LINEMAX]);
    void setValue(std::string *);
    void setURL(const char *_url);

private:


    CLinkCtrl *link;
    CEdit *edit;
    std::string* value;
    const char *url;
    CString m_strDlgCaption;
    CStatic *dialogStatics[LINEMAX];
    std::string *texts;

public:
    afx_msg void OnNMClickSyslinkOmdb(NMHDR *pNMHDR, LRESULT *pResult);
};
