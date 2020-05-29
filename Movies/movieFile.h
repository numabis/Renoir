#ifndef _MOVIE_FILE_H_
#define _MOVIE_FILE_H_
#pragma once

#include <string>
#include "Movie.h"
#include "MovieFolder.h"
#include "omdb.h"


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
    std::string getOmdbXml(void);
    void setOmdbXml(std::string, bool);
    void clearSearchResults();
    void setSearchResults(std::vector <omdbSearchValues> *_results);
    bool isOmdbXmlSetted(void);

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

    std::vector <omdbSearchValues> * getOmdbSearchResults();

    /*FOLDER*/
    //void setPath(std::string);
    //void setIsSub(bool);
    //int pathCompare(std::string);
    //void setIsRead(bool);
    //bool guessType(std::string);
    //std::string getPath(void);
    //void setIdPath(short);

#ifdef USEJSON
    std::string getOmdbJson();
    void setOmdbJson(std::string, bool);
    bool isOmdbJson();
#endif

private:

    Omdb omdb;
    MovieFolder folder;

    FS_CONFIG *fsConfig;

    std::string ext;
    std::string filename;

    //friend class MovieFolder;
    //friend class Omdb;

    Movie movie;
    std::string omdbXml;
    std::vector <omdbSearchValues> omdbSearchResults;
    bool isOmdbXml;
    bool omdbResponse;
    int omdbResult;
#ifdef USEJSON
    std::string omdbJson;
    bool isOmdbJson;
#endif
    int findYear(const std::string str, short* year);
    void normalize(std::string *str);
  
};

#endif // _MOVIE_FILE_H_