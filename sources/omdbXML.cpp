#include "omdbXML.h"
#include "Convert.h"
#include "definitions.h"
#include "database.h"
#include "omdb.h"
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
std::string omdbSearchAttr[] = { "title", "year", "imdbID", "type", "poster" };
//std::vector<Movie> vectorMovies;

omdbXML::omdbXML()
{
    init();
}

omdbXML::omdbXML(std::string _xml)
{
    init();
    omdbXml = _xml;
    setRootNode();
}

void omdbXML::init(void)
{
    response = false;
    totalResults = 0;
}


void omdbXML::setXML(std::string _xml)
{
    omdbXml = _xml;
    setRootNode();
}

omdbXML::~omdbXML(void)
{
}

bool omdbXML::readOmdbError(MovieFile *_file)
{ // //<root response="False"><error>Movie not found!</error></root>
    TiXmlDocument docOmdbXml;
    TiXmlElement *nodeRoot = BUTIL::Xml::parse(docOmdbXml, _file->getOmdbXml());

    if (nodeRoot && std::string(nodeRoot->Value()) == NODE_ROOT)
    {
        bool response = BUTIL::Xml::XML_ATTR_bool(nodeRoot, ATTR_RESPONSE);
        if (response == false)
        {
            TiXmlElement *nodeError = nodeRoot->FirstChildElement(NODE_ERROR);
            if (nodeError)
            {
                _file->setOmdbXml(nodeError->GetText(), false);
                _file->imdbId = OMDBNOTFOUND;
            }
            return true;
        }
    }
    return false;
}

bool omdbXML::parseXml()
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
void omdbXML::clearSearchResults(void)
{
    searchResults.clear();
}
int omdbXML::parseSearchResults()
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
                    //std::string tmpResults[XMLS_NBATTRIBUTES];
                    omdbSearchValues tmpResults;
                    for (int attr = 0; attr < XMLS_NBATTRIBUTES; attr++)
                    {
                        tmpResults.values[attr] = BUTIL::Xml::XML_ATTR_string(e, omdbSearchAttr[attr]);
                    }
                    bool addResult = true;
                    std::vector<omdbSearchValues>::iterator it;
                    for (it = searchResults.begin(); it != searchResults.end(); it++)
                    {
                        if (it->values[XMLS_IMDBID] == tmpResults.values[XMLS_IMDBID])
                            addResult = false;
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

void omdbXML::setRootNode(std::string _xml)
{
    omdbXml = _xml;
    rootNode = BUTIL::Xml::parse(genXmlDoc, omdbXml);
}

void omdbXML::setRootNode()
{
    rootNode = BUTIL::Xml::parse(genXmlDoc, omdbXml);
}

int omdbXML::readTotalResults(std::string _xml)
{
    omdbXml = _xml;
    setRootNode();

    return readTotalResults();
}

int omdbXML::readTotalResults()
{
    totalResults = 0;

    if (rootNode && std::string(rootNode->Value()) == NODE_ROOT)
    {
        totalResults += BUTIL::Xml::XML_ATTR_int(rootNode, ATTR_TOTALRESULTS);
    }
    return totalResults;
}

bool omdbXML::readResponse(std::string _xml)
{
    omdbXml = _xml;
    setRootNode();

    return readResponse();
}

bool omdbXML::readResponse()
{
    response = false;
    if (rootNode && std::string(rootNode->Value()) == NODE_ROOT)
    {
        response = BUTIL::Xml::XML_ATTR_bool(rootNode, ATTR_RESPONSE);
    }
    return response;
}

bool omdbXML::getResponse()
{
    return response;
}

int omdbXML::getTotalResults()
{
    return totalResults;
}

std::vector <omdbSearchValues> * omdbXML::getSearchResults()
{
    return &searchResults;
}

std::string omdbXML::movieToXml(Movie _movie)
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

bool omdbXML::loadMoviesFromDB()
{
    MovieFile file;
    return false;
}

bool  omdbXML::readJSON(std::string _xml)
{
    exLOGERROR("JSON not implemented");
    return false;
}

bool omdbXML::readOmdbXml(MovieFile *_file)
{
    bool ret = false;
    rootNode = BUTIL::Xml::parse(genXmlDoc, _file->getOmdbXml());
    if (parseXml())
    {
        _file->initMovieValue(xmlValues, omdbSeparator);
        ret = true;
    }
    return ret;
}

//bool omdbXML::isRunning(HANDLE  _hThread)
//{
//    DWORD exitCode = 0;
//    if (_hThread)
//        GetExitCodeThread(_hThread, &exitCode);
//    if (exitCode == STILL_ACTIVE)
//        return true;
//    return false;
//}
//std::string omdbXML::getXmlFilePath()
//{
//    return GETCM.getConfigStr(CONF_XML_PATH) + "\\" + GETCM.getConfigStr(CONF_XML_FILENAME) + "." + GETCM.getConfigStr(CONF_XML_FILENAMEEXT) ;
//}
//void omdbXML::clearMovieVector()
//{
//    v_movies.clear();
//}
//bool omdbXML::ifExistsUpdateMovieVector(Movie *_movie)
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
//void omdbXML::addToMovieVector(Movie *_movie)
//{
//    v_movies.push_back(*_movie);
//}
//int omdbXML::loadDBfromVector()
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
//std::vector<MovieFile> *omdbXML::getV_load() {
//    return &v_movieFiles;
//}
//BUTIL::Mutex* omdbXML::getMtx_vLoad()
//{
//    return &mtx_vLoad;
//}
//int omdbXML::readOmdbXml(TiXmlElement *_nodeRoot, MovieFile *_file)
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
//int omdbXML::readOmdbXml(TiXmlElement *_nodeRoot, MovieFile *_file, Movie *_movie)
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
//int omdbXML::readXml(MovieFile *_file, Movie *_movie)
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
//HANDLE omdbXML::saveMoviesToXml(bool _compress)
//{
//    saveXmlCompress = _compress;
//    DWORD  hThreadIdArray;
//    return  CreateThread(NULL, 0, saveMoviesToXmlThread, (void*) this, 0, &hThreadIdArray);
//}
//static DWORD WINAPI saveMoviesToXmlThread(void* Param)
//{
//    omdbXML* This = (omdbXML*)Param;
//    return This->saveMoviesToXmlStart();
//}
//bool omdbXML::saveMoviesToXmlStart()
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
//HANDLE omdbXML::loadOmdbFile()
//{
//    DWORD  hThreadIdArray;
//    stopThreads = false;
//    return  CreateThread(NULL, 0, loadOmdbFileThread, (void*) this, 0, &hThreadIdArray);
//}
//static DWORD WINAPI loadOmdbFileThread(void* Param)
//{
//    omdbXML* This = (omdbXML*)Param;
//    return This->loadOmdbFileStart();
//}
//int omdbXML::loadOmdbFileStart()
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
//bool omdbXML::terminateThreads()
//{
//    stopThreads = true;
//    return true;
//}
//void omdbXML::setProgressCounter(size_t * _progressCounter)
//{
//    progressCounter = _progressCounter;
//}
