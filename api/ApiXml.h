#ifndef _OMDB_XML_H_
#define _OMDB_XML_H_
#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream
#include "variables.h"

#include "Xml.h"
#include "Mutex.h"

class MovieFile;
class Movie;

class ApiXml
{

public:

    std::vector<std::string> *ptr_consoleDBG;
    states *ptr_globalState;

    ApiXml();
    ApiXml(std::string _xml);
    void init(void);
    void setXML(std::string _xml);
    ~ApiXml(void);
    bool readXml(MovieFile *_file);
    
    bool readOmdbError(MovieFile *_file);
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
    std::vector <apiSearchValues> *getSearchResults();

private:

    TiXmlDocument genXmlDoc;
    TiXmlElement *rootNode;

    std::string omdbXml;
    std::string xmlValues[XML_NBATTRIBUTES];
    //std::vector <std::string*> searchResults;
    std::vector <apiSearchValues> searchResults;

    bool response;
    int totalResults;

    bool parseXml();
    void setRootNode();
    std::string omdbSeparator;

};

#endif // _OMDB_XML_H_