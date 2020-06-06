#ifndef _APIJSON_H_74
#define _APIJSON_H_
#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream
#include "variables.h"

#include "json.hpp"
#include "Xml.h"
#include "Mutex.h"

using nlohmann::json;

class MovieFile;
class Movie;

class ApiJson
{

public:

    std::vector<std::string> *ptr_consoleDBG;
    states *ptr_globalState;

    ApiJson();
    ApiJson(std::string _json);
    void init(void);
    void setJson(std::string _json);
    ~ApiJson(void);
    bool readApiJson(MovieFile *_file);
    
    bool readOmdbError(MovieFile *_file);
    std::string movieToJson(Movie _movie);
    bool loadMoviesFromDB(void);
    int readTotalResults(std::string _json);
    int readTotalResults();
    bool readResponse(std::string _json);
    bool readResponse();
    void clearSearchResults(void);
    int parseSearchResults(std::string _json);
    int parseSearchResults();

    bool getResponse();
    int getTotalResults();
    std::vector <apiSearchValues> *getSearchResults();

private:

    std::string jsonStr;
    std::string jsonValues[XML_NBATTRIBUTES];
    std::vector <apiSearchValues> searchResults;

    bool response;
    int totalResults;

    json jsonStruct;

    bool parseJson();
    bool readJSON(std::string _json);
    std::string omdbSeparator;

};

#endif // _APIJSON_H_