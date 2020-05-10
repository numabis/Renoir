#include "stdafx.h"
#include "afxdialogex.h"

#include "omdbClientDlg.h"
#include "CAboutDlg.h"
#include "setOmdbKey.h"
#include "ShowColumns.h"
#include "variables.h"
#include "renoir.h"
//#include "database.h"
#include "Convert.h"
#include "exLog.h"
#include "util.h"
#include "version.h"
#include "warning.h"

#include <mmsystem.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>

using std::string;
using std::vector;
using BUTIL::Convert;

#define LOGDEBUG(...)      exLOGDEBUG(__VA_ARGS__); //textDebugWindow(__VA_ARGS__);
#define LOGINFO(...)       exLOGINFO(__VA_ARGS__); //textDebugWindow(__VA_ARGS__);
#define LOGERROR(...)      exLOGERROR(__VA_ARGS__); //textDebugWindow(__VA_ARGS__);

#define SETTIMER(time)        SetTimer(_REFRESH_UI_EVENT_, time, NULL);

omdbClientDlg::omdbClientDlg(CWnd* pParent /*=NULL*/)
//omdbClientDlg::omdbClientDlg(void)
//    : CDialogEx(omdbClientDlg::IDD, pParent)
    : CDialogEx(omdbClientDlg::IDD)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    logLevel = 0;

//    nSortedCol = -1;
//    bSortAscending[0] = TRUE;
//    bSortAscending[1] = TRUE;
}

omdbClientDlg::~omdbClientDlg()
{
    //KillTimer(_REFRESH_UI_EVENT_);
    //if (editFolderPath != NULL)
    //    delete editFolderPath;
    //if (consoleLOG != NULL)
    //    delete consoleLOG;
    delete myMenu[MENU_BAR];
    delete myMenu[MENU_CON];
}

#define IDM_MAX_CMD 8
#define MENU_MAX 7
//int myMenuCmds[] = { IDM_ABOUTBOX, IDM_DIALOG_SETKEY, IDM_CHANGE_FOLDER, IDM_DIALOG_COLUMNS, IDM_DIALOG_VPLAYER, IDM_READFOLDER, IDM_RESETFS, IDM_RESETMOVIES, IDM_MAX_CMD };
//enum mySysMenu { MENU_ABOUTBOX, MENU_SETKEY, MENU_SELECT_FOLDER, MENU_SHOWCOLUMNS, MENU_VIDEOPLAYER, MENU_READFOLDER, MENU_DB, MENU_MAX };
//enum subMenus { SUBMENU_DB, SUBMENU_MAX };

//Add a message handler for the "NM_CUSTOMDRAW" message, so that you can get
//    the messages corrsponding to the custom drawing of the list.Lets name it
//    OnCustomDrawList :
//
#pragma region OnSys
//#define ID_DIALOG_SETKEY 0x0100
//#define ID_Load 2
//#define ID_Exit 3
//#define ID_Undo 4
//#define ID_Redo 5
//#define ID_VWS 6  
BEGIN_MESSAGE_MAP(omdbClientDlg, CDialogEx)
    ON_WM_CONTEXTMENU()
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_INITMENUPOPUP()
    ON_WM_TIMER()
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_MOVIE_LIST, &omdbClientDlg::OnLvnItemchangedMovieList)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_MOVIE_LIST, &omdbClientDlg::OnLvnColumnClickMovieList)
    //ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTVIEW, OnSortInfo)
    //ON_NOTIFY_RANGE(wNotifyCode, id, idLast, memberFxn)
    ON_BN_CLICKED(IDCANCEL, &omdbClientDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_OMDB_REQUEST, &omdbClientDlg::OnBnClickedOmdbRequest)
    ON_BN_CLICKED(IDC_BUTTON_READFOLDER, &omdbClientDlg::OnBnClickedButtonReadfolder)
    ON_BN_CLICKED(IDC_BUTTON_LOADXMLFILE, &omdbClientDlg::OnBnClickedButtonLoadxmlfile)
    ON_BN_CLICKED(IDC_CHECK_SHOWDELETED, &omdbClientDlg::OnBnClickedCheckShowdeleted)
    ON_BN_CLICKED(IDC_BUTTON_OMDB_ALL, &omdbClientDlg::OnBnClickedButtonOmdbAll)
    ON_BN_CLICKED(IDC_BUTTON_WRITEXMLFILE, &omdbClientDlg::OnBnClickedButtonWritexmlfile)
    ON_BN_CLICKED(IDC_CHECK_FORCE, &omdbClientDlg::OnBnClickedCheckForce)
//    ON_BN_CLICKED(IDC_BUTTON_EDIT_MINOCCURRENCES, &omdbClientDlg::OnBnClickedButtonEditMinoccurrences)
    ON_BN_CLICKED(IDC_CLEARMOVIES, &omdbClientDlg::OnBnClickedClearmovies)
    ON_BN_CLICKED(IDC_CLEARFS, &omdbClientDlg::OnBnClickedClearfs)
    ON_BN_CLICKED(IDC_BUTTON_OMDB_SETKEY, &omdbClientDlg::OnBnClickedButtonOmdbSetkey)
    ON_BN_CLICKED(IDC_CHECK_AUTOOMDB, &omdbClientDlg::OnBnClickedCheckAutoOmdb)
    ON_BN_CLICKED(IDC_CHECK_SERIE, &omdbClientDlg::OnBnClickedCheckSerie)
    ON_BN_CLICKED(IDC_CHECK_ANIM, &omdbClientDlg::OnBnClickedCheckAnim)
    ON_BN_CLICKED(IDC_CHECK_DOC, &omdbClientDlg::OnBnClickedCheckDoc)
    ON_BN_CLICKED(IDC_CHECK_SHORT, &omdbClientDlg::OnBnClickedCheckShort)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &omdbClientDlg::OnBnClickedButtonPlay)
    ON_BN_CLICKED(IDC_CHECK_GENRES, &omdbClientDlg::OnBnClickedCheckGenres)
    ON_BN_CLICKED(IDC_CHECK_GENRES2, &omdbClientDlg::OnBnClickedCheckGenres2)
    ON_BN_CLICKED(IDC_CHECK_DIRECTORS, &omdbClientDlg::OnBnClickedCheckDirectors)
    ON_BN_CLICKED(IDC_CHECK_ACTORS, &omdbClientDlg::OnBnClickedCheckActors)

    ON_CBN_SELCHANGE(IDC_COMBO_GENRES, &omdbClientDlg::OnCbnSelchangeComboGenres)
    ON_CBN_SELCHANGE(IDC_COMBO_DIRECTOR, &omdbClientDlg::OnCbnSelchangeComboDirector)
    ON_CBN_SELCHANGE(IDC_COMBO_ACTORS, &omdbClientDlg::OnCbnSelchangeComboActors)
    ON_CBN_SELCHANGE(IDC_COMBO_SERIES, &omdbClientDlg::OnCbnSelchangeComboSeries)
    ON_CBN_SELCHANGE(IDC_COMBO_ANIMS, &omdbClientDlg::OnCbnSelchangeComboAnims)
    ON_CBN_SELCHANGE(IDC_COMBO_YEARMIN, &omdbClientDlg::OnCbnSelchangeComboYearmin)
    ON_CBN_SELCHANGE(IDC_COMBO_YEARMAX, &omdbClientDlg::OnCbnSelchangeComboYearmax)
    ON_CBN_SELCHANGE(IDC_COMBO_IMDBRATINGMIN, &omdbClientDlg::OnCbnSelchangeComboImdbratingmin)
    ON_CBN_SELCHANGE(IDC_COMBO_IMDBRATINGMAX, &omdbClientDlg::OnCbnSelchangeComboImdbratingmax)
    ON_CBN_SELCHANGE(IDC_COMBO_DOCS, &omdbClientDlg::OnCbnSelchangeComboDocs)
    ON_CBN_SELCHANGE(IDC_COMBO_SHORTS, &omdbClientDlg::OnCbnSelchangeComboShorts)
    ON_CBN_SELCHANGE(IDC_COMBO_GENRES2, &omdbClientDlg::OnCbnSelchangeComboGenres2)
    ON_BN_CLICKED(IDC_BUTTON_RESET_DIRECTORS, &omdbClientDlg::OnBnClickedButtonResetDirectors)
    ON_BN_CLICKED(IDC_BUTTON_RESET_ACTORS, &omdbClientDlg::OnBnClickedButtonResetActors)
//    ON_EN_SETFOCUS(IDC_EDIT_FOLDER, &omdbClientDlg::OnEnChangeEditFolder)
    ON_STN_CLICKED(IDC_STATIC_APIKEY, &omdbClientDlg::OnStnClickedStaticApikey)
    ON_CBN_SELCHANGE(IDC_COMBO_FOLDERS, &omdbClientDlg::OnCbnSelchangeComboFolders)
END_MESSAGE_MAP()
void omdbClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PICTURE, m_picCtrl);
}
omdbClientDlg::structMenu* omdbClientDlg::getStructMenu(short _nID)
{
    UINT_PTR subId;
    for (size_t menu = 0; menu < myTopSysMenu.size(); menu++)
        //for (size_t item = 0; item < IDM_MAX_CMD; item++)
    {
        switch (myTopSysMenu[menu].type)
        {
        case MF_STRING:
            if ((_nID & 0xFFF0) == myTopSysMenu[menu].id)
            {
                return &myTopSysMenu[menu];
            }
            break;
        case MF_POPUP:
            subId = myTopSysMenu[menu].id;
            for (size_t subMenu = 0; subMenu < mySubMenus[subId].size(); subMenu++)
                if ((_nID & 0xFFF0) == mySubMenus[subId][subMenu].id)
                {
                    return &mySubMenus[subId][subMenu];
                }
            break;
        default:
            break;
        }
    }
    return nullptr;
}
void omdbClientDlg::OnSysCommand(UINT nID, WPARAM wParam)
{
    if (!HIWORD(wParam))
    {
        if (LOWORD(wParam) == 1) // Checks for the menu identifier of the Exit option
        {
            //DestroyWindow(hwnd);
        }
    }
}
void omdbClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    //if (nID != 61589)
    //    CDialogEx::OnSysCommand(nID, lParam);
    //LPARAM wParam = lParam & 0xfff0;
    //CString tmp;
    //tmp.Format(L"%d", nID);
    //GetDlgItem(IDC_EDIT_TESTnID)->SetWindowTextW(tmp);
    //tmp.Format(L"%d", nID & 0xfff0);
    //GetDlgItem(IDC_EDIT_TESTnID3)->SetWindowTextW(tmp);
//int xPos = GET_X_LPARAM(lParam);    // horizontal position 
//int yPos = GET_Y_LPARAM(lParam);    // vertical position
//tmp.Format(L"%d", (int)xPos);
//GetDlgItem(IDC_EDIT_TESTlParam)->SetWindowTextW(tmp);
//tmp.Format(L"%d", (int)yPos);
//GetDlgItem(IDC_EDIT_TESTlParam2)->SetWindowTextW(tmp);

structMenu* l_structMenu = getStructMenu(nID);

if (l_structMenu && l_structMenu->method)
(this->*l_structMenu->method)();

if (nID > IDC_BUTTON_FILTER_DIRECTORS && nID < IDC_BUTTON_FILTER_ACTORS)
{
    std::vector<std::string>* names = selectedFile.getMovie()->getvRoles(ROLES_DIRECTOR);
    std::string name = names->at(nID - IDC_BUTTON_FILTER_DIRECTORS - 1);
    int pos = comboFilters[FILTERS_DIRECTORS]->FindString(-1, CString(name.c_str()));
    comboFilters[FILTERS_DIRECTORS]->SetCurSel(pos);
    OnCbnSelchangeComboDirector();
}
if (nID > IDC_BUTTON_FILTER_ACTORS && nID < IDC_BUTTON_FILTER_ACTORS + 5)
{
    std::vector<std::string>* names = selectedFile.getMovie()->getvRoles(ROLES_ACTOR);
    std::string name = names->at(nID - IDC_BUTTON_FILTER_ACTORS - 1);
    int pos = comboFilters[FILTERS_ACTORS]->FindString(-1, CString(name.c_str()));
    comboFilters[FILTERS_ACTORS]->SetCurSel(pos);
    OnCbnSelchangeComboActors();
}

//if (nID == 57665)
//    CDialogEx::OnSysCommand(nID, lParam);

CDialogEx::OnSysCommand(nID, lParam);
}
void omdbClientDlg::OnPaint(void)
{
    if (IsIconic())
    {
        CPaintDC dc(this); // Contexto de dispositivo para dibujo

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Centrar icono en el rectángulo de cliente
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Dibujar el icono
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}
HCURSOR omdbClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}
void omdbClientDlg::OnTimer(UINT_PTR _nIDEvent)
{

    if (_nIDEvent == _REFRESH_UI_EVENT_)
    {
        setStaticStatus();

        while (consoleDBG.empty() == false)
        {
            textDebugWindow(consoleDBG.back());
            consoleDBG.pop_back();
        }

        loadFilesFromVector();
        findFilesFromVector();

        if (fs.isRunning(hThreadReadFolder))
        {
            SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
        }
        else
        {
            SetTimer(_REFRESH_UI_EVENT_, 2000, NULL);
        }
        if (xmlFiles.isRunning(hThreadWriteXML))
        {
            SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
            threadSaveXmlIsRunning = true;
        }
        else if (threadSaveXmlIsRunning == true)
        {
            threadSaveXmlIsRunning = false;
            SETTIMER(2000);
            std::ostringstream msg;
            msg << "Saved " << progressCounter[CNT_READ] << " movie(s) to " << GETCM.getConfigStr("xmlLocalFullPath"); //.getXmlLocalFullPath();
            MBOX(msg.str(), "Info", MB_OK);
            progressCounter[CNT_READ] = 0;
            progressCounter[CNT_TOTAL] = 0;
        }
        if (xmlFiles.isRunning(hThreadReadXML))
        {
            SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
            threadLoadXmlIsRunning = true;

        }
        else if (threadLoadXmlIsRunning == true)
        {
            threadLoadXmlIsRunning = false;
            SETTIMER(2000);
            std::ostringstream msg;
            msg << "Loaded " << progressCounter[CNT_READ] << " movie(s) from " << GETCM.getConfigStr("xmlLocalFullPath"); //xmlConfig.getXmlLocalFullPath();
            MBOX(msg.str(), "Info", MB_OK);
            progressCounter[CNT_READ] = 0;
            progressCounter[CNT_TOTAL] = 0;
        }
        if (threadOmdbAllIsRunning == false && isRunning(hThreadOmdbAll))
        {
            threadOmdbAllIsRunning = true;
        }
        if (threadOmdbAllIsRunning == true && !isRunning(hThreadOmdbAll))
        {
            threadOmdbAllIsRunning = false;
            loadAllCombosExcept();
        }
        if (v_FsFiles->empty() == false)
        {
            SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
        }
    }
}
void omdbClientDlg::onExit(void)
{
    KillTimer(_REFRESH_UI_EVENT_);
    CDialogEx::OnCancel();
}
void omdbClientDlg::onAbout(void)
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}
void omdbClientDlg::OnContextMenu(CWnd* pWnd, CPoint pos)
{
    //pWnd->CheckDlgButton[] ;
    
    int idClick = pWnd->GetDlgCtrlID();
    int idList = lstMovies->GetDlgCtrlID();
    int retVal = 0;

    if (idClick == idList)
    {
        //int nID = conMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD, pos.x, pos.y, this);
        int nID = myMenu[MENU_CON]->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD, pos.x, pos.y, this);
        OnSysCommand(nID, MAKELPARAM(pos.x, pos.y));
    }
    else
        CDialogEx::OnContextMenu(pWnd, pos);

    //CListCtrl* pList = (CListCtrl*)GetDlgItem(ID_YOUR_LIST);
    //if ((CWnd*)lstMovies == pWnd)
    //{
    //    int retVal = conMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD, pos.x, pos.y, this);
    //
    //    // Handle your returns here.
    //}

    //LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pWnd);
    //ASSERT(pNMLV);
    //if (pNMLV->uChanged & LVIF_STATE)
    //{
    //    if (pNMLV->uNewState & LVIS_SELECTED)
    //    {
    //        displayContextMenu(pNMLV->iItem);
    //    }
    //}
}
bool omdbClientDlg::FindMenuPos(CMenu *pBaseMenu, UINT myID, CMenu * & pMenu, int & mpos)
{
    // REMARK: pMenu is a pointer to a Cmenu-Pointer
    int myPos = -1;
    if (pBaseMenu == NULL)
    {
        // Sorry, Wrong Number
        pMenu = NULL;
        mpos = -1;
        return true;
    }
    int totalItem = pBaseMenu->GetMenuItemCount();
    for (myPos = totalItem - 1; myPos >= 0; myPos--)
    {
        int Status = pBaseMenu->GetMenuState(myPos, MF_BYPOSITION);
        //pBaseMenu->get
        CMenu* mNewMenu;

        if (Status == 0xFFFFFFFF)
        {
            // That was not an legal Menu/Position-Combination
            pMenu = NULL;
            mpos = -1;
            return true;
        }
        // Is this the real one?
        UINT curID = pBaseMenu->GetMenuItemID(myPos);
        
        if (curID == myID)
        {
            // Yep!
            pMenu = pBaseMenu;
            mpos = myPos;
            return true;
        }
        // Maybe a subMenu?
        mNewMenu = pBaseMenu->GetSubMenu(myPos);
        // This function will return NULL if ther is NO SubMenu
        
        if (mNewMenu != NULL)
        {
            HMENU hMenuG = pBaseMenu->m_hMenu;
            HMENU hMenu = mNewMenu->m_hMenu;
            if (hMenu != NULL)
            {
                // rekursive!
                int totalSubItem = GetMenuItemCount(hMenu);
                if (totalSubItem>0)
                {
                    bool found = FindMenuPos(mNewMenu, myID, pMenu, mpos);
                    if (found)
                        return true;	// return this loop
                }
            }
        }
        // I have to check the next in my loop
    }
    return false; // iterate in the upper stackframe
}
//int omdbClientDlg::myMsgBox(std::string _str, std::string _title, int _btns)
//{
//    return MessageBox(CString(_str.c_str()), CString(_title.c_str()), _btns);
//}

#pragma endregion OnSys

#pragma region OnInitDialog
BOOL omdbClientDlg::OnInitDialog()
{
    ST_SAVE(ST_INITIATING);

//    consoleDBG.push_back(stNames[ST_INITIATING]);
    GETLOG.ptr_consoleDBG = ptr_consoleDBG;
    GETDB.ptr_consoleDBG = ptr_consoleDBG;
    GETDB.ptr_globalState = ptr_globalState;
    xmlFiles.ptr_consoleDBG = ptr_consoleDBG;
    xmlFiles.ptr_globalState = ptr_globalState;
    fs.ptr_consoleDBG = ptr_consoleDBG;
    fs.ptr_globalState = ptr_globalState;
    dlgWindow.Create(IDD_DIALOG1);
#if defined (_DEBUG) || defined(_DEMO)
    dlgWindow.ShowWindow(SW_SHOW);
#endif
    //editConsole = dlgWindow.getConsole();
    LOGDEBUG("");

    textDebugWindow("Initializing Console");

    LOGINFO("[------------------------------]");
    LOGINFO("[         STARTING APP         ]");
    LOGINFO("[------------------------------]");

    CDialogEx::OnInitDialog();

    OnInitDialogMenus();

    // Establecer el icono para este cuadro de diálogo. El marco de trabajo realiza esta operación
    //  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
    SetIcon(m_hIcon, TRUE);			// Establecer icono grande
    SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

    srand((unsigned int)time(NULL));
    OnInitDialogVars();

    if (OnInitDialogConfig() == false)
    {
        onExit();
        return false;
    }

    //BUTIL::exLog::getInstance().setLogLevel(xmlConfig.getLogConfig()->logLevel);
    logLevel = GETCM.getConfigInt(CONF_LOG_LOGLEVEL);
    if (logLevel == -1) logLevel = 0;
    BUTIL::exLog::getInstance().setLogLevel(&logLevel); //);
    
    textDebugWindow("setLogLevel (%d)", logLevel);
    UpdateData(FALSE);
    SetTimer(_REFRESH_UI_EVENT_, 1, NULL);

    OnInitDialogValues();

    if (OnInitDialogData() == false)
    {
        onExit();
        return false;
    }

    ST_SET(ST_STANDBY);
    return true;  // Devuelve TRUE  a menos que establezca el foco en un control
}
void omdbClientDlg::OnInitDialogCreateMenu(int _menuId/*, int _location*/)
{
    LOGDEBUG(""); 

    //int myMenuId = 0;
    //CMenu pSubMenus[SUBMENU_MAX];
    int loc = menuLocations[_menuId];

    for (size_t sub = 0; sub < SUBMENU_MAX; sub++)
    {
        VERIFY(SubMenus[_menuId][sub].CreateMenu());
        if (SubMenus[_menuId][sub] != NULL)
        {
            for (size_t menu = 0; menu < mySubMenus[sub].size(); menu++)
            {
                if ((loc & mySubMenus[sub][menu].location) == loc)
                {
                    SubMenus[_menuId][sub].AppendMenu(mySubMenus[sub][menu].type, mySubMenus[sub][menu].id, mySubMenus[sub][menu].name);
                }
                //pSubMenus[sub].EnableMenuItem(mySubMenus[sub][menu].id, MF_DISABLED | MF_GRAYED);
                //if ((_location & mySubMenus[sub][menu].location) == _location)
                //    pSubMenus[sub].EnableMenuItem(mySubMenus[sub][menu].id, MF_ENABLED);
            }
        }

    }

    //CMenu* pSysMenu = GetSystemMenu(FALSE);
    //CString strMenu[MENU_MAX];
    if (myMenu[_menuId] != NULL)
    {
        for (size_t menu = 0; menu < myTopSysMenu.size(); menu++)
        {
            if ((loc & myTopSysMenu[menu].location) == loc)
            {
            switch (myTopSysMenu[menu].type)
            {
            case MF_SEPARATOR:
                myMenu[_menuId]->AppendMenu(MF_SEPARATOR);
                break;
            case MF_STRING:
                myMenu[_menuId]->AppendMenu(myTopSysMenu[menu].type, myTopSysMenu[menu].id, myTopSysMenu[menu].name);
                //_m->EnableMenuItem(myTopSysMenu[menu].id, MF_DISABLED | MF_GRAYED);
                //if ((_location & myTopSysMenu[menu].location) == _location)
                //    _m->EnableMenuItem(myTopSysMenu[menu].id, MF_ENABLED);
                break;
            case MF_POPUP:
                myMenu[_menuId]->AppendMenu(myTopSysMenu[menu].type, (UINT_PTR)SubMenus[_menuId][myTopSysMenu[menu].id].m_hMenu, myTopSysMenu[menu].name);
                //_m->EnableMenuItem(myTopSysMenu[menu].id, MF_DISABLED | MF_GRAYED);
                //if ((_location & myTopSysMenu[menu].location) == _location)
                //    _m->EnableMenuItem(myTopSysMenu[menu].id, MF_ENABLED);
                break;
            default:
                break;
            }

            }
        }
    }
}
void omdbClientDlg::OnInitDialogMenus()
{
    LOGDEBUG("");

    // IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    myMenu[MENU_SYS] = GetSystemMenu(FALSE);
    OnInitDialogCreateMenu(MENU_SYS);

    myMenu[MENU_BAR] = new CMenu;
    VERIFY(myMenu[MENU_BAR]->CreateMenu());
    OnInitDialogCreateMenu(MENU_BAR);
    SetMenu(myMenu[MENU_BAR]);
    
    //DrawMenuBar();
    // Remove and destroy old menu
    //CMenu *m_hMenuDefault = GetMenu();
    //SetMenu(NULL);
    //CMenu *old_menu = CMenu::FromHandle(m_hMenuDefault->m_hMenu);
    //old_menu->DestroyMenu();
    // Add new menu.
    //SetMenu(&menuFile);
    // Assign default menu
    //m_hMenuDefault = menuFile.m_hMenu;

    myMenu[MENU_CON] = new CMenu;
    VERIFY(myMenu[MENU_CON]->CreatePopupMenu());
    OnInitDialogCreateMenu(MENU_CON);

}
bool omdbClientDlg::OnInitDialogConfig()
{
    LOGDEBUG("");

    std::wstring wcmdLine(GetCommandLine());
    std::string cmdLine = WS2S(wcmdLine);
    std::string readXML = "-readXML";
    std::size_t found = cmdLine.find(readXML);

    bool connected = GETDB.setDB();

    if (found != string::npos)
        GETCM.readConfFile();

    if (connected)
    {
        GETCM.init();
        if (GETDB.getUpdate())
        {
            saveXmlfile();
            GETDB.initDB(true);
            hThreadReadXML = xmlFiles.loadOmdbFile();
        }
    }
    return true;
        //int ret = xmlConfig.initLocalConfigFile();
    //if (ret == XMLERROR)
    //{
    //    xmlConfig.initConfigFile();
    //    MBOX("Please, select the home folder hosting your movies", "Info", MB_OK);
    //    LOGDEBUG("selectFolder");
    //    selectFolder();
    //    LOGDEBUG("changeOmdbKey");
    //    changeOmdbKey();
    //}
    //
    //ret = xmlConfig.loadLocalConfigFile();
    //if ((ret & XMLNOFSPATH) == XMLNOFSPATH)
    //{
    //    MBOX("Please, select the home folder hosting your movies", "Info", MB_OK);
    //    ret = xmlConfig.initConfigFile();
    //    selectFolder();
    //} else
    //    ret = xmlConfig.initConfigFile();
    //
    //if ((ret & XMLERROR) == XMLERROR)
    //{
    //    return false;
    //}
    ////while ((ret & XMLOK) != XMLOK)
    //    ret = xmlConfig.initConfigFile();
    //ret = xmlConfig.initConfigFile();
    //if ((ret & XMLOK) != XMLOK)
    //{
    //    return false;
    //}
    //
    //return true;
}
void omdbClientDlg::OnInitDialogVars()
{
    LOGDEBUG("");

    for (int info = 0; info < INFO_MAX; info++)
    {
        movieInfoEdit[info] = (CEdit *)GetDlgItem(idcMovieInfo[info]);
        movieInfoStatic[info] = (CStatic *)GetDlgItem(idcStaticMovieInfo[info]);
    }
    for (int btn = 0; btn < BTN_ALL_MAX; btn++)
    {
        buttons[btn] = (CButton *)GetDlgItem(idcBtns[btn]);
    }
    for (int omdbRequest = 0; omdbRequest < BTN_OMDB_MAX; omdbRequest++)
    {
        btnOmdb[omdbRequest] = (CButton *)GetDlgItem(idcOmdb[omdbRequest]);
    }
    //for (int xmlFile = 0; xmlFile < BTN_XMLFILE_MAX; xmlFile++)
    //{
    //    btnXmlFile[xmlFile] = (CButton *)GetDlgItem(idcXmlFile[xmlFile]);
    //}
    for (int combo = FILTERS_DIRECTORS; combo < FILTERS_MAX; combo++)
    {
        comboFilters[combo] = (CComboBox *)GetDlgItem(idcCombos[combo]);
    }
    for (int infoType = 0; infoType < TYPE_MAX; infoType++)
    {
        checkIs[infoType] = (CButton *)GetDlgItem(idcInfoTypes[infoType]);
    }
    for (int text = 0; text < STATIC_ALL_MAX; text++)
    {
        staticTexts[text] = (CStatic *)GetDlgItem(idcStatics[text]);
    }
    for (int check = 0; check < DataBase::INVERT_FILTERS_MAX; check++)
    {
        checkInvert[check] = (CButton *)GetDlgItem(idcInvertFilters[check]);
    }

    lstMovies = (CListCtrl *)GetDlgItem(IDC_MOVIE_LIST);
    editFolderPath = (CEdit *)GetDlgItem(IDC_EDIT_FOLDER);
    //editComboOccurrences = (CEdit *)GetDlgItem(IDC_EDIT_MINOCCURRENCES);
    checkShowDeleted = (CButton *)GetDlgItem(IDC_CHECK_SHOWDELETED);
    checkConfirmImdbIdUpd = (CButton *)GetDlgItem(IDC_CHECK_IMDBIDUPDCONF);

    progressBar = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS);

    comboFolderPath = (CComboBox *)GetDlgItem(IDC_COMBO_FOLDERS);
    

    btnOmdb[CHK_OMDB_AUTO]->SetCheck(1);
    //btnOmdb[BTN_OMDB_ALL]->EnableWindow(true);
    myMenu[MENU_CON]->EnableMenuItem(idcOmdb[BTN_OMDB_ALL], MF_ENABLED);
    filters = GETDB.getFilters();
    for (short invert = DataBase::INVERT_FILTERS_GENRES; invert < DataBase::INVERT_FILTERS_MAX; invert++)
        checkInvert[invert]->SetCheck(filters->invertFilters[invert]);

}
void omdbClientDlg::OnInitDialogValues()
{
    LOGDEBUG("");
    DWORD style = lstMovies->GetExtendedStyle();
    style |= LVS_EX_DOUBLEBUFFER | LVS_EX_BORDERSELECT | LVS_EX_FULLROWSELECT;
    lstMovies->SetExtendedStyle(style);

    resetColumnSize();

    //FS_FILENAME, FS_DATEADDED, FS_FILESIZE, FS_IMDBID, MOVIES_IMDBRATING, FS_ID,FS_PATH, FS_PHYSICAL, 
    for (int col=0;col<DataBase::NUM_COLUMNS;col++)
        lstMovies->InsertColumn(col, BUTIL::Convert::charToWchar((LPSTR)columNames[col]), LVCFMT_LEFT, columnMinSizes[col]);

    movieInfoEdit[INFO_GENRES]->ShowWindow(SW_HIDE);

    for (int info= INFO_DIRECTORS; info<= INFO_ACTORS; info++)
        movieInfoStatic[info]->ShowWindow(SW_HIDE);
    
    for (int text = STATIC_TEXT; text < STATIC_ALL_MAX; text++)
        showText((STATIC_ALL)text, true);

    stopThreads = false;
    threadSaveXmlIsRunning = false;
    threadSaveXmlIsRunning = false;
}
bool omdbClientDlg::OnInitDialogData()
{

    threadOmdbAllIsRunning = false;

    log.init();
    omdb.init();
    fs.init();
    v_FsFiles = fs.getVfiles();
    mtx_readfiles = fs.getMutex();
    fs.setProgressCounter(progressCounter);
    xmlFiles.setProgressCounter(progressCounter);

    xmlFiles.init();
    v_XmlLoad = xmlFiles.getV_load();
    mtx_vLoad = xmlFiles.getMtx_vLoad();

    filters->exists = (checkShowDeleted->GetCheck() == 0);

    forceOmdb = (btnOmdb[CHK_OMDB_FORCE]->GetCheck() == 1);
    autoOmdb = (btnOmdb[CHK_OMDB_AUTO]->GetCheck() == 1);

    if (GETCM.getConfigStr(CONF_APP_CURRENTFOLDER).empty() == true)
        initFolder();
    else
        GETDB.PATHFS_insert(GETCM.getConfigStr(CONF_APP_CURRENTFOLDER));

    //CString tmp;
    //tmp.Format(L"%s", BUTIL::Convert::string2wstring(GETCM.getConfigStr(CONF_APP_CURRENTFOLDER)).c_str());
    //editFolderPath->SetWindowTextW(tmp);

    CString cs_loaded;

    if (GETCM.getConfigInt(CONF_FS_AUTOREADFOLDER) == 1)
    {
        GETDB.MOVIESFS_setPhysical(false);
        hThreadReadFolder = fs.readFolder();
    }

    filters->orderByAsc = true;
    progressCounter[CNT_TOTAL] = 0;
    progressCounter[CNT_SUCCESS] = 0;
    progressCounter[CNT_ERROR] = 0;
    
    //filters->folderFilter = xmlConfig.getGlobalConfig()->fsPath;
    int folderId = GETDB.getPathId(GETCM.getConfigStr(CONF_APP_CURRENTFOLDER));
    if (folderId != -1)
        filters->folderFilterId = folderId;

    resetColumnSize();
    setSavedFilters();
    loadDatabase();
    isApiKeySet = !GETCM.getConfigStr(CONF_OMDB_APIKEY).empty();
    showApiKey();

    return true;
}
#pragma endregion OnInitDialog

#pragma region dataIO
void omdbClientDlg::loadFilesFromVector(void)
{
    int i = 0;
    static size_t logPos = 0;
    while (v_FsFiles->empty() == false)
    {
        size_t v_sz = v_FsFiles->size();
        if (!fs.isRunning(hThreadReadFolder))
        {
            if (progressCounter[CNT_TOTAL] < v_sz)
            {
                progressCounter[CNT_TOTAL] += v_sz;
            }
            progressCounter[CNT_READ] = progressCounter[CNT_TOTAL] - v_sz;
            logPos = textDebugWindowAt(logPos, "Loading from vector %d/%d", progressCounter[CNT_READ] + 1, progressCounter[CNT_TOTAL]);
        }

        if (i > 100)
        {
            setColumnLength();
            break;
        }
        
        mtx_readfiles->lock();
        MovieFile file = v_FsFiles->back();
        v_FsFiles->pop_back();
        mtx_readfiles->unlock();
        updateLstMovies(&file, false);
        if (v_FsFiles->empty() == true)
        {
            if (!fs.isRunning(hThreadReadFolder))
            {
                loadFolders();
                textDebugWindowAt(logPos, "Loading from vector %d/%d", progressCounter[CNT_READ] + 1, progressCounter[CNT_TOTAL]);
                logPos = 0;
            }
            setColumnLength();

            size_t lastpos = max(0, selectedFile.LstPosition);
            lstMovies->SetItemState(lastpos, LVIS_SELECTED, LVIS_SELECTED);

            if (!fs.isRunning(hThreadReadFolder))
            {
                progressCounter[CNT_READ] = 0;
                progressCounter[CNT_TOTAL] = 0;
            }

        }
        i++;
    }
}
void omdbClientDlg::findFilesFromVector(void)
{
    int i = 0;

    while (v_XmlLoad->size() != 0)
    { 
        if (i > 10)
            break;
        mtx_vLoad->lock();
        MovieFile file = v_XmlLoad->back();
        v_XmlLoad->pop_back();
        mtx_vLoad->unlock();
        if(findFileInLstMovies(&file))
            updateLstMovies(&file);
        i++;
    }
}
void omdbClientDlg::setSavedFilters()
{
    LOGDEBUG("");
    //FILTERS_SERIE, FILTERS_ANIM, FILTERS_DOC, FILTERS_SHORT
    for (int f = FILTERS_DIRECTORS; f < FILTERS_MAX; f++)
    {
        filterTypes filter = (filterTypes)f;
        std::string savedValue = GETCM.getFilter(filter);
        if (savedValue.empty() == false)
            GETDB.setFilterSelectedValue(filter, savedValue);
    }
}
void omdbClientDlg::loadDatabase(bool _readfolder)
{
    LOGDEBUG("");
    filters->orderByColumn = DataBase::COL_FILENAME+1;
    //loadMoviesFS();
    comboFilters[FILTERS_YEARMIN]->SetCurSel(0);
    comboFilters[FILTERS_YEARMAX]->SetCurSel(0);
    loadAllCombosExcept();
    readFolderIfEmpty(_readfolder);
    loadFolders();
}
void omdbClientDlg::loadFolders()
{
    LOGDEBUG("");
    GETDB.PATHFS_loadFolders();
    rebuildComboFolder();
}
 int omdbClientDlg::loadFilters(filterTypes _filter)
{
     LOGDEBUG("");
    int nbTypes = GETDB.loadFilter(_filter);
    if (nbTypes >= 0)
    {
        filters->v_combos[_filter][0] = BUTIL::Util::format("%s (%d)", ALL, 0);
        rebuildCombo(_filter);
    }
    return nbTypes;
}
void omdbClientDlg::loadFilterDirectors(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_DIRECTORS);
}
void omdbClientDlg::loadFilterWriters(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_WRITERS);
}
void omdbClientDlg::loadFilterActors(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_ACTORS);
}
void omdbClientDlg::loadFilterGenres(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_GENRES);
}
void omdbClientDlg::loadFilterGenres2(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_GENRES2);
}
void omdbClientDlg::loadFilterYearMin(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_YEARMIN);
}
void omdbClientDlg::loadFilterYearMax(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_YEARMAX);
}
void omdbClientDlg::loadFilterImdbRatingMin(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_IMDBRATINGMIN);
}
void omdbClientDlg::loadFilterImdbRatingMax(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_IMDBRATINGMAX);
}
void omdbClientDlg::loadFilterSerie(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_SERIE);
}
void omdbClientDlg::loadFilterAnim(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_ANIM);
}
void omdbClientDlg::loadFilterDoc(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_DOC);
}
void omdbClientDlg::loadFilterShort(void)
{
    LOGDEBUG("");
    loadFilters(FILTERS_SHORT);
}
 int omdbClientDlg::loadMoviesFS(void)
{
     LOGDEBUG("");
    size_t logPos = 0;
    MovieFile file;
    std::wstringstream wss;
    progressCounter[CNT_READ] = 0;
    progressCounter[CNT_TOTAL] = 0;
    mtx_readfiles->lock();
    v_FsFiles->clear();
    mtx_readfiles->unlock();
    ST_SAVE(ST_READING_DB);
    lstMovies->DeleteAllItems();
    int count = 0;
    if (GETDB.MOVIESFS_iniAll())
    {
        while (GETDB.MOVIESFS_getAll(&file))
        {
            mtx_readfiles->lock();
            v_FsFiles->push_back(file);
            mtx_readfiles->unlock();
            progressCounter[CNT_READ]++;
            logPos = textDebugWindowAt(logPos, "Loading moviesFS : %04d", progressCounter[CNT_READ]);
        }
    }
    ST_RESTORE();
    count = progressCounter[CNT_READ];
    progressCounter[CNT_TOTAL] = 0;
    progressCounter[CNT_READ] = 0;

    return progressCounter[CNT_READ];
}
void omdbClientDlg::loadDatabaseToVector(void)
{
    LOGDEBUG("");
    MovieFile file;
    std::wstringstream wss;
    Movie movie;

    ST_SAVE(ST_READING_DB);
    progressCounter[CNT_TOTAL] = GETDB.MOVIES_iniAll(GETCM.getConfigStr(CONF_OMDB_LISTSEPARATOR));
    xmlFiles.clearMovieVector();
    progressCounter[CNT_READ] = 1;
    while (GETDB.MOVIES_getAll(&movie))
    {
        progressCounter[CNT_READ]++;
        xmlFiles.addToMovieVector(&movie);
    }
}
void omdbClientDlg::loadAllCombosExcept(filterTypes _filter)
{
    LOGDEBUG("");
    //for (short filter = 0; filter <= FILTERS_MAX; filter++)
    for (short filter = 0; filter < FILTERS_MAX; filter++)
    {
        if (filter == FILTERS_WRITERS)
            continue;
        if (_filter != filter)
        {
            loadFilters((filterTypes)filter);
        }
    }
}
void omdbClientDlg::loadAllCombos(void)
{
    LOGDEBUG("");
    //for (short filter = 0; filter <= FILTERS_MAX; filter++)
    for (short filter = 0; filter < FILTERS_MAX; filter++)
    {
        if (filter == FILTERS_WRITERS)
            continue;
        loadFilters((filterTypes)filter);
    }
}
void omdbClientDlg::readFolder(void)
{
    LOGDEBUG("");
    SetTimer(_REFRESH_UI_EVENT_, 1, NULL);
    GETDB.MOVIESFS_setPhysical(false);
    lstMovies->DeleteAllItems();
    hThreadReadFolder = fs.readFolder();
}
void omdbClientDlg::readFolderIfEmpty(bool _readfolder)
{
    LOGDEBUG("");
    if (_readfolder)
    {
        MovieFolder folder;
        std::string currentFolder = selectedFile.getPath();
        if (currentFolder.empty() == true)
            //currentFolder = *(filters->folderFilter);
            currentFolder = GETCM.getConfigStr(CONF_APP_CURRENTFOLDER);
        if (currentFolder.empty() == false)
        {
            folder.setPath(currentFolder);
            GETDB.isFolderRead(&folder);
            if (folder.isRead() == false)
            {
                fs.readFolder();
                return;
            }
        }
    }
    loadMoviesFS();
    loadAllCombosExcept();
}
void omdbClientDlg::stopReadFolder(void)
{
    LOGDEBUG("");
    fs.terminateThreads();
}
void omdbClientDlg::dbClearMoviesFS(void)
{
    LOGDEBUG("");
    if (MBOX("Reset scanned files from folder?", "Info", MB_OKCANCEL) == IDOK)
    {
        if (GETDB.MOVIESFS_deleteAll())
            //loadFolders();
            loadDatabase(false);
    }
}
void omdbClientDlg::dbClearMovies(void)
{
    LOGDEBUG("");
    //int ret = MessageBox((LPCWSTR)L"Reset downloaded information from OMDB to the database?", (LPCWSTR)L"Info", MB_OKCANCEL);
    if (MBOX("Reset downloaded information from OMDB to the database?", "Info", MB_OKCANCEL) == IDOK)
    {
        GETDB.deleteAllMoviesData();
        short count = lstMovies->GetItemCount();
        for (short pos = 0; pos < count; pos++)
        {
            lstMovies->SetItemText(pos, DataBase::COL_IMDBRATING, CString(""));
        }
    }
}
void omdbClientDlg::loadXmlfile(void)
{
    LOGDEBUG("");
//    btnXmlFile[BTN_XMLFILE_READ]->EnableWindow(false);
    CString defaultPath(GETCM.getConfigStr(CONF_XML_PATH).c_str());
    CString defaultName(GETCM.getConfigStr(CONF_XML_FILENAME).c_str());
    CString defaultExtension(GETCM.getConfigStr(CONF_XML_FILENAMEEXT).c_str());
    std::string fullPath, path, name, extention;
    size_t foundName, foundDot;
    bool compressXmlFile = false;

    CFileDialog dlg(
        true,                                   // true for File Open dialog box
        defaultExtension,               // The default file name extension
        defaultName,                    // The default file name
        OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,    // bunch of flags http://msdn.microsoft.com/en-us/library/wh5hz49d.aspx
        NULL
    );

    dlg.m_ofn.lpstrFilter = L"XML Files (*.xml)\0*.xml\0ZXML Compressed Files(*.zxml)\0*.zxml\0All Files (*.*)\0*.*\0\0";
    dlg.m_ofn.lpstrTitle = L"Select file containing the data to load";
    dlg.m_ofn.lpstrInitialDir = defaultPath;

    if (dlg.DoModal() == IDOK)
    {
        fullPath = CStringToString(dlg.GetPathName());

        foundName = fullPath.find_last_of("\\");
        foundDot = fullPath.find_last_of(".");
        path = fullPath.substr(0, foundName);
        name = fullPath.substr(foundName + 1, foundDot - foundName - 1);
        extention = fullPath.substr(foundDot + 1, string::npos);

        GETCM.setConfigValue(CONF_XML_PATH, path);
        GETCM.setConfigValue(CONF_XML_FILENAME, name);
        GETCM.setConfigValue(CONF_XML_FILENAMEEXT, extention);
//        xmlConfig.saveLocalConfigFile(false);
        hThreadReadXML = xmlFiles.loadOmdbFile();
        threadLoadXmlIsRunning = true;
    }
}
void omdbClientDlg::saveXmlfile(void)
{
    LOGDEBUG("");
    CString defaultPath(GETCM.getConfigStr(CONF_XML_PATH).c_str());
    CString defaultName(GETCM.getConfigStr(CONF_XML_FILENAME).c_str());
    CString defaultExtension(GETCM.getConfigStr(CONF_XML_FILENAMEEXT).c_str());

    std::string fullPath, path, name, extention;
    size_t foundName, foundDot;
    bool compressXmlFile = false;

    CFileDialog dlg(
        false,                                   // true for File Open dialog box
        defaultExtension,               // The default file name extension
        defaultName,                    // The default file name
        OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,    // bunch of flags http://msdn.microsoft.com/en-us/library/wh5hz49d.aspx
        NULL
    );
    if (extention.compare("zxml") == 0)
    {
        compressXmlFile = true;
        dlg.m_ofn.lpstrFilter = L"ZXML Compressed Files(*.zxml)\0*.zxml\0XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0\0";
    }
    else
        dlg.m_ofn.lpstrFilter = L"XML Files (*.xml)\0*.xml\0ZXML Compressed Files(*.zxml)\0*.zxml\0All Files (*.*)\0*.*\0\0";
    //dlg.m_ofn.lpstrFilter= "XML Files (*.xml)\0*.xml\0           Text Files (*.txt)\0*.txt \0All Files (*.*)\0*.*\0\0";
    dlg.m_ofn.lpstrTitle = L"Save XML File As";
    dlg.m_ofn.lpstrInitialDir = defaultPath;

    if (dlg.DoModal() == IDOK)
    {
        fullPath = CStringToString(dlg.GetPathName());

        foundName = fullPath.find_last_of("\\");
        foundDot = fullPath.find_last_of(".");
        path = fullPath.substr(0, foundName);
        name = fullPath.substr(foundName + 1, foundDot-foundName-1);
        extention = fullPath.substr(foundDot + 1, string::npos);

        GETCM.setConfigValue(CONF_XML_PATH, path);
        GETCM.setConfigValue(CONF_XML_FILENAME, name);
        GETCM.setConfigValue(CONF_XML_FILENAMEEXT, extention);
        //xmlConfig.saveLocalConfigFile(false);

        loadDatabaseToVector();
        hThreadWriteXML = xmlFiles.saveMoviesToXml(fullPath, compressXmlFile);
        threadSaveXmlIsRunning = true;
    }
}
#pragma endregion dataInOut

#pragma region getFile
void omdbClientDlg::getFileFromSelection(bool showMsg)
{
    POSITION pos = lstMovies->GetFirstSelectedItemPosition();
    if (pos == NULL /*&& */) {
        //MessageBox((LPCWSTR)L"Select file first", (LPCWSTR)L"Info", MB_OK);

        if (showMsg)
            MBOX("Select file first", "Info", MB_OK);
        return ;
    }
    getFile(pos);
}
void omdbClientDlg::getFile(POSITION pos)
{
    if (pos == NULL) {
        //MessageBox((LPCWSTR)L"Select file first", (LPCWSTR)L"Info", MB_OK);
        //MBOX("Select file first", "Info", MB_OK);

        return ;
    }

    getFile(lstMovies->GetNextSelectedItem(pos));
}
void omdbClientDlg::getFile(int _position)
{
    //CString path, filename, imdbId, size, Rating;
    //MovieFile file;

    selectedFile.LstPosition = _position;
    //path = lstMovies->GetItemText(file.position, COL_PATH);
    //filename = lstMovies->GetItemText(file.position, COL_NAME);
    //imdbId = lstMovies->GetItemText(file.position, COL_IMDBID);
    //size = lstMovies->GetItemText(file.position, COL_SIZE);
    //Rating = lstMovies->GetItemText(file.position, COL_RATING);

    CString getItem;
    for (int column = 0; column < DataBase::NUM_COLUMNS; column++)
    {
        DataBase::column dbCol = (DataBase::column)column;
        std::string tmpRating;
        getItem = lstMovies->GetItemText(selectedFile.LstPosition, column);
        switch (column)
        {
        case DataBase::COL_FSID:
            selectedFile.moviesFSId = CStringToInt(getItem);
            break;
        case DataBase::COL_PATH:
            //selectedFile.path = CStringToString(getItem);
            selectedFile.setPath(CStringToString(getItem));
            break;
        case DataBase::COL_FILENAME:
            selectedFile.setFilename(CStringToString(getItem));
            break;
        case DataBase::COL_PHYSICAL:
            selectedFile.exists = (CStringToInt(getItem) == 1);
            break;
        case DataBase::COL_DATE:
            selectedFile.dateAdded = CStringToString(getItem);
            break;
        case DataBase::COL_SIZE:
            selectedFile.size = CStringToInt(getItem);
            break;
        case DataBase::COL_SERIE:
            selectedFile.typeIs[TYPE_SERIE] = (typesValues)CStringToInt(getItem);
            break;
        case DataBase::COL_ANIM:
            selectedFile.typeIs[TYPE_ANIM] = (typesValues)CStringToInt(getItem);
            break;
        case DataBase::COL_DOC:
            selectedFile.typeIs[TYPE_DOC] = (typesValues)CStringToInt(getItem);
            break;
        case DataBase::COL_SHORT:
            selectedFile.typeIs[TYPE_SHORT] = (typesValues)CStringToInt(getItem);
            break;
        case DataBase::COL_IMDBID:
            selectedFile.imdbId = CStringToString(getItem);
            break;
        case DataBase::COL_IMDBRATING:
            tmpRating = CStringToString(getItem);
            if (tmpRating.empty() == false && tmpRating[1] == '.')
                tmpRating.erase(1, 1);
            selectedFile.imdbRating = atoi(tmpRating.c_str());
            break;
        default:
            __debugbreak();
            break;
        }
    }
    
    if (GETCM.getConfigInt(CONF_FS_SEARCHTYPES) == 1)
    {
        bool fields[FS_MAX_FIELDS - FS_ID];
        short fieldsCount = 0;
        for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
            fields[i] = false;

        for (short type = 0; type < TYPE_MAX; type++)
        {
            if (selectedFile.typeIs[type] == TYPE_UNKNOWN && selectedFile.guessType(GETCM.getFsType(type)))
            {
                fields[FS_SERIE - FS_ID + type] = true;
                selectedFile.typeIs[type] = TYPE_NOTMOVIE;
                fieldsCount++;
            }
        }
        if (fieldsCount>0)
            GETDB.MOVIESFS_update(&selectedFile, fields);
    }

    selectedFile.resetMovie();
    
}
#pragma endregion getFile

#pragma region displayInfo
void omdbClientDlg::showText(STATIC_ALL _text, bool _show)
{
    staticTexts[_text]->ShowWindow(_show ? SW_SHOW : SW_HIDE);
}
void omdbClientDlg::showText(STATIC_ALL _text, CString _str)
{
    staticTexts[_text]->SetWindowText(_str);
    staticTexts[_text]->ShowWindow(SW_SHOW);
}
void omdbClientDlg::setStaticText(STATIC_ALL _static, std::string _str)
{
    if (_str.empty() == true)
        return;
    CString cs_loaded;

    cs_loaded.Format(BUTIL::Convert::string2wstring(_str).c_str());
    showText(_static, cs_loaded);
}
void omdbClientDlg::setStaticStatus(void)
{
    setStaticStatus(std::string());
}
void omdbClientDlg::setStaticStatus(std::string _text)
{
    std::string text;
    int lower, upper;
    if (progressCounter[CNT_READ] + progressCounter[CNT_TOTAL] + progressCounter[CNT_ERROR] == 0)
    {
        progressBar->ShowWindow(SW_HIDE);
        staticTexts[STATIC_DESC]->ShowWindow(SW_HIDE);
        return;
    }
    if (progressCounter[CNT_READ] > progressCounter[CNT_TOTAL])
        progressCounter[CNT_TOTAL] = progressCounter[CNT_READ];

    progressBar->GetRange(lower, upper);

    if (upper != progressCounter[CNT_TOTAL]) {
        staticTexts[STATIC_DESC]->ShowWindow(SW_SHOW);
        progressBar->ShowWindow(SW_SHOW);
        progressBar->SetRange(0, (short)progressCounter[CNT_TOTAL]);
    }

    progressBar->SetPos(progressCounter[CNT_READ]);

    text = _text + BUTIL::Util::format(" %d / %d", progressCounter[CNT_READ], progressCounter[CNT_TOTAL]);
    if (progressCounter[CNT_ERROR] != 0)
        text += BUTIL::Util::format(" (%d error(s))", progressCounter[CNT_ERROR]);

    setStaticText(STATIC_DESC, text);
}
void omdbClientDlg::displayMovieInfo(Movie *_m, movieInfo _info)
{
    CString text(C2WC(_m->getInfo(_info).c_str()));
    if (text.IsEmpty() == false)
    {
        movieInfoEdit[_info]->SetWindowTextW(text);
        movieInfoEdit[_info]->ShowWindow(SW_SHOW);
        if (movieInfoStatic[_info] != NULL)
            movieInfoStatic[_info]->ShowWindow(SW_SHOW);
    }
    else
    {
        movieInfoEdit[_info]->ShowWindow(SW_HIDE);
        if (movieInfoStatic[_info] != NULL)
            movieInfoStatic[_info]->ShowWindow(SW_HIDE);
    }
}
void omdbClientDlg::displayMovieInfos(MovieFile *_file)
{
    Movie *movie = _file->getMovie();
    CString text;

    if (movie->getTitle().empty() == false)
    {
        CString title(S2WS(movie->getTitle()).c_str());
        CString year;
        year.Format(L" (%d)", movie->getYear());
        if (movie->getYear() != 0)
            title += year;
        //text.Format(L"%s (%d)", S2WS(movie->getTitle()).c_str(), movie->getYear());
        //else
        //    text.Format(L"%s", S2WS(movie->getTitle()).c_str());
        movieInfoEdit[INFO_TITLE]->SetWindowTextW(title);
        movieInfoEdit[INFO_TITLE]->ShowWindow(SW_SHOW);
    }
    else
        movieInfoEdit[INFO_TITLE]->ShowWindow(SW_HIDE);

    for (int info= INFO_RUNTIME; info<INFO_MAX; info++)
        displayMovieInfo(movie, (movieInfo)info);
    for (int infoType = TYPE_SERIE; infoType < TYPE_MAX; infoType++)
    {
        checkIs[infoType]->SetCheck(selectedFile.typeIs[infoType] == TYPE_NOTMOVIE);
    }
    movie->consoleDBG = &consoleDBG;

    if (movie->getJpeg())
    {
        string path = movie->getJpegPath().c_str();
//        textDebugWindow("picture :%s", path.c_str());
        m_picCtrl.Load(CString(path.c_str()));
        m_picCtrl.ShowWindow(SW_SHOW);
    }
    else {
        if (movie->getPoster().empty() == false)
            textDebugWindow("No picture for %s (%s)", movie->getTitle().c_str(), movie->getPoster().c_str());
        //textDebugWindow("URL: %s", movie->getPoster().c_str());
        m_picCtrl.ShowWindow(SW_HIDE);
    }

    createFiltersSubMenu(movie);

    //CMenu *temp;
    //int m_Pos = 0;
    //
    //if (FindMenuPos(myMenu[MENU_CON], subMenuFilter[SUBMENUFILTER_DIRECTORS], temp, m_Pos))
    //{
    //    m_Pos += 2;
    //    LPCWSTR director = (LPCWSTR)(C2WC(movie->getInfo(INFO_DIRECTORS).c_str()));
    //    temp->InsertMenu(m_Pos, MF_BYPOSITION, (UINT_PTR)subMenuFilter[SUBMENUFILTER_D1], director); /*(SubMenus[2][4], m_Pos, MF_BYPOSITION, subMenuFilter[SUBMENUFILTER_D1], "Print Record")*/;
    //}
}
bool omdbClientDlg::createFiltersSubMenu(Movie *_movie)
{
    int _menuId = MENU_CON;
    int sub = SUBMENU_FILTERS;
    int loc = menuLocations[_menuId];
    int m_Pos = 0;
    int totalItem = myMenu[_menuId]->GetMenuItemCount();
    myMenu[_menuId]->RemoveMenu(totalItem-1, MF_BYPOSITION);
    SubMenus[_menuId][sub].DestroyMenu();
    VERIFY(SubMenus[_menuId][sub].CreateMenu());
    if (SubMenus[_menuId][sub] != NULL)
    {
        for (size_t menu = 0; menu < mySubMenus[sub].size(); menu++)
        {
            if ((loc & mySubMenus[sub][menu].location) == loc)
            {
                SubMenus[_menuId][sub].AppendMenu(mySubMenus[sub][menu].type, mySubMenus[sub][menu].id, mySubMenus[sub][menu].name);
                UINT_PTR role = mySubMenus[sub][menu].id;
                std::vector<std::string> emptyV;
                std::vector<std::string>* names = &emptyV;
                //std::string *name;
                int indice;
                if (role == subMenuFilter[SUBMENUFILTER_DIRECTORS])
                {
                    names = _movie->getvRoles(ROLES_DIRECTOR);
                    indice = SUBMENUFILTER_DIRECTORS + 1;
                }
                else if (role == subMenuFilter[SUBMENUFILTER_ACTORS])
                {
                    names = _movie->getvRoles(ROLES_ACTOR);
                    indice = SUBMENUFILTER_ACTORS + 1;
                }
                //else
                //{
                //    names = {};
                //}
                int vSz = names->size();
                if (vSz != 0)
                {
                    SubMenus[_menuId][sub].AppendMenu(MF_SEPARATOR, 0);
                    std::vector<string>::iterator name;
                    int i = 0;
                    for (name = names->begin(); name != names->end(); name++)
                    {
                        i++;
                        CString cname(C2WC((*name).c_str()));
                        SubMenus[_menuId][sub].AppendMenu(MF_STRING, (UINT_PTR)subMenuFilter[indice + i], cname);
                    }
                    //for (int person = 0; person < vSz; person++)
                    //{
                    //    //std::string name = names->at(person);
                    //    LPCWSTR lName = (LPCWSTR)(C2WC(names->at(person).c_str()));
                    //    CString cname(S2WS(names->at(person)).c_str());
                    //
                    //    SubMenus[_menuId][sub].AppendMenu(MF_STRING, (UINT_PTR)subMenuFilter[indice +person], cname);
                    //}
                    SubMenus[_menuId][sub].AppendMenu(MF_SEPARATOR, 0);
                }
    
    
            }
            //pSubMenus[sub].EnableMenuItem(mySubMenus[sub][menu].id, MF_DISABLED | MF_GRAYED);
            //if ((_location & mySubMenus[sub][menu].location) == _location)
            //    pSubMenus[sub].EnableMenuItem(mySubMenus[sub][menu].id, MF_ENABLED);
        }
        size_t menu = myTopSysMenu.size()-1;
        myMenu[_menuId]->AppendMenu(myTopSysMenu[menu].type, (UINT_PTR)SubMenus[_menuId][myTopSysMenu[menu].id].m_hMenu, myTopSysMenu[menu].name);
        return true;
    }
    return false;

}
bool omdbClientDlg::findFileInLstMovies(MovieFile *_file)
{
    int pos = lstMovies->GetItemCount();
    if (_file->LstPosition >= 0 && _file->LstPosition < pos)
        return true;
    _file->LstPosition = pos;
    if (_file->moviesFSId >= 0)
    {
        for (int i = 0; i < _file->LstPosition; i++)
        {
            int id = _wtoi(lstMovies->GetItemText(pos, DataBase::COL_FSID));
            if (id == _file->moviesFSId)
            {
                _file->LstPosition = i;
                return true;
            }
        }
        return false;
    }
    else if (_file->getFilename().size() == 0 && _file->title.size() != 0)
    {
        for (int i = 0; i < _file->LstPosition; i++)
        {
            getFile(i);
            selectedFile.fileNameToDetails();
            if (selectedFile.imdbRating != 0)
                continue;
            transform(_file->title.begin(), _file->title.end(), _file->title.begin(), ::tolower);
            transform(selectedFile.title.begin(), selectedFile.title.end(), selectedFile.title.begin(), ::tolower);

            if (_file->title.compare(selectedFile.title) == 0)
            {
                _file->setFolder(selectedFile);
                _file->LstPosition = i;
                return true;
            }
        }
    }
    return false;
}
void omdbClientDlg::setColumnLength()
{
    for (int col = 0; col<DataBase::NUM_COLUMNS; col++)
        if (columnsToSHow[col])
            lstMovies->SetColumnWidth(col, columnSizes[col]);
        else
            lstMovies->SetColumnWidth(col, 0);
}
void omdbClientDlg::setColumnSize(int _col, size_t _sz)
{
    if (columnsToSHow[_col] == false)
    {
        return;
    }
    size_t size = _sz * columnMultipli[_col];
    if (size > columnSizes[_col])
    {
        if (size > columnMaxSizes[_col])
            size = columnMaxSizes[_col];
        columnSizes[_col] = min(size, columnMaxSizes[_col]);
    }
}
void omdbClientDlg::resetColumnSize(int _col)
{
    if (_col >= 0 && _col < DataBase::NUM_COLUMNS)
    {
        columnSizes[_col] = columnMinSizes[_col];
    } 
    else
    {
        for (int col=0;col<DataBase::NUM_COLUMNS;col++)
            columnSizes[col] = columnMinSizes[col];
    }
}
void omdbClientDlg::updateLstMovies(MovieFile* _file, bool _find)
{
    ST_SAVE(ST_UPDATE_DISPLAY);
    if (_find==false) 
        _file->LstPosition = lstMovies->GetItemCount();
    bool exists = findFileInLstMovies(_file);

    if (_find == true)
    {
        std::string imdbId = _file->imdbId;
        _file->fileNameToDetails();
        _file->initMovieValue();
        if (GETDB.getMovieData(_file) && imdbId.compare(_file->imdbId))
        {
            bool fields[FS_MAX_FIELDS - FS_ID];
            for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
                fields[i] = false;
            fields[FS_IMDBID - FS_ID] = true;
            GETDB.MOVIESFS_update(_file, fields);
        }
    }

    CString strItem;
    strItem.Format(_T("%d"), _file->moviesFSId);
    //wss << _file->moviesFSId;
    if (exists == false)
    {
        lstMovies->InsertItem(_file->LstPosition, strItem);
    }
    else
    {
        lstMovies->SetItemText(_file->LstPosition, DataBase::COL_FSID, strItem);
    }
    setColumnSize(DataBase::COL_FSID, strItem.GetLength());

    if(_file->isPathEmpty())
        __debugbreak();

    lstMovies->SetItemText(_file->LstPosition, DataBase::COL_PATH, CString(_file->getPath().c_str()));
    setColumnSize(DataBase::COL_PATH, _file->getPath().size());

    if (_file->getFilename().size() != 0)
    {
        strItem = CString(_file->getFilename().c_str());
        if (_file->exists == false)
        {
            strItem.Append(_T(" [DELETED]"));
        }

        lstMovies->SetItemText(_file->LstPosition, DataBase::COL_FILENAME, strItem);
        setColumnSize(DataBase::COL_FILENAME, strItem.GetLength());
    }

    strItem.Format(_T("%d"), _file->exists);
    lstMovies->SetItemText(_file->LstPosition, DataBase::COL_PHYSICAL, strItem);
    setColumnSize(DataBase::COL_PHYSICAL, strItem.GetLength());
    
    if (_file->dateAdded.size() != 0)
    {
        lstMovies->SetItemText(_file->LstPosition, DataBase::COL_DATE, CString(_file->dateAdded.c_str()));
        setColumnSize(DataBase::COL_DATE, _file->dateAdded.size());
    }
    
    strItem.Format(_T("%d MB"), _file->size);
    lstMovies->SetItemText(_file->LstPosition, DataBase::COL_SIZE, strItem);
    setColumnSize(DataBase::COL_SIZE, strItem.GetLength());

    for (int type = TYPE_SERIE, col = DataBase::COL_SERIE; type < TYPE_MAX; type++, col++)
    {
        strItem.Format(_T("%d"), _file->typeIs[type]);
        lstMovies->SetItemText(_file->LstPosition, col, strItem);
        setColumnSize(col, strItem.GetLength());
    }

    lstMovies->SetItemText(_file->LstPosition, DataBase::COL_IMDBID, CString(_file->imdbId.c_str()));
    setColumnSize(DataBase::COL_IMDBID, _file->imdbId.size());

    strItem = CString((BUTIL::Util::formatNumber1Dec(_file->imdbRating / 10.0)).c_str());

    lstMovies->SetItemText(_file->LstPosition, DataBase::COL_IMDBRATING, strItem);
    setColumnSize(DataBase::COL_IMDBRATING, strItem.GetLength());

    ST_RESTORE();
}
void omdbClientDlg::UpdateDisplay(MovieFile* _file)
{
    ST_SAVE(ST_UPDATE_DISPLAY);
    bool FsNeedUpdate = false;
    short tmpImdbRating ;
    typesValues tmpType[TYPE_MAX];
    bool isInDB;
    std::string tmpImdbId;

    bool fields[FS_MAX_FIELDS - FS_ID];
    for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
        fields[i] = false;

    if (_file != NULL)
        selectedFile = *_file;

    tmpImdbId = selectedFile.imdbId;
    tmpImdbRating = selectedFile.imdbRating;

    for (int type= TYPE_SERIE;type<TYPE_MAX;type++)
        tmpType[type] = selectedFile.typeIs[type];

    selectedFile.fileNameToDetails();
    selectedFile.initMovieValue();
    isInDB = GETDB.getMovieData(&selectedFile);

    if (tmpImdbId != selectedFile.imdbId)
    {
        fields[FS_IMDBID - FS_ID] = true;
        FsNeedUpdate = true;
    }
    short conversion = FS_SERIE - FS_ID; // = 7
    for (int type = TYPE_SERIE; type < TYPE_MAX; type++) // !! TYPE_SERIE=0 -> TYPE_SHORT=3
    {
        if (tmpType[type] != selectedFile.typeIs[type] && selectedFile.typeIs[type] == TYPE_UNKNOWN) //  !! FS_SERIE=307 -> FS_SHORT = 310
        {
            selectedFile.typeIs[type] = tmpType[type];
            fields[conversion + type] = true;
            FsNeedUpdate = true;
        }
    }

    if (!isInDB && autoOmdb &&
        ((tmpImdbId.substr(0, 1) == OMDBNOTFOUND && forceOmdb) ||
        (tmpImdbId.substr(0, 1) != OMDBNOTFOUND)))
    {
        progressCounter[CNT_TOTAL] = 0;
        progressCounter[CNT_SUCCESS] = 0;
        progressCounter[CNT_ERROR] = 0;
        omdbRequest(&selectedFile); // updateMovieFS done in omdbRequest
        FsNeedUpdate = true;
    }
    else if (FsNeedUpdate)
    {
        GETDB.MOVIESFS_update(&selectedFile, fields);
    }
    if (FsNeedUpdate)
    {
        if (tmpImdbRating != selectedFile.imdbRating)
            updateLstMovies(&selectedFile);
        loadAllCombosExcept();
    }
    setBtnOmdbSingleState();
    displayMovieInfos(&selectedFile);
    ST_RESTORE();
}
std::string omdbClientDlg::getStringPond(DataBase::pondStr _str)
{
    return _str.str + std::string(" (") + STR(_str.pond) + std::string(")");
}
void omdbClientDlg::rebuildComboFolder()
{
    std::vector<DataBase::pondStr>::iterator it;
    comboFolderPath->ResetContent();
    comboFolderPath->Clear();
    int pos = 0;

    for (it = filters->folders.begin(); it != filters->folders.end(); it++)
    {
        comboFolderPath->AddString(CString(((*it).str.c_str())));
    }

    if (filters->folders.size() == 0)
    {
        comboFolderPath->AddString(CString(GETCM.getConfigStr(CONF_APP_CURRENTFOLDER).c_str()));
        pos = 0;
    }
    else
        pos = comboFolderPath->FindString(-1, CString(GETCM.getConfigStr(CONF_APP_CURRENTFOLDER).c_str()));

    comboFolderPath->SetCurSel(pos);
}
void omdbClientDlg::rebuildCombo(filterTypes _combo)
{
    std::vector<string>::iterator name;
    comboFilters[_combo]->ResetContent();
    comboFilters[_combo]->Clear();
    int pos = 0;

    for (name = filters->v_combos[_combo].begin(); name != filters->v_combos[_combo].end(); name++)
    {
        CString cstr(C2WC((*name).c_str()));
        comboFilters[_combo]->AddString(cstr);
    }
    pos = 0;

    if (GETDB.ftype[_combo] == DataBase::NORM_COMBOT)
    {
        if (filters->valueSelected[_combo] >= 0)
            pos = comboFilters[_combo]->FindString(-1, CString(filters->nameSelected[_combo].c_str()));
    }
    if (GETDB.ftype[_combo] == DataBase::GENRE_COMBOT)
    {
        if (filters->valueSelected[_combo] < TYPE_ALL)
            pos = filters->valueSelected[_combo] / 2;
    }
    if (GETDB.ftype[_combo] == DataBase::RANGE_COMBOT)
    {
        if (filters->valueSelected[_combo] >= 0)
            pos = comboFilters[_combo]->FindString(-1, CString(filters->nameSelected[_combo].c_str()));
        if (pos == -1)
        {
            pos = 0;
        }
    }
    comboFilters[_combo]->SetCurSel(pos);
}
void omdbClientDlg::setBtnOmdbSingleState(void)
{
    CMenu *temp;
    bool subMenu = false;
    int m_Pos = 0;
    if (FindMenuPos(myMenu[MENU_SYS], idcOmdb[BTN_OMDB_SINGLE], temp, m_Pos))
        subMenu = true;

    if ((!selectedFile.isImdbId() || forceOmdb == true) /*&& isApiKeySet*/ && !autoOmdb)
    {
        //btnOmdb[BTN_OMDB_SINGLE]->EnableWindow(true);
        myMenu[MENU_CON]->EnableMenuItem(idcOmdb[BTN_OMDB_SINGLE], MF_ENABLED);
        if (subMenu && temp)
          temp->EnableMenuItem(m_Pos, MF_ENABLED);
    }
    else
    {
        //btnOmdb[BTN_OMDB_SINGLE]->EnableWindow(false);
        myMenu[MENU_CON]->EnableMenuItem(idcOmdb[BTN_OMDB_SINGLE], MF_GRAYED | MF_DISABLED);
        if (subMenu && temp)
            temp->EnableMenuItem(m_Pos, MF_GRAYED | MF_DISABLED);
    }
}
void omdbClientDlg::displayContextMenu(UINT _row)
{
    myMenu[MENU_CON]->TrackPopupMenu(TPM_RETURNCMD, 100, 100, this);
}
size_t omdbClientDlg::textDebugWindow(const char *_format, ...)
{
    char linea[1024];

    va_list ap;
    va_start(ap, _format);
    vsprintf_s(linea, sizeof(linea), _format, ap);
    va_end(ap);

    return dlgWindow.writeToConsole(C2WC(linea));
}
size_t omdbClientDlg::textDebugWindow(const wchar_t *_format, ...)
{
    wchar_t linea[1024];

    va_list ap;
    va_start(ap, _format);
    wvsprintf(linea, _format, ap);
    va_end(ap);

    return dlgWindow.writeToConsole(linea);
}
size_t omdbClientDlg::textDebugWindow(std::string _text)
{
#ifdef _WSTRING
    return dlgWindow.writeToConsole(S2WS(_text).c_str());
#else
    return dlgWindow.writeToConsole(_text);
#endif
}
size_t omdbClientDlg::textDebugWindowAt(int _pos, const char *_format, ...)
{
    char linea[1024];

    va_list ap;
    va_start(ap, _format);
    vsprintf_s(linea, sizeof(linea), _format, ap);
    va_end(ap);

    return dlgWindow.writeToConsole(C2WC(linea), _pos);
}
size_t omdbClientDlg::textDebugWindowAt(int _pos, const wchar_t *_format, ...)
{
    wchar_t linea[1024];

    va_list ap;
    va_start(ap, _format);
    wvsprintf(linea, _format, ap);
    va_end(ap);

    return dlgWindow.writeToConsole(linea, _pos);
}
size_t omdbClientDlg::textDebugWindowAt(int _pos, std::string _text)
{
#ifdef _WSTRING
    return dlgWindow.writeToConsole(S2WS(_text).c_str(), _pos);
#else
    return dlgWindow.writeToConsole(_text, _pos);
#endif
}
void omdbClientDlg::editImdbId()
{
    setOmdbKey dlgEditImdbId;
    string texts[3] = { "", "   You can edit the value", "Id :" };
    string tmpImdbId = selectedFile.imdbId;
    dlgEditImdbId.setValue(&tmpImdbId);
    dlgEditImdbId.setTexts(texts);
    //std::string url = "";
    //LPCWSTR LPCWSTRurl = BUTIL::Convert::charToWchar((LPSTR)url.c_str());
    //dlgEditImdbId.setURL((wchar_t*)LPCWSTRurl);
    if (dlgEditImdbId.DoModal() == IDOK)
    {
        if (tmpImdbId != selectedFile.imdbId)
        {
            selectedFile.imdbId = tmpImdbId;

            bool fields[FS_MAX_FIELDS - FS_ID];
            for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
                fields[i] = false;
            fields[FS_IMDBID - FS_ID] = true;
            if (GETDB.MOVIESFS_update(&selectedFile, fields) == true)
            {
                UpdateDisplay();
                updateLstMovies(&selectedFile);
                //lstMovies->SetItemText(selectedFile.LstPosition, DataBase::COL_IMDBID, CString(selectedFile.imdbId.c_str()));
                //lstMovies->SetItemText(selectedFile.LstPosition, DataBase::COL_IMDBRATING, CString((int)selectedFile.imdbRating));
            }
            LOGINFO("Imdb Id set to [%s]", tmpImdbId.c_str());
            setStaticText(STATIC_DESC, "");
        }
    }
}
void omdbClientDlg::openImdbWeb(void)
{
    string url;
    if (selectedFile.isImdbId())
        url = GETCM.getImdbTitleUrl(selectedFile.imdbId);
    else
    {
        string title = selectedFile.title;
        setOmdbKey dlgEditTitle;
        string texts[3] = { "   Edit text to search", "", "Search :" };
        dlgEditTitle.setValue(&title);
        dlgEditTitle.setTexts(texts);
        dlgEditTitle.DoModal();
        url = GETCM.getImdbFindUrl(title);
    }
    wchar_t *wurl = C2WC(url.c_str());

    "https://www.imdb.com/find?q=A+Space+Odyssey";

    ShellExecute(
        NULL, // or your can use GetSafeHwnd()
        L"open",
        //        L"https://www.omdbapi.com/apikey.aspx",
        wurl,
        NULL,
        NULL,
        SW_SHOWNORMAL
    );
}
void omdbClientDlg::editFileName()
{
    setOmdbKey dlgEditFileName;
    string texts[3] = { "   You can edit the value", "", "Name :" };
    string tmpFileName;
    if (selectedFile.year != 0)
        tmpFileName = BUTIL::Util::format("%s (%d)%s", selectedFile.title.c_str(), selectedFile.year, selectedFile.getExt().c_str());
    else
        tmpFileName = BUTIL::Util::format("%s%s", selectedFile.title.c_str(), selectedFile.getExt().c_str());
    texts[1] = selectedFile.getFilename();
    dlgEditFileName.setValue(&tmpFileName);
    dlgEditFileName.setTexts(texts);

    if (dlgEditFileName.DoModal() == IDOK)
    {
        if (tmpFileName != selectedFile.getFilename())
        {
            string s_oldName = selectedFile.getFullPath().c_str();
            string s_newName = selectedFile.getFullPath(tmpFileName).c_str();

            int res = rename(s_oldName.c_str(), s_newName.c_str());
            int error = ::GetLastError();
            if (error == ERROR_SUCCESS) {
                selectedFile.setFilename(tmpFileName);

                bool fields[FS_MAX_FIELDS - FS_ID];
                for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
                    fields[i] = false;
                fields[FS_FILENAME - FS_ID] = true;

                if (GETDB.MOVIESFS_update(&selectedFile, fields) == true)
                {
                    lstMovies->SetItemText(selectedFile.LstPosition, DataBase::COL_FILENAME, CString(selectedFile.getFilename().c_str()));
                    UpdateDisplay();
                }
                string text = BUTIL::Util::format("Filename set to [%s]", selectedFile.getFilename().c_str());
                LOGINFO(text.c_str());
                setStaticText(STATIC_DESC, text.c_str());
            }
            else
            {
                //wchar_t wmsg[256];
                //char* msg = BUTIL::Util::GetLastErrorAsString(error);
                //_swprintf(wmsg, L"Error %s - file [%s]->[%s]", BUTIL::Convert::charToWchar(msg), BUTIL::Convert::charToWchar((LPSTR)selectedFile.filename.c_str()), BUTIL::Convert::charToWchar((LPSTR)tmpFileName.c_str()));
                //MessageBox(wmsg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
                MBOX(std::string(BUTIL::Util::GetLastErrorAsString(error)) + " - file [" + selectedFile.getFilename() + "]->[" + tmpFileName + "]", "Error", MB_ICONERROR | MB_OK);
                LOGERROR("Error renaming file [%s]->[%s]", selectedFile.getFilename().c_str(), tmpFileName.c_str());
            }
        }
    }
}
void omdbClientDlg::showDebugWindow(void)
{
    dlgWindow.ShowWindow(SW_SHOW);
    textDebugWindow("Opening Console");
}
#pragma endregion displayInfo

#pragma region omdb
void omdbClientDlg::CountOmdbAllStart()
{
    LOGDEBUG("");
    ST_SAVE(ST_OMDB_REQUEST_ALL);
    short count = lstMovies->GetItemCount();
    progressCounter[CNT_TOTAL] = 0;
    for (short pos = 0; pos < count; pos++)
    {
        getFile(pos);
        if (selectedFile.imdbId.size() == 0)
            progressCounter[CNT_TOTAL]++;
    }
    ST_RESTORE();
}
int  omdbClientDlg::getOmdbAllStart()
{
    LOGDEBUG("");
    ST_SAVE(ST_OMDB_REQUEST_ALL);
    if (isApiKeySet == false)
    {
        ST_RESTORE();
        return -1;
    }
    
    short count = lstMovies->GetItemCount();
    MovieFile file;

    progressCounter[CNT_TOTAL] = 0;
    progressCounter[CNT_SUCCESS] = 0;
    progressCounter[CNT_ERROR] = 0;

    for (short pos = 0; pos < count; pos++)
    {
        getFile(pos);
        selectedFile.fileNameToDetails();
        selectedFile.initMovieValue();

        if ((forceOmdb == true) && selectedFile.isImdbId() == true)
        {
            progressCounter[CNT_TOTAL]++;
        }
        if (selectedFile.isImdbId() == false)
            progressCounter[CNT_TOTAL]++;
    }

    for (short pos = 0; pos < count; pos++)
    {
        if (stopThreads == true)
        {
            stopThreads = false;
            progressCounter[CNT_ERROR] = 0;
            progressCounter[CNT_SUCCESS] = 0;
            progressCounter[CNT_TOTAL] = 0;
            break;
        }

        if (isApiKeySet == false)
            break;

        getFile(pos);

        selectedFile.fileNameToDetails();
        selectedFile.initMovieValue();
        
        if ((forceOmdb == true) && selectedFile.isImdbId() == true)
        {
            if (GETDB.getMovieData(&selectedFile))
            {
                progressCounter[CNT_SUCCESS]++;
                continue;
            }
        }

        //if ((forceOmdb == false) && (selectedFile.imdbId.size() == 1))
        //    continue;
        //omdbCountTotal++;
        omdbRequest(&selectedFile);
    }
    ST_RESTORE();
    return progressCounter[CNT_SUCCESS];
}
bool omdbClientDlg::omdbRequest(MovieFile *_file)
{
    LOGDEBUG("");
    ST_SAVE(ST_OMDB_REQUEST_SINGLE);
    //progressCounter[CNT_TOTAL] = 1;
    bool ret = false;
    if (selectedFile.isImdbId() == true && GETDB.getMovieData(&selectedFile))
    {
        ret = true;
    }
    else
    {
        textDebugWindow("OmdbRequest: %s", _file->title.c_str());
        omdb.search(_file);

        ret = xmlFiles.loadOmdbStr(_file);
        //omdbCountTotal++;
        if (ret)
        {
            textDebugWindow("OmdbRequest: OK", _file->title);
            progressCounter[CNT_SUCCESS]++;
            if (progressCounter[CNT_SUCCESS] > progressCounter[CNT_TOTAL])
                progressCounter[CNT_TOTAL] = progressCounter[CNT_SUCCESS];
            exLOGINFO("Downloaded %s", _file->getFilename().c_str());
        }
        else
        {
            textDebugWindow("OmdbRequest: KO", _file->title);
            progressCounter[CNT_ERROR]++;
            xmlFiles.readOmdbError(_file);
            _file->getMovie()->setPlot(_file->getOmdbXml());
            exLOGINFO("ERROR Downloading %s", _file->getFilename().c_str());
            if (_file->getOmdbXml() == "Invalid API key!")
            {
                isApiKeySet = false;
                showApiKey();
            }
        }
        updateLstMovies(_file);

        bool fields[FS_MAX_FIELDS - FS_ID];
        for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
            fields[i] = false;
        fields[FS_IMDBID - FS_ID] = true;

        GETDB.MOVIESFS_update(_file, fields);


    }
    ST_RESTORE();
    return ret;
}
int  omdbClientDlg::onOmdbReadFinish()
{
    LOGDEBUG("");
    ST_SET(ST_STANDBY);
    return 1;
}
static DWORD WINAPI getOmdbAllThread(void* Param)
{
    LOGDEBUG("");
    omdbClientDlg* This = (omdbClientDlg*)Param;
    return This->getOmdbAllStart();
}
void omdbClientDlg::omdbSingleRequest()
{
    LOGDEBUG("");
    ST_SAVE(ST_OMDB_REQUEST_SINGLE);
    if (!isApiKeySet)
        changeOmdbKey();
    progressCounter[CNT_TOTAL] = 1;
    if (isApiKeySet)
    {
        selectedFile.fileNameToDetails();
        selectedFile.initMovieValue();
        if (!forceOmdb && GETDB.getMovieData(selectedFile.getMovie()))
        {
            return;
        }

        if (omdbRequest(&selectedFile))
            setStaticText(STATIC_DESC, "1 found");
        else
            setStaticText(STATIC_DESC, selectedFile.getOmdbXml());
        UpdateDisplay(&selectedFile);
    } else
        setStaticText(STATIC_DESC, "noApiKey");
    ST_RESTORE();

}
void omdbClientDlg::omdbAllRequest()
{
    LOGDEBUG("");
    ST_SAVE(ST_OMDB_REQUEST_ALL);
    DWORD  hThreadIdArray;

    if (!isApiKeySet)
        changeOmdbKey();

    if (isRunning(hThreadOmdbAll))
        return;

    if (isApiKeySet)
    {
        hThreadOmdbAll = CreateThread(NULL, 0, getOmdbAllThread, (void*) this, 0, &hThreadIdArray);
    }
    else
        setStaticText(STATIC_DESC, "noApiKey");

}
void omdbClientDlg::omdbAllStop(void)
{
    LOGDEBUG("");
    if (isRunning(hThreadOmdbAll))
    {
        ST_SET(ST_STANDBY);
        stopThreads = true;
    }
}
void omdbClientDlg::showApiKey()
{
    LOGDEBUG("");
    if (isApiKeySet == false)
    {
        autoOmdb = false;
        forceOmdb = false;
    }

    btnOmdb[CHK_OMDB_FORCE]->EnableWindow(isApiKeySet);
    btnOmdb[CHK_OMDB_AUTO]->EnableWindow(isApiKeySet);

    showText(STATIC_DESC, isApiKeySet);

        std::stringstream ss;
        if (isApiKeySet)
        {
            ss << "Omdb Key : " << GETCM.getConfigStr(CONF_OMDB_APIKEY);
        }
        else
        {
            ss << "No Omdb API Key";
        }
        setStaticText(STATIC_KEY, ss.str());
}
void omdbClientDlg::changeOmdbKey(void)
{
    LOGDEBUG("");
    OmdbSetkey();
    showApiKey();
}
bool omdbClientDlg::OmdbSetkey()
{
    LOGDEBUG("");
    setOmdbKey dlgOmdbKey;
    string texts[3] = { "Please, enter your OMDB Key.", "If you don't have, request one at", "Key :" };
    string tmpApikey = GETCM.getConfigStr(CONF_OMDB_APIKEY);
    string newApikey = tmpApikey;
    dlgOmdbKey.setValue(&newApikey);
    dlgOmdbKey.setTexts(texts);
    //std::string url = xmlConfig.getOmdbConfig()->omdbUrl + xmlConfig.getOmdbConfig()->apiRequestUrl;
    std::string url = GETCM.getConfigStr(CONF_OMDB_URL) + GETCM.getConfigStr(CONF_OMDB_LISTSEPARATOR);
    //LPCWSTR LPCWSTRurl = C2WC(url.c_str());
    wchar_t * wurl = C2WC(url.c_str());
    dlgOmdbKey.setURL(wurl);
    dlgOmdbKey.DoModal();
    //xmlConfig.setXmlLocalConfig(XML_LOCAL_APIKEY, newApikey);
    GETCM.setConfigValue(CONF_OMDB_APIKEY, newApikey);
    LOGINFO("Omdb Key set to [%s]", newApikey.c_str());
    isApiKeySet = !newApikey.empty();
    setStaticText(STATIC_DESC, "");
    return (tmpApikey.compare(newApikey) != 0);
}
#pragma endregion omdb

#pragma region filters
void omdbClientDlg::filterComboSeriesAnims(filterTypes _filter)
{
    LOGDEBUG("");
    CString c_value;
    int cursor = comboFilters[_filter]->GetCurSel();
    comboFilters[_filter]->GetLBText(cursor, c_value);

    wchar_t first = c_value.GetAt(0);
    if (first == L'A') //ALL
        filters->valueSelected[_filter] = TYPE_ALL;
    if (first == L'Y') //Yes
        filters->valueSelected[_filter] = TYPE_NOTMOVIE;
    if (first == L'N') //No
        filters->valueSelected[_filter] = TYPE_MOVIE;

    loadMoviesFS();
    loadAllCombosExcept(_filter);
    SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
}
string omdbClientDlg::changeCombo(filterTypes _filter)
{
    LOGDEBUG("");
    CString c_text;
    string text;

    int cursor = comboFilters[_filter]->GetCurSel();
    if (cursor < 0)
        return "";
    comboFilters[_filter]->GetLBText(cursor, c_text);

    std::wstring w_c_text((LPCTSTR)c_text);
    std::string SelectedValue = WS2S(w_c_text);
    GETDB.setFilterSelectedValue(_filter, SelectedValue);
    if (ftypeSave[_filter] == true)
    {
        GETCM.setConfigValue(_filter, filters->nameSelected[_filter]);
    }
    loadMoviesFS();
    loadAllCombosExcept(_filter);
    GotoDlgCtrl(GetDlgItem(IDC_MOVIE_LIST));
    SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
    return SelectedValue;
}
string omdbClientDlg::changeComboFolders()
{
    LOGDEBUG("");
    ST_SAVE(ST_SELECT_FOLDER);
    CString c_text;
    string text;

    int cursor = comboFolderPath->GetCurSel();
    if (cursor < 0)
        return "";
    comboFolderPath->GetLBText(cursor, c_text);

    std::wstring w_c_text((LPCTSTR)c_text);
    std::string SelectedValue = WS2S(w_c_text);
    std::string actualValue = GETCM.getConfigStr(CONF_APP_CURRENTFOLDER);
    GETCM.setConfigValue(CONF_APP_CURRENTFOLDER, SelectedValue);

    if (ST_PREDIFF(ST_INITIATING))
    {
        int folderId = GETDB.getPathId(SelectedValue);
        if (folderId != -1)
            filters->folderFilterId = folderId;
        //xmlConfig.saveLocalConfigFile(false);
        selectedFile.setPath(SelectedValue);
        readFolderIfEmpty();
    }

    GotoDlgCtrl(GetDlgItem(IDC_MOVIE_LIST));
    ST_RESTORE();
    return SelectedValue;
}
void omdbClientDlg::checkMovieTypes(DataBase::column _col)
{
    LOGDEBUG("");
    GotoDlgCtrl(GetDlgItem(IDC_MOVIE_LIST));
    movieInfoTypes infoType = col2InfoType(_col);
    selectedFile.typeIs[infoType] = (checkIs[infoType]->GetCheck() == 1) ? TYPE_NOTMOVIE : TYPE_MOVIE;

    bool fields[FS_MAX_FIELDS - FS_ID];
    for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
        fields[i] = false;
    fields[FS_SERIE - FS_ID + infoType] = true;

    GETDB.MOVIESFS_update(&selectedFile, fields);
    std::wstringstream wss;
    wss.str(L"");
    wss << selectedFile.typeIs[infoType];
    lstMovies->SetItemText(selectedFile.LstPosition, _col, wss.str().c_str());
    loadFilters(col2FilterTypes(_col));
}
void omdbClientDlg::checkInvertFilters(DataBase::invertFilters _filter)
{
    LOGDEBUG("");
    filters->invertFilters[_filter] = (checkInvert[_filter]->GetCheck() == 1) ? true : false;
    loadMoviesFS();
    loadAllCombosExcept();
    SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
}
void omdbClientDlg::resetFilters()
{
    LOGDEBUG("");
    resetFilters(FILTERS_MAX);
}
void omdbClientDlg::resetFilters(filterTypes _filter)
{
    LOGDEBUG("");
    if (_filter == FILTERS_MAX)
    {
        for (short i = FILTERS_DIRECTORS; i < FILTERS_MAX; i++)
        {
            filterTypes f = (filterTypes)i;
            comboFilters[f]->SetCurSel(0);
        }
        changeCombo(FILTERS_DIRECTORS);
    }
    else
    {
        comboFilters[_filter]->SetCurSel(0);
        changeCombo(_filter);
    }
}
#pragma endregion filters

#pragma region typesAndConvertions
CString omdbClientDlg::stringToCString(std::string _str) {
    CString cstr(_str.c_str());
    return cstr;
}
CString omdbClientDlg::intToCString(int _i) {
    CString cstr;
    cstr.Format(L"%d", _i); // search for _itot_s performance
    return cstr;
}
std::string omdbClientDlg::CStringToString(CString _cstr)
{
    CT2CA psz(_cstr);
    //std::string(psz);
    return std::string(psz);
}
int omdbClientDlg::CStringToInt(CString _cstr)
{
    return _wtoi(_cstr);
}
std::string omdbClientDlg::GetEditString(CEdit *_edit)
{
    CString cstring;
    _edit->GetWindowText(cstring);
    return CStringToString(cstring);
}
int omdbClientDlg::GetEditInt(CEdit *_edit)
{
    CString cstring;
    _edit->GetWindowText(cstring);
    return _wtoi(cstring);
}
movieInfoTypes omdbClientDlg::col2InfoType(DataBase::column _col)
{
    switch (_col)
    {
    case DataBase::COL_SERIE:
        return TYPE_SERIE;
        break;
    case DataBase::COL_ANIM:
        return TYPE_ANIM;
        break;
    case DataBase::COL_DOC:
        return TYPE_DOC;
        break;
    case DataBase::COL_SHORT:
        return TYPE_SHORT;
        break;
    default:
        break;
    }
    return TYPE_MAX;
}
filterTypes omdbClientDlg::col2FilterTypes(DataBase::column _col)
{
    switch (_col)
    {
    case DataBase::COL_SERIE:
        return FILTERS_SERIE;
        break;
    case DataBase::COL_ANIM:
        return FILTERS_ANIM;
        break;
    case DataBase::COL_DOC:
        return FILTERS_DOC;
        break;
    case DataBase::COL_SHORT:
        return FILTERS_SHORT;
        break;
    default:
        break;
    }
    return FILTERS_MAX;
}
#pragma endregion typesAndConvertions

#pragma region threads
bool omdbClientDlg::isRunning(HANDLE  _hThread)
{
    DWORD exitCode = 0;
    if (_hThread)
        GetExitCodeThread(_hThread, &exitCode);
    if (exitCode == STILL_ACTIVE)
        return true;
    return false;
}
#pragma endregion threads

#pragma region config
void omdbClientDlg::initFolder()
{
    std::string path = GETDB.getPath(1);
    if (path.empty() == false)
        GETCM.setConfigValue(CONF_APP_CURRENTFOLDER, path);
    else
        selectFolder();
}
void omdbClientDlg::selectFolder()
{
    ST_SAVE(ST_SELECT_FOLDER);
    LOGDEBUG("");
    CFolderPickerDialog dlgFolder;
    bool exit = false;

    GotoDlgCtrl(GetDlgItem(IDC_MOVIE_LIST));

    while (exit == false)
    {
        if (dlgFolder.DoModal() == IDOK)
        {
            std::string path = CStringToString(dlgFolder.GetPathName()) + "\\";
            LOGDEBUG("Folder: [%s]", path.c_str());
            //if (ST_PREDIFF(ST_INITIATING))
            //{
                GETDB.PATHFS_insert(path);
            //}

            //editFolderPath->SetWindowTextW(CString(path.c_str()));
            GETCM.setConfigValue(CONF_APP_CURRENTFOLDER, path);
            //xmlConfig.setXmlLocalConfig(XML_LOCAL_PATH, path);
            //xmlConfig.saveLocalConfigFile(false);
            
            //if (ST_PREDIFF(ST_INITIATING))
            //{
                LOGDEBUG("loading Folders");
                loadFolders();
            //}
            exit = true;

        }
        else
            exit = true;
    }
    ST_RESTORE();

    if (GETDB.isConnected() == false)
    {
        LOGINFO("Can't connnect to DB");
        MBOX("This app will now close", "Info", MB_OK);
        onExit();
        CDialogEx::OnCancel();
    } else if (ST_PREDIFF(ST_INITIATING))
    {
        selectedFile.resetFile();
        //getFileFromSelection(false);
        if (selectedFile.getFilename().empty() == false)
            UpdateDisplay();
    }
}
void omdbClientDlg::showColumnSelector()
{
    ShowColumns dlgShowCols;
    dlgShowCols.setParams(columnsToSHow, lstMovies, columnSizes);
    ST_SAVE(ST_SELECT_COLUMNS);
    dlgShowCols.DoModal();
    ST_RESTORE();
}
void omdbClientDlg::selectVideoPlayer()
{
    //return false;
    LOGDEBUG("");
    CString defaultName = L"";
    CString defaultExtension = L"exe";
    CString extensions = L"exe";

    CFileDialog dlg(
        true,                                   // true for File Open dialog box
        defaultExtension,               // The default file name extension
        NULL,                    // The default file name
        OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,    // bunch of flags http://msdn.microsoft.com/en-us/library/wh5hz49d.aspx
        NULL
    );

    ST_SAVE(ST_VPLAYER);
    auto result = dlg.DoModal();
    ST_RESTORE();

    //if (result != IDOK) return false; // failed
    if (result == IDOK)
    {
        LOGINFO("VLC:%s", CStringToString(dlg.GetPathName()).c_str());
        GETCM.setConfigValue(CONF_APP_VIDEOPLAYER, CStringToString(dlg.GetPathName()));
        //xmlConfig.setXmlLocalConfig(XML_LOCAL_VIDEOPLAYER, CStringToString(dlg.GetPathName()));
        //xmlConfig.saveLocalConfigFile(false);
    }
    //return true;
}
#pragma endregion config

#pragma region OnList
void omdbClientDlg::OnLvnItemchangedMovieList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    ASSERT(pNMLV);
    //globalState = globalState;
    if (pNMLV->uChanged & LVIF_STATE && ST_DIFF(ST_READING_FODLER))
    {
        if (pNMLV->uNewState & LVIS_SELECTED)
        {
            ST_SAVE(ST_UPDATE_DISPLAY);
            getFileFromSelection();
            editFolderPath->SetWindowTextW(CString(selectedFile.getPath().c_str()));
            UpdateDisplay();
            ST_RESTORE();
        }
    }

    *pResult = 0;
}
void omdbClientDlg::OnLvnColumnClickMovieList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    ASSERT(pNMLV);
    filters->orderByAsc = !filters->orderByAsc;
    filters->orderByColumn = pNMLV->iSubItem + 1;
    LOGDEBUG("column [%s] pressed", columNames[pNMLV->iSubItem]);
    loadMoviesFS();
    *pResult = 0;
    SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
}
void omdbClientDlg::OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVCUSTOMDRAW* pLV = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    *pResult = 0;
    switch (pLV->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        int pos = pLV->nmcd.dwItemSpec;
        getFile(pos);
        bool exits = selectedFile.exists;
        COLORREF crText;

        //if ((pos % 2) == 0)
        //    crText = RGB(191, 255, 202); //soft green
        //else
        //    crText = RGB(255, 208, 115); // pastel
        //                                 // Store the color back in the NMLVCUSTOMDRAW struct.
        //                                 //pLV->clrTextBk = crText;
        //pLV->clrText = crText;

        if (!exits)
        {
            crText = RGB(255, 128, 128);
            pLV->clrTextBk = crText;
        }

        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
        //
        //COLORREF backgroundColor = RGB(255, 0, 0);
        ////lstMovies->SetTextBkColor(backgroundColor);
        //pLV->clrTextBk = RGB(255, 0, 0);
        //*pResult = CDRF_DODEFAULT;
        break;
    }
}
void omdbClientDlg::OnListViewMouseInput(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    ASSERT(pNMLV);
    if (pNMLV->uChanged & LVIF_STATE)
    {

        //if ((pNMLV->uNewState ^ pNMLV->uOldState) & LVIS_SELECTED)
        if (pNMLV->uNewState & LVIS_SELECTED)
        {
            int a = 0;
        }
    }

    *pResult = 0;
}
#pragma endregion OnList

#pragma region OnBnClicked
void omdbClientDlg::OnBnClickedOmdbRequest()
{
    LOGDEBUG("button [OmdbRequest] pressed");
    omdbSingleRequest();
}
void omdbClientDlg::OnBnClickedButtonReadfolder()
{
    LOGDEBUG("button [Readfolder] pressed");
    readFolder();
}
void omdbClientDlg::OnBnClickedButtonLoadxmlfile()
{
    LOGDEBUG("button [Loadxmlfile] pressed");
    loadXmlfile();
}
void omdbClientDlg::OnBnClickedCancel()
{
    LOGDEBUG("button [EXIT] pressed");
    onExit();
}
void omdbClientDlg::OnBnClickedCheckShowdeleted()
{
    //showDeleted = (checkShowDeleted->GetCheck() == 1);
    filters->exists = (checkShowDeleted->GetCheck() == 0);
    LOGDEBUG("Filter [showDeleted] ", filters->exists ? "Uncheck" : "Check");
    //setstaticLoadDB(loadMoviesFS(FS_FILENAME, sortingOrder));
    //    filters->orderByColumn = COL_NAME + 1;
    loadMoviesFS();
    SetTimer(_REFRESH_UI_EVENT_, 10, NULL);
}
void omdbClientDlg::OnBnClickedButtonOmdbAll()
{
    LOGDEBUG("button [OmdbAll] pressed");
    omdbAllRequest();
}
void omdbClientDlg::OnBnClickedButtonWritexmlfile()
{
    LOGDEBUG("button [Writexmlfile] pressed");
    saveXmlfile();
}
void omdbClientDlg::OnBnClickedCheckForce()
{
    forceOmdb = (btnOmdb[CHK_OMDB_FORCE]->GetCheck() == 1);
    LOGDEBUG("Filter [forceOmdb] %s", forceOmdb ? "Check" : "Uncheck");
    GotoDlgCtrl(GetDlgItem(IDC_MOVIE_LIST));
    setBtnOmdbSingleState();
}
void omdbClientDlg::OnBnClickedCheckAutoOmdb()
{
    autoOmdb = (btnOmdb[CHK_OMDB_AUTO]->GetCheck() == 1);
    LOGDEBUG("Filter [autoOmdb] %s", autoOmdb ? "Check" : "Uncheck");
    GotoDlgCtrl(GetDlgItem(IDC_MOVIE_LIST));
    setBtnOmdbSingleState();
}
void omdbClientDlg::OnBnClickedButtonOmdbSetkey()
{
    LOGDEBUG("button [OmdbSetkey] pressed");
    changeOmdbKey();
}
void omdbClientDlg::OnBnClickedCheckSerie()
{
    checkMovieTypes(DataBase::COL_SERIE);
    LOGDEBUG("check [isSerie] %s", selectedFile.typeIs[TYPE_SERIE] ? "check" : "uncheck");
}
void omdbClientDlg::OnBnClickedCheckAnim()
{
    checkMovieTypes(DataBase::COL_ANIM);
    LOGDEBUG("check [isSerie] %s", selectedFile.typeIs[TYPE_ANIM] ? "check" : "uncheck");
}
void omdbClientDlg::OnBnClickedCheckDoc()
{
    checkMovieTypes(DataBase::COL_DOC);
    LOGDEBUG("check [isSerie] %s", selectedFile.typeIs[TYPE_DOC] ? "check" : "uncheck");
}
void omdbClientDlg::OnBnClickedCheckShort()
{
    checkMovieTypes(DataBase::COL_SHORT);
    LOGDEBUG("check [isSerie] %s", selectedFile.typeIs[TYPE_SHORT] ? "check" : "uncheck");
}
void omdbClientDlg::OnBnClickedButtonPlay()
{
    if (selectedFile.getFilename().empty() == true)
        return;

    //string videoPlayer = xmlConfig.getXmlLocalConfig(XML_LOCAL_VIDEOPLAYER);
    string videoPlayer = GETCM.getConfigStr(CONF_APP_VIDEOPLAYER);
    
    if (videoPlayer.empty())
    {
        selectVideoPlayer();
    }

    videoPlayer += " \"" + selectedFile.getFullPath() + "\"";

    // additional information
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(si);

    // set the size of the structures
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    if (!CreateProcessA(NULL,   // the path
        (LPSTR)videoPlayer.c_str(),      // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    ))
    {
        LOGERROR("Can't launch app: %d", videoPlayer.c_str());
        MessageBoxA(NULL, "Can't launch multimedia app", "Error", MB_ICONERROR | MB_OK);
    }
    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
void omdbClientDlg::OnBnClickedClearmovies()
{
    LOGDEBUG("button [Clearmovies] pressed");
    dbClearMovies();
}
void omdbClientDlg::OnBnClickedClearfs()
{
    LOGDEBUG("button [Clearfs] pressed");
    dbClearMoviesFS();
}
void omdbClientDlg::OnBnClickedCheckGenres()
{
    checkInvertFilters(DataBase::INVERT_FILTERS_GENRES);
}
void omdbClientDlg::OnBnClickedCheckGenres2()
{
    checkInvertFilters(DataBase::INVERT_FILTERS_GENRES2);
}
void omdbClientDlg::OnBnClickedCheckDirectors()
{
    checkInvertFilters(DataBase::INVERT_FILTERS_DIRECTORS);
}
void omdbClientDlg::OnBnClickedCheckActors()
{
    checkInvertFilters(DataBase::INVERT_FILTERS_ACTORS);
}
void omdbClientDlg::OnBnClickedButtonResetDirectors()
{
    LOGDEBUG("button [Reset] Directors");
    resetFilters(FILTERS_DIRECTORS);
}
void omdbClientDlg::OnBnClickedButtonResetActors()
{
    LOGDEBUG("button [Reset] Actors");
    resetFilters(FILTERS_ACTORS);

}
//void omdbClientDlg::OnEnChangeEditFolder()
//{
//    selectFolder();
//    
//}
void omdbClientDlg::OnStnClickedStaticApikey()
{
    LOGDEBUG("button [OmdbSetkey] pressed");
    changeOmdbKey();
}

#pragma endregion OnBnClicked

#pragma region OnCbnSelchange
void omdbClientDlg::OnCbnSelchangeComboGenres()
{
    LOGDEBUG("button [ComboGenre] <%s> selected", changeCombo(FILTERS_GENRES).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboGenres2()
{
    LOGDEBUG("button [ComboGenre2] <%s> selected", changeCombo(FILTERS_GENRES2).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboDirector()
{
    LOGDEBUG("button [ComboDirector] <%s> selected", changeCombo(FILTERS_DIRECTORS).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboActors()
{
    LOGDEBUG("button [ComboActor] <%s> selected", changeCombo(FILTERS_ACTORS).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboYearmin()
{
    LOGDEBUG("button [ComboYearMin] <%s> selected", changeCombo(FILTERS_YEARMIN).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboYearmax()
{
    LOGDEBUG("button [ComboYearMax] <%s> selected", changeCombo(FILTERS_YEARMAX).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboImdbratingmin()
{
    LOGDEBUG("button [ComboYearMax] <%s> selected", changeCombo(FILTERS_IMDBRATINGMIN).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboImdbratingmax()
{
    LOGDEBUG("button [ComboYearMax] <%s> selected", changeCombo(FILTERS_IMDBRATINGMAX).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboSeries()
{
    LOGDEBUG("button [ComboSerie] <%s> selected", changeCombo(FILTERS_SERIE).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboAnims()
{
    LOGDEBUG("button [ComboAnim] <%s> selected", changeCombo(FILTERS_ANIM).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboDocs()
{
    LOGDEBUG("button [ComboDoc] <%s> selected", changeCombo(FILTERS_DOC).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboShorts()
{
    LOGDEBUG("button [ComboShort] <%s> selected", changeCombo(FILTERS_SHORT).c_str());
}
void omdbClientDlg::OnCbnSelchangeComboFolders()
{
    LOGDEBUG("button [ComboShort] <%s> selected", changeComboFolders().c_str());
}

#pragma endregion OnCbnSelchange





