#include <string>
#include "variables.h"
#include "database.h"

class ConfigManager
{

public:

    ConfigManager(void);
    ~ConfigManager(void);
    bool init();
    
    std::string getHomeFolder();
    std::string getConfigStr(appConfig);
    std::string getConfigStr(std::string);
    int getConfigInt(appConfig);
    int getConfigInt(std::string);
    bool setConfigValue(appConfig, std::string);
    bool setConfigValue(std::string, std::string);
    bool setConfigValue(appConfig, int);
    bool setConfigValue(std::string, int);
    bool setConfigValue(filterTypes _filter, std::string);
    bool saveConfigValue(appConfig);
    bool saveConfigValue(std::string);

    std::string getFilter(filterTypes _var);
    std::string getFsType(int type);

    FS_CONFIG   *getFsConfig();
    OMDB_CONFIG *getOmdbConfig();
    DB_CONFIG   *getDbConfig();
    LOG_CONFIG  *getLogConfig();

    std::string getOmdb(XML_OMDB_TAGS);
    std::string getImdb(XML_IMDB_TAGS);
    std::string getImdbTitleUrl(std::string);
    std::string getImdbFindUrl(std::string);

private:

    std::string homeFolder;

    //std::string configuration[CONF_MAX_VALUES];
    std::vector<dbConfiguration> *dbConf; // [CONF_MAX_VALUES] = { DBCONFFIELDS };
    DATA_CONFIG oldConfig;

    bool readConfigFromDB();
    bool saveConfigToDB(dbConfiguration *);
    dbConfiguration *getConfigRow(appConfig);
    dbConfiguration *getConfigRow(std::string);
    void separateValues(std::vector<std::string>*, std::string);

};