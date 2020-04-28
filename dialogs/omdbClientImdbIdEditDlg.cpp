//#ifdef NEW_IMDB_DIALOG
//#include "stdafx.h"
//#include "afxdialogex.h"
//
//#include "dialogs\omdbClientImdbIdEditDlg.h"
//#include "definitions.h"
//#include "omdbClient.h"
//#include "utils\Convert.h"
//#include "utils\exLog.h"
//#include "utils\util.h"
//#include "version.h"
//
//#include <mmsystem.h>
//#include <sstream>
//#include <iomanip>
//#include <string>
//#include <iostream>
//
//
//omdbClientImdbIdEdit::omdbClientImdbIdEdit(CWnd* pParent /*=NULL*/)
//    : CDialogEx(omdbClientImdbIdEdit::IDD, pParent)
//{
//    m_parent = pParent;
//}
//
//void omdbClientImdbIdEdit::DoDataExchange(CDataExchange* pDX)
//{
//    CDialogEx::DoDataExchange(pDX);
//}
//
//BEGIN_MESSAGE_MAP(omdbClientImdbIdEdit, CDialogEx)
//END_MESSAGE_MAP()
//
//BOOL omdbClientImdbIdEdit::OnInitDialog()
//{
//    CDialogEx::OnInitDialog();
//
//    OnInitDialogVars();
//    OnInitDialogValues();
//    OnInitDialogData();
//
//    return TRUE;
//}
//
//void omdbClientImdbIdEdit::OnInitDialogVars()
//{
//    editImdbId = (CEdit *)GetDlgItem(IDC_EDIT_IMDBID);
//}
//void omdbClientImdbIdEdit::OnInitDialogValues()
//{}
//void omdbClientImdbIdEdit::OnInitDialogData()
//{}
//
//void omdbClientImdbIdEdit::OnOK()
//{
//    if (UpdateData(true))
//    {
//        //(omdbClientApp*)m_parent->setEditedImdbid();
//        //GetParent()->ContinueModal();// >setEditedImdbid(imdbid);
//        //((CMainFrame*)m_parent)->setEditedImdbid();// = m_sss;
//        //DestroyWindow();
//    }
//}
//
//void omdbClientImdbIdEdit::OnCancel()
//{
//    //DestroyWindow();
//}
//
//
//void omdbClientImdbIdEdit::setImdb(std::string _str)
//{
//    imdbId = _str;
//    CString cs_imdb;
//    cs_imdb.Format(L"%s", BUTIL::Convert::string2wstring(imdbId).c_str());
//    editImdbId->SetWindowText(cs_imdb);
//}
//#endif