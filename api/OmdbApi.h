#ifndef _OMDB_H_
#define _OMDB_H_
#pragma once

#include "Movie.h"
#include "ApiXML.h"

class ApiXML
{

public:
    ApiXML();
    ~ApiXML(void);
    int request(MovieFile *file);
    bool parseOmdbXml(MovieFile *_file);
    int searchRequest(MovieFile *_file);
    int getTotalResults();
    int getLimit();
    void setLimit(int _limit);
    void setThreadRunning(bool _val);

private:
    std::stringstream get_response(std::string url);

    int getByImdb(MovieFile *file);
    int getByTitle(MovieFile *file);
    int searchByTitle(MovieFile *file);
    std::string getRequest(MovieFile *_file);
    int search(MovieFile *_file);

    std::string getComonUrl();

    bool threadRunning;

    bool omdbRes;
    ApiXml omdbXml;
    int totalResults ;
    int limit;

//    int page;
};

#endif // _OMDB_H_