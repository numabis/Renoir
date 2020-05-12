#pragma once

#include <windows.h>
#include <string>
#include <vector>
//#include <map>
#include <sstream>      // std::stringstream
#include "variables.h"
#include "Xml.h"
//#include "definitions.h"

#define XMLOK        0
#define XMLNOFSPATH  (0x0001 << 0)
#define XMLNOOMDBKEY (0x0001 << 1)
#define XMLERROR     (0x0001 << 2)

#define XMLTAG_CONFIG      "config"
//
//#define XMLTAG_DB              "database"
//#define XMLTAG_FS              "filesystem"
//#define XMLTAG_OMDB            "omdb"
//#define XMLTAG_IMDB            "imdb"
//#define XMLTAG_LOG             "log"
////#define XMLTAG_XML             "xml"
//
//#define XML_CONFIGFILE      "configfile"
//
//#define XMLTAG_CONFIG_FSPATH   "fspath"
//
//#define XMLTAG_OMDB_KEY        "apikey"
//#define XMLTAG_OMDB_URL        "omdburl"
//#define XMLTAG_OMDB_API        "apiRequest"
//#define XMLTAG_OMDB_TYPE       "type"
//#define XMLTAG_OMDB_PLOT       "plot"
//#define XMLTAG_OMDB_SEP        "listseparator"
//#define XMLTAG_OMDB_TEST       "testmode"
//
//#define XMLTAG_IMDB_URL      "url"
//#define XMLTAG_IMDB_TITLE    "title"
//#define XMLTAG_IMDB_FIND     "find"
//
//#define XMLTAG_VPLAYER        "videoplayer"
//
//#define XMLTAG_XMLPATH        "xmlpath"
//#define XMLTAG_XMLNAME        "xmlname"
//#define XMLTAG_XMLEXT         "xmlext"

//#define XMLTAG_FILTERS_G              "filters"
//#define XMLTAG_FILTERS_DIRECTORS      "directors"
//#define XMLTAG_FILTERS_WRITERS        "writers"
//#define XMLTAG_FILTERS_ACTORS         "actors"
//#define XMLTAG_FILTERS_GENRES         "genres"
//#define XMLTAG_FILTERS_GENRES2        "genres2"
//#define XMLTAG_FILTERS_YEARMIN        "yearmin"
//#define XMLTAG_FILTERS_YEARMAX        "yearmax"
//#define XMLTAG_FILTERS_IMDBRATINGMIN  "imdbratingmin"
//#define XMLTAG_FILTERS_IMDBRATINGMAXN "imdbratingmax"
//#define XMLTAG_FILTERS_SERIE          "serie"
//#define XMLTAG_FILTERS_ANIM           "anim"
//#define XMLTAG_FILTERS_DOC            "doc"
//#define XMLTAG_FILTERS_SHORT          "short"


//#define XMLVAL_TYPE_SERIE      "serie"
//#define XMLVAL_TYPE_ANIM       "animation"
//#define XMLVAL_TYPE_DOC        "documentary"
//#define XMLVAL_TYPE_SHORT      "short"

//#define XMLVAL_OMDB_PLOT_SHORT     "short"
//#define XMLVAL_OMDB_PLOT_FULL      "full"
//#define XMLVAL_OMDB_TYPE_XML       "xml"
//#define XMLVAL_OMDB_TYPE_JSON      "json"
/// <summary>
/// Soporte acceso base de datos SQLITE
/// </summary>
class ReadConfig
{

public:
    ReadConfig();
    ~ReadConfig(void);

    enum XML_CONFIG_TAGS { XML_CONFIG_DB, XML_CONFIG_FS, XML_CONFIG_OMDB, XML_CONFIG_IMDB, XML_CONFIG_LOG/*, XML_CONFIG_XML*/, XML_CONFIG_MAX };

    enum XML_TAG_TYPE { XML_TAG, XML_TAG_SUB, XML_TAG_MAX};

    //char* xmlConfigTags[XML_CONFIG_MAX] = { XMLTAG_DB, XMLTAG_FS, XMLTAG_OMDB, XMLTAG_IMDB, XMLTAG_LOG/*, XMLTAG_XML*/ };
    //char* xmlOmdbTags[XML_OMDB_MAX] = { XMLTAG_OMDB_KEY,XMLTAG_OMDB_URL,XMLTAG_OMDB_API, XMLTAG_OMDB_TYPE, XMLTAG_OMDB_PLOT, XMLTAG_OMDB_SEP, XMLTAG_OMDB_TEST };
    //char* xmlImdbTags[XML_IMDB_MAX] = { XMLTAG_IMDB_URL, XMLTAG_IMDB_TITLE, XMLTAG_IMDB_FIND };
    //std::string xmlLocalTag[XML_LOCAL_MAX] = { XMLTAG_CONFIG_FSPATH, XMLTAG_OMDB_KEY, XMLTAG_VPLAYER, XMLTAG_XMLPATH, XMLTAG_XMLNAME, XMLTAG_XMLEXT, XMLTAG_FILTERS_G };
    //XML_TAG_TYPE xmlLocalTagType[XML_LOCAL_MAX] = { XML_TAG, XML_TAG, XML_TAG, XML_TAG, XML_TAG, XML_TAG, XML_TAG_SUB };
    //std::string xmlLocalFiltersTag[FILTERS_MAX] = { XMLTAG_FILTERS_DIRECTORS,XMLTAG_FILTERS_WRITERS,XMLTAG_FILTERS_ACTORS,XMLTAG_FILTERS_GENRES,XMLTAG_FILTERS_GENRES2,XMLTAG_FILTERS_YEARMIN,XMLTAG_FILTERS_YEARMAX,XMLTAG_FILTERS_IMDBRATINGMIN,XMLTAG_FILTERS_IMDBRATINGMAXN, XMLTAG_FILTERS_SERIE, XMLTAG_FILTERS_ANIM, XMLTAG_FILTERS_DOC, XMLTAG_FILTERS_SHORT };
    //XML_TAG_TYPE xmlLocalFiltersTagType[FILTERS_MAX] = { XML_TAG, XML_TAG, XML_TAG, XML_TAG };
    //std::string xmlTypes[TYPE_MAX] = { XMLVAL_TYPE_SERIE, XMLVAL_TYPE_ANIM, XMLVAL_TYPE_DOC, XMLVAL_TYPE_SHORT };

    int initLocalConfigFile();
    //int initConfigFile();
    bool initConfigFile(confMap *_map);
    DATA_CONFIG *getGlobalConfig();
    FS_CONFIG *getFsConfig();
    OMDB_CONFIG *getOmdbConfig();
    std::string getOmdb(XML_OMDB_TAGS);
    std::string getImdb(XML_IMDB_TAGS);
    std::string getImdbTitleUrl(std::string);
    std::string getImdbFindUrl(std::string);
    DB_CONFIG *getDbConfig();
    LOG_CONFIG *getLogConfig();
 //   XML_CONFIG *getXmlConfig();

//    std::string* getXmlLocalConfig();
//    std::string (XML_LOCAL_VARS);
    std::string getXmlLocalFilters(filterTypes);
//    std::string getXmlLocalFullPath();
//    void setXmlLocalConfig(XML_LOCAL_VARS, std::string);
    void setXmlLocalFilters(filterTypes, std::string);

//    int loadLocalConfigFile();

    bool readXML(wchar_t *, DATA_CONFIG *);
    //bool saveLocalConfigFile(bool);

private:

    std::string homeFolder;
    std::string configPath;
    std::string localConfigPath;
    DATA_CONFIG xmlConfig;
    std::string getVersionApp();
    bool loadConfigFile(confMap* _map);
    bool saveConfigFile();
    void setConfigValue(confMap* _map, TiXmlElement* _element, std::string _var);
    //int loadConfigFile();
    //int readConfigFS(TiXmlElement *node);
    //int readConfigOMDB(TiXmlElement *node);
    //int readConfigIMDB(TiXmlElement *node);
    //int readConfigDB(TiXmlElement *node);
    //int readConfigLOG(TiXmlElement *node);
//    int readConfigXML(TiXmlElement *node);

};

