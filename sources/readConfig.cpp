#include "ReadConfig.h"
#include "Convert.h"
#include "version.h"
#include "util.h"

#define HOMEFOLDER "\\Renoir\\"
using std::string;

ReadConfig::ReadConfig()
{
//    homeFolder = HOMEFOLDER;

    wchar_t* pathRoamingAppData = NULL;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &pathRoamingAppData);
    std::wstring wPathRoamingAppData(pathRoamingAppData);
    homeFolder = WS2S(pathRoamingAppData) + HOMEFOLDER;
    CreateDirectoryA(homeFolder.c_str(), NULL);
}

ReadConfig::~ReadConfig(void)
{
}

bool ReadConfig::readXML(wchar_t *_configPath, DATA_CONFIG *_data)
{
    return false;
}

int ReadConfig::initConfigFile()
{
    FILE* file = NULL;
    int ret = XMLOK;
#if defined(_DEBUG) && !defined(_DEMO)
    configPath = DBGCONFIGFILE;
#else
    configPath = CONFIGFILE;
#endif

    ret |= loadConfigFile();
    if ((ret & XMLERROR) == XMLERROR)
    {
        exLOGERROR("Error loading config file %s", configPath);
        MessageBox(NULL, (LPCWSTR)L"Error loading config file", (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        return ret;
    }
    exLOGINFO("reading config file [%s]", configPath.c_str());
    return ret;
}

std::string ReadConfig::getVersionApp()
{
    //    xmlConfig.appVersion = MY_PRODUCT_FILE_VERSION2;
    return MY_PRODUCT_FILE_VERSION2;
}

int  ReadConfig::loadConfigFile()
{
    int ret = XMLOK;

    TiXmlDocument docConfig;
    TiXmlElement *nodeConfig = BUTIL::Xml::load(docConfig, configPath);

    if (nodeConfig && std::string(nodeConfig->Value()) == XMLTAG_CONFIG)
    {
        TiXmlElement *node[XML_CONFIG_MAX];
        for (short i=0;i<XML_CONFIG_MAX;i++)
            node[i] = nodeConfig->FirstChildElement(xmlConfigTags[i]);

        xmlConfig.fsPath = &xmlConfig.xmlLocal[XML_LOCAL_PATH];

        if (node[XML_CONFIG_FS])
            ret |= readConfigFS(node[XML_CONFIG_FS]);
        else
            exLOGERROR("%s", configPath, xmlConfigTags[XML_CONFIG_FS]);

        if (node[XML_CONFIG_DB])
            ret |= readConfigDB(node[XML_CONFIG_DB]);
        else
            exLOGERROR("%s", configPath, xmlConfigTags[XML_CONFIG_DB]);

        if (node[XML_CONFIG_OMDB])
            ret |= readConfigOMDB(node[XML_CONFIG_OMDB]);
        else
            exLOGERROR("%s", configPath, xmlConfigTags[XML_CONFIG_OMDB]);

        if (node[XML_CONFIG_IMDB])
            ret |= readConfigIMDB(node[XML_CONFIG_IMDB]);
        else
            exLOGERROR("%s", configPath, xmlConfigTags[XML_CONFIG_IMDB]);

        if (node[XML_CONFIG_LOG])
            ret |= readConfigLOG(node[XML_CONFIG_LOG]);
        else
            exLOGERROR("%s", configPath, xmlConfigTags[XML_CONFIG_LOG]);
    }
    return ret;
}

int ReadConfig::initLocalConfigFile()
{
    FILE* file;
    char configLocalPath[PATHSIZE];
    localConfigPath = LOCALCONFIGFILE;
    sprintf(configLocalPath, "%s", LOCALCONFIGFILE);

    file = fopen(configLocalPath, "r");
    if (file != NULL)
    {
        fclose(file);
    }
    else
        return XMLERROR;


    return XMLOK;
}

bool ReadConfig::saveLocalConfigFile(bool _ask)
{
    exLOGDEBUG("");
    bool saved = false;
    int ret;
    if (_ask == true)
        ret = MessageBox(NULL, (LPCWSTR)L"Save configuration?", (LPCWSTR)L"Info", MB_YESNO);
    else
        ret = IDYES;
    if (ret == IDYES)
    {
        std::stringstream sstr;
        for (int tag = XML_LOCAL_PATH; tag < XML_LOCAL_MAX; tag++)
        {
            if (xmlLocalTagType[tag] == XML_TAG)
                sstr << "<" << xmlLocalTag[tag] << ">" << xmlConfig.xmlLocal[tag] << "</" << xmlLocalTag[tag] << ">\n";
            if (xmlLocalTagType[tag] == XML_TAG_SUB)
            {
                sstr << "<" << xmlLocalTag[tag] << ">";
                for (int tagSub = FILTERS_DIRECTORS; tagSub < FILTERS_MAX; tagSub++)
                {
                    sstr << "<" << xmlLocalFiltersTag[tagSub] << ">" << xmlConfig.xmlLocalFilters[tagSub] << "</" << xmlLocalFiltersTag[tagSub] << ">\n";
                }
                sstr << "</" << xmlLocalTag[tag] << ">\n";
            }


        }

        std::string xml = BUTIL::Xml::createConfig(sstr.str());
#if defined(_DEBUG) || defined(_DEMO)
        saved = BUTIL::Xml::save(localConfigPath, xml);
#else
        saved = BUTIL::Xml::saveZ(localConfigPath, xml);
#endif

        if (saved == true)
        {
            exLOGINFO("saved configuration to [%s]", localConfigPath.c_str());
        }
        else
        {
            exLOGINFO("ERROR writting configuration file [%s]", localConfigPath.c_str());

            std::ostringstream msg;
            msg << "ERROR writting configuration file " << localConfigPath.c_str() << "\nPlease, check the WRITE permission to folder\nYou may need to execute the app as admin";
            MBOX(msg.str(), "Error", MB_OK);
        }

    }
    return saved;

}

int  ReadConfig::loadLocalConfigFile()
{
    TiXmlDocument docConfig;
#if defined(_DEBUG) || defined(_DEMO) || !defined(_COMPRESSFILES)
    TiXmlElement *nodeConfig = BUTIL::Xml::load(docConfig, localConfigPath);
#else
    TiXmlElement *nodeConfig = BUTIL::Xml::loadZ(docConfig, localConfigPath);
#endif

    int ret = XMLERROR;

    if (nodeConfig && std::string(nodeConfig->Value()) == XMLTAG_CONFIG)
    {
        ret = XMLOK;

        short tag = XML_LOCAL_PATH;
        //xmlConfig.xmlLocal[tag] = homeFolder;
        xmlConfig.xmlLocal[tag] = BUTIL::Xml::XML_string(nodeConfig, xmlLocalTag[tag]);

        for (tag = XML_LOCAL_APIKEY; tag < XML_LOCAL_MAX; tag++)
        {
             if (xmlLocalTagType[tag] == XML_TAG)
                 xmlConfig.xmlLocal[tag] = BUTIL::Xml::XML_string(nodeConfig, xmlLocalTag[tag]);
            if (xmlLocalTagType[tag] == XML_TAG_SUB)
            {
                TiXmlElement *nodeFilters = nodeConfig->FirstChildElement(xmlLocalTag[tag].c_str());
                for (int tagSub = FILTERS_DIRECTORS; tagSub < FILTERS_MAX; tagSub++)
                {
                    xmlConfig.xmlLocalFilters[tagSub] = BUTIL::Xml::XML_string(nodeFilters, xmlLocalFiltersTag[tagSub]);
                }
            }
        }

        if (xmlConfig.xmlLocal[XML_LOCAL_PATH].empty() == false)
        {
            DWORD ftyp = GetFileAttributesA(xmlConfig.xmlLocal[XML_LOCAL_PATH].c_str());
            int error = ::GetLastError();
            if (error != ERROR_SUCCESS)
            {
                wchar_t wmsg[256];
                char* msg = BUTIL::Util::GetLastErrorAsString(error);
                _swprintf(wmsg, L"%s %s", BUTIL::Convert::charToWchar(msg), BUTIL::Convert::charToWchar((LPSTR)xmlConfig.xmlLocal[XML_LOCAL_PATH].c_str()));
                MessageBox(NULL, wmsg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
                xmlConfig.xmlLocal[XML_LOCAL_PATH] = "";
            }

        }
        if (xmlConfig.xmlLocal[XML_LOCAL_PATH].empty() == true)
            ret |= XMLNOFSPATH;
        
        xmlConfig.omdb.apikey = &xmlConfig.xmlLocal[XML_LOCAL_APIKEY];


        if (xmlConfig.xmlLocal[XML_LOCAL_APIKEY].empty() == true)
            ret |= XMLNOOMDBKEY;

    }
    exLOGINFO("loaded configuration from [%s], res=%s", localConfigPath.c_str(), ret == 0 ? "ok" : "ko");
    for (short tag = XML_LOCAL_PATH; tag < XML_LOCAL_MAX; tag++)
    {
        exLOGINFO("[%s] = [%s]", xmlLocalTag[tag].c_str(), xmlConfig.xmlLocal[tag].c_str());
    }

    return ret;
}

int  ReadConfig::readConfigFS(TiXmlElement *_node) {
    int ret = XMLOK;

    xmlConfig.fs.path = xmlConfig.fsPath;

    if (BUTIL::Xml::existsNode(_node, "autoreadfolder"))
        xmlConfig.fs.autoReadFolder = BUTIL::Xml::XML_bool(_node, "autoreadfolder");
    else
        xmlConfig.fs.autoReadFolder = false;

    TiXmlElement *nodeExt = _node->FirstChildElement("extentions");
    if (nodeExt)
    {
        std::string ext, ext2;

        for (TiXmlElement* e = nodeExt->FirstChildElement("extention"); e != NULL; e = e->NextSiblingElement("extention"))
        {
            ext = BUTIL::Xml::XML_string(e);
            BUTIL::Convert::toLower(&ext);
            xmlConfig.fs.extentions.push_back(ext);
        }
    }
    TiXmlElement *nodeTypes = _node->FirstChildElement("types");
    if (nodeTypes)
    {
        xmlConfig.fs.searchTypes = BUTIL::Xml::XML_bool(nodeTypes, "searchtypes");
        for (int type= TYPE_SERIE; type<TYPE_MAX; type++)
            xmlConfig.fs.strType[type] = BUTIL::Xml::XML_string(nodeTypes, xmlTypes[type]);
    }
    return ret;
}

int  ReadConfig::readConfigDB(TiXmlElement *node) {
    int ret = XMLOK;
    xmlConfig.db.dbPath = homeFolder;
    if (BUTIL::Xml::existsNode(node, "dbpath") && BUTIL::Xml::XML_string(node, "dbpath").size() != 0)
    {
        xmlConfig.db.dbPath += BUTIL::Xml::XML_string(node, "dbpath");
    }

    if (BUTIL::Xml::existsNode(node, "dbname") && BUTIL::Xml::XML_string(node, "dbname").size() != 0)
        xmlConfig.db.dbName = BUTIL::Xml::XML_string(node, "dbname");
    else
        xmlConfig.db.dbName = DBNAME;

    if (BUTIL::Xml::existsNode(node, "resetdb"))
        xmlConfig.db.resetDB = BUTIL::Xml::XML_bool(node, "resetdb");
    else
        xmlConfig.db.resetDB = DBRESETDEFAULT;
    return ret;
}

int  ReadConfig::readConfigOMDB(TiXmlElement *_node) {
    int ret = XMLOK;
    OMDB_CONFIG *omdb = &xmlConfig.omdb;
    omdb->omdbUrl = BUTIL::Xml::XML_string(_node, xmlOmdbTags[XML_OMDB_URL]);
    omdb->apiRequestUrl = BUTIL::Xml::XML_string(_node, xmlOmdbTags[XML_OMDB_API]);
    omdb->plot = BUTIL::Xml::XML_string(_node, xmlOmdbTags[XML_OMDB_PLOT]);
    omdb->type = BUTIL::Xml::XML_string(_node, xmlOmdbTags[XML_OMDB_TYPE]);
    omdb->separator = BUTIL::Xml::XML_string(_node, xmlOmdbTags[XML_OMDB_SEP]);
    omdb->testmode = BUTIL::Xml::XML_bool(_node, xmlOmdbTags[XML_OMDB_TEST]);
    omdb->plotShort = xmlConfig.omdb.plot == xmlOmdbTags[XML_OMDB_PLOT] ? true : false;
    omdb->typeXml = xmlConfig.omdb.type == xmlOmdbTags[XML_OMDB_TYPE] ? true : false;
    return ret;
}

int  ReadConfig::readConfigIMDB(TiXmlElement *_node) {
    int ret = XMLOK;
    for (int i= XML_IMDB_URL ; i < XML_IMDB_MAX ; i++)
        xmlConfig.imdb[i] = BUTIL::Xml::XML_string(_node, xmlImdbTags[i]);
    return ret;
}

int  ReadConfig::readConfigLOG(TiXmlElement *_node) {
    int ret = XMLOK;
    LOG_CONFIG *log = &xmlConfig.log;
    log->logPath = homeFolder;
    if (BUTIL::Xml::existsNode(_node, "logPath") && BUTIL::Xml::XML_string(_node, "logPath").size() != 0)
    {
        log->logPath += BUTIL::Xml::XML_string(_node, "logPath");
    }

    if (BUTIL::Xml::existsNode(_node, "level"))
        log->logLevel = BUTIL::Xml::XML_int(_node, "level");
    else
        log->logLevel = 0;
    return ret;
}

DATA_CONFIG *ReadConfig::getGlobalConfig() {
    return &xmlConfig;
}
FS_CONFIG   *ReadConfig::getFsConfig() {
    return &xmlConfig.fs;
}
OMDB_CONFIG *ReadConfig::getOmdbConfig() {
    return &xmlConfig.omdb;
}
std::string ReadConfig::getOmdb(XML_OMDB_TAGS tag)
{
    switch (tag)
    {
    case XML_OMDB_KEY:
        return *xmlConfig.omdb.apikey;
    case XML_OMDB_URL:
        return xmlConfig.omdb.omdbUrl;
        //    case XML_OMDB_IMURL:
        //        return xmlConfig.omdb.imdbUrl;
    case XML_OMDB_PLOT:
        return xmlConfig.omdb.plot;
    case XML_OMDB_SEP:
        return xmlConfig.omdb.separator;
    case XML_OMDB_TYPE:
        return xmlConfig.omdb.type;
    default:
        return std::string();
    }
    return std::string();
}
std::string ReadConfig::getImdb(XML_IMDB_TAGS _tag)
{
    if (_tag>XML_IMDB_URL && _tag<XML_IMDB_MAX)
        return xmlConfig.imdb[_tag];
    return std::string();
}

std::string ReadConfig::getImdbTitleUrl(std::string _imdbId)
{
    return xmlConfig.imdb[XML_IMDB_URL] + xmlConfig.imdb[XML_IMDB_TITLE] + "/" + _imdbId;
}
std::string ReadConfig::getImdbFindUrl(std::string _title)
{
    BUTIL::Convert::string2url(&_title);
    return xmlConfig.imdb[XML_IMDB_URL] + xmlConfig.imdb[XML_IMDB_FIND] + _title;
}

DB_CONFIG   *ReadConfig::getDbConfig() {
    return &xmlConfig.db;
}
LOG_CONFIG  *ReadConfig::getLogConfig() {
    return &xmlConfig.log;
}
string ReadConfig::getXmlLocalConfig(XML_LOCAL_VARS _var)
{
    return xmlConfig.xmlLocal[_var];
}
string* ReadConfig::getXmlLocalConfig()
{
    return xmlConfig.xmlLocal;
}
std::string ReadConfig::getXmlLocalFullPath()
{
    return xmlConfig.xmlLocal[XML_LOCAL_SAVE_PATH] + "\\" + xmlConfig.xmlLocal[XML_LOCAL_SAVE_NAME] + "." + xmlConfig.xmlLocal[XML_LOCAL_SAVE_EXT];
}
void ReadConfig::setXmlLocalConfig(XML_LOCAL_VARS _var, std::string _value)
{
    xmlConfig.xmlLocal[_var] = _value;
}
std::string ReadConfig::getXmlLocalFilters(filterTypes _var)
{
    return xmlConfig.xmlLocalFilters[_var];
}

void ReadConfig::setXmlLocalFilters(filterTypes _var, std::string _value)
{
    xmlConfig.xmlLocalFilters[_var] = _value;
}