#pragma once

#include <string>
#include "Movie.h"

enum FOLDER_STAT { FOLDER_ISREAD, FOLDER_ISSUB, FOLDER_ISHIDDEN };

class MovieFolder
{
public:
    MovieFolder(std::string);
    MovieFolder(std::string, int, bool, bool, bool);
    MovieFolder(std::string, bool);
    MovieFolder(void);
    ~MovieFolder(void);
    void resetFolder(void);
    
    void setFolder(MovieFolder _folder);
    void setFolder(MovieFile _file);
    void setPath(std::string);
    void setIdPath(short);
    void setIsSub(bool);
    void setIsHide(bool);
    void setIsRead(bool);
    
    std::string getPath(void);
    short getIdPath(void);
    bool isSub(void);
    bool isHide(void);
    bool isRead(void);
    
    int pathCompare(std::string);
    bool guessType(std::string);
    void guessType();
    bool isPathEmpty();
    int  insertDB();
    int  updateDB();
    bool updateIsRead(FOLDER_STAT);

private:
    char * strType[TYPE_MAX] = MOVIE_TYPES;
    std::string path;
    short id;
    bool  subfolder;
    bool  hide;
    bool  read;
    typesValues typeIs[TYPE_MAX];
};

class MovieFile: public MovieFolder
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
    std::string getOmdbXml(void);
    void setOmdbXml(std::string, bool);
    bool isOmdbXmlSetted(void);
    void initMovieValue();
    void initMovieValue(std::string*, std::string);
    void fileNameToDetails(void);
    std::string getFullPath(void);
    std::string getFullPath(std::string);

#ifdef USEJSON
    std::string getOmdbJson();
    void setOmdbJson(std::string, bool);
    bool isOmdbJson();
#endif

private:
    FS_CONFIG *fsConfig;

    std::string ext;
    std::string filename;

    friend class MovieFolder;
    Movie movie;
    std::string omdbXml;
    bool isOmdbXml;
#ifdef USEJSON
    std::string omdbJson;
    bool isOmdbJson;
#endif
    int findYear(const std::string str, short* year);
    void normalize(std::string *str);
  
};