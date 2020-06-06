#include "manageXML.h"
#include "Convert.h"
#include "definitions.h"
#include "database.h"
#include "Util.h"
#include "configManager.h"

#include <fstream>

#define NODE_ROOT "root"
#define NODE_MOVIE "movie"
#define NODE_ERROR "error"
#define ATTR_RESPONSE "response"
#define ATTR_TOTALRESULTS "totalResults"

std::string omdbXmlAttr[] = { "title", "year", "rated", "released", "runtime", "genre", "director", "writer", "actors", "plot", "language", "country", "awards", "poster", "metascore", "imdbRating", "imdbVotes", "imdbID", "type" };
std::vector<Movie> vectorMovies;

ManageXML::ManageXML()
{
    //countXmlLoad[CNT_READ] = 0;
    //countXmlSave[CNT_READ] = 0;
    //countXmlLoad[CNT_TOTAL] = 0;
    //countXmlSave[CNT_TOTAL] = 0;
    stopThreads = false;
}

ManageXML::~ManageXML(void)
{
}

//void ManageXML::init(std::string* _config, std::string _separator)
//{
//    xmlConfig = _config;
//    omdbSeparator = _separator;
//}

//void ManageXML::init()
//{
//    xmlpath = GETCM.getConfigStr(CONF_XML_PATH);
//}
//
//void ManageXML::init(std::string _path)
//{
//    xmlpath = _path;
//}


//std::string ManageXML::getXmlLocalFullPath()
//{
//    return xmlConfig[XML_LOCAL_SAVE_PATH] + "\\" + xmlConfig[XML_LOCAL_SAVE_NAME] + "." + xmlConfig[XML_LOCAL_SAVE_EXT];
//}

void ManageXML::clearMovieVector()
{
    vectorMovies.clear();
}

bool ManageXML::ifExistsUpdateMovieVector(Movie *_movie)
{
    for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it)
    {
        if (it->compare(*_movie) == true)
        {
            it->updateMovie(*_movie);
            return true;
        }
    }
    return false;
}
 
void ManageXML::addToMovieVector(Movie *_movie)
{
    vectorMovies.push_back(*_movie);
}

int ManageXML::loadDBfromVector()
{
    int count = 0;
    GETDB.TRANSACTION_begin();
    for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it)
    {
        if (GETDB.MOVIES_insert(&(*it)) == true)
            count++;
    }
    GETDB.TRANSACTION_commit();
    return count;
}

bool ManageXML::readOmdbError(MovieFile *_file)
{ // //<root response="False"><error>Movie not found!</error></root>
    TiXmlDocument docOmdbXml;
    TiXmlElement *nodeRoot = BUTIL::Xml::parse(docOmdbXml, _file->getXmlStr());

    if (nodeRoot && std::string(nodeRoot->Value()) == NODE_ROOT)
    {
        bool response = BUTIL::Xml::XML_ATTR_bool(nodeRoot, ATTR_RESPONSE);
        if (response == false)
        {
            TiXmlElement *nodeError = nodeRoot->FirstChildElement(NODE_ERROR);
            if (nodeError)
            {
                _file->setXmlStr(nodeError->GetText(), false);
                _file->imdbId = OMDBNOTFOUND;
            }
            return true;
        }
    }
    return false;
}

std::vector<MovieFile> *ManageXML::getV_load() {
    return &v_load;
}
BUTIL::Mutex* ManageXML::getMtx_vLoad()
{
    return &mtx_vLoad;
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

int ManageXML::readOmdbXml(TiXmlElement *_nodeRoot, MovieFile *_file)
{ 
    //ST_SAVE(ST_READING_FILE);
    if (_nodeRoot && std::string(_nodeRoot->Value()) == NODE_ROOT)
    {
        bool response = BUTIL::Xml::XML_ATTR_bool(_nodeRoot, ATTR_RESPONSE);
        if (response == true)
        {
            TiXmlElement *nodeMovie = _nodeRoot->FirstChildElement(NODE_MOVIE);
            if (nodeMovie)
            {
                if (progressCounter[CNT_TOTAL] == 0)
                for (TiXmlElement* e = _nodeRoot->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
                {
                    progressCounter[CNT_TOTAL]++;
                }
                GETDB.TRANSACTION_begin();
                for (TiXmlElement* e = _nodeRoot->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
                {
                    if (stopThreads)
                    {
                        stopThreads = false;
                        break;
                    }
                    std::string xmlValues[XML_NBATTRIBUTES];
                    memset(&xmlValues, 0, sizeof(xmlValues[XML_NBATTRIBUTES]));
                    //memset(&_file, 0, sizeof(MovieFile));
                    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
                    {
                        xmlValues[attr] = BUTIL::Xml::XML_ATTR_string(e, omdbXmlAttr[attr]);
                    }
                    _file->initMovieValue(xmlValues, omdbSeparator);

                        GETDB.MOVIES_insert(_file->getMovie());
                        if (!_file->exists)
                        {
                            MovieFile file;
                            mtx_vLoad.lock();
                            v_load.push_back(*_file);
                            mtx_vLoad.unlock();
                        }
                        progressCounter[CNT_READ]++;
                }
                GETDB.TRANSACTION_commit();
            }
        }
    }
//    progressCounter[CNT_TOTAL] = 0;
//    progressCounter[CNT_READ] = 0;
    //ST_RESTORE();
        return progressCounter[CNT_READ];

}

void ManageXML::setGenRootNode(std::string _xml)
{
    //TiXmlDocument docOmdbXml;
    genRootNode = BUTIL::Xml::parse(genXmlDoc, _xml);

}

int ManageXML::readTotalResults(TiXmlElement *_node)
{
    int totalResults = 0;

    if (_node && std::string(_node->Value()) == NODE_ROOT)
    {
        totalResults = BUTIL::Xml::XML_ATTR_int(_node, ATTR_TOTALRESULTS);
    }
    return totalResults;
}

int ManageXML::readTotalResults(std::string _omdbRes)
{
    TiXmlDocument docOmdbXml;
    TiXmlElement *nodeRoot = BUTIL::Xml::parse(docOmdbXml, _omdbRes);

    return readTotalResults(nodeRoot);
}

bool ManageXML::readResponse(TiXmlElement *_node)
{
    bool response = false;

    if (_node && std::string(_node->Value()) == NODE_ROOT)
    {
        response = BUTIL::Xml::XML_ATTR_bool(_node, ATTR_RESPONSE);
    }
    return response;
}

bool ManageXML::readResponse(std::string _omdbRes)
{
    TiXmlDocument docOmdbXml;
    TiXmlElement *nodeRoot = BUTIL::Xml::parse(docOmdbXml, _omdbRes);

    return readResponse(nodeRoot);
}

int ManageXML::readOmdbXml(TiXmlElement *_nodeRoot, MovieFile *_file, Movie *_movie)
{ //<root response="False"><error>Movie not found!</error></root>
    progressCounter[CNT_TOTAL] = 0;
    progressCounter[CNT_READ] = 0;
    if (_nodeRoot && std::string(_nodeRoot->Value()) == NODE_ROOT)
    {
        bool response = BUTIL::Xml::XML_ATTR_bool(_nodeRoot, ATTR_RESPONSE);
        if (response == true)
        {
            TiXmlElement *nodeMovie = _nodeRoot->FirstChildElement(NODE_MOVIE);
            if (nodeMovie) {
                for (TiXmlElement* e = _nodeRoot->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
                {
                    progressCounter[CNT_TOTAL]++;
                }
            }

            nodeMovie = _nodeRoot->FirstChildElement(NODE_MOVIE);
            if (nodeMovie)
            {
                for (TiXmlElement* e = _nodeRoot->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
                {
                    std::string xmlValues[XML_NBATTRIBUTES];
                    memset(&xmlValues, 0, sizeof(xmlValues[XML_NBATTRIBUTES]));
                    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
                    {
                        xmlValues[attr] = BUTIL::Xml::XML_ATTR_string(e, omdbXmlAttr[attr]);
                    }
                    _movie->initValues(xmlValues);
                    _movie->setSeparator(omdbSeparator);
                    if (_file != NULL)
                        _file->imdbId = _movie->getImdbId();
                    progressCounter[CNT_READ]++;
                }
            }
        }
    }
    return progressCounter[CNT_READ];

}

HANDLE ManageXML::saveMoviesToXml(std::string _path, bool _compress)
{
    //xmlpath = _path;
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
    bool ret = false;
    if (saveXmlCompress)
    {
        std::string movie;
        std::stringstream str;
        progressCounter[CNT_TOTAL] = vectorMovies.size();
        progressCounter[CNT_READ] = 0;
        for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it)
        {
            movie = movieToXml(*it);
            str << "\n" << movie;
            progressCounter[CNT_READ]++;
        }
        std::string xml = BUTIL::Xml::createRoot(str.str(), true);

        std::string xmlpath = GETCM.getConfigStr(CONF_XML_PATH) + "\\" + GETCM.getConfigStr(CONF_XML_FILENAME) + "." + GETCM.getConfigStr(CONF_XML_FILENAMEEXT);

        ret = BUTIL::Xml::saveZ(xmlpath, xml);

    }
    else
    {
        //        std::wstring movie;
        //        std::wstringstream wstr;
        //        progressCounter[CNT_TOTAL] = vectorMovies.size();
        //        progressCounter[CNT_READ] = 0;
        //        for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it)
        //        {
        //            movie = movieToWxml(*it);
        //            wstr << L"\n" << movie;
        //            progressCounter[CNT_READ]++;
        //        }
        //        std::wstring xml = BUTIL::Xml::createRoot(wstr.str(), true);
        //
        //        ret = BUTIL::Xml::save(saveXmlpath, xml);

        std::string movie;
        std::stringstream str;
        progressCounter[CNT_TOTAL] = vectorMovies.size();
        progressCounter[CNT_READ] = 0;
        for (std::vector<Movie>::iterator it = vectorMovies.begin(); it != vectorMovies.end(); ++it)
        {
            movie = movieToXml(*it);
            str << L"\n" << movie;
            progressCounter[CNT_READ]++;
        }
        std::string xml = BUTIL::Xml::createRoot(str.str(), true);
        std::string xmlpath = GETCM.getConfigStr(CONF_XML_PATH) + "\\" + GETCM.getConfigStr(CONF_XML_FILENAME) + "." + GETCM.getConfigStr(CONF_XML_FILENAMEEXT);
        ret = BUTIL::Xml::save(xmlpath, xml);
    }
    return ret;
}

std::string ManageXML::movieToXml(Movie _movie)
{
    std::stringstream str;
    std::string xmlValues[XML_NBATTRIBUTES];
    _movie.movie2string(xmlValues);
    str << "<movie ";
    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
    {
        std::string tag = xmlValues[attr];
        str << BUTIL::Xml::XML_attr(omdbXmlAttr[attr].c_str(), tag);
    }
    str << "/>";
    return str.str();
}

bool ManageXML::loadMoviesFromDB()
{
    MovieFile file;
    return false;
}

bool  ManageXML::readJSON(std::string _xml)
{
    exLOGERROR("JSON not implemented");
    return false;
}

bool ManageXML::loadOmdbStr(MovieFile *_file)
{
    TiXmlDocument docOmdbXml;
    TiXmlElement *nodeRoot = BUTIL::Xml::parse(docOmdbXml, _file->getXmlStr());
    return (readOmdbXml(nodeRoot, _file)>0);
}

HANDLE ManageXML::loadXmlFile()
{
    DWORD  hThreadIdArray;
    stopThreads = false;
    return  CreateThread(NULL, 0, loadOmdbFileThread, (void*) this, 0, &hThreadIdArray);
}

static DWORD WINAPI loadOmdbFileThread(void* Param)
{
    ManageXML* This = (ManageXML*)Param;
    return This->loadOmdbFileStart();
}

int ManageXML::loadOmdbFileStart()
{
    
    MovieFile file;
    file.exists = false;

    std::string xmlpath = GETCM.getConfigStr(CONF_XML_PATH) + "\\" + GETCM.getConfigStr(CONF_XML_FILENAME) + "." + GETCM.getConfigStr(CONF_XML_FILENAMEEXT);

    TiXmlDocument docOmdbXml;

    TiXmlElement *nodeRoot = BUTIL::Xml::load(docOmdbXml, xmlpath);

    if (nodeRoot)
        return readOmdbXml(nodeRoot, &file);
    
    std::string text;

    text = "Error reading file";

    progressCounter[CNT_TOTAL] = 0;
    return -1;
}

bool ManageXML::terminateThreads()
{
    stopThreads = true;
    return true;
}

void ManageXML::setProgressCounter(size_t * _progressCounter)
{
    progressCounter = _progressCounter;
}
