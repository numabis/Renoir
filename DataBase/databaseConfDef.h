///file "databaseConfDef.h"
#ifndef _DATABASECONFDEF_
#define _DATABASECONFDEF_

//#include "variables.h"

#define CONF_FS_AUTOREADFOLDER	        "FS_autoreadfolder"
#define CONF_FS_EXTENTIONS	            "FS_extentions"
#define CONF_FS_SEARCHTYPES	            "FS_TYPE_searchtypes"
#define CONF_FS_TYPESERIE	            "FS_TYPE_serie"
#define CONF_FS_TYPEANIM	            "FS_TYPE_anim"
#define CONF_FS_TYPEDOC	                "FS_TYPE_doc"
#define CONF_FS_TYPESHORT	            "FS_TYPE_short"
#define CONF_FS_SEARCHLANGUAGE	        "FS_LANG_searchlanguage"
#define CONF_FS_LANGFRENCH	            "FS_LANG_french"
#define CONF_FS_LANGSPANISH	            "FS_LANG_spanish"
#define CONF_FS_LANGENGLISH	            "FS_LANG_english"
#define CONF_FS_LANGITALIAN	            "FS_LANG_italian"

#define CONF_DB_RESET	                "DB_reset"

#define CONF_LOG_LOGLEVEL	            "LOG_loglevel"

#define CONF_APP_CURRENTFOLDER	        "APP_currentfolder"
#define CONF_APP_VIDEOPLAYER	        "APP_videoplayer"
#define CONF_APP_SHOWDEBUG  	        "APP_showdebug"
#define CONF_APP_AUTOREQUEST            "APP_autorequest"
#define CONF_APP_FORCEREQUEST           "APP_forcerequest"
#define CONF_APP_REQUESTTYPEMANUAL      "APP_requesttypemanual"
#define CONF_APP_JPGNOTFOUND            "APP_jpgnotfound"

#define CONF_OMDB_QUERY_URL             "OMDB_urlQ"
#define CONF_OMDB_QUERY_REQUEST	        "OMDB_urlreqQ"
#define CONF_OMDB_LISTSEPARATOR	        "OMDB_listseparator"
#define CONF_OMDB_TESTMODE	            "OMDB_testmode"
#define CONF_OMDB_QUERY_SEARCH          "OMDB_searchQ"
#define CONF_OMDB_QUERY_TITLE           "OMDB_titleQ"
#define CONF_OMDB_QUERY_YEAR            "OMDB_yearQ"
#define CONF_OMDB_QUERY_PAGE            "OMDB_pageQ"
#define CONF_OMDB_QUERY_IMDBID          "OMDB_imdbidQ"
#define CONF_OMDB_QUERY_CALLBACK        "OMDB_callbackQ"
#define CONF_OMDB_QUERY_VERSION         "OMDB_versionQ"
#define CONF_OMDB_QUERY_APIKEY	        "OMDB_apikeyQ"
#define CONF_OMDB_APIKEY	            "OMDB_apikey"
#define CONF_OMDB_QUERY_MOVIE_TYPE      "OMDB_movietypeQ"
#define CONF_OMDB_MOVIE_TYPE            "OMDB_movietype"
#define CONF_OMDB_QUERY_PLOT            "OMDB_plotQ"
#define CONF_OMDB_PLOT	                "OMDB_plot"
#define CONF_OMDB_QUERY_RETURN_TYPE     "OMDB_returntypeQ"
#define CONF_OMDB_RETURN_TYPE           "OMDB_returntype"

#define CONF_IMDBAPI_QUERY_URL              "IMDBAPI_urlQ" // https://imdb-api.com/
#define CONF_IMDBAPI_QUERY_REQUEST	        "IMDBAPI_urlreqQ" // API/
#define CONF_IMDBAPI_QUERY_SEARCH           "IMDBAPI_searchQ" // Search/
#define CONF_IMDBAPI_QUERY_SEARCH_TITLE     "IMDBAPI_searchtitleQ" // Search/
#define CONF_IMDBAPI_QUERY_SEARCH_MOVIE     "IMDBAPI_searchmovieQ" // Search/
#define CONF_IMDBAPI_QUERY_SEARCH_SERIES    "IMDBAPI_searchSeriesQ" // Search/
#define CONF_IMDBAPI_QUERY_SEARCH_NAME      "IMDBAPI_searchnameQ" // Search/
#define CONF_IMDBAPI_QUERY_SEARCH_EPISODE   "IMDBAPI_searchepisodeQ" // Search/
#define CONF_IMDBAPI_QUERY_SEARCH_COMPANY   "IMDBAPI_searchcompanyQ" // Search/
#define CONF_IMDBAPI_QUERY_SEARCH_KEYWORD   "IMDBAPI_searchkeywordQ" // Search/
#define CONF_IMDBAPI_QUERY_SEARCH_ALL       "IMDBAPI_searchallQ" // Search/
#define CONF_IMDBAPI_QUERY_GETMOVIE         "IMDBAPI_getMovieQ" 

#define CONF_IMDBAPI_APIKEY	            "IMDBAPI_apikey" // k_12345678

#define CONF_IMDB_URL	                "IMDB_url"
#define CONF_IMDB_TITLE	                "IMDB_title"
#define CONF_IMDB_FIND	                "IMDB_find"

#define CONF_FILTER_DIRECTORS	        "FILTER_directors"
#define CONF_FILTER_USE_DIRECTORS	    "FILTER_USE_directors"
#define CONF_FILTER_WRITERS	            "FILTER_writers"
#define CONF_FILTER_USE_WRITERS	        "FILTER_USE_writers"
#define CONF_FILTER_ACTORS	            "FILTER_actors"
#define CONF_FILTER_USE_ACTORS	        "FILTER_USE_actors"
#define CONF_FILTER_GENRES	            "FILTER_genres"
#define CONF_FILTER_USE_GENRES	        "FILTER_USE_genres"
#define CONF_FILTER_GENRES2	            "FILTER_genres2"
#define CONF_FILTER_USE_GENRES2	        "FILTER_USE_genres2"
#define CONF_FILTER_YEARMIN	            "FILTER_yearmin"
#define CONF_FILTER_USE_YEARMIN	        "FILTER_USE_yearmin"
#define CONF_FILTER_YEARMAX	            "FILTER_yearmax"
#define CONF_FILTER_USE_YEARMAX	        "FILTER_USE_yearmax"
#define CONF_FILTER_IMDBRATINGMIN	    "FILTER_imdbratingmin"
#define CONF_FILTER_USE_IMDBRATINGMIN	"FILTER_USE_imdbratingmin"
#define CONF_FILTER_IMDBRATINGMAX	    "FILTER_imdbratingmax"
#define CONF_FILTER_USE_IMDBRATINGMAX	"FILTER_USE_imdbratingmax"
#define CONF_FILTER_SERIE	            "FILTER_serie"
#define CONF_FILTER_USE_SERIE	        "FILTER_USE_serie"
#define CONF_FILTER_ANIM	            "FILTER_anim"
#define CONF_FILTER_USE_ANIM	        "FILTER_USE_anim"
#define CONF_FILTER_DOC 	            "FILTER_doc"
#define CONF_FILTER_USE_DOC	            "FILTER_USE_doc"
#define CONF_FILTER_SHORT	            "FILTER_short"
#define CONF_FILTER_USE_SHORT	        "FILTER_USE_short"

#define CONF_XML_PATH	                "XML_path"
#define CONF_XML_FILENAME	            "XML_filename"
#define CONF_XML_FILENAMEEXT  	        "XML_filenameext"  

#define DBCONFFIELDS \
{ CONF_FS_AUTOREADFOLDER,           { "false", "AutoRead when app start" } }, \
{ CONF_FS_EXTENTIONS,               { ".mp4,.avi,.mkv", "Movie files extentions"} }, \
{ CONF_FS_SEARCHTYPES,              { "true", "Guess movie type from folder" } }, \
{ CONF_FS_TYPESERIE,                { "serie", "Wild card for series" } }, \
{ CONF_FS_TYPEANIM,                 { "anim", "Wild card for anims" } }, \
{ CONF_FS_TYPEDOC,                  { "doc", "Wild card for docs" } }, \
{ CONF_FS_TYPESHORT,                { "short", "Wild card for shorts" } }, \
{ CONF_FS_SEARCHLANGUAGE,           { "true", "Guess movie lang from folder"} }, \
{ CONF_FS_LANGFRENCH,               { "francais", "Wild card for french" } }, \
{ CONF_FS_LANGSPANISH,              { "espagnol", "Wild card for spanish" } }, \
{ CONF_FS_LANGENGLISH,              { "english", "Wild card for english" } }, \
{ CONF_FS_LANGITALIAN,              { "italien", "Wild card for italian" } }, \
{ CONF_DB_RESET,                    { "false", "Reset DB" } }, \
{ CONF_LOG_LOGLEVEL,                { "6", "DISABLED=0 ERROR=1 INFO=2 DB=3 XML=4 DEBUG=5 SQL=6" } }, \
{ CONF_APP_CURRENTFOLDER,           { "", "Actual Folder"} }, \
{ CONF_APP_VIDEOPLAYER,             { "", "Path to player"} }, \
{ CONF_APP_AUTOREQUEST,             { "1", "Automatically send requests to OMDB" } }, \
{ CONF_APP_FORCEREQUEST,            { "0", "Force requests to OMDB" } }, \
{ CONF_APP_REQUESTTYPEMANUAL,        { "1", "user validation is request to send queries to OMDB" } }, \
{ CONF_APP_JPGNOTFOUND,             { ".\\notFound.jpg", ""}},\
{ CONF_OMDB_APIKEY,                 { "", "Omdb Key"} }, \
{ CONF_OMDB_QUERY_URL,              { "http://www.omdbapi.com/", "Omdb web URL"} }, \
{ CONF_OMDB_QUERY_REQUEST,          { "apikey.aspx", "Omdb page"} }, \
{ CONF_OMDB_QUERY_APIKEY,           { "apikey=", ""} }, \
{ CONF_OMDB_QUERY_TITLE,            { "t=", "Movie title to search for." } }, \
{ CONF_OMDB_QUERY_SEARCH,           { "s=", "Movie title to search for." } }, \
{ CONF_OMDB_QUERY_PAGE,             { "page=", "Page number to return." } }, \
{ CONF_OMDB_QUERY_YEAR,             { "y=", "Year of release." } }, \
{ CONF_OMDB_QUERY_RETURN_TYPE,      { "r=", "The data type to return (json, xml)." } }, \
{ CONF_OMDB_QUERY_IMDBID,           { "i=", "A valid IMDb ID" } }, \
{ CONF_OMDB_QUERY_MOVIE_TYPE,       { "type=", "Type of result to return (movie, series, episode)." } }, \
{ CONF_OMDB_QUERY_PLOT,	            { "plot=", "Return short or full plot (short, full)." } }, \
{ CONF_OMDB_QUERY_CALLBACK,	        { "callback=", "JSONP callback name." } }, \
{ CONF_OMDB_QUERY_VERSION,	        { "v=", "API version (reserved for future use)." } }, \
{ CONF_OMDB_APIKEY,	                { "", "A valid OMDB API KEY" } }, \
{ CONF_OMDB_RETURN_TYPE,            { "xml", "data in XML or JSON"} }, \
{ CONF_OMDB_MOVIE_TYPE,             { "movie", "Type of result to return (movie, series, episode)"} }, \
{ CONF_OMDB_PLOT,                   { "full", "plot:full or short"} }, \
{ CONF_OMDB_LISTSEPARATOR,          { ",", "Actual Folder"} }, \
{ CONF_OMDB_TESTMODE,               { "false", "Actual Folder"} }, \
{ CONF_IMDBAPI_QUERY_URL,           { "https://imdb-api.com/", "ImdbAPI web URL"} }, \
{ CONF_IMDBAPI_QUERY_REQUEST,	    { "API" , ""} }, \
{ CONF_IMDBAPI_QUERY_SEARCH,        { "Search", ""} }, \
{ CONF_IMDBAPI_QUERY_SEARCH_TITLE,  { "Searchtitle", "Search"} }, \
{ CONF_IMDBAPI_QUERY_SEARCH_MOVIE,  { "SearchMovie", "Recommended for searching Movies" } }, \
{ CONF_IMDBAPI_QUERY_SEARCH_SERIES, { "SearchSeries", "Recommended for searching Series TVs" } }, \
{ CONF_IMDBAPI_QUERY_SEARCH_NAME,   { "SearchName", "Search into all people." } }, \
{ CONF_IMDBAPI_QUERY_SEARCH_EPISODE,{ "SearchEpisode", "Search into all TV Episode titles."} }, \
{ CONF_IMDBAPI_QUERY_SEARCH_ALL,    { "SearchAll", "Search into all items" } }, \
{ CONF_IMDBAPI_QUERY_GETMOVIE,      { "Title", "Get Movies or Series TV information." } }, \
{ CONF_IMDBAPI_APIKEY,	            { "k_9ZnN7HB8", "A valid OMDB API KEY" } }, \
{ CONF_IMDB_URL,                    { "https://www.imdb.com/", "Imdb URL"} }, \
{ CONF_IMDB_TITLE,                  { "title", "IMDB page"} }, \
{ CONF_IMDB_FIND,                   { "find?q=", "find sintax"} }, \
{ CONF_FILTER_DIRECTORS,            { "", "Selected filter for director"} }, \
{ CONF_FILTER_WRITERS,              { "", "Selected filter for writer"} }, \
{ CONF_FILTER_ACTORS,               { "", "Selected filter for actor"} }, \
{ CONF_FILTER_GENRES,               { "", "Selected filter for genre1"} }, \
{ CONF_FILTER_GENRES2,              { "", "Selected filter for genre2"} }, \
{ CONF_FILTER_YEARMIN,              { "", "Selected filter for min year"} }, \
{ CONF_FILTER_YEARMAX,              { "", "Selected filter for max year"} }, \
{ CONF_FILTER_IMDBRATINGMAX,        { "", "Selected filter for max imdb rating"} }, \
{ CONF_FILTER_SERIE,                { "", "Selected filter for serie"} }, \
{ CONF_FILTER_ANIM,                 { "", "Selected filter for anim"} }, \
{ CONF_FILTER_DOC ,                 { "", "Selected filter for doc"} }, \
{ CONF_FILTER_SHORT,                { "", "Selected filter for short"} }, \
{ CONF_FILTER_IMDBRATINGMIN,        { "", "Selected filter for min imdb rating"} }, \
{ CONF_FILTER_USE_DIRECTORS,        { "false", "Selected filter for director"} }, \
{ CONF_FILTER_USE_WRITERS,          { "false", "Selected filter for writer"} }, \
{ CONF_FILTER_USE_ACTORS,           { "false", "Selected filter for actor"} }, \
{ CONF_FILTER_USE_GENRES,           { "false", "Selected filter for genre1"} }, \
{ CONF_FILTER_USE_GENRES2,          { "false", "Selected filter for genre2"} }, \
{ CONF_FILTER_USE_YEARMIN,          { "false", "Selected filter for min year"} }, \
{ CONF_FILTER_USE_YEARMAX,          { "false", "Selected filter for max year"} }, \
{ CONF_FILTER_USE_IMDBRATINGMIN,    { "false", "Selected filter for min imdb rating"} }, \
{ CONF_FILTER_USE_IMDBRATINGMAX,    { "false", "Selected filter for max imdb rating"} }, \
{ CONF_FILTER_USE_SERIE,            { "false", "Selected filter for serie"} }, \
{ CONF_FILTER_USE_ANIM,             { "false", "Selected filter for anim"} }, \
{ CONF_FILTER_USE_DOC,              { "false", "Selected filter for doc"} }, \
{ CONF_FILTER_USE_SHORT,            { "false", "Selected filter for short"} }, \
{ CONF_XML_PATH,                    { "", ""} }, \
{ CONF_XML_FILENAME,                { "movieDB", ""} }, \
{ CONF_XML_FILENAMEEXT,             { "xml", ""} }

#endif // _DATABASECONFDEF_