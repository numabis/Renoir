// setOmdbKey.cpp: archivo de implementación
//

#include "stdafx.h"
#include "renoir.h"
#include "setOmdbKey.h"
#include "afxdialogex.h"
#include "Convert.h"

// Cuadro de diálogo de setOmdbKey

IMPLEMENT_DYNAMIC(setOmdbKey, CDialogEx)

BOOL setOmdbKey::OnInitDialog()
{
    CString cs_loaded;
    if (texts != NULL)
    {
        for (short line = 0; line < LINEMAX; line++)
        {
            dialogStatics[line] = (CStatic *)GetDlgItem(idcLines[line]);
            cs_loaded.Format(BUTIL::Convert::string2wstring(texts[line]).c_str());
            dialogStatics[line]->SetWindowText(cs_loaded);
            dialogStatics[line]->ShowWindow(SW_SHOW);
        }
    }
    
    link = (CLinkCtrl*)GetDlgItem(IDC_SYSLINK_OMDB);
    if (wurl != nullptr)
    {
        //"<a>https://www.omdbapi.com/apikey.aspx</a>"
        link->SetWindowText(m_strDlgCaption);
        link->ShowWindow(SW_SHOW);
    }
    else
        link->ShowWindow(SW_HIDE);

    edit = (CEdit *)GetDlgItem(IDC_EDIT_APIKEY);
    CString tmp;
    tmp.Format(L"%s", BUTIL::Convert::string2wstring(*value).c_str());
    edit->SetWindowTextW(tmp);
    return true;
}

setOmdbKey::setOmdbKey() : CDialogEx(setOmdbKey::IDD)
{
    
}

setOmdbKey::~setOmdbKey()
{
}

void setOmdbKey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(setOmdbKey, CDialogEx)
    ON_BN_CLICKED(IDOK, &setOmdbKey::OnBnClickedOk)
    ON_NOTIFY(NM_CLICK, IDC_SYSLINK_OMDB, &setOmdbKey::OnNMClickSyslinkOmdb)
END_MESSAGE_MAP()

void setOmdbKey::OnBnClickedOk()
{
    CString cstring;
    edit->GetWindowText(cstring);
    CT2CA psz(cstring);
    std::string editValue(psz);
    *value = editValue;
    CDialog::OnOK();
}

void setOmdbKey::setTexts(std::string _l[LINEMAX])
{
    texts = _l;
}

void setOmdbKey::setValue(std::string *_value)
{
    value = _value;
}

void setOmdbKey::setURL(wchar_t *_url)
{
    wurl = _url;
    m_strDlgCaption.Format(L"<a>%s</a>", _url);
}

void setOmdbKey::OnNMClickSyslinkOmdb(NMHDR *pNMHDR, LRESULT *pResult)
{ //IDC_SYSLINK_OMDB
    ShellExecute(
        NULL, // or your can use GetSafeHwnd()
        L"open",
        //        L"https://www.omdbapi.com/apikey.aspx",
        wurl,
        NULL,
        NULL,
        SW_SHOWNORMAL
    );
    // TODO: Agregue aquí su código de controlador de notificación de control
    *pResult = 0;
}
