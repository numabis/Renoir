#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_
#include <string>
#include "variables.h"
#include "singleton.h"
#include "readConfig.h"
#include "databaseConfDef.h"

#define CONFIGMAX 7

#ifdef _DEBUG
#define HOMEFOLDER "\\Renoir_debug\\"
#elif defined (_DEMO)
#define HOMEFOLDER "\\Renoir_demo\\"
#else
#define HOMEFOLDER "\\Renoir\\"
#endif

class ConfigManager: public BUTIL::Singleton<ConfigManager>
{

public:
    std::string fsMoviesType[TYPE_MAX] = { CONF_FS_TYPESERIE , CONF_FS_TYPEANIM , CONF_FS_TYPEDOC ,CONF_FS_TYPESHORT };


    ConfigManager(void);
    ~ConfigManager(void);
    bool init();
    bool loadConfFile();
    bool resetDBConfig();
    int checkConfig();
    std::string getErrorConfig();
    bool saveConfigToDB();
    bool configFile2configDB();

    std::string getHomeFolder();
    std::string getConfigStr(std::string);
    std::vector<std::string> getConfigVect(std::string, std::string _sep);
    int getConfigInt(std::string);
    bool getConfigBool(std::string);
    bool setConfigValue(std::string, std::string);
    bool setConfigValue(std::string, int);
    bool setConfigValue(std::string, bool);
    bool setConfigValue(filterTypes _filter, std::string);

    std::string getFilter(filterTypes _var);
    std::string getFsType(int type);

    //FS_CONFIG   *getFsConfig();
    //OMDB_CONFIG *getOmdbConfig();
    //DB_CONFIG   *getDbConfig();
    //LOG_CONFIG  *getLogConfig();

    //std::string getOmdb(XML_OMDB_TAGS);
    //std::string getImdb(XML_IMDB_TAGS);
    //std::string getImdbTitleUrl(std::string);
    //std::string getImdbFindUrl(std::string);
    TiXmlDocument getConfXml();

private:

    std::string configMin[CONFIGMAX] = { CONF_LOG_LOGLEVEL, CONF_OMDB_QUERY_URL, CONF_OMDB_QUERY_REQUEST, CONF_OMDB_RETURN_TYPE, CONF_OMDB_PLOT, CONF_OMDB_LISTSEPARATOR, CONF_FS_EXTENTIONS };
    bool configMinTest[CONFIGMAX];

    ReadConfig xmlConfig;

    friend class BUTIL::Singleton<ConfigManager>;

    bool isDataLoaded;

    std::string homeFolder;

    //std::string configuration[CONF_MAX_VALUES];
    //std::vector<dbConfiguration> *dbConf; // [CONF_MAX_VALUES] = { DBCONFFIELDS };
    confMap dbConf;
    confMap fileConf;
    DATA_CONFIG oldConfig;

    bool readConfigFromDB();
    bool saveConfigToDB(std::string _var);
    //dbConfiguration *getConfigRow(appConfig);
    dbConfiguration *getConfigRow(std::string);
    std::string *getConfigValue(std::string);
    void separateValues(std::vector<std::string>* v_list, std::string _list, std::string _sep);

};
#endif //_CONFIG_MANAGER_H_