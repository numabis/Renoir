#ifndef _OMDB_XML_H_
#define _OMDB_XML_H_
#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream
#include "variables.h"
//#include "movieFile.h"

#ifdef USEJSON
#include "json.hpp"
#endif
#include "Xml.h"
#include "Mutex.h"

#ifdef USEJSON
using nlohmann::json;
#endif

//static DWORD WINAPI loadOmdbFileThread(void* Param);
//static DWORD WINAPI saveMoviesToXmlThread(void* Param);

class MovieFile;
class Movie;

class omdbXML
{

public:

    std::vector<std::string> *ptr_consoleDBG;
    states *ptr_globalState;

    omdbXML();
    omdbXML(std::string _xml);
    void init(void);
    void setXML(std::string _xml);
    ~omdbXML(void);
    bool readOmdbXml(MovieFile *_file);
    
    bool readOmdbError(MovieFile *_file);
    std::wstring movieToWxml(Movie _movie);
    std::string movieToXml(Movie _movie);
    bool loadMoviesFromDB(void);
    void setRootNode(std::string _xml);
    int readTotalResults(std::string _xml);
    int readTotalResults();
    bool readResponse(std::string _xml);
    bool readResponse();
    void clearSearchResults(void);
    int parseSearchResults();

    bool getResponse();
    int getTotalResults();
    std::vector <omdbSearchValues> *getSearchResults();

//bool loadOmdbStr(MovieFile *_file);
//    HANDLE loadOmdbFile();
//    HANDLE saveMoviesToXml(bool _compress);
//    void clearMovieVector();
//    bool ifExistsUpdateMovieVector(Movie *_movie);
//    void addToMovieVector(Movie *_movie);
//    int loadDBfromVector();
//    bool isRunning(HANDLE  hThread);
//    int loadOmdbFileStart();
//    bool terminateThreads();
    //bool saveMoviesToXmlStart();
//    std::vector<MovieFile> *getV_load();
//    BUTIL::Mutex* getMtx_vLoad();
//    void setProgressCounter(size_t *);

private:

    TiXmlDocument genXmlDoc;
    TiXmlElement *rootNode;

    std::string omdbXml;
    std::string xmlValues[XML_NBATTRIBUTES];
    //std::vector <std::string*> searchResults;
    std::vector <omdbSearchValues> searchResults;

    bool response;
    int totalResults;


#ifdef USEJSON
    json j;
#endif
    bool parseXml();
    void setRootNode();
    bool readJSON(std::string _xml);
    int readOmdbSearch(std::string _omdbRes);
    std::string omdbSeparator;

//    int readOmdbXml(TiXmlElement *nodeRoot, MovieFile *_file);
    //    int readXml(MovieFile *_file, Movie *_movie);
    //std::vector<MovieFile> v_movieFiles;
    //std::vector<Movie> v_movies;
    //BUTIL::Mutex mtx_vLoad;
    //size_t *progressCounter;
    //bool saveXmlCompress;
    //int readOmdbXml(TiXmlElement *nodeRoot, MovieFile *_file, Movie *_movie);
    //    bool stopThreads;
//    std::string getXmlFilePath();
};

#endif // _OMDB_XML_H_