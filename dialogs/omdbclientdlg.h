#pragma once

#include "renoir.h"
#include "omdbClientInfo.h"
#include "debugWindow.h"
#include "FolderManager.h"
#include "configManager.h"
#include "database.h"

#include "fileSystem.h"
#include "manageXML.h"
#include "log.h"
#include "xml.h"
#include "PictureControl\PictureCtrl.h"
#include "menus.h"

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream

#define _REFRESH_UI_EVENT_	1000		// Timer de actualización del UI
#define _REFRESH_UI_TIMER	1000		// Tiempo de actualización del UI em ms

#define LOC_SYSMENU     0x0001
#define LOC_BARMENU     0x0010
#define LOC_CONMENU     0x0100
#define LOC_ALLMENU     LOC_SYSMENU | LOC_BARMENU | LOC_CONMENU

static DWORD WINAPI getOmdbAllThread(void* Param);


class omdbClientDlg : public CDialogEx/*, public CListCtrl*/
{

public:

    omdbClientDlg(CWnd* pParent = NULL);
    ~omdbClientDlg();
    enum { IDD = IDD_RENOIR_DIALOG };

    size_t columnSizes   [DataBase::NUM_COLUMNS];
//    char* columNames     [DataBase::NUM_COLUMNS] = { "FsId", "Path", "Name", "isDel", "Added", "Size", "ImdbID", "isSerie", "isAnim", "iDoc", "isShort", "Rating" };
    wchar_t* columNames  [DataBase::NUM_COLUMNS] = { L"FsId", L"Path", L"Name", L"isDel", L"Added", L"Size", L"ImdbID", L"isSerie", L"isAnim", L"iDoc", L"isShort", L"Rating" };
    size_t columnMinSizes[DataBase::NUM_COLUMNS] = {    10,     50,    100,      0,        50,     50,       80,        50,       50,     50,        50,      25  };
    size_t columnMaxSizes[DataBase::NUM_COLUMNS] = {    20,    400,    400,     20,       150,    120,      150,       120,      120,    120,       120,      90  };
    size_t columnMultipli[DataBase::NUM_COLUMNS] = {    12,     15,     15,     10,        12,     15,       12,        15,       15,     15,        15,      15  };
    bool columnsToSHow   [DataBase::NUM_COLUMNS] = { false,  false,   true,  false,      true,   true,     true,     false,    false,  false,     false,    true  };

    enum BTN_OMDB { BTN_OMDB_SINGLE, BTN_OMDB_SEARCH, BTN_OMDB_ALL, BTN_OMDB_ALL_STOP, CHK_OMDB_FORCE, CHK_OMDB_AUTO, RADIO_AUTO, RADIO_MANUAL, BTN_OMDB_MAX };
    enum BTN_XMLFILE { BTN_XMLFILE_READ, BTN_XMLFILE_WRITE, BTN_XMLFILE_MAX };
    enum BTN_ALL { BTN_PLAY, BTN_RESET_DIRECTORS, BTN_RESET_ACTORS, BTN_ALL_MAX };
    enum STATIC_ALL { STATIC_TEXT, STATIC_DESC, STATIC_KEY, STATIC_ALL_MAX };
    enum SUBMENUFILTER  { SUBMENUFILTER_DIRECTORS, SUBMENUFILTER_D1, SUBMENUFILTER_D2, SUBMENUFILTER_D3, SUBMENUFILTER_D4, SUBMENUFILTER_ACTORS, SUBMENUFILTER_A1, SUBMENUFILTER_A2, SUBMENUFILTER_A3, SUBMENUFILTER_A4, SUBMENUFILTER_A5, SUBMENUFILTER_MAX };

//    bool ftypeSave[FILTERS_MAX] = { true, true, true, true, true, true, true, true, true, true, true, true, true };

    short subMenuFilter[SUBMENUFILTER_MAX] = { IDC_BUTTON_FILTER_DIRECTORS, IDC_BUTTON_FILTER_DIRECTORS + 1, IDC_BUTTON_FILTER_DIRECTORS + 2, IDC_BUTTON_FILTER_DIRECTORS + 3, IDC_BUTTON_FILTER_DIRECTORS + 4, IDC_BUTTON_FILTER_ACTORS, IDC_BUTTON_FILTER_ACTORS + 1, IDC_BUTTON_FILTER_ACTORS + 2, IDC_BUTTON_FILTER_ACTORS + 3, IDC_BUTTON_FILTER_ACTORS + 4, IDC_BUTTON_FILTER_ACTORS + 5 };

    short idcOmdb[BTN_OMDB_MAX] = { IDC_BUTTON_OMDB_REQUEST, IDC_BUTTON_OMDB_SEARCH, IDC_BUTTON_OMDB_ALL, IDC_BUTTON_OMDB_ALL_STOP, IDC_CHECK_FORCE, IDC_CHECK_AUTOOMDB, IDC_RADIO_AUTO, IDC_RADIO_MANUAL/*, IDC_BUTTON_OMDB_SETKEY*/ };
    short idcXmlFile[BTN_XMLFILE_MAX] = { IDC_BUTTON_LOADXMLFILE, IDC_BUTTON_WRITEXMLFILE };
    short idcMovieInfo[INFO_MAX] = { IDC_EDIT_TITLE , IDC_EDIT_RUNTIME, IDC_EDIT_GENRES, IDC_EDIT_DIRECTOR, IDC_EDIT_WRITERS, IDC_EDIT_ACTORS,IDC_EDIT_PLOT };
    short idcStaticMovieInfo[INFO_MAX] = { 0 , 0, 0, IDC_STATIC_DIRECTOR, IDC_STATIC_WRITER, IDC_STATIC_ACTORS,0 };
    short idcCombos[FILTERS_MAX] = { IDC_COMBO_DIRECTOR , IDC_COMBO_DIRECTOR,IDC_COMBO_ACTORS, IDC_COMBO_GENRES, IDC_COMBO_GENRES2, IDC_COMBO_YEARMIN, IDC_COMBO_YEARMAX, IDC_COMBO_IMDBRATINGMIN, IDC_COMBO_IMDBRATINGMAX, IDC_COMBO_SERIES, IDC_COMBO_ANIMS, IDC_COMBO_DOCS, IDC_COMBO_SHORTS };
    short idcBtns[BTN_ALL_MAX] = { IDC_BUTTON_PLAY, IDC_BUTTON_RESET_DIRECTORS, IDC_BUTTON_RESET_ACTORS };
    short idcInfoTypes[TYPE_MAX] = { IDC_CHECK_SERIE , IDC_CHECK_ANIM,IDC_CHECK_DOC, IDC_CHECK_SHORT };
    short idcStatics[STATIC_ALL_MAX] = { IDC_STATIC_STATUS_TEXT , IDC_STATIC_STATUS_DESC,  IDC_STATIC_APIKEY };
    short idcInvertFilters[DataBase::INVERT_FILTERS_MAX] = { IDC_CHECK_GENRES, IDC_CHECK_GENRES2, IDC_CHECK_DIRECTORS, IDC_CHECK_ACTORS };

    enum gloMenus { MENU_SYS, MENU_BAR, MENU_CON, MENU_MAX} ;
    enum subMenus { SUBMENU_FILE, SUBMENU_FS, SUBMENU_OMDB, SUBMENU_DB, SUBMENU_HELP, SUBMENU_FILTERS, SUBMENU_COMBOS, SUBMENU_MAX };

    int menuLocations[MENU_MAX] = { LOC_SYSMENU, LOC_BARMENU , LOC_CONMENU };

    typedef struct
    {
        int location;
        long type;
        UINT_PTR id;
        CString name;
        void (omdbClientDlg::*method)(void);
    } structMenu;

    std::vector <omdbClientDlg::structMenu> mySubMenus[SUBMENU_MAX] = {
        { // SUBMENU_FILE
            { LOC_SYSMENU, MENU_FOLDER },
            { LOC_SYSMENU, MENU_READFS },
            { LOC_SYSMENU, MENU_STOPREADFS },
            { LOC_SYSMENU, MENU_SAVEDATA },
            { LOC_SYSMENU, MENU_LOADDATA },
            { LOC_SYSMENU, MENU_EXIT },
        },
        { // SUBMENU_FS
            { LOC_SYSMENU, MENU_FOLDER },
            { LOC_SYSMENU, MENU_READFS },
        },
        { // SUBMENU_OMDB
            { LOC_SYSMENU, MENU_OMDBSETKEY },
            { LOC_SYSMENU, MENU_OMDB1REQ },
            { LOC_SYSMENU, MENU_OMDBSEARCH },
            { LOC_SYSMENU, MENU_OMDBXREQ },
            { LOC_SYSMENU, MENU_OMDBSTOP },
        },
        { // SUBMENU_DB
            { LOC_SYSMENU, MENU_RESETTABFS },
            { LOC_SYSMENU, MENU_RESETTABMOVIES },
        },
        { // SUBMENU_HELP
            { LOC_SYSMENU,MENU_HELP_INDEX },
            { LOC_SYSMENU,MENU_HELP_FINDER },
            { LOC_SYSMENU,MENU_HELP_USING },
            { LOC_SYSMENU,MENU_HELP_ABOUT },
        },
        { // SUBMENU_FILTERS
            { LOC_CONMENU, MENU_FILTERS_RESET },
            { LOC_CONMENU, MF_SEPARATOR, 0 },
            { LOC_CONMENU, MENU_FILTERS_DIRECTORS },
            { LOC_CONMENU, MENU_FILTERS_ACTORS},
        },
        { // SUBMENU_COMBOS 
            { LOC_SYSMENU, MENU_COMBO_LOAD_ALL  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_DIR  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_WRI  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_ACT  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_GEN  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_GE2  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_YMN  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_YMX  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_IRMN },
            { LOC_SYSMENU, MENU_COMBO_LOAD_IRMX },
            { LOC_SYSMENU, MENU_COMBO_LOAD_SERI },
            { LOC_SYSMENU, MENU_COMBO_LOAD_ANIM },
            { LOC_SYSMENU, MENU_COMBO_LOAD_DOC  },
            { LOC_SYSMENU, MENU_COMBO_LOAD_SHOR },
        }
    };

    std::vector <omdbClientDlg::structMenu> myTopSysMenu = {
        { LOC_CONMENU, MENU_PLAY_MOVIE },
        { LOC_CONMENU, MENU_OPEN_EXPLORER },
        { LOC_CONMENU, MENU_EDIT_FILENAME },
        { LOC_CONMENU, MENU_EDIT_IMDBID },
        { LOC_SYSMENU | LOC_CONMENU, MF_SEPARATOR, 0 },
        { LOC_CONMENU, MENU_OPEN_WEB },
        { LOC_SYSMENU | LOC_CONMENU, MF_SEPARATOR, 0 },
        { LOC_CONMENU, MENU_OMDB1REQ },
        { LOC_CONMENU, MENU_OMDBSEARCH },
        { LOC_SYSMENU            , MF_POPUP, SUBMENU_FILE, L"&File" },
        { LOC_SYSMENU            , MF_POPUP, SUBMENU_OMDB, L"&Omdb" },
        //{ LOC_SYSMENU            , MF_POPUP, SUBMENU_FS, L"File &System" },
        { LOC_SYSMENU, MENU_SELECTCOLUMNS },
        { LOC_SYSMENU            , MENU_SELECTPLAYER },
        { LOC_SYSMENU            , MF_POPUP, SUBMENU_DB, L"&DataBase" },
        { LOC_SYSMENU            , MF_POPUP, SUBMENU_COMBOS, L"&Combos" },
        { LOC_SYSMENU            , MF_SEPARATOR, 0 },
        { LOC_SYSMENU            , MF_POPUP, SUBMENU_HELP, L"&Help" },
        { LOC_SYSMENU            , MENU_DBGWND },
        { LOC_SYSMENU            , MENU_HELP_ABOUT },
        { LOC_CONMENU            , MF_POPUP, SUBMENU_FILTERS, L"&Filter" }
    };

//    void setEditedImdbid(std::string);
    std::string changeCombo(filterTypes);
    std::string editCombo(filterTypes);
    std::string changeComboFolders();
    std::string editComboFolders();
    int getOmdbAllStart(void);
    int onOmdbReadFinish(void);
    void CountOmdbAllStart(void);
    void initFolder(void);
    void selectFolder(void);
/*    bool selectFolder(bool _readfs = false);*/
    void showColumnSelector(void);
    void readFolder(void);
    void readFolderIfEmpty(bool _readfolder = true);
    void stopReadFolder(void);
    void changeOmdbKey(void);
    void selectVideoPlayer(void);
    void dbClearMoviesFS(void);
    void dbClearMovies(void);
    void onAbout(void);
    void onExit(void);
    void loadXmlfile(void);
    void saveXmlfile(void);

protected:

    HICON m_hIcon;

    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnSysCommand(UINT nID, WPARAM wParam);
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    //CMenu* DoDataExchange(short _id);
    structMenu *getStructMenu(short _nID);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg HCURSOR OnQueryDragIcon();

    DECLARE_MESSAGE_MAP()

private:

    std::string versionRenoir = MY_PRODUCT_FILE_VERSION2;
    std::string versionDB;

    MovieFile selectedFile;
//    Omdb omdb;
    ManageXML xmlFiles;
//    ReadConfig xmlConfig;
    fileSystem fs;
    Log log;
    //ConfigManager configManager;
    int logLevel;

    HANDLE  hThreadReadFolder;
    HANDLE  hThreadReadXML;
    HANDLE  hThreadWriteXML;
    HANDLE  hThreadOmdbAll;
    bool threadOmdbAllIsRunning;
    bool threadSaveXmlIsRunning;
    bool threadLoadXmlIsRunning;
    //std::string pathXML;

    std::vector<MovieFile> *v_XmlLoad;
    std::vector<MovieFile> *v_FsFiles;
    std::vector<MovieFile> v_DBLoad;

    BUTIL::Mutex *mtx_readfiles;
    BUTIL::Mutex *mtx_vLoad;

    CMenu* myMenu[MENU_MAX];
    CMenu SubMenus[MENU_MAX][SUBMENU_MAX];
    CListCtrl *lstMovies;
    CEdit *editFolderPath;
    CEdit *editSearch;
    CComboBox *comboFolderPath;

    CStatic *movieInfoStatic[INFO_MAX];
    CEdit *movieInfoEdit[INFO_MAX];
    CButton *buttons[BTN_ALL_MAX];
    CStatic *staticTexts[STATIC_ALL_MAX];
    CButton *btnOmdb[BTN_OMDB_MAX];
    CComboBox *comboFilters[FILTERS_MAX];
    CButton *checkIs[TYPE_MAX];
    CButton *checkInvert[DataBase::INVERT_FILTERS_MAX];

    CButton *checkShowDeleted;
    CButton *checkConfirmImdbIdUpd;//IDC_CHECK_IMDBIDUPDCONF

    CProgressCtrl * progressBar; //IDC_PROGRESS

    CPictureCtrl m_picCtrl;
    
    debugWindow dlgWindow;

    FolderManager dlgFolderManager;

    states globalState;
    states *ptr_globalState = &globalState;
    std::vector<std::string> consoleDBG;
    std::vector<std::string> * ptr_consoleDBG = &consoleDBG;

    std::string apiKey;
    bool forceOmdb;
    bool autoOmdb;
    bool isApiKeySet;
    bool manualRequest;
    size_t progressCounter[CNT_FIELD];

    DataBase::moviesFSFilters *filters;

    bool stopThreads;

    void showText(STATIC_ALL _text, bool _show);
    void showText(STATIC_ALL _text, CString _str);
    void OnInitDialogVars();
    void OnInitDialogCreateMenu(int _menuId/*, int _location*/);
    void OnInitDialogMenus();
    bool OnInitDialogConfig();
    void OnInitDialogValues();
    bool OnInitDialogAppVars();

    void setSavedFilters();
    void loadDatabase(bool _readfolder = true);
    void loadDatabaseToVector();
    void loadAllCombosExcept(filterTypes _filter = FILTERS_MAX);
    void loadAllCombos(void);
    void loadFolders();
    int loadFilters(filterTypes _filter);
    void loadFilterDirectors(void);
    void loadFilterWriters(void);
    void loadFilterActors(void);
    void loadFilterGenres(void);
    void loadFilterGenres2(void);
    void loadFilterYearMin(void);
    void loadFilterYearMax(void);
    void loadFilterImdbRatingMin(void);
    void loadFilterImdbRatingMax(void);
    void loadFilterSerie(void);
    void loadFilterAnim(void);
    void loadFilterDoc(void);
    void loadFilterShort(void);
    int loadMoviesFS();
    void displayMovieInfos(MovieFile*);
    bool createFiltersSubMenu(Movie *movie);
    void displayMovieInfo(Movie *_m, movieInfo _info);
    void getFile(POSITION pos);
    void getFile(int pos);
    void getFileFromSelection(bool show = true);
    void setStaticText(STATIC_ALL, std::string);
    void setStaticStatus(std:: string );
    void setStaticStatus(void);
    void UpdateDisplay(MovieFile *file = NULL);
    void rebuildCombo(filterTypes);
    void rebuildComboFolder();
    void loadFilesFromVector();
    void findFilesFromVector();
    bool findFileInLstMovies(MovieFile *_file);
    void setColumnLength();
    void setColumnSize(int col, size_t sz);
    void resetColumnSize(int col = -1);
    void updateLstMovies(MovieFile *_file, bool _find = true);
    CString intToCString(int);
    CString stringToCString(std::string);
    std::string CStringToString(CString);
    int CStringToInt(CString);
    std::string GetEditString(CEdit *_edit);
    int GetEditInt(CEdit *_edit);
    bool isRunning(HANDLE  hThread);
    void showApiKey(void);
    bool OmdbSetkey(void);
    void editImdbId(void);
    void showDebugWindow(void);
    size_t textDebugWindowAt(int _pos, const char *, ...);
    //size_t textDebugWindowAt(int _pos, const wchar_t *, ...);
    size_t textDebugWindowAt(int _pos, std::string);
    size_t textDebugWindow(const char *, ...);
    //size_t textDebugWindow(const wchar_t *, ...);
    size_t textDebugWindow(std::string);
    void editFileName(void);
    void openImdbWeb(void);
    void openExplorer(void);
    void showSearchResult(void);
    bool omdbRequest();
    void omdbAllRequest(void);
    void omdbAllStop(void);
    void omdbSingleRequest(void);
    void resetFilters();
    void resetFilters(filterTypes _filter);
    void filterComboSeriesAnims(filterTypes _filter);
    movieInfoTypes col2InfoType(DataBase::column _col);
    filterTypes col2FilterTypes(DataBase::column _col);
    void checkMovieTypes(DataBase::column _col);
    void checkInvertFilters(DataBase::invertFilters _filter);
    void setBtnOmdbSingleState(void);
    void displayContextMenu(UINT uNewState);
    bool FindMenuPos(CMenu *pBaseMenu, UINT myID, CMenu * & pMenu, int & mpos);
    std::string getStringPond(DataBase::pondStr);
    void playMovie(void);

    virtual void OnOK(void);
    virtual void OnCancel(void);
public:

    afx_msg void OnLvnItemchangedMovieList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnColumnClickMovieList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnListViewMouseInput(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedOmdbRequest();
    afx_msg void OnBnClickedButtonReadfolder();
    afx_msg void OnBnClickedButtonLoadxmlfile();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedCheckShowdeleted();
    afx_msg void OnBnClickedButtonOmdbAll();
    afx_msg void OnBnClickedButtonWritexmlfile();
    afx_msg void OnCbnSelchangeComboGenres();
    afx_msg void OnCbnSelchangeComboDirector();
    afx_msg void OnCbnSelchangeComboActors();
    afx_msg void OnCbnEditchangeComboActors();
    afx_msg void OnCbnEditchangeComboDirector();
    afx_msg void OnCbnEditchangeComboGenres();
    afx_msg void OnCbnEditchangeComboGenres2();
    afx_msg void OnBnClickedCheckForce();
    //afx_msg void OnBnClickedButtonEditMinoccurrences();
    afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

    afx_msg void OnBnClickedClearmovies();
    afx_msg void OnBnClickedClearfs();
    afx_msg void OnBnClickedButtonOmdbSetkey();
    afx_msg void OnBnClickedCheckAutoOmdb();
    afx_msg void OnBnClickedCheckSerie();
    afx_msg void OnBnClickedCheckAnim();
    afx_msg void OnCbnSelchangeComboSeries();
    afx_msg void OnCbnSelchangeComboAnims();
    afx_msg void OnCbnSelchangeComboYearmin();
    afx_msg void OnCbnSelchangeComboYearmax();
    afx_msg void OnCbnSelchangeComboImdbratingmin();
    afx_msg void OnCbnSelchangeComboImdbratingmax();
    afx_msg void OnBnClickedCheckDoc();
    afx_msg void OnBnClickedCheckShort();
    afx_msg void OnCbnSelchangeComboDocs();
    afx_msg void OnCbnSelchangeComboShorts();
    afx_msg void OnCbnSelchangeComboGenres2();
    afx_msg void OnBnClickedButtonPlay();
    afx_msg void OnBnClickedCheckGenres();
    afx_msg void OnBnClickedCheckGenres2();
    afx_msg void OnBnClickedCheckDirectors();
    afx_msg void OnBnClickedCheckActors();
    afx_msg void OnBnClickedButtonResetDirectors();
    afx_msg void OnBnClickedButtonResetActors();
//    afx_msg void OnEnChangeEditFolder();
    afx_msg void OnStnClickedStaticApikey();
    afx_msg void OnCbnSelchangeComboFolders();
    afx_msg void OnCbnEditchangeComboFolders();

    afx_msg void OnEnChangeEditSearch();
    afx_msg void OnEnChangeEditSearch2();
    //afx_msg void OnLButtonDblClk(NMHDR *pNMHDR, LRESULT *pResult);
    //afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedRadioAuto();
    afx_msg void OnBnClickedRadioManual();
};