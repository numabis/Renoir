#include <string>
#include "variables.h"
#include "singleton.h"
#include "readConfig.h"

class ConfigManager: public BUTIL::Singleton<ConfigManager>
{

public:

    ConfigManager(void);
    ~ConfigManager(void);
    bool init();
    bool readConfFile();
    bool saveConfigToDB();

    std::string getHomeFolder();
    std::string getConfigStr(std::string);
    int getConfigInt(std::string);
    bool setConfigValue(std::string, std::string);
    bool setConfigValue(std::string, int);
    bool setConfigValue(filterTypes _filter, std::string);

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
    TiXmlDocument getConfXml();

private:

    ReadConfig xmlConfig;

    friend class BUTIL::Singleton<ConfigManager>;

    bool isDataLoaded;

    std::string homeFolder;

    //std::string configuration[CONF_MAX_VALUES];
    //std::vector<dbConfiguration> *dbConf; // [CONF_MAX_VALUES] = { DBCONFFIELDS };
    confMap dbConf;
    DATA_CONFIG oldConfig;

    bool readConfigFromDB();
    bool saveConfigToDB(std::string _var);
    //dbConfiguration *getConfigRow(appConfig);
    dbConfiguration *getConfigRow(std::string);
    std::string *getConfigValue(std::string);
    void separateValues(std::vector<std::string>* v_list, std::string _list, std::string _sep);

};