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
    void init(std::string*, std::string);
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
    //int *getCountXmlLoad();
    //int *getCountXmlSave();
    bool isRunning(HANDLE  hThread);
    int loadOmdbFileStart();
    bool terminateThreads();
    bool saveMoviesToXmlStart();
    std::vector<MovieFile> *getV_load();
    BUTIL::Mutex* getMtx_vLoad();
    void setProgressCounter(size_t *);
    std::string getXmlLocalFullPath();
private:
#ifdef USEJSON
    json j;
#endif
    std::vector<MovieFile> v_load;
    BUTIL::Mutex mtx_vLoad;
    size_t *progressCounter;
    //int countXmlLoad[CNT_MAX_READ];
    //int countXmlSave[CNT_MAX_READ];
    std::string saveXmlpath;
    bool saveXmlCompress;
    int readOmdbXml(TiXmlElement *nodeRoot, MovieFile *_file, Movie *_movie);
    int readOmdbXml(TiXmlElement *nodeRoot, MovieFile *_file);
    bool readJSON(std::string _xml);
//    XML_CONFIG *xmlConfig;
    std::string* xmlConfig;
    std::string omdbSeparator;
    bool stopThreads;
};
