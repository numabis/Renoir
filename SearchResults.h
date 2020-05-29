#pragma once

#include <vector>
#include "variables.h"
#include "movieFile.h"
#include "PictureControl\PictureCtrl.h"

// Cuadro de diálogo de SearchResults

class SearchResults : public CDialogEx
{
	DECLARE_DYNAMIC(SearchResults)

public:
	SearchResults(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~SearchResults();

    void setFile(MovieFile *_file);

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCH_DIALOG };
#endif

protected:
    HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
    virtual BOOL OnInitDialog();
    virtual void OnCancel(void);
    DECLARE_MESSAGE_MAP()

private:
    enum SearchColumns { COL_TITLE, COL_YEAR, COL_IMDBID, COL_TYPE, NUM_COLUMNS };
    enum movieEnum { TYPE_MOVIE, TYPE_SERIE, TYPE_EPISODE, TYPE_MAX } ;
    std::string movieType[TYPE_MAX] = { "movie", "series", "episode" };
    wchar_t* columNames[NUM_COLUMNS] = { L"Title", L"Year", L"ImdbId", L"Type" };
    size_t columnMinSizes[NUM_COLUMNS] = { 400,     40,    80,      50 };

    CListCtrl *lstSearch;
    CEdit *CEditPath, *CEditSearch, *CEditYear, *CEditLimit;
    CButton * Csearch, *Cweb;

    CStatic * CtotalResults;

    CComboBox *CmovieType; // IDC_COMBO_TYPE
    std::string movieTypeSelected;

    MovieFile *file;

    CPictureCtrl m_picCtrl;
    std::string posterURL;
    std::string tmpImdbId;
    int limit, totalResults;

    void SearchResults::populateList();
    void getUrlFromSelection();
    //void openImdbWeb(void);

public:
    afx_msg void OnBnClickedSearchOk();
    afx_msg void OnLvnItemchangedMovieList(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnBnClickedSearchCancel();
    afx_msg void OnBnClickedSearchRequest();
    afx_msg void OnEnChangeEditFile();
    afx_msg void OnEnChangeEditYear();
    afx_msg void OnBnClickedOpenWeb();
    afx_msg void OnEnChangeEditLimit();
    afx_msg void OnCbnSelchangeComboType();
};
