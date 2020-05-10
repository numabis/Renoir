#ifndef _DATABASECONFDEF_
#define _DATABASECONFDEF_
//#include "databaseSQL.h"

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
#define CONF_OMDB_APIKEY	            "OMDB_apikey"
#define CONF_OMDB_URL	                "OMDB_url"
#define CONF_OMDB_APIREQUEST	        "OMDB_apirequest"
#define CONF_OMDB_TYPE	                "OMDB_type"
#define CONF_OMDB_PLOT	                "OMDB_plot"
#define CONF_OMDB_LISTSEPARATOR	        "OMDB_listseparator"
#define CONF_OMDB_TESTMODE	            "OMDB_testmode"
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
#define CONF_XML_TAG_SERIE	            "XML_TAG_serie"
#define CONF_XML_TAG_ANIMATION	        "XML_TAG_animation"
#define CONF_XML_TAG_DOCUMENTARY	    "XML_TAG_documentary"
#define CONF_XML_TAG_SHORT	            "XML_TAG_short"
#define CONF_XML_PATH	                "XML_path"
#define CONF_XML_FILENAME	            "XML_filename"
#define CONF_XML_FILENAMEEXT  	        "XML_filenameext"  

#define DBCONFFIELDS \
{ CONF_FS_AUTOREADFOLDER,           { "false", "AutoRead when app start" } }, \
{ CONF_FS_EXTENTIONS,               { ".mp4,.avi,.mkv", "Movie files extentions"} }, \
{ CONF_FS_SEARCHTYPES,              { "true", "Guess movie type from folder" } }, \
{ CONF_FS_TYPESERIE,                { "serie", "" } }, \
{ CONF_FS_TYPEANIM,                 { "anim", "" } }, \
{ CONF_FS_TYPEDOC,                  { "doc", "" } }, \
{ CONF_FS_TYPESHORT,                { "short", "" } }, \
{ CONF_FS_SEARCHLANGUAGE,           { "true", ""} }, \
{ CONF_FS_LANGFRENCH,               { "francais", "" } }, \
{ CONF_FS_LANGSPANISH,              { "espagnol", "" } }, \
{ CONF_FS_LANGENGLISH,              { "english", "" } }, \
{ CONF_FS_LANGITALIAN,              { "italien", "" } }, \
{ CONF_DB_RESET,                    { "false", "" } }, \
{ CONF_LOG_LOGLEVEL,                { "6", "DISABLED=0 ERROR=1 INFO=2 DB=3 XML=4 DEBUG=5 SQL=6" } }, \
{ CONF_APP_CURRENTFOLDER,           { "", "Actual Folder"} }, \
{ CONF_APP_VIDEOPLAYER,             { "", "Path to player"} }, \
{ CONF_OMDB_APIKEY,                 {"", "Omdb Key"} }, \
{ CONF_OMDB_URL,                    { "http://www.omdbapi.com/", "Omdb URL"} }, \
{ CONF_OMDB_APIREQUEST,             { "apikey.aspx", "Omdb page"} }, \
{ CONF_OMDB_TYPE,                   { "xml",           "data in XML or JSON"} }, \
{ CONF_OMDB_PLOT,                   { "full",          "plot:full or short"} }, \
{ CONF_OMDB_LISTSEPARATOR,          { ",", "Actual Folder"} }, \
{ CONF_OMDB_TESTMODE,               { "false", "Actual Folder"} }, \
{ CONF_IMDB_URL,                    { "https://www.imdb.com/", "Imdb URL"} }, \
{ CONF_IMDB_TITLE,                  { "title", "IMDB page"} }, \
{ CONF_IMDB_FIND,                   { "find?q=", "find sintax"} }, \
{ CONF_FILTER_DIRECTORS,            { "", "Selected filter for director"} }, \
{ CONF_FILTER_USE_DIRECTORS,        { "false", "Selected filter for director"} }, \
{ CONF_FILTER_WRITERS,              { "", "Selected filter for writer"} }, \
{ CONF_FILTER_USE_WRITERS,          { "false", "Selected filter for writer"} }, \
{ CONF_FILTER_ACTORS,               { "", "Selected filter for actor"} }, \
{ CONF_FILTER_USE_ACTORS,           { "false", "Selected filter for actor"} }, \
{ CONF_FILTER_GENRES,               { "", "Selected filter for genre1"} }, \
{ CONF_FILTER_USE_GENRES,           { "false", "Selected filter for genre1"} }, \
{ CONF_FILTER_GENRES2,              { "", "Selected filter for genre2"} }, \
{ CONF_FILTER_USE_GENRES2,          { "false", "Selected filter for genre2"} }, \
{ CONF_FILTER_YEARMIN,              { "", "Selected filter for min year"} }, \
{ CONF_FILTER_USE_YEARMIN,          { "false", "Selected filter for min year"} }, \
{ CONF_FILTER_YEARMAX,              { "", "Selected filter for max year"} }, \
{ CONF_FILTER_USE_YEARMAX,          { "false", "Selected filter for max year"} }, \
{ CONF_FILTER_IMDBRATINGMIN,        { "", "Selected filter for min imdb rating"} }, \
{ CONF_FILTER_USE_IMDBRATINGMIN,    { "false", "Selected filter for min imdb rating"} }, \
{ CONF_FILTER_IMDBRATINGMAX,        { "", "Selected filter for max imdb rating"} }, \
{ CONF_FILTER_USE_IMDBRATINGMAX,    { "false", "Selected filter for max imdb rating"} }, \
{ CONF_FILTER_SERIE,                { "", "Selected filter for serie"} }, \
{ CONF_FILTER_USE_SERIE,            { "false", "Selected filter for serie"} }, \
{ CONF_FILTER_ANIM,                 { "", "Selected filter for anim"} }, \
{ CONF_FILTER_USE_ANIM,             { "false", "Selected filter for anim"} }, \
{ CONF_FILTER_DOC ,                 { "", "Selected filter for doc"} }, \
{ CONF_FILTER_USE_DOC,              { "false", "Selected filter for doc"} }, \
{ CONF_FILTER_SHORT,                { "", "Selected filter for short"} }, \
{ CONF_FILTER_USE_SHORT,            { "false", "Selected filter for short"} }, \
{ CONF_XML_TAG_SERIE,               { "serie", "XML TAG for serie"} }, \
{ CONF_XML_TAG_ANIMATION,           { "animation", "XML TAG for animation"} }, \
{ CONF_XML_TAG_DOCUMENTARY,         { "documentary", "XML TAG for documentary"} }, \
{ CONF_XML_TAG_SHORT,               { "short", "XML TAG for short"} }, \
{ CONF_XML_PATH,                    { "", ""} }, \
{ CONF_XML_FILENAME,                { "movieDB", ""} }, \
{ CONF_XML_FILENAMEEXT,             { "xml", ""} }


#endif // _DATABASECONFDEF_