#pragma once
#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include <string>
#include <vector>
#include "definitions.h"
#include "exLog.h"
#include <shlobj.h>

enum states { ST_INITIATING, ST_STANDBY, ST_READING_FODLER, ST_READING_DB, ST_READING_FILE, ST_WRITING_FILE, ST_OMDB_REQUEST_SINGLE, ST_OMDB_REQUEST_ALL, ST_SELECT_FOLDER, ST_SELECT_COLUMNS, ST_VPLAYER, ST_UPDATE_DISPLAY, ST_MAX };
#define STATESNAMES { "ST_INITIATING", "ST_STANDBY", "ST_READING_FODLER", "ST_READING_DB", "ST_READING_FILE", "ST_WRITING_FILE", "ST_OMDB_REQUEST_SINGLE", "ST_OMDB_REQUEST_ALL", "ST_SELECT_FOLDER", "ST_SELECT_COLUMNS", "ST_VPLAYER", "ST_UPDATE_DISPLAY" };
//const std::string stNames[ST_MAX] = 

enum omdbXmlAttributes { XML_TITLE, XML_YEAR, XML_RATED, XML_RELEASED, XML_RUNTIME, XML_GENRE, XML_DIRECTORS, XML_WRITERS, XML_ACTORS, XML_PLOT, XML_LANGUAGE, XML_COUNTRY, XML_AWARDS, XML_POSTER, XML_METASCORE, XML_IMDBRATING, XML_IMDBVOTES, XML_IMDBID, XML_TYPE, XML_NBATTRIBUTES };
enum movieInfoTypes { TYPE_SERIE, TYPE_ANIM, TYPE_DOC, TYPE_SHORT, TYPE_MAX };
enum XML_OMDB_TAGS { XML_OMDB_KEY, XML_OMDB_URL, XML_OMDB_API, XML_OMDB_TYPE, XML_OMDB_PLOT, XML_OMDB_SEP, XML_OMDB_TEST, XML_OMDB_MAX };
enum XML_IMDB_TAGS { XML_IMDB_URL, XML_IMDB_TITLE, XML_IMDB_FIND, XML_IMDB_MAX };
enum XML_LOCAL_VARS { XML_LOCAL_PATH, XML_LOCAL_APIKEY, XML_LOCAL_VIDEOPLAYER, XML_LOCAL_SAVE_PATH, XML_LOCAL_SAVE_NAME, XML_LOCAL_SAVE_EXT, XML_LOCAL_FILTERS, XML_LOCAL_MAX };
enum filterTypes { FILTERS_DIRECTORS, FILTERS_WRITERS, FILTERS_ACTORS, FILTERS_GENRES, FILTERS_GENRES2, FILTERS_YEARMIN, FILTERS_YEARMAX, FILTERS_IMDBRATINGMIN, FILTERS_IMDBRATINGMAX, FILTERS_SERIE, FILTERS_ANIM, FILTERS_DOC, FILTERS_SHORT, FILTERS_MAX };

enum ROLES_PERSONS { ROLES_DIRECTOR, ROLES_WRITER, ROLES_ACTOR, ROLES_MAX };

enum typesValues { TYPE_UNSET = -1, TYPE_UNKNOWN = 1, TYPE_MOVIE = 2, TYPE_NOTMOVIE = 4, TYPE_ALL = 8 };

//enum appConfig { CONF_CURRENTFOLDER, CONF_VIDEOPLAYER, CONF_OMDB_APIKEY, CONF_OMDB_URL, CONF_OMDB_APIREQUEST, CONF_OMDB_TYPE, CONF_OMDB_PLOT, CONF_OMDB_LISTSEPARATOR, CONF_OMDB_TESTMODE, CONF_IMDB_URL, CONF_IMDB_TITLE, CONF_IMDB_FIND, CONF_FILTER_DIRECTORS, CONF_FILTER_WRITERS, CONF_FILTER_ACTORS, CONF_FILTER_GENRES, CONF_FILTER_GENRES2, CONF_FILTER_YEARMIN, CONF_FILTER_YEARMAX, CONF_FILTER_IMDBRATINGMIN, CONF_FILTER_IMDBRATINGMAX, CONF_FILTER_SERIE, CONF_FILTER_ANIM, CONF_FILTER_DOC, CONF_FILTER_SHORT, CONF_XMLTAG_SERIE, CONF_XMLTAG_ANIMATION, CONF_XMLTAG_DOCUMENTARY, CONF_XMLTAG_SHORT, CONF_MAX_VALUES };

typedef struct {
    std::string value;
    bool multi;
    std::string description;
} dbConfiguration;

typedef std::map<std::string, dbConfiguration> confMap;

struct FS_CONFIG
{
    //std::string *path;
    bool autoReadFolder;
    std::vector<std::string> extentions;
    bool searchTypes;
    std::string strType[TYPE_MAX];
};

struct OMDB_CONFIG
{
    std::string apikey; // "35a5ce9a"
    std::string omdbUrl; //http://www.omdbapi.com/?</url>
    std::string apiRequestUrl;
    std::string type;
    std::string plot;
    std::string separator;
    bool testmode;
    bool typeXml; // "xml"
    bool plotShort; //"short"

};

struct DB_CONFIG
{
    std::string dbPath;
    std::string dbName;
    bool resetDB;
};

struct LOG_CONFIG
{
    std::string logPath;
    short logLevel;
};

struct DATA_CONFIG
{
    //std::string *fsPath;
    LOG_CONFIG  log;
    DB_CONFIG   db;
    OMDB_CONFIG omdb;
    std::string imdb[XML_IMDB_MAX];
    FS_CONFIG   fs;
    //std::string xmlLocal[XML_LOCAL_MAX];
    std::string xmlLocalFilters[FILTERS_MAX];
};

struct DATA_MOVIE
{
    std::string name;
    std::string date;
    float rating;
};

#endif // _VARIABLES