#define IDM_DIALOG_SETKEY               0x0020
#define IDM_CHANGE_FOLDER               0x0030
#define IDM_DIALOG_COLUMNS              0x0040
#define IDM_DIALOG_VPLAYER              0x0050
#define IDM_READFOLDER                  0x0060
#define IDM_RESETFS                     0x0070
#define IDM_RESETMOVIES                 0x0080
#define IDM_EXIT                        0x0090
#define IDC_BUTTON_OMDB_REQUEST         0x00A0
#define IDC_BUTTON_OMDB_ALL             0x00B0
#define IDC_BUTTON_LOADXMLFILE          0x00C0
#define IDC_BUTTON_WRITEXMLFILE         0x00D0
#define IDC_BUTTON_EDIT_IMDBID          0x00E0
#define IDC_BUTTON_EDIT_FNAME           0x00F0
#define IDC_BUTTON_OPEN_IMDBWEB         0x0100
#define IDC_BUTTON_FILTER_DIRECTORS     0x0110
#define IDC_BUTTON_FILTER_ACTORS        0x0150
#define IDC_DIALOG_DEBUG_WINDOW         0x0200
#define IDC_BUTTON_OMDB_ALL_STOP        0x0210
#define IDC_BUTTON_RESET_FILTERS        0x0220
#define IDC_STOP_READFOLDER             0x0230
#define MENU_RELOAD_ALL                 0x0240
#define MENU_RELOAD_DIRECTORS           0x0250
#define MENU_RELOAD_WRITERS             0x0260
#define MENU_RELOAD_ACTORS              0x0270
#define MENU_RELOAD_GENRE               0x0280
#define MENU_RELOAD_GENRE2              0x0290
#define MENU_RELOAD_YEARMIN             0x02A0
#define MENU_RELOAD_YEARMAX             0x02B0
#define MENU_RELOAD_IMDBRATINGMIN       0x02C0
#define MENU_RELOAD_IMDBRATINGMAX       0x02D0
#define MENU_RELOAD_SERIE               0x02E0
#define MENU_RELOAD_ANIM                0x02F0
#define MENU_RELOAD_DOC                 0x0300
#define MENU_RELOAD_SHORT               0x0310
#define IDC_MENU_OPEN_EXPLORER          0x0320
#define IDC_MENU_PLAY_MOVIE             0x0330
#define IDC_BUTTON_OMDB_SEARCH          0x0340


#define MENU_FOLDER             MF_STRING, IDM_CHANGE_FOLDER, L"&Add Folder", &omdbClientDlg::selectFolder
#define MENU_READFS             MF_STRING, IDM_READFOLDER, L"&Read Folder", &omdbClientDlg::readFolder
#define MENU_STOPREADFS         MF_STRING, IDC_STOP_READFOLDER, L"&Stop Reading", &omdbClientDlg::stopReadFolder
#define MENU_SAVEDATA           MF_STRING, (UINT_PTR)idcXmlFile[BTN_XMLFILE_WRITE], L"&Save data", &omdbClientDlg::saveXmlfile
#define MENU_LOADDATA           MF_STRING, (UINT_PTR)idcXmlFile[BTN_XMLFILE_READ], L"&Load data", &omdbClientDlg::loadXmlfile
#define MENU_EXIT               MF_STRING, IDM_EXIT, L"&Exit", &omdbClientDlg::onExit

#define MENU_OMDBSETKEY         MF_STRING, IDM_DIALOG_SETKEY, L"&Change Omdb Key", &omdbClientDlg::changeOmdbKey
#define MENU_OMDB1REQ           MF_STRING, (UINT_PTR)idcOmdb[BTN_OMDB_SINGLE], L"Omdb Request", &omdbClientDlg::omdbSingleRequest
#define MENU_OMDBSEARCH         MF_STRING, (UINT_PTR)idcOmdb[BTN_OMDB_SEARCH], L"Omdb Search", &omdbClientDlg::showSearchResult
#define MENU_OMDBXREQ           MF_STRING, (UINT_PTR)idcOmdb[BTN_OMDB_ALL], L"&All Request", &omdbClientDlg::omdbAllRequest
#define MENU_OMDBSTOP           MF_STRING, (UINT_PTR)idcOmdb[BTN_OMDB_ALL_STOP], L"&All Stop", &omdbClientDlg::omdbAllStop

#define MENU_EDIT_IMDBID        MF_STRING, IDC_BUTTON_EDIT_IMDBID, L"&Edit ImdbId", &omdbClientDlg::editImdbId
#define MENU_EDIT_FILENAME      MF_STRING, IDC_BUTTON_EDIT_FNAME, L"&Edit File Name", &omdbClientDlg::editFileName
#define MENU_OPEN_WEB           MF_STRING, IDC_BUTTON_OPEN_IMDBWEB, L"&Open Imdb Web", &omdbClientDlg::openImdbWeb
#define MENU_OPEN_EXPLORER      MF_STRING, IDC_MENU_OPEN_EXPLORER, L"&Show in Windows Explorer", &omdbClientDlg::openExplorer
#define MENU_PLAY_MOVIE         MF_STRING, IDC_MENU_PLAY_MOVIE, L"&Play Movie", &omdbClientDlg::playMovie


#define MENU_SELECTCOLUMNS      MF_STRING, IDM_DIALOG_COLUMNS, L"Select &Colums to show", &omdbClientDlg::showColumnSelector
#define MENU_SELECTPLAYER       MF_STRING, IDM_DIALOG_VPLAYER, L"Select &Video Player", &omdbClientDlg::selectVideoPlayer
#define MENU_DBGWND             MF_STRING, IDC_DIALOG_DEBUG_WINDOW, L"&Debug Window", &omdbClientDlg::showDebugWindow

#define MENU_RESETTABFS         MF_STRING, IDM_RESETFS, L"Reset &FS Table", &omdbClientDlg::dbClearMoviesFS
#define MENU_RESETTABMOVIES     MF_STRING, IDM_RESETMOVIES, L"Reset &Movies Table", &omdbClientDlg::dbClearMovies

#define MENU_HELP_INDEX         MF_STRING, ID_HELP_INDEX, L"Index"
#define MENU_HELP_FINDER        MF_STRING, ID_HELP_FINDER, L"Finder"
#define MENU_HELP_USING         MF_STRING, ID_HELP_USING, L"Using"
#define MENU_HELP_ABOUT         MF_STRING, ID_APP_ABOUT, L"About", &omdbClientDlg::onAbout

#define MENU_FILTERS_RESET      MF_STRING, IDC_BUTTON_RESET_FILTERS, L"Reset Filters", &omdbClientDlg::resetFilters
#define MENU_FILTERS_DIRECTORS  MF_STRING, (UINT_PTR)subMenuFilter[SUBMENUFILTER_DIRECTORS], L"-- Directors --"
#define MENU_FILTERS_ACTORS     MF_STRING, (UINT_PTR)subMenuFilter[SUBMENUFILTER_ACTORS], L"-- Actors --"

#define MENU_COMBO_LOAD_ALL     MF_STRING, MENU_RELOAD_ALL, L"Reload All", &omdbClientDlg::loadAllCombos
#define MENU_COMBO_LOAD_DIR     MF_STRING, MENU_RELOAD_DIRECTORS, L"Reload Directors", &omdbClientDlg::loadFilterDirectors
#define MENU_COMBO_LOAD_WRI     MF_STRING, MENU_RELOAD_WRITERS, L"Reload Writers", &omdbClientDlg::loadFilterWriters
#define MENU_COMBO_LOAD_ACT     MF_STRING, MENU_RELOAD_ACTORS, L"Reload Actors", &omdbClientDlg::loadFilterActors
#define MENU_COMBO_LOAD_GEN     MF_STRING, MENU_RELOAD_GENRE, L"Reload Genres", &omdbClientDlg::loadFilterGenres
#define MENU_COMBO_LOAD_GE2     MF_STRING, MENU_RELOAD_GENRE2, L"Reload Genres2", &omdbClientDlg::loadFilterGenres2 
#define MENU_COMBO_LOAD_YMN     MF_STRING, MENU_RELOAD_YEARMIN, L"Reload YearMin", &omdbClientDlg::loadFilterYearMin
#define MENU_COMBO_LOAD_YMX     MF_STRING, MENU_RELOAD_YEARMAX, L"Reload YearMax", &omdbClientDlg::loadFilterYearMax
#define MENU_COMBO_LOAD_IRMN    MF_STRING, MENU_RELOAD_IMDBRATINGMIN, L"Reload ImdbRatingMin", &omdbClientDlg::loadFilterImdbRatingMin
#define MENU_COMBO_LOAD_IRMX    MF_STRING, MENU_RELOAD_IMDBRATINGMAX, L"Reload ImdbRatingMax", &omdbClientDlg::loadFilterImdbRatingMax
#define MENU_COMBO_LOAD_SERI    MF_STRING, MENU_RELOAD_SERIE, L"Reload Serie", &omdbClientDlg::loadFilterSerie
#define MENU_COMBO_LOAD_ANIM    MF_STRING, MENU_RELOAD_ANIM, L"Reload Anim", &omdbClientDlg::loadFilterAnim
#define MENU_COMBO_LOAD_DOC     MF_STRING, MENU_RELOAD_DOC, L"Reload Doc", &omdbClientDlg::loadFilterDoc
#define MENU_COMBO_LOAD_SHOR    MF_STRING, MENU_RELOAD_SHORT, L"Reload Short", &omdbClientDlg::loadFilterShort