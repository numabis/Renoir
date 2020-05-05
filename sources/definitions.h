#ifndef _DEFINICIONES_H_
#define _DEFINICIONES_H_
#pragma once

#define NEWLINE             "\r\n"
#ifdef _WSTRING
#define S2WS                BUTIL::Xml::utf8_wstring
#else
#define S2WS                
#endif
#ifdef _WSTRING
#define WS2S                BUTIL::Xml::wstring_utf8
#else
#define WS2S                BUTIL::Convert::wstring2string
#endif
#define C2WC(wc)            (wchar_t *)BUTIL::Convert::charToWchar((char*)wc)
#define WC2C                BUTIL::Convert::wcharToChar

#define STR2INT             BUTIL::Util::stringToInt

#define MBOX                BUTIL::Util::msgBox

#define DEFAULTSEP ", "
#define OMDBNOTFOUND    "?"
#define OMDBTT          "tt"
#define CNT_TOTAL           0
#define CNT_READ            1
#define CNT_SUCCESS         1
#define CNT_ERROR           2
#define CNT_MAX_READ        2
#define CNT_FIELD           3
#define MIN             0
#define MAX             1
#define MIN_YEAR        1900
#define MAX_YEAR        2099
#define MIN_IMDBRATING  -100
#define MAX_IMDBRATING  100
#define MIN_OCCURRENCES 1
#define _MAX_LIST   10


#define PPCAT(T1, T2)	T1 ## T2
#define FOLDERSEPARATOR "\\"
#define MOVIEDB         "movieDB"
#define CONFIGPATH		"Config" FOLDERSEPARATOR
#define CACHEPATH		"Cache"
#define	CONFIGFILE		CONFIGPATH "config.xml"
#define	DBGCONFIGFILE	CONFIGPATH "configDebug.xml"
//#if defined(_DEBUG) || defined(_DEMO)
#define	LOCALCONFIGFILE	CONFIGPATH MOVIEDB ".xml"
//#else
//#define	LOCALCONFIGFILE	CONFIGPATH MOVIEDB ".zxml"
//#endif
#define DBNAME          MOVIEDB ".s3db"
#define XMLLOGNAME      "logXML"
#define DBRESETDEFAULT  false
#define MAX_COL_TAB 20

#define MAX_GENRES  50
#define MAX_DIRECTORS 1
#define MAX_WRITERS  1
#define MAX_ACTORS  10
#define PATHSIZE   128

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define CAT(A, B)   A##B
#define L(A)  CAT(L, #A)

#define STR std::to_string
#define GETDB         DataBase::getInstance()
#define GETLOG        BUTIL::exLog::getInstance()

//#define SERIES      "series"
//#define EPISODE     "episode"
//#define ANIMATION   "Animation"
//#define DOCUMENTARY "Documentary"
//#define SHORT       "short"

//#define MOVIE_TYPES   { SERIES, ANIMATION, DOCUMENTARY, SHORT }

#endif