// SearchResults.cpp: archivo de implementación
//

#include "stdafx.h"
#include "renoir.h"
#include "SearchResults.h"
#include "afxdialogex.h"

#include "Convert.h"
#include "Movie.h"
#include "configManager.h"

#define _REFRESH_UI_EVENT_	1000		// Timer de actualización del UI
// Cuadro de diálogo de SearchResults

IMPLEMENT_DYNAMIC(SearchResults, CDialogEx)

SearchResults::SearchResults(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SEARCH_DIALOG, pParent)
{
//    file = _file;
}

SearchResults::~SearchResults()
{
}


BEGIN_MESSAGE_MAP(SearchResults, CDialogEx)
    ON_BN_CLICKED(ID_SEARCH_OK, &SearchResults::OnBnClickedSearchOk)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_SEARCH_LIST, &SearchResults::OnLvnItemchangedMovieList)
    ON_BN_CLICKED(ID_SEARCH_CANCEL, &SearchResults::OnBnClickedSearchCancel)
    ON_BN_CLICKED(ID_SEARCH_REQUEST, &SearchResults::OnBnClickedSearchRequest)
    ON_EN_CHANGE(IDC_EDIT_FILE, &SearchResults::OnEnChangeEditFile)
    ON_EN_CHANGE(IDC_EDIT_YEAR, &SearchResults::OnEnChangeEditYear)
    ON_BN_CLICKED(ID_OPEN_WEB, &SearchResults::OnBnClickedOpenWeb)
    ON_EN_CHANGE(IDC_EDIT_LIMIT, &SearchResults::OnEnChangeEditLimit)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &SearchResults::OnCbnSelchangeComboType)
END_MESSAGE_MAP()

void SearchResults::OnCancel()
{
    file->imdbId = "?";
    file->getMovie()->setPoster(std::string());
    file->clearSearchResults();
    CDialog::OnCancel();
}

void SearchResults::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PICTURE, m_picCtrl);
}

BOOL SearchResults::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    srand((unsigned int)time(NULL));
    UpdateData(FALSE);
    SetTimer(_REFRESH_UI_EVENT_, 1, NULL);

    CEditPath = (CEdit *)GetDlgItem(IDC_EDIT_FOLDER);
    CEditSearch = (CEdit *)GetDlgItem(IDC_EDIT_FILE);
    CEditYear = (CEdit *)GetDlgItem(IDC_EDIT_YEAR);
    CEditLimit = (CEdit *)GetDlgItem(IDC_EDIT_LIMIT);

    Csearch = (CButton *)GetDlgItem(ID_SEARCH_REQUEST);
    Cweb = (CButton *)GetDlgItem(ID_OPEN_WEB);
    lstSearch = (CListCtrl *)GetDlgItem(IDC_SEARCH_LIST);

    CtotalResults = (CStatic*)GetDlgItem(IDC_TOTAL);

    CmovieType = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE); 

    DWORD style = lstSearch->GetExtendedStyle();
    style |= LVS_EX_DOUBLEBUFFER | LVS_EX_BORDERSELECT | LVS_EX_FULLROWSELECT;
    lstSearch->SetExtendedStyle(style);

    CEditPath->SetWindowTextW(CString(file->getFullPath().c_str()));
    CEditSearch->SetWindowTextW(CString(file->title.c_str()));
    if (file->year != 0)
        CEditYear->SetWindowTextW(CString(std::to_string(file->year).c_str()));

    for (int col = 0; col<NUM_COLUMNS; col++)
        lstSearch->InsertColumn(col, (LPCTSTR)columNames[col], LVCFMT_LEFT, columnMinSizes[col]);

    for (int i=0;i<TYPE_MAX;i++)
    {
        STRING2CSTRING(movieType[i], cstr)
        CmovieType->AddString(cstr);
    }
    CmovieType->SetCurSel(0);

    OnCbnSelchangeComboType();

    if (file->getOmdbSearchResults()->size() == 0)
        file->omdbSearch();

    //std::vector<omdbSearchValues>::iterator it;

    populateList();

    limit = file->omdbGetLimit();
    CEditLimit->SetWindowTextW(CString(std::to_string(limit).c_str()));

    totalResults = file->omdbGetTotalResults();
    CString txt;
    txt.Format(L"/ %d of total results", totalResults);
    CtotalResults->SetWindowTextW(txt);

    return true;
}

void SearchResults::populateList()
{
    std::vector<omdbSearchValues>::iterator it;
    lstSearch->DeleteAllItems();
    for (it = file->getOmdbSearchResults()->end(); it != file->getOmdbSearchResults()->begin(); )
    {
        it--;
        CString strItem;
        strItem.Format(_T("%d"), it->values[COL_TITLE]);

        STRING2CSTRING(it->values[COL_TITLE], title)

        lstSearch->InsertItem(0, title);
        lstSearch->SetItemText(0, COL_YEAR, CString(it->values[COL_YEAR].c_str()));
        lstSearch->SetItemText(0, COL_IMDBID, CString(it->values[COL_IMDBID].c_str()));
        lstSearch->SetItemText(0, COL_TYPE, CString(it->values[COL_TYPE].c_str()));
    }
}

void SearchResults::setFile(MovieFile* _file)
{
    file = _file;
}

void SearchResults::OnBnClickedSearchOk()
{
    CDialog::OnOK();
}

void SearchResults::OnLvnItemchangedMovieList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    ASSERT(pNMLV);
    //globalState = globalState;
    if (pNMLV->uChanged & LVIF_STATE)
    {
        if (pNMLV->uNewState & LVIS_SELECTED)
        {
            getUrlFromSelection();
            //Movie moviePoster;
            //file.setPoster(posterURL);
            std::string jpgPath;
            if (file->getMovie()->getJpeg())
            {
                //std::string path = file->getMovie()->getJpegPath().c_str();
                jpgPath = file->getMovie()->getJpegPath();
            }
            else {
                //if (movie.getPoster().empty() == false)
                jpgPath = GETCM.getConfigStr(CONF_APP_JPGNOTFOUND);
            }
            m_picCtrl.Load(CString(jpgPath.c_str()));
            m_picCtrl.ShowWindow(SW_SHOW);
        }
    }

    *pResult = 0;
}

void SearchResults::getUrlFromSelection()
{
    POSITION pos = lstSearch->GetFirstSelectedItemPosition();
    short lstPosition =  lstSearch->GetNextSelectedItem(pos);
    CString imdbIdSelected = lstSearch->GetItemText(lstPosition, COL_IMDBID);

    std::vector<omdbSearchValues>::iterator it;
    //posterURL = "";
    file->getMovie()->setPoster(std::string());
    for (it = file->getOmdbSearchResults()->begin(); it != file->getOmdbSearchResults()->end(); it++)
    {
        CString imdbId = CString(it->values[XMLS_IMDBID].c_str());
        if (imdbId.Compare(imdbIdSelected) == 0)
        {
            file->getMovie()->setPoster(it->values[XMLS_POSTER]);
            file->imdbId = it->values[XMLS_IMDBID];
            //posterURL = it->values[XMLS_POSTER];
            //tmpImdbId = it->values[XMLS_IMDBID];
            break;
        }
    }
}

void SearchResults::OnBnClickedSearchCancel()
{
    file->imdbId = "?";
    file->getMovie()->setPoster(std::string());
    CDialog::OnCancel();
}

void SearchResults::OnBnClickedSearchRequest()
{
    CString CSLimit;
    CEditLimit->GetWindowText(CSLimit);
    int tmpLimit = _wtoi(CSLimit);
    if (tmpLimit > 0 && tmpLimit < 10)
    {
        tmpLimit = 10;
    }
    limit = tmpLimit;
    CEditLimit->SetWindowTextW(CString(std::to_string(limit).c_str()));
    file->omdbSetLimit(limit);

    totalResults = file->omdbSearch();
    CString txt;
    txt.Format(L"/ %d of total results", totalResults);
    CtotalResults->SetWindowTextW(txt);
    populateList();
}

void SearchResults::OnEnChangeEditFile()
{
    CString CSTitle;
    CEditSearch->GetWindowText(CSTitle);
    std::wstring w_title((LPCTSTR)CSTitle);
    file->title = WS2S(w_title);
}

void SearchResults::OnEnChangeEditYear()
{
    CString CSYear;
    CEditYear->GetWindowText(CSYear);
    file->year = _wtoi(CSYear);
}

void SearchResults::OnBnClickedOpenWeb()
{
    std::string url;
    std::string title = file->title;
    BUTIL::Convert::string2url(&title);

    url = GETCM.getConfigStr(CONF_IMDB_URL) + GETCM.getConfigStr(CONF_IMDB_FIND) + title;

    if (file->year)
        url  += "+" + std::to_string(file->year);

    //wchar_t *wurl = C2WC(url.c_str());

    "https://www.imdb.com/find?q=A+Space+Odyssey";

    ShellExecuteA(
        NULL, // or your can use GetSafeHwnd()
        "open",
        //        L"https://www.omdbapi.com/apikey.aspx",
        url.c_str(),
        NULL,
        NULL,
        SW_SHOWNORMAL
    );
}

void SearchResults::OnEnChangeEditLimit()
{
    //CString CSLimit;
    //CEditLimit->GetWindowText(CSLimit);
    //int tmpLimit = _wtoi(CSLimit);
    //if (tmpLimit >= 10)
    //{
    //    limit = tmpLimit;
    //    file->omdbSetLimit(limit);
    //    //CEditLimit->SetWindowTextW(CString(std::to_string(limit).c_str()));
    //}
}

void SearchResults::OnCbnSelchangeComboType()
{
    CString c_text;
    std::string text;

    int cursor = CmovieType->GetCurSel();
    if (cursor < 0)
        return ;
    CmovieType->GetLBText(cursor, c_text);

    std::wstring w_c_text((LPCTSTR)c_text);
    movieTypeSelected = WS2S(w_c_text);

    GETCM.setConfigValue(CONF_OMDB_MOVIE_TYPE, movieTypeSelected);
}
