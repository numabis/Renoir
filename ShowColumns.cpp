// ShowColumns.cpp: archivo de implementación
//

#include "stdafx.h"
#include "renoir.h"
#include "ShowColumns.h"
#include "afxdialogex.h"
#include "omdbClientDlg.h"

IMPLEMENT_DYNAMIC(ShowColumns, CDialogEx)

ShowColumns::ShowColumns() : CDialogEx(ShowColumns::IDD)
{
}

ShowColumns::~ShowColumns()
{
}

void ShowColumns::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void ShowColumns::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDC_CHECK_FSID)
    {
        int a = 0;
    }
}

BOOL ShowColumns::OnInitDialog()
{
    short IDC_CHECKS[DataBase::NUM_COLUMNS] = { IDC_CHECK_FSID , IDC_CHECK_PATH,IDC_CHECK_FILENAME, IDC_CHECK_FILEEXISTS, IDC_CHECK_DATE, IDC_CHECK_SIZE,
        IDC_CHECK_IMDBID, IDC_CHECK_ISSERIE, IDC_CHECK_ISANIM, IDC_CHECK_ISDOC, IDC_CHECK_ISSHORT , IDC_CHECK_RATING };

    for (int check = 0; check < DataBase::NUM_COLUMNS; check++)
    {
        checkBox[check] = (CButton *)GetDlgItem(IDC_CHECKS[check]);
        checkBox[check]->SetCheck(showCols[check]);
    }
    return true;
}

BEGIN_MESSAGE_MAP(ShowColumns, CDialogEx)
    ON_BN_CLICKED(IDC_CHECK_FSID, &ShowColumns::OnBnClickedCheckFsid)
    ON_BN_CLICKED(IDC_CHECK_PATH, &ShowColumns::OnBnClickedCheckPath)
    ON_BN_CLICKED(IDC_CHECK_FILENAME, &ShowColumns::OnBnClickedCheckFilename)
    ON_BN_CLICKED(IDC_CHECK_FILEEXISTS, &ShowColumns::OnBnClickedCheckFileexists)
    ON_BN_CLICKED(IDC_CHECK_DATE, &ShowColumns::OnBnClickedCheckDate)
    ON_BN_CLICKED(IDC_CHECK_SIZE, &ShowColumns::OnBnClickedCheckSize)
    ON_BN_CLICKED(IDC_CHECK_IMDBID, &ShowColumns::OnBnClickedCheckImdbid)
    ON_BN_CLICKED(IDC_CHECK_RATING, &ShowColumns::OnBnClickedCheckRating)
    ON_BN_CLICKED(IDC_CHECK_ISSERIE, &ShowColumns::OnBnClickedCheckIsserie)
    ON_BN_CLICKED(IDC_CHECK_ISANIM, &ShowColumns::OnBnClickedCheckIsanim)
    ON_BN_CLICKED(IDC_CHECK_ISDOC, &ShowColumns::OnBnClickedCheckIsdoc)
    ON_BN_CLICKED(IDC_CHECK_ISSHORT, &ShowColumns::OnBnClickedCheckIsshort)
END_MESSAGE_MAP()

void ShowColumns::setParams(bool *_cols, CListCtrl * _list, size_t * _colSizes)
{
    showCols = _cols;
    lstMovies = _list;
    columnSizes = _colSizes;
}
void ShowColumns::setCol(short _col)
{
    showCols[_col] = checkBox[_col]->GetCheck() == 1;
    if (showCols[_col] == false)
        lstMovies->SetColumnWidth(_col, 0);
    else
        lstMovies->SetColumnWidth(_col, columnSizes[_col]);
}

void ShowColumns::OnBnClickedCheckFsid()
{
    setCol(DataBase::COL_FSID);
}
void ShowColumns::OnBnClickedCheckPath()
{
    setCol(DataBase::COL_PATH);
}
void ShowColumns::OnBnClickedCheckFilename()
{
    setCol(DataBase::COL_FILENAME);
}
void ShowColumns::OnBnClickedCheckFileexists()
{
    setCol(DataBase::COL_PHYSICAL);
}
void ShowColumns::OnBnClickedCheckDate()
{
    setCol(DataBase::COL_DATE);
}
void ShowColumns::OnBnClickedCheckSize()
{
    setCol(DataBase::COL_SIZE);
}
void ShowColumns::OnBnClickedCheckImdbid()
{
    setCol(DataBase::COL_IMDBID);
}
void ShowColumns::OnBnClickedCheckRating()
{
    setCol(DataBase::COL_IMDBRATING);
}
void ShowColumns::OnBnClickedCheckIsserie()
{
    setCol(DataBase::COL_SERIE);
}
void ShowColumns::OnBnClickedCheckIsanim()
{
    setCol(DataBase::COL_ANIM);
}
void ShowColumns::OnBnClickedCheckIsdoc()
{
    setCol(DataBase::COL_DOC);
}
void ShowColumns::OnBnClickedCheckIsshort()
{
    setCol(DataBase::COL_SHORT);
}
