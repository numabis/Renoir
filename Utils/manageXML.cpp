#include "manageXML.h"
#include "Convert.h"
#include "version.h"
#include "definitions.h"
#include "database.h"
#include "omdb.h"

#include <fstream>

#define OMDBPLOTSHORT   "short"
#define OMDBPLOTFULL    "full"
#define OMDBTYPEXML     "xml"
#define OMDBTYPEJSON    "json"

#define XMLFORMATCONFIG     "config"
#define XMLFORMATDB         "database"
#define XMLFORMATFS         "filesystem"
#define XMLFORMATOM         "omdb"
#define XMLFORMATLOG        "log"
#define XMLFORMATXML        "xml"
#define XMLFORMATFSPATH     "fspath"
#define XMLFORMATCONFIGFILE "configfile"


std::string omdbXmlAttr[] = { "title", "year", "rated", "released", "runtime", "genre", "director", "writer", "actors", "plot", "language", "country", "awards", "poster", "metascore", "imdbRating", "imdbVotes", "imdbID", "type" };
std::vector<Movie> vectorMovies;

ManageXML::ManageXML()
{
    countXmlLoad[READ] = 0;
    countXmlSave[READ] = 0;
    countXmlLoad[TOTAL] = 0;
    countXmlSave[TOTAL] = 0;
}

ManageXML::~ManageXML(void)
{
}

int ManageXML::initConfigFile()
{
    FILE* file = NULL;
    int ret = XMLOK;
#ifdef _DEBUG
    configPath = DBGCONFIGFILE;
#else
    configPath = CONFIGFILE;
#endif
    file = _wfopen(configPath.c_str(), L"r");
    if (file != NULL)
        fclose(file);
    else
    {
        wchar_t msg[PATHSIZE * 2];
        wsprintf(msg, L"%s %s", configPath, L" is missing");
        MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        return XMLERROR;
    }

    ret |= loadConfigFile();
    if ((ret & XMLERROR) == XMLERROR)
    {
        MessageBox(NULL, (LPCWSTR)L"Error loading config file", (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        return ret;
    }
    
    return ret;
}

std::string ManageXML::getVersionApp()
{
//    xmlConfig.appVersion = MY_PRODUCT_FILE_VERSION2;
    return MY_PRODUCT_FILE_VERSION2;
}

void ManageXML::addMovie2vector(Movie *_movie)
{
    for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it)
    {
        if (it->compare(*_movie) == true)
        {
            it->updateMovie(*_movie);
            return;
        }
    }
    vectorMovies.push_back(*_movie);
    //DataBase::getInstance().insertMovie(*_movie);
}

int ManageXML::loadDBfromVector()
{
    int count = 0;
    for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it)
    {
        if (DataBase::getInstance().insertMovie(*it) == true)
            count++;
    }
    return count;
}

//<root response="False"><error>Movie not found!</error></root>
bool ManageXML::readOmdbError(std::string *_xml, filetype *_file)
{
    TiXmlDocument docOmdbXml;
    TiXmlElement *nodeRoot = BUTIL::Xml::parse(docOmdbXml, *_xml);

    if (nodeRoot && std::string(nodeRoot->Value()) == "root")
    {
        bool response = BUTIL::Xml::XML_ATTR_bool(nodeRoot, "response");
        if (response == false)
        {
            TiXmlElement *nodeError = nodeRoot->FirstChildElement("error");
            if (nodeError)
            {
                *_xml = nodeError->GetText();
                _file->imdbId = OMDBNOTFOUND;
            }
            return true;
        }
    }
    return false;
}

std::vector<filetype> *ManageXML::getV_load() {
    return &v_load;
}

bool ManageXML::isRunning(HANDLE  _hThread)
{
    DWORD exitCode = 0;
    if (_hThread)
        GetExitCodeThread(_hThread, &exitCode);
    if (exitCode == STILL_ACTIVE)
        return true;
    return false;
}

//<root response="False"><error>Movie not found!</error></root>
int ManageXML::readOmdbXml(TiXmlElement *_nodeRoot, filetype *_file)
{
    countXmlLoad[TOTAL] = 0;
    countXmlLoad[READ] = 0;
    if (_nodeRoot && std::string(_nodeRoot->Value()) == "root")
    {
        bool response = BUTIL::Xml::XML_ATTR_bool(_nodeRoot, "response");
        if (response == true)
        {
            TiXmlElement *nodeMovie = _nodeRoot->FirstChildElement("movie");
            if (nodeMovie) 
            {
                for (TiXmlElement* e = _nodeRoot->FirstChildElement("movie"); e != NULL; e = e->NextSiblingElement("movie"))
                {
                    countXmlLoad[TOTAL]++;
                }
            }

            nodeMovie = _nodeRoot->FirstChildElement("movie");
            if (nodeMovie)
            {
                for (TiXmlElement* e = _nodeRoot->FirstChildElement("movie"); e != NULL; e = e->NextSiblingElement("movie"))
                {
                    std::string xmlValues[XML_NBATTRIBUTES];
                    memset(&xmlValues, 0, sizeof(xmlValues[XML_NBATTRIBUTES]));
                    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
                    {
                        xmlValues[attr] = BUTIL::Xml::XML_ATTR_string(e, omdbXmlAttr[attr]);
                    }
                    Movie movie(xmlValues, xmlConfig.omdb.separator);
                    if (_file != NULL) {
                        _file->imdbId = movie.getImdbId();
                        _file->imdbRating = movie.getImdbRating();
                    }

                    int movieId = DataBase::getInstance().getMovieId(movie);
                    if (movieId < 0) 
                    {
                        DataBase::getInstance().insertMovie(movie);
                        if (_file == NULL)
                        {
                            filetype file;
                            file.title = movie.getTitleASCII();
                            file.year = movie.getYear();
                            file.imdbId = movie.getImdbId();
                            v_load.push_back(file);
                        }
                    }
                    countXmlLoad[READ]++;
                }
            }
        }
    }
    return countXmlLoad[READ];

}

//<root response="False"><error>Movie not found!</error></root>
int ManageXML::readOmdbXml(TiXmlElement *_nodeRoot, filetype *_file, Movie *_movie)
{
    countXmlLoad[TOTAL] = 0;
    countXmlLoad[READ] = 0;
    if (_nodeRoot && std::string(_nodeRoot->Value()) == "root")
    {
        bool response = BUTIL::Xml::XML_ATTR_bool(_nodeRoot, "response");
        if (response == true)
        {
            TiXmlElement *nodeMovie = _nodeRoot->FirstChildElement("movie");
            if (nodeMovie) {
                for (TiXmlElement* e = _nodeRoot->FirstChildElement("movie"); e != NULL; e = e->NextSiblingElement("movie"))
                {
                    countXmlLoad[TOTAL]++;
                }
            }

            nodeMovie = _nodeRoot->FirstChildElement("movie");
            if (nodeMovie)
            {
                for (TiXmlElement* e = _nodeRoot->FirstChildElement("movie"); e != NULL; e = e->NextSiblingElement("movie"))
                {
                    std::string xmlValues[XML_NBATTRIBUTES];
                    memset(&xmlValues, 0, sizeof(xmlValues[XML_NBATTRIBUTES]));
                    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
                    {
                        xmlValues[attr] = BUTIL::Xml::XML_ATTR_string(e, omdbXmlAttr[attr]);
                    }
                    _movie->initValues(xmlValues);
                    _movie->setSeparator(xmlConfig.omdb.separator);
                    //Movie newMovie(xmlValues, xmlConfig.omdb.separator);
                    if (_file != NULL)
                        _file->imdbId = _movie->getImdbId();
                    //
                    //                  //  addMovie2vector(&newMovie);
                    //DataBase::getInstance().insertMovie(newMovie);
                    countXmlLoad[READ]++;
                }
            }
        }
    }
    return countXmlLoad[READ];

}

int *ManageXML::getCountXmlLoad()
{
    return countXmlLoad;
}
int *ManageXML::getCountXmlSave()
{
    return countXmlSave;
}

HANDLE ManageXML::saveMoviesToXml(std::string _path, bool _compress)
{
    saveXmlpath = _path;
    saveXmlCompress = _compress;
    DWORD  hThreadIdArray;
    return  CreateThread(NULL, 0, saveMoviesToXmlThread, (void*) this, 0, &hThreadIdArray);
}

static DWORD WINAPI saveMoviesToXmlThread(void* Param)
{
    ManageXML* This = (ManageXML*)Param;
    return This->saveMoviesToXmlStart();
}

bool ManageXML::saveMoviesToXmlStart()
{
    std::string movie;
    std::stringstream str;
    countXmlSave[TOTAL] = vectorMovies.size();
    countXmlSave[READ] = 0;
    for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it)
    {
        movie = movieToXml(*it);
        str << movie;
        countXmlSave[READ]++;
    }
    std::string xml = BUTIL::Xml::create(str.str(), true);
    if (saveXmlCompress)
        return BUTIL::Xml::saveZ(saveXmlpath, xml);
    else
        return BUTIL::Xml::save(saveXmlpath, xml);
    return false;
}

std::string ManageXML::movieToXml(Movie _movie)
{
    std::stringstream str;
    std::string xmlValues[XML_NBATTRIBUTES];
    _movie.movie2string(xmlValues);
    str << "<movie ";
    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
    {
        str << BUTIL::Xml::XML_attr(omdbXmlAttr[attr].c_str(), xmlValues[attr]);
    }
    str << "/>";
    return str.str();
}

Movie ManageXML::getMovie(filetype _file)
{
    Movie movie(_file);
    //for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it) 
    //{
    //    if (it->compare(movie) == true)
    //    {
    //        return *it;
    //    }
    //}
    if (DataBase::getInstance().getMovieData(&movie))
    {
        movie.setSeparator(xmlConfig.omdb.separator);
        //addMovie2vector(&movie);
    }
    return movie;
}

bool ManageXML::loadMoviesFromDB()
{
    filetype file;
    return false;
}

bool  ManageXML::readJSON(std::string _xml)
{
    exLOGERROR("JSON not implemented");
    return false;
}

bool ManageXML::loadOmdbStr(std::string _xml, filetype *_file)
{
    TiXmlDocument docOmdbXml;
    TiXmlElement *nodeRoot = BUTIL::Xml::parse(docOmdbXml, _xml);
    return (readOmdbXml(nodeRoot, _file)>0);
}

HANDLE ManageXML::loadOmdbFile()
{
    DWORD  hThreadIdArray;
    return  CreateThread(NULL, 0, loadOmdbFileThread, (void*) this, 0, &hThreadIdArray);
}

static DWORD WINAPI loadOmdbFileThread(void* Param)
{
    ManageXML* This = (ManageXML*)Param;
    return This->loadOmdbFileStart();
}

int ManageXML::loadOmdbFileStart()
{
    std::string pathXml = xmlConfig.xml.xmlPath + "\\" + xmlConfig.xml.xmlName;
    TiXmlDocument docOmdbXml;
    if (xmlConfig.xml.compressXmlFile)
    {
        TiXmlElement *nodeRoot = BUTIL::Xml::loadZ(docOmdbXml, pathXml + ".zxml");
        if (nodeRoot)
        {
            int count = readOmdbXml(nodeRoot, NULL);
            if (count > 0)
                return count;
        }
    }

    TiXmlElement *nodeRoot = BUTIL::Xml::load(docOmdbXml, pathXml + ".xml");

    if (nodeRoot)
        return readOmdbXml(nodeRoot, NULL);
    
    return 0;
}

int  ManageXML::loadConfigFile()
{
    int ret = XMLOK;

    TiXmlDocument docConfig;
    TiXmlElement *nodeConfig = BUTIL::Xml::load(docConfig, BUTIL::Convert::wstring2string(configPath));

    if (nodeConfig && std::string(nodeConfig->Value()) == XMLFORMATCONFIG)
    {
        TiXmlElement *nodeDB = nodeConfig->FirstChildElement(XMLFORMATDB);
        TiXmlElement *nodeFs = nodeConfig->FirstChildElement(XMLFORMATFS);
        TiXmlElement *nodeOmdb = nodeConfig->FirstChildElement(XMLFORMATOM);
        TiXmlElement *nodeLog = nodeConfig->FirstChildElement(XMLFORMATLOG);
        TiXmlElement *nodeXml = nodeConfig->FirstChildElement(XMLFORMATXML);

        xmlConfig.configFile = BUTIL::Xml::XML_string(nodeConfig, XMLFORMATCONFIGFILE);
        xmlConfig.fsPath = BUTIL::Xml::XML_string(nodeConfig, XMLFORMATFSPATH);
        if (xmlConfig.fsPath.size() == 0)
            ret |= XMLNOFSPATH;

        if (nodeFs)
            ret |= readConfigFS(nodeFs);
        else
            exLOGERROR("%s", configPath, XMLFORMATFS);

        if (nodeDB)
            ret |= readConfigDB(nodeDB);
        else
            exLOGERROR("%s", configPath, XMLFORMATDB);

        if (nodeOmdb)
            ret |= readConfigOMDB(nodeOmdb);
        else
            exLOGERROR("%s", configPath, XMLFORMATOM);

        if (nodeLog)
            ret |= readConfigLOG(nodeLog);
        else
            exLOGERROR("%s", configPath, XMLFORMATLOG);

        if (nodeXml)
            ret |= readConfigXML(nodeXml);
        else
            exLOGERROR("%s", configPath, XMLFORMATXML);

    }
    return ret;
}

bool  ManageXML::loadSavedConfigFile()
{
    TiXmlDocument docConfig;
    TiXmlElement *nodeConfig = BUTIL::Xml::load(docConfig, xmlConfig.configFile);
    
    bool ret = false;

    if (nodeConfig && std::string(nodeConfig->Value()) == XMLFORMATCONFIG)
    {
        ret = true;
        //TiXmlElement *nodeDB = nodeConfig->FirstChildElement(XMLFORMATDB);
        //TiXmlElement *nodeFs = nodeConfig->FirstChildElement(XMLFORMATFS);
        //TiXmlElement *nodeOmdb = nodeConfig->FirstChildElement(XMLFORMATOM);
        //TiXmlElement *nodeLog = nodeConfig->FirstChildElement(XMLFORMATLOG);
        //TiXmlElement *nodeXml = nodeConfig->FirstChildElement(XMLFORMATXML);

        if (BUTIL::Xml::existsNode(nodeConfig, XMLFORMATFSPATH))
        {
            xmlConfig.fsPath = BUTIL::Xml::XML_string(nodeConfig, XMLFORMATFSPATH);
            xmlConfig.fs.path = xmlConfig.fsPath;
        }
        else
            ret = false;

        if (BUTIL::Xml::existsNode(nodeConfig, XMLFORMATFSPATH))
            xmlConfig.omdb.apikey = BUTIL::Xml::XML_string(nodeConfig, "apikey");
        else
            ret = false;

        //if (nodeFs)
        //    readConfigFS(nodeFs);
        //
        //if (nodeDB)
        //    readConfigDB(nodeDB);
        //
        //if (nodeOmdb)
        //    readConfigOMDB(nodeOmdb);
        //
        //if (nodeLog)
        //    readConfigLOG(nodeLog);
        //
        //if (nodeXml)
        //    readConfigXML(nodeXml);

    }
    return ret;
}

int  ManageXML::readConfigFS(TiXmlElement *_node) {
    int ret = XMLOK;
    if (BUTIL::Xml::existsNode(_node, "path") && BUTIL::Xml::XML_string(_node, "path").size() != 0)
        xmlConfig.fs.path = BUTIL::Xml::XML_string(_node, "path");
    else
        xmlConfig.fs.path = xmlConfig.fsPath;
//    BUTIL::Convert::toLower(&xmlConfig.fs.path);

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
    return ret;
}

int  ManageXML::readConfigDB(TiXmlElement *node) {
    int ret = XMLOK;
    if (BUTIL::Xml::existsNode(node, "dbpath") && BUTIL::Xml::XML_string(node, "dbpath").size() != 0)
        xmlConfig.db.dbPath = BUTIL::Xml::XML_string(node, "dbpath");
    else
        xmlConfig.db.dbPath = xmlConfig.fsPath;
//    BUTIL::Convert::toLower(&xmlConfig.db.dbPath);
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

int  ManageXML::readConfigOMDB(TiXmlElement *_node) {
    int ret = XMLOK;
    OMDB_CONFIG *omdb = &xmlConfig.omdb;
    omdb->apikey = BUTIL::Xml::XML_string(_node, "apikey");
    omdb->url = BUTIL::Xml::XML_string(_node, "url");
    omdb->plot = BUTIL::Xml::XML_string(_node, "plot");
    omdb->type = BUTIL::Xml::XML_string(_node, "type");
    omdb->separator= BUTIL::Xml::XML_string(_node, "listseparator");
    omdb->testmode = BUTIL::Xml::XML_bool(_node, "testmode");
    //omdb->writeSingleFileXml = BUTIL::Xml::XML_bool(_node, "writeSingleFileXml");
    omdb->plotShort = xmlConfig.omdb.plot == OMDBPLOTSHORT ? true : false;
    omdb->typeXml = xmlConfig.omdb.type == OMDBTYPEXML ? true : false;
    return ret;
}

int  ManageXML::readConfigLOG(TiXmlElement *_node) {
    int ret = XMLOK;
    LOG_CONFIG *log = &xmlConfig.log;
    if (BUTIL::Xml::existsNode(_node, "logPath") && BUTIL::Xml::XML_string(_node, "logPath").size() != 0)
        log->logPath = BUTIL::Xml::XML_string(_node, "logPath");
    else
        log->logPath = xmlConfig.fsPath;
//    BUTIL::Convert::toLower(&log->logPath);

    if (BUTIL::Xml::existsNode(_node, "level"))
        log->logLevel = BUTIL::Xml::XML_int(_node, "level");
    else
        log->logLevel = 0;
    return ret;
}

int  ManageXML::readConfigXML(TiXmlElement *_node) {
    int ret = XMLOK;
    XML_CONFIG *xml = &xmlConfig.xml;

//    xml->writeSingleFileXml = BUTIL::Xml::XML_bool(_node, "writeSingleFileXml");

    if (BUTIL::Xml::existsNode(_node, "xmlpath") && BUTIL::Xml::XML_string(_node, "xmlpath").size() != 0)
        xml->xmlPath = BUTIL::Xml::XML_string(_node, "xmlpath");
    else
        xml->xmlPath = xmlConfig.fsPath;
//    BUTIL::Convert::toLower(&xml->xmlPath);

    if (BUTIL::Xml::existsNode(_node, "xmlname") && BUTIL::Xml::XML_string(_node, "xmlname").size() != 0)
        xml->xmlName = BUTIL::Xml::XML_string(_node, "xmlname");
    else
        xml->xmlName = XMLLOGNAME;

    if (BUTIL::Xml::existsNode(_node, "compressxmlfile"))
        xml->compressXmlFile = BUTIL::Xml::XML_bool(_node, "compressxmlfile");
    else
        xml->compressXmlFile = false;
    return ret;

}

DATA_CONFIG *ManageXML::getGlobalConfig() {
    return &xmlConfig;
}
FS_CONFIG   *ManageXML::getFsConfig() {
    return &xmlConfig.fs;
}
OMDB_CONFIG *ManageXML::getOmdbConfig() {
    return &xmlConfig.omdb;
}
DB_CONFIG   *ManageXML::getDbConfig() {
    return &xmlConfig.db;
}
LOG_CONFIG  *ManageXML::getLogConfig() {
    return &xmlConfig.log;
}
XML_CONFIG  *ManageXML::getXmlConfig() {
    return &xmlConfig.xml;
}