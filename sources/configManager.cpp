#include "configManager.h"
#include "util.h"
#include "Convert.h"
#include "database.h"
#include <map>


//confMap dbConfTmp = { DBCONFFIELDS } ;

ConfigManager::ConfigManager(void)
{
    dbConf = { DBCONFFIELDS };
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
    if (readConfigFromDB() == false)
        return false;
    return (checkConfig() == -1);
}
int ConfigManager::checkConfig()
{
    int ret = -1;
    for (int c = 0; c < CONFIGMAX; c++)
    {
        configMinTest[c] = true;
        if (dbConf[configMin[c]].value.empty())
        {
            configMinTest[c] = false;
            ret = c;
        }
    }
    return ret;
}
std::string ConfigManager::getErrorConfig()
{
    for (int c = 0; c < CONFIGMAX; c++)
    {
        if (configMinTest[c] == false)
        {
            return configMin[c];
        }
    }
    return std::string();
}
bool ConfigManager::loadConfFile()
{
    return xmlConfig.initConfigFile(&fileConf);
}
bool ConfigManager::resetDBConfig()
{
    return GETDB.resetConfiguration();
}
std::string ConfigManager::getHomeFolder()
{
    return homeFolder;
}

std::string ConfigManager::getConfigStr(std::string _variable)
{
    dbConfiguration * row = getConfigRow(_variable);
    if (row != nullptr)
        return row->value;

    return std::string();
}

std::vector<std::string> ConfigManager::getConfigVect(std::string _variable, std::string _sep)
{
    std::string value;
    dbConfiguration * row = getConfigRow(_variable);
    if (row != nullptr)
    {
        value = row->value;
        std::vector<std::string> v_list;
        separateValues(&v_list, value, _sep);
        return v_list;
    }
    return std::vector<std::string>();
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

bool ConfigManager::getConfigBool(std::string _variable)
{
    bool ret = false;
    dbConfiguration * row = getConfigRow(_variable);
    if (row != nullptr)
    {
        std::string val = row->value;
        if (val.empty() == false)
            ret = BUTIL::Convert::stob(val);
    }

    return ret;
}

bool ConfigManager::setConfigValue(std::string _variable, std::string _value)
{
    dbConf[_variable].value = _value;
    return saveConfigToDB(_variable);
}

bool ConfigManager::setConfigValue(std::string _variable, int _value)
{
    dbConf[_variable].value = STR(_value);
    return saveConfigToDB(_variable);
}

bool ConfigManager::setConfigValue(std::string _variable, bool _value)
{
    dbConf[_variable].value = _value?"true":"false";
    return saveConfigToDB(_variable);
}

bool ConfigManager::setConfigValue(filterTypes _filter, std::string _value)
{
    std::string filter2config[FILTERS_MAX] = { "FILTER_directors", "FILTER_writers", "FILTER_actors",  "FILTER_genres",  "FILTER_genres2", "FILTER_yearmin", "FILTER_yearmax", "FILTER_imdbratingmin", "FILTER_imdbratingmax", "FILTER_serie", "FILTER_anim", "FILTER_doc", "FILTER_short" };
    std::string variable = filter2config[_filter];
    setConfigValue(variable, _value);
    return true;
}

std::string ConfigManager::getFilter(filterTypes _var)
{
    std::string filter2config[FILTERS_MAX] = { CONF_FILTER_DIRECTORS , CONF_FILTER_WRITERS , CONF_FILTER_ACTORS , CONF_FILTER_GENRES , CONF_FILTER_GENRES2, CONF_FILTER_YEARMIN , CONF_FILTER_YEARMAX , CONF_FILTER_IMDBRATINGMIN , CONF_FILTER_IMDBRATINGMAX , CONF_FILTER_SERIE , CONF_FILTER_ANIM , CONF_FILTER_DOC , CONF_FILTER_SHORT } ;
    return  getConfigStr(filter2config[_var]);
}

std::string ConfigManager::getFsType(int _type)
{
    std::string type2config[TYPE_MAX] = { CONF_FS_TYPESERIE , CONF_FS_TYPEANIM , CONF_FS_TYPEDOC , CONF_FS_TYPESHORT };
    return  getConfigStr(type2config[_type]);
    //switch (_type)
    //{
    //case TYPE_SERIE:
    //    return getConfigStr(CONF_FS_TYPESERIE);
    //    break;
    //case TYPE_ANIM:
    //    return getConfigStr(CONF_FS_TYPEANIM);
    //    break;
    //case TYPE_DOC:
    //    return getConfigStr(CONF_FS_TYPEDOC);
    //    break;
    //case TYPE_SHORT:
    //    return getConfigStr(CONF_FS_TYPESHORT);
    //    break;
    //}
    //return std::string();
}

bool ConfigManager::readConfigFromDB()
{
    return GETDB.getConfiguration(&dbConf);
}

bool ConfigManager::configFile2configDB()
{
    dbConf.clear();
    confMap::iterator conf;
    for (conf = fileConf.begin(); conf != fileConf.end(); conf++)
    {
        dbConf[conf->first] = conf->second;
    }
    return true;
}

bool ConfigManager::saveConfigToDB()
{
    confMap::iterator conf;

    for (conf = dbConf.begin(); conf != dbConf.end(); conf++)
    {
        GETDB.setConfiguration(conf->first, conf->second);
    }
    return true;
}

TiXmlDocument ConfigManager::getConfXml()
{
    confMap::iterator conf;

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);

    TiXmlElement* config = new TiXmlElement("config");
    doc.LinkEndChild(config);

    std::map<std::string, TiXmlElement*> xmlMap;
    std::map<std::string, TiXmlNode*> nodeMap;

    TiXmlNode* nodeTmp1, *nodeTmp2;
    TiXmlElement* elTmp;

    std::vector<std::string> v_list;
    std::string tag, tagTmp;

    for (auto const& conf : dbConf)
    {
        tagTmp = "";
        if (conf.second.value.empty() == false)
        {
            separateValues(&v_list, conf.first, "_");
            nodeTmp1 = config;
            nodeTmp2 = nodeTmp1;
            while (v_list.empty() == false)
            {
                tag = v_list.back();
                v_list.pop_back();
                tagTmp += tag;
                if (nodeMap.count(tagTmp) == 0)
                {
                    elTmp = new TiXmlElement(tag.c_str());
                    nodeMap[tagTmp] = nodeTmp1->LinkEndChild(elTmp);
                    nodeTmp1 = nodeMap[tagTmp];
                }
                else
                {
                    nodeTmp1 = nodeMap[tagTmp];
                }
            }
            TiXmlText* text = new TiXmlText(conf.second.value.c_str());
            TiXmlElement *el = nodeTmp1->ToElement();
            nodeTmp1->LinkEndChild(text);
            el->SetAttribute(CONF_DESC, conf.second.description.c_str());
        }
    }
    return doc;
}

bool ConfigManager::saveConfigToDB(std::string _var)
{
    return GETDB.setConfiguration(_var, dbConf[_var]);
}

dbConfiguration * ConfigManager::getConfigRow(std::string _variable)
{
    return &dbConf[_variable];
}
std::string * ConfigManager::getConfigValue(std::string _variable)
{
    return &dbConf[_variable].value;
}

//FS_CONFIG   *ConfigManager::getFsConfig() {
//    oldConfig.fs.autoReadFolder = (getConfigInt("FS_autoreadFolder") == 1);
//    separateValues(&oldConfig.fs.extentions, getConfigStr("FS_extentions"), ",");
//    oldConfig.fs.searchTypes = (getConfigInt("FS_searchTypes") == 1);
//    oldConfig.fs.strType[TYPE_SERIE] = getConfigStr("FS_typeSerie");
//    oldConfig.fs.strType[TYPE_ANIM] = getConfigStr("FS_typeAnim");
//    oldConfig.fs.strType[TYPE_DOC] = getConfigStr("FS_typeDoc");
//    oldConfig.fs.strType[TYPE_SHORT] = getConfigStr("FS_typeShort");
//    return &oldConfig.fs;
//}
//OMDB_CONFIG *ConfigManager::getOmdbConfig() {
//
//    oldConfig.omdb.apikey = getConfigStr("FS_typeSerie");
//    return &oldConfig.omdb;
//}

void ConfigManager::separateValues(std::vector<std::string> *v_list, std::string _list, std::string _sep) {
    v_list->clear();
    size_t list_sz = _list.size();
    size_t list_end = list_sz;
    size_t found = _list.find_last_of(_sep);
    size_t next = found;
    while (_list[next + 1] == ' ') next++;
    while (found != std::string::npos)
    {
        list_end = _list.find_last_of("(");
        if (list_end == std::string::npos)
            list_end = list_sz;
        else
            list_end -= 1;
        v_list->push_back(_list.substr(next + 1, list_end - next - 1));
        list_sz = found;
        _list.resize(found);
        found = _list.find_last_of(_sep);
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

//std::string ConfigManager::getImdbTitleUrl(std::string _imdbId)
//{
//    return getConfigStr(CONF_IMDB_URL) + getConfigStr(CONF_IMDB_TITLE) + "/" + _imdbId;
//}

//std::string ConfigManager::getImdbFindUrl(std::string _title)
//{
//    BUTIL::Convert::string2url(&_title);
//    return getConfigStr(CONF_IMDB_URL) + getConfigStr(CONF_IMDB_FIND) + _title;
//}