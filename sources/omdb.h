#pragma once

#include "variables.h"
#include "movieFile.h"


/// Soporte acceso base de datos SQLITE

class Omdb
{

public:
    Omdb();
    ~Omdb(void);
    void init(OMDB_CONFIG*);
    void searchByImdb(MovieFile *file);
    //    std::string string2url(std::string str);
#ifdef WSTRINGS
    std::wstring searchByImdb(MovieFile *file);
    std::wstring search(std::wstring _name, short _year);
#endif
    void searchByName(MovieFile *file);
    void search(MovieFile *file);
//    std::string search(std::string _xmlFilePath);
    //bool writeSingleFileXml();
private:
    std::stringstream get_response(std::wstring url);
    std::stringstream get_response(std::string url);
    OMDB_CONFIG *omdbConfig;
};
