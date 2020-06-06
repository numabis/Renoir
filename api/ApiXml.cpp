#include "ApiXML.h"
#include "Convert.h"
#include "definitions.h"
#include "database.h"
#include "omdbApi.h"
#include "Util.h"
#include "configManager.h"

#include <fstream>

#define NODE_ROOT "root"
#define NODE_MOVIE "movie"
#define NODE_RESULT "result"
#define NODE_ERROR "error"
#define ATTR_RESPONSE "response"
#define ATTR_TOTALRESULTS "totalResults"

std::string movieXmlAttr[] = { "title", "year", "rated", "released", "runtime", "genre", "director", "writer", "actors", "plot", "language", "country", "awards", "poster", "metascore", "imdbRating", "imdbVotes", "imdbID", "type" };
std::string omdbSearchAttr[API_NBATTRIBUTES] = { APIATRTIBUTES };
//std::vector<Movie> vectorMovies;

ApiXml::ApiXml()
{
    init();
}

ApiXml::ApiXml(std::string _xml)
{
    init();
    omdbXml = _xml;
    setRootNode();
}

void ApiXml::init(void)
{
    response = false;
    totalResults = 0;
}


void ApiXml::setXML(std::string _xml)
{
    omdbXml = _xml;
    setRootNode();
}

ApiXml::~ApiXml(void)
{
}

bool ApiXml::readOmdbError(MovieFile *_file)
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

bool ApiXml::parseXml()
{
    bool ret = false;
    if (rootNode && std::string(rootNode->Value()) == NODE_ROOT)
    {
        response = BUTIL::Xml::XML_ATTR_bool(rootNode, ATTR_RESPONSE);
        if (response == true)
        {
            TiXmlElement *nodeMovie = rootNode->FirstChildElement(NODE_MOVIE);
            if (nodeMovie)
            {
                for (TiXmlElement* e = rootNode->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
                {
                    memset(&xmlValues, 0, sizeof(xmlValues[XML_NBATTRIBUTES]));
                    ret = true;
                    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
                    {
                        xmlValues[attr] = BUTIL::Xml::XML_ATTR_string(e, movieXmlAttr[attr]);
                    }
                }
            }
        }
    }
    return ret;

}
void ApiXml::clearSearchResults(void)
{
    searchResults.clear();
}
int ApiXml::parseSearchResults()
{
    int parcialResults = 0;
    if (rootNode && std::string(rootNode->Value()) == NODE_ROOT)
    {
        response = BUTIL::Xml::XML_ATTR_bool(rootNode, ATTR_RESPONSE);
        if (response == true)
        {
            TiXmlElement *nodeResult = rootNode->FirstChildElement(NODE_RESULT);
            if (nodeResult)
            {
                //searchResults.clear();
                for (TiXmlElement* e = rootNode->FirstChildElement(NODE_RESULT); e != NULL; e = e->NextSiblingElement(NODE_RESULT))
                {
                    //std::string tmpResults[API_NBATTRIBUTES];
                    apiSearchValues tmpResults;
                    for (int attr = 0; attr < API_NBATTRIBUTES; attr++)
                    {
                        tmpResults.values[attr] = BUTIL::Xml::XML_ATTR_string(e, omdbSearchAttr[attr]);
                        tmpResults.isFromJson = false;
                    }
                    bool addResult = true;
                    std::vector<apiSearchValues>::iterator it;
                    for (it = searchResults.begin(); it != searchResults.end(); it++)
                    {
                        if (it->values[API_OMDBID] == tmpResults.values[API_OMDBID])
                        {
                            addResult = false;
                            break;
                        }
                    }
                    if (addResult)
                    {
                        searchResults.push_back(tmpResults);
                        parcialResults++;
                    }
                }
            }
        }
    }
    return parcialResults;
}

void ApiXml::setRootNode(std::string _xml)
{
    omdbXml = _xml;
    rootNode = BUTIL::Xml::parse(genXmlDoc, omdbXml);
}

void ApiXml::setRootNode()
{
    rootNode = BUTIL::Xml::parse(genXmlDoc, omdbXml);
}

int ApiXml::readTotalResults(std::string _xml)
{
    omdbXml = _xml;
    setRootNode();

    return readTotalResults();
}

int ApiXml::readTotalResults()
{
    totalResults = 0;

    if (rootNode && std::string(rootNode->Value()) == NODE_ROOT)
    {
        totalResults += BUTIL::Xml::XML_ATTR_int(rootNode, ATTR_TOTALRESULTS);
    }
    return totalResults;
}

bool ApiXml::readResponse(std::string _xml)
{
    omdbXml = _xml;
    setRootNode();

    return readResponse();
}

bool ApiXml::readResponse()
{
    response = false;
    if (rootNode && std::string(rootNode->Value()) == NODE_ROOT)
    {
        response = BUTIL::Xml::XML_ATTR_bool(rootNode, ATTR_RESPONSE);
    }
    return response;
}

bool ApiXml::getResponse()
{
    return response;
}

int ApiXml::getTotalResults()
{
    return totalResults;
}

std::vector <apiSearchValues> * ApiXml::getSearchResults()
{
    return &searchResults;
}

std::string ApiXml::movieToXml(Movie _movie)
{
    std::stringstream str;
    std::string xmlValues[XML_NBATTRIBUTES];
    _movie.movie2string(xmlValues);
    str << "<movie ";
    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
    {
        std::string tag = xmlValues[attr];
        str << BUTIL::Xml::XML_attr(movieXmlAttr[attr].c_str(), tag);
    }
    str << "/>";
    return str.str();
}

bool ApiXml::loadMoviesFromDB()
{
    MovieFile file;
    return false;
}

bool ApiXml::readXml(MovieFile *_file)
{
    bool ret = false;
    rootNode = BUTIL::Xml::parse(genXmlDoc, _file->getXmlStr());
    if (parseXml())
    {
        _file->initMovieValue(xmlValues, omdbSeparator);
        ret = true;
    }
    return ret;
}

//bool ApiXml::isRunning(HANDLE  _hThread)
//{
//    DWORD exitCode = 0;
//    if (_hThread)
//        GetExitCodeThread(_hThread, &exitCode);
//    if (exitCode == STILL_ACTIVE)
//        return true;
//    return false;
//}
//std::string ApiXml::getXmlFilePath()
//{
//    return GETCM.getConfigStr(CONF_XML_PATH) + "\\" + GETCM.getConfigStr(CONF_XML_FILENAME) + "." + GETCM.getConfigStr(CONF_XML_FILENAMEEXT) ;
//}
//void ApiXml::clearMovieVector()
//{
//    v_movies.clear();
//}
//bool ApiXml::ifExistsUpdateMovieVector(Movie *_movie)
//{
//    for (std::vector<Movie>::iterator it = v_movies.begin(); it != v_movies.end(); ++it)
//    {
//        if (it->compare(*_movie) == true)
//        {
//            it->updateMovie(*_movie);
//            return true;
//        }
//    }
//    return false;
//}
//void ApiXml::addToMovieVector(Movie *_movie)
//{
//    v_movies.push_back(*_movie);
//}
//int ApiXml::loadDBfromVector()
//{
//    int count = 0;
//    GETDB.TRANSACTION_begin();
//    for (std::vector<Movie>::iterator it = v_movies.begin(); it != v_movies.end(); ++it)
//    {
//        if (GETDB.MOVIES_insert(&(*it)) == true)
//            count++;
//    }
//    GETDB.TRANSACTION_commit();
//    return count;
//}
//std::vector<MovieFile> *ApiXml::getV_load() {
//    return &v_movieFiles;
//}
//BUTIL::Mutex* ApiXml::getMtx_vLoad()
//{
//    return &mtx_vLoad;
//}
//int ApiXml::readOmdbXml(TiXmlElement *_nodeRoot, MovieFile *_file)
//{
//    //ST_SAVE(ST_READING_FILE);
//    if (_nodeRoot && std::string(_nodeRoot->Value()) == NODE_ROOT)
//    {
//        bool response = BUTIL::Xml::XML_ATTR_bool(_nodeRoot, ATTR_RESPONSE);
//        if (response == true)
//        {
//            TiXmlElement *nodeMovie = _nodeRoot->FirstChildElement(NODE_MOVIE);
//            if (nodeMovie)
//            {
//                //if (progressCounter[CNT_TOTAL] == 0)
//                //for (TiXmlElement* e = _nodeRoot->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
//                //{
//                //    progressCounter[CNT_TOTAL]++;
//                //}
//                GETDB.TRANSACTION_begin();
//                for (TiXmlElement* e = _nodeRoot->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
//                {
//                    if (stopThreads)
//                    {
//                        stopThreads = false;
//                        break;
//                    }
//                    std::string xmlValues[XML_NBATTRIBUTES];
//                    memset(&xmlValues, 0, sizeof(xmlValues[XML_NBATTRIBUTES]));
//                    //memset(&_file, 0, sizeof(MovieFile));
//                    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
//                    {
//                        xmlValues[attr] = BUTIL::Xml::XML_ATTR_string(e, movieXmlAttr[attr]);
//                    }
//                    _file->initMovieValue(xmlValues, omdbSeparator);
//
//                    GETDB.MOVIES_insert(_file->getMovie());
//                    if (!_file->exists)
//                    {
//                        MovieFile file;
//                        mtx_vLoad.lock();
//                        v_movieFiles.push_back(*_file);
//                        mtx_vLoad.unlock();
//                    }
//                    //progressCounter[CNT_READ]++;
//                }
//                GETDB.TRANSACTION_commit();
//            }
//        }
//    }
//    //    progressCounter[CNT_TOTAL] = 0;
//    //    progressCounter[CNT_READ] = 0;
//    //ST_RESTORE();
//    //return progressCounter[CNT_READ];
//    return 0;
//
//}
//int ApiXml::readOmdbXml(TiXmlElement *_nodeRoot, MovieFile *_file, Movie *_movie)
//{ //<root response="False"><error>Movie not found!</error></root>
//    progressCounter[CNT_TOTAL] = 0;
//    progressCounter[CNT_READ] = 0;
//    if (_nodeRoot && std::string(_nodeRoot->Value()) == NODE_ROOT)
//    {
//        bool response = BUTIL::Xml::XML_ATTR_bool(_nodeRoot, ATTR_RESPONSE);
//        if (response == true)
//        {
//            TiXmlElement *nodeMovie = _nodeRoot->FirstChildElement(NODE_MOVIE);
//            if (nodeMovie) {
//                for (TiXmlElement* e = _nodeRoot->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
//                {
//                    progressCounter[CNT_TOTAL]++;
//                }
//            }
//
//            nodeMovie = _nodeRoot->FirstChildElement(NODE_MOVIE);
//            if (nodeMovie)
//            {
//                for (TiXmlElement* e = _nodeRoot->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
//                {
//                    std::string xmlValues[XML_NBATTRIBUTES];
//                    memset(&xmlValues, 0, sizeof(xmlValues[XML_NBATTRIBUTES]));
//                    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
//                    {
//                        xmlValues[attr] = BUTIL::Xml::XML_ATTR_string(e, movieXmlAttr[attr]);
//                    }
//                    _movie->initValues(xmlValues);
//                    _movie->setSeparator(omdbSeparator);
//                    if (_file != NULL)
//                        _file->imdbId = _movie->getImdbId();
//                    progressCounter[CNT_READ]++;
//                }
//            }
//        }
//    }
//    return progressCounter[CNT_READ];
//
//}
//int ApiXml::readXml(MovieFile *_file, Movie *_movie)
//{ //<root response="False"><error>Movie not found!</error></root>
//    if (rootNode && std::string(rootNode->Value()) == NODE_ROOT)
//    {
//        bool response = BUTIL::Xml::XML_ATTR_bool(rootNode, ATTR_RESPONSE);
//        if (response == true)
//        {
//            TiXmlElement *nodeMovie = rootNode->FirstChildElement(NODE_MOVIE);
//
//            if (nodeMovie)
//            {
//                for (TiXmlElement* e = rootNode->FirstChildElement(NODE_MOVIE); e != NULL; e = e->NextSiblingElement(NODE_MOVIE))
//                {
//                    std::string xmlValues[XML_NBATTRIBUTES];
//                    memset(&xmlValues, 0, sizeof(xmlValues[XML_NBATTRIBUTES]));
//                    for (int attr = 0; attr < XML_NBATTRIBUTES; attr++)
//                    {
//                        xmlValues[attr] = BUTIL::Xml::XML_ATTR_string(e, movieXmlAttr[attr]);
//                    }
//                    _movie->initValues(xmlValues);
//                    _movie->setSeparator(omdbSeparator);
//                    if (_file != NULL)
//                        _file->imdbId = _movie->getImdbId();
//                }
//            }
//        }
//    }
//    return 1;
//
//}
//HANDLE ApiXml::saveMoviesToXml(bool _compress)
//{
//    saveXmlCompress = _compress;
//    DWORD  hThreadIdArray;
//    return  CreateThread(NULL, 0, saveMoviesToXmlThread, (void*) this, 0, &hThreadIdArray);
//}
//static DWORD WINAPI saveMoviesToXmlThread(void* Param)
//{
//    ApiXml* This = (ApiXml*)Param;
//    return This->saveMoviesToXmlStart();
//}
//bool ApiXml::saveMoviesToXmlStart()
//{
//    bool ret = false;
//    if (saveXmlCompress)
//    {
//        std::string movie;
//        std::stringstream str;
//        progressCounter[CNT_TOTAL] = v_movies.size();
//        progressCounter[CNT_READ] = 0;
//        for (std::vector<Movie>::iterator it = v_movies.begin(); it != v_movies.end(); ++it)
//        {
//            movie = movieToXml(*it);
//            str << "\n" << movie;
//            progressCounter[CNT_READ]++;
//        }
//        std::string xml = BUTIL::Xml::createRoot(str.str(), true);
//
//        ret = BUTIL::Xml::saveZ(getXmlFilePath(), xml);
//
//    }
//    else
//    {
//        std::string movie;
//        std::stringstream str;
//        progressCounter[CNT_TOTAL] = v_movies.size();
//        progressCounter[CNT_READ] = 0;
//        for (std::vector<Movie>::iterator it = v_movies.begin(); it != v_movies.end(); ++it)
//        {
//            movie = movieToXml(*it);
//            str << L"\n" << movie;
//            progressCounter[CNT_READ]++;
//        }
//        std::string xml = BUTIL::Xml::createRoot(str.str(), true);
//
//        ret = BUTIL::Xml::save(getXmlFilePath(), xml);
//    }
//    return ret;
//}
//HANDLE ApiXml::loadOmdbFile()
//{
//    DWORD  hThreadIdArray;
//    stopThreads = false;
//    return  CreateThread(NULL, 0, loadOmdbFileThread, (void*) this, 0, &hThreadIdArray);
//}
//static DWORD WINAPI loadOmdbFileThread(void* Param)
//{
//    ApiXml* This = (ApiXml*)Param;
//    return This->loadOmdbFileStart();
//}
//int ApiXml::loadOmdbFileStart()
//{
//    
//    MovieFile file;
//    file.exists = false;
//
//    TiXmlDocument docOmdbXml;
//
//    TiXmlElement *nodeRoot = BUTIL::Xml::load(docOmdbXml, getXmlFilePath());
//
//    if (nodeRoot)
//        return readOmdbXml(nodeRoot, &file);
//    
//    std::string text;
//
//    text = "Error reading file";
//
//    progressCounter[CNT_TOTAL] = 0;
//    return -1;
//}
//bool ApiXml::terminateThreads()
//{
//    stopThreads = true;
//    return true;
//}
//void ApiXml::setProgressCounter(size_t * _progressCounter)
//{
//    progressCounter = _progressCounter;
//}
