#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream
#include "variables.h"
#include "movie.h"
#include "movieFile.h"
#ifdef USEJSON
#include "json.hpp"
#endif
#include "Xml.h"
#include "Mutex.h"

#ifdef USEJSON
using nlohmann::json;
#endif

static DWORD WINAPI loadOmdbFileThread(void* Param);
static DWORD WINAPI saveMoviesToXmlThread(void* Param);

class ManageXML
{

public:

    std::vector<std::string> *ptr_consoleDBG;
    states *ptr_globalState;

    ManageXML();
    ~ManageXML(void);
//    void init(std::string*, std::string);
    //void init();
    //void init(std::string _path);
    bool loadOmdbStr(MovieFile *_file);
    bool readOmdbError(MovieFile *_file);
    HANDLE loadOmdbFile();
    std::wstring movieToWxml(Movie _movie);
    std::string movieToXml(Movie _movie);
    HANDLE saveMoviesToXml(std::string _path, bool _compress);
    bool loadMoviesFromDB(void);
    void clearMovieVector();
    bool ifExistsUpdateMovieVector(Movie *_movie);
    void addToMovieVector(Movie *_movie);
    int loadDBfromVector();
    bool isRunning(HANDLE  hThread);
    int loadOmdbFileStart();
    bool terminateThreads();
    bool saveMoviesToXmlStart();
    std::vector<MovieFile> *getV_load();
    BUTIL::Mutex* getMtx_vLoad();
    void setProgressCounter(size_t *);
    void setGenRootNode(std::string _xml);
    int readTotalResults(TiXmlElement *_node);
    int readTotalResults(std::string _xml);
    bool readResponse(TiXmlElement *_node);
    bool readResponse(std::string _xml);

private:
#ifdef USEJSON
    json j;
#endif
    std::vector<MovieFile> v_load;
    BUTIL::Mutex mtx_vLoad;
    size_t *progressCounter;
    //std::string xmlpath;
    bool saveXmlCompress;
    int readOmdbXml(TiXmlElement *nodeRoot, MovieFile *_file, Movie *_movie);
    int readOmdbXml(TiXmlElement *nodeRoot, MovieFile *_file);
    bool readJSON(std::string _xml);
    int readOmdbSearch(std::string _omdbRes);
    std::string omdbSeparator;
    bool stopThreads;

    TiXmlDocument genXmlDoc;
    TiXmlElement *genRootNode;
};
