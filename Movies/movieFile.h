#ifndef _MOVIE_FILE_H_
#define _MOVIE_FILE_H_
#pragma once

#include <string>
#include "Movie.h"
#include "MovieFolder.h"
#include "omdbApi.h"
#include "ImdbApi.h"


class MovieFile: /*public Omdb, */public MovieFolder
{

public:

    MovieFile();
    MovieFile(Movie _movie);
    //MovieFile(FS_CONFIG *_config);
    ~MovieFile();
    void resetFile();
    void resetMovie();

    short moviesFSId ;
    short LstPosition ;
    
    //MovieFolder folder;
    std::string title;
    bool exists = true;
    std::string imdbId;
    std::string dateAdded;

    size_t size ;
    short year ;
    short imdbRating ;
    typesValues typeIs[TYPE_MAX];

    Movie *getMovie();
    
    bool fileIsMovie();
    bool fileIsMovie(std::string _ext);
    //void setFsConfig(FS_CONFIG *_config);
    //void setFsConfig(std::vector<std::string> extentions);
    void setFilename(std::string);
    std::string getFilename(void);
    void setExt(std::string);
    std::string getExt(void);
    void setMovie(Movie _movie);
    bool isImdbId(void);

    /* OMDB */
    void clearSearchResults();

    void initMovieValue();
    void initMovieValue(std::string*, std::string);
    void fileNameToDetails(void);
    std::string getFullPath(void);
    std::string getFullPath(std::string);
    bool omdbRequest();
    int omdbSearch();
    void omdbSetLimit(int _limit);
    int omdbGetLimit();
    int omdbGetTotalResults();
    /* OMDB END */

    /* IMDB API */
    int imdbApiSearch();
    /* IMDB API END */

    /* JSON INIT */
    void setSearchResults(std::vector <apiSearchValues> *_results);
    void addSearchResults(std::vector <apiSearchValues> *_results);
    void setJsonStr(std::string, bool);
    std::string getJsonStr(void);
    bool isJsonSetted(void);
    /* JSON END */
    /* XML INIT */
    void setXmlStr(std::string, bool);
    bool isXmlSetted(void);
    std::string getXmlStr(void);
    /* XML END */

    std::vector <apiSearchValues> * getApiSearchResults();

private:

    ApiXML omdbClient;
    ImdbApi imdbClient;
    MovieFolder folder;

    FS_CONFIG *fsConfig;

    std::string ext;
    std::string filename;

    Movie movie;
    std::string xmlStr,jsonStr;
    std::vector <apiSearchValues> apiSearchResults;
    //std::vector <jsonSearchValues> jsonSearchResults;
    bool isXmlSet, isJsonSet;
    int findYear(const std::string str, short* year);
    void normalize(std::string *str);
  
};

#endif // _MOVIE_FILE_H_