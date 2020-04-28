#pragma once
#include "database.h"

class ShowColumns : public CDialogEx
{
	DECLARE_DYNAMIC(ShowColumns)

public:
    ShowColumns();
	virtual ~ShowColumns();
    void setParams(bool *, CListCtrl *, size_t *);
    void setCol(short _col);
    size_t *columnSizes;
    bool *showCols;
    CListCtrl *lstMovies;
    CButton * checkBox[DataBase::NUM_COLUMNS];

//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COL };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCheckFsid();
    afx_msg void OnBnClickedCheckPath();
    afx_msg void OnBnClickedCheckFilename();
    afx_msg void OnBnClickedCheckFileexists();
    afx_msg void OnBnClickedCheckDate();
    afx_msg void OnBnClickedCheckSize();
    afx_msg void OnBnClickedCheckImdbid();
    afx_msg void OnBnClickedCheckRating();
    afx_msg void OnBnClickedCheckIsserie();
    afx_msg void OnBnClickedCheckIsanim();
    afx_msg void OnBnClickedCheckIsdoc();
    afx_msg void OnBnClickedCheckIsshort();
};
