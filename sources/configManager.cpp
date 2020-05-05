#include "configManager.h"
#include "util.h"
#include "Convert.h"
//#include <vector>

std::vector<dbConfiguration> dbConfTmp = { DBCONFFIELDS };

#define HOMEFOLDER "\\Renoir\\"

ConfigManager::ConfigManager(void)
{
    dbConf = &dbConfTmp;
    wchar_t* pathRoamingAppData = NULL;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &pathRoamingAppData);
    std::wstring wPathRoamingAppData(pathRoamingAppData);
    homeFolder = WS2S(pathRoamingAppData) + HOMEFOLDER;
}

ConfigManager::~ConfigManager(void)
{
}

bool ConfigManager::init()
{
    return readConfigFromDB();
}

std::string ConfigManager::getHomeFolder()
{
    return homeFolder;
}

std::string ConfigManager::getConfigStr(appConfig _id)
{
    return dbConf->at(_id).value ;
}

std::string ConfigManager::getConfigStr(std::string _variable)
{
    dbConfiguration * row = getConfigRow(_variable);
    if (row != nullptr)
        return row->value;

    return std::string();
}

int ConfigManager::getConfigInt(appConfig _id)
{
    std::string val = dbConf->at(_id).value;
    if (val.empty() == false)
        return atoi(val.c_str());

    return -1;
}

int ConfigManager::getConfigInt(std::string _variable)
{
    dbConfiguration * row = getConfigRow(_variable);
    if (row != nullptr)
    {
        std::string val = row->value;
        if (val.empty() == false)
            return atoi(val.c_str());
    }

    return -1;
}

bool ConfigManager::setConfigValue(appConfig _id, std::string _value)
{
    dbConfiguration * row = &(dbConf->at(_id));
    if (row != nullptr)
    {
        row->value = _value;
        saveConfigToDB(row);
        return true;
    }
    return false;
}

bool ConfigManager::setConfigValue(std::string _variable, std::string _value)
{
    dbConfiguration * row = getConfigRow(_variable);
    if (row != nullptr)
    {
        row->value = _value;
        saveConfigToDB(row);
        return true;
    }
    return false;
}

bool ConfigManager::setConfigValue(appConfig _id, int _value)
{
    dbConfiguration * row = &(dbConf->at(_id));
    if (row != nullptr)
    {
        row->value = STR(_value);
        saveConfigToDB(row);
        return true;
    }
    return false;
}

bool ConfigManager::setConfigValue(std::string _variable, int _value)
{
    dbConfiguration * row = getConfigRow(_variable);
    if (row != nullptr)
    {
        row->value = STR(_value);
        saveConfigToDB(row);
        return true;
    }
    return false;
}

bool ConfigManager::setConfigValue(filterTypes _filter, std::string _value)
{
    std::string filter2config[FILTERS_MAX] = { "FILTER_directors", "FILTER_writers", "FILTER_actors",  "FILTER_genres",  "FILTER_genres2", "FILTER_yearmin", "FILTER_yearmax", "FILTER_imdbratingmin", "FILTER_imdbratingmax", "FILTER_serie", "FILTER_anim", "FILTER_doc", "FILTER_short" };
    std::string variable = filter2config[_filter];
    setConfigValue(variable, _value);
    return true;
}

bool ConfigManager::saveConfigValue(appConfig _id)
{
    return saveConfigToDB(&dbConf->at(_id));
}

bool ConfigManager::saveConfigValue(std::string _variable)
{
    return saveConfigToDB(getConfigRow(_variable));
}

std::string ConfigManager::getFilter(filterTypes _var)
{
    switch (_var)
    {
    case FILTERS_DIRECTORS:
        return getConfigStr("FILTER_directors");
        break;
    }
    return std::string();
}

std::string ConfigManager::getFsType(int _type)
{
    switch (_type)
    {
    case TYPE_SERIE:
        return getConfigStr("FS_typeSerie");
        break;
    case TYPE_ANIM:
        return getConfigStr("FS_typeAnim");
        break;
    case TYPE_DOC:
        return getConfigStr("FS_typeDoc");
        break;
    case TYPE_SHORT:
        return getConfigStr("FS_typeShort");
        break;
    }
    return std::string();
}

bool ConfigManager::readConfigFromDB()
{
    return GETDB.getConfiguration(dbConf);
}

bool ConfigManager::saveConfigToDB(dbConfiguration * _row)
{
    return GETDB.setConfiguration(_row);
}

dbConfiguration * ConfigManager::getConfigRow(appConfig _id)
{
    return &dbConf->at(_id);
}

dbConfiguration * ConfigManager::getConfigRow(std::string _variable)
{
    std::vector<dbConfiguration>::iterator conf;
    for (conf = dbConf->begin(); conf != dbConf->end(); conf++)
    {
        if (conf->variable.compare(_variable) == 0)
        {
            return &(*conf);
        }
    }
    return nullptr;

}

FS_CONFIG   *ConfigManager::getFsConfig() {
    oldConfig.fs.autoReadFolder = (getConfigInt("FS_autoreadFolder") == 1);
    separateValues(&oldConfig.fs.extentions, getConfigStr("FS_autoreadFolder"));
    //oldConfig.fs.extentions = (bool)getConfigInt("FS_autoreadFolder");
    oldConfig.fs.searchTypes = (getConfigInt("FS_searchTypes") == 1);
    oldConfig.fs.strType[TYPE_SERIE] = getConfigStr("FS_typeSerie");
    oldConfig.fs.strType[TYPE_ANIM] = getConfigStr("FS_typeAnim");
    oldConfig.fs.strType[TYPE_DOC] = getConfigStr("FS_typeDoc");
    oldConfig.fs.strType[TYPE_SHORT] = getConfigStr("FS_typeShort");
    return &oldConfig.fs;
}
OMDB_CONFIG *ConfigManager::getOmdbConfig() {

    return &oldConfig.omdb;
}

void ConfigManager::separateValues(std::vector<std::string> *v_list, std::string _list) {
    v_list->clear();
    size_t list_sz = _list.size();
    size_t list_end = list_sz;
    size_t found = _list.find_last_of(",");
    size_t next = found;
    while (_list[next + 1] == ' ') next++;
    while (found != std::string::npos)
    {
        list_end = _list.find_last_of("(");
        if (list_end == std::string::npos)
            list_end = list_sz;
        else
            list_end -= 1;
        //std::string str = _list.substr(next + 1, list_end - next - 1);
        v_list->push_back(_list.substr(next + 1, list_end - next - 1));
        list_sz = found;
        _list.resize(found);
        found = _list.find_last_of(",");
        next = found;
        while (_list[next + 1] == ' ') next++;
    }
    list_end = _list.find_last_of("(");
    if (list_end == std::string::npos)
        list_end = list_sz;
    else
        list_end--;
    v_list->push_back(_list.substr(0, list_end));
}

std::string ConfigManager::getImdbTitleUrl(std::string _imdbId)
{
    return getConfigStr("IMDB_url") + getConfigStr("IMDB_title") + "/" + _imdbId;
}

std::string ConfigManager::getImdbFindUrl(std::string _title)
{
    BUTIL::Convert::string2url(&_title);
    return getConfigStr("IMDB_url") + getConfigStr("IMDB_find") + _title;
}