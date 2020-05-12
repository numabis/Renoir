#pragma once

#include <vector>
#include "variables.h"
#include "databaselite.h"
//#include "database.h"
#include "omdbClientInfo.h"

#define SERIES      "series"
#define EPISODE     "episode"
#define ANIMATION   "Animation"
#define DOCUMENTARY "Documentary"
#define SHORT       "short"

#define MOVIE_TYPES   { SERIES, ANIMATION, DOCUMENTARY, SHORT }

class MovieFile;

class Movie
{

public:

    char * strType[TYPE_MAX] = MOVIE_TYPES;

    Movie();
    Movie(std::string*);
    Movie(std::string*, std::string);
    Movie(MovieFile);
    ~Movie();

    void initValues(std::string*);
    void initValues(std::string*, std::string);

    void movie2string(std::string* _data);
    bool compare(Movie);

    void copyMovie(Movie);
    void updateMovie(Movie);

    int getIdMovie();
    std::string getInfo(movieInfo);
    std::string getTitle();
    std::string getTitleASCII();
    int getYear();
    std::string getRated();
    std::string getRuntime();
    std::string getGenres();
    std::string getRoles(ROLES_PERSONS);
    std::vector<std::string> *getvGenres();
    std::vector<std::string> *getvRoles(ROLES_PERSONS);
    std::string getPlot();
    std::string getLanguage();
    std::string getCountry();
    std::string getAwards();
    std::string getPoster();
    int getMetascore();
    int getImdbRating();
    int getImdbVotes();
    std::string getImdbId();
    std::string getType();

    void setIdMovie(int id);
    void setTitle(std::string);
    void setTitle(SAString);
    void setTitleASCII(std::string);
    void setTitleASCII(SAString);
    void setYear(int);
    void setRated(std::string);
    void setRated(SAString);
    void setRuntime(std::string);
    void setRuntime(SAString);
    void setGenres(std::vector<std::string>);
    void setRoles(std::vector<std::string>, ROLES_PERSONS);
    void setPlot(std::string);
    void setPlot(SAString);
    void setLanguage(std::string);
    void setLanguage(SAString);
    void setCountry(std::string);
    void setCountry(SAString);
    void setAwards(std::string);
    void setAwards(SAString);
    void setPoster(std::string);
    void setPoster(SAString);
    void setMetascore(int);
    void setImdbRating(int);
    void setImdbVotes(int);
    void setImdbId(std::string);
    void setImdbId(SAString);
    void setType(std::string);
    void setType(SAString);
    void setSeparator(std::string);
    std::string getSeparator(void);
    bool getTypeIs(movieInfoTypes);
    bool getJpeg(void);
    std::string getJpegPath(void);

    std::vector<std::string>* consoleDBG;
    states *globalState;

private:

    void separateValues(std::vector<std::string>*, std::string);
    std::string vector2std(std::vector<std::string>, std::string _separator);
    void initValuesFromFile(MovieFile);
    void setTypes(movieInfoTypes);
    void setSerie();
    static size_t callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata);
    bool downloadJpeg(std::string _url, std::string _file);
    //bool folderExists(std::string path);
    bool fileExists(std::string path);
    //bool folderCreates(std::string path);
    std::string url2fileName(std::string _url);

    int idMovie;

    std::string title;              //About Elly;
    std::string titleASCII;         //About Elly;
    std::string plot;               //It's a story about three Iranian families who are traveling to North of Iran in order that they can...;
    std::string language;
    std::string country;            //http://ia.media-imdb.com/images/M/MV5BMTI4MDAyOTQzM15BMl5BanBnXkFtZTcwMTYxMDkyMg@@._V1_SX640.jpg;
    std::string awards;
    std::string poster;             //http://ia.media-imdb.com/images/M/MV5BMTI4MDAyOTQzM15BMl5BanBnXkFtZTcwMTYxMDkyMg@@._V1_SX640.jpg;
    std::string imdbId;             //tt1360860;
    std::string type;
    std::string separator; 
    std::string rated;              //U;
    std::string runtime;            //1 h 59 min;
    std::vector<std::string> v_genres; //Drama, Mystery;
    std::vector<std::string> v_roles[ROLES_MAX];

    std::string posterPath;

    short year;                     //2009;
    int metascore;              
    short imdbRating;               //8.0;
    int imdbVotes;                  //8,257;

    bool typeIs[TYPE_MAX];

};
