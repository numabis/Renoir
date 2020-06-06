#ifndef _IMDBAPI_H_
#define _IMDBAPI_H_
#pragma once

#include "Movie.h"
#include "ApiJson.h"

class ImdbApi
{

public:
    ImdbApi();
    ~ImdbApi(void);
    int request(MovieFile *file);
    bool readImdbApiResult(MovieFile *_file);
    int searchRequest(MovieFile *_file);
    int getTotalResults();
    int getLimit();
    void setLimit(int _limit);
    void setThreadRunning(bool _val);

private:
    std::stringstream get_response(std::string url);

    int getByImdb(MovieFile *file);
    //int getByTitle(MovieFile *file);
    int searchByTitle(MovieFile *file);
    std::string getRequest(MovieFile *_file);
    int search(MovieFile *_file);

    std::string getSearchUrl();
    std::string getMovieUrl();

    bool threadRunning;

    bool imdbApiRes;
    ApiJson imdbJson;
    int totalResults ;
    int limit;

};

#endif // _IMDBAPI_H_