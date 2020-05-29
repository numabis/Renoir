#ifndef _MOVIE_FOLDER_H_
#define _MOVIE_FOLDER_H_
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

#endif //_MOVIE_FOLDER_H_