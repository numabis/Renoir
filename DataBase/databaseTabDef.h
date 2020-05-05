#ifndef _DATABASETABDEF_
#define _DATABASETABDEF_
//#include "databaseSQL.h"

#define TABNAMELIST     {   "persons",  "genres",  "roles",  "paths",    "moviesFS",   "movies",   "personInMovie",    "genreInMovie",   "dbversions",  "configuration" }
#define TABINDEX        { TAB_PERSONS, TAB_GENRES, TAB_ROLES, TAB_PATHFS, TAB_MOVIESFS, TAB_MOVIES, TAB_PERSONINMOVIE, TAB_GENREINMOVIE, TAB_DBVERSIONS, TAB_CONFIGURATION, MAXTABLES }

#define DBTABVERSION    { "version",     E_DEF_DOUBLE, "UNIQUE NOT NULL PRIMARY KEY" }, \
                        { "DateAdded",   E_DEF_STRING, "NOT NULL" }, \
                        { "Description", E_DEF_STRING, "" }, \
                        { "",            E_END_TABLE,  "CONSTRAINT[VERSION] UNIQUE([version])" }

#define DBTABPATHFS     { "idPath",   E_DEF_INT,      "DEFAULT '0' NOT NULL PRIMARY KEY AUTOINCREMENT" }, \
                        { "path",     E_DEF_STRING,   "NOT NULL" }, \
                        { "isRead",   E_DEF_BOOL,     "NOT NULL DEFAULT '0'" }, \
                        { "isSub",    E_DEF_BOOL,     "NOT NULL DEFAULT '0'" }, \
                        { "hide",     E_DEF_BOOL,     "NOT NULL DEFAULT '0'" }, \
                        { "",         E_END_TABLE,    "CONSTRAINT[PATH] UNIQUE([path] COLLATE NOCASE)" }

#define DBTABMOVIESFS    \
    { "idMovieFS",   E_DEF_INT,             "DEFAULT '0' NOT NULL PRIMARY KEY AUTOINCREMENT" },  \
    { "idPath",      E_DEF_STRING, "NOT NULL" }, \
    { "filepath",    E_DEF_STRING,          "NOT NULL" }, \
    { "filename",    E_DEF_STRING,          "NOT NULL" }, \
    { "physical",    E_DEF_BOOL,            "DEFAULT '1'" }, \
    { "imdbid",      E_DEF_STRING,          "" }, \
    { "dateAdded",   E_DEF_STRING,          "NOT NULL" }, \
    { "filesize",    E_DEF_INT,             "NOT NULL" }, \
    { "serie",       E_DEFILTERS_SHORT,     "DEFAULT '1'" }, \
    { "animation",   E_DEFILTERS_SHORT,     "DEFAULT '1'" }, \
    { "documentary", E_DEFILTERS_SHORT,     "DEFAULT '1'" }, \
    { "short",       E_DEFILTERS_SHORT,     "DEFAULT '1'" }, \
    { "",            E_END_TABLE,           "CONSTRAINT [FS_PATH_FILE] UNIQUE ([filepath] COLLATE NOCASE,[filename] COLLATE NOCASE), FOREIGN KEY(idPath) REFERENCES paths(idPath) ON DELETE CASCADE" }


#define DBTABMOVIESFS \
    { "idMovieFS",   E_DEF_INT,             "DEFAULT '0' NOT NULL PRIMARY KEY AUTOINCREMENT" }, \
    { "idPath",      E_DEF_STRING,          "NOT NULL" }, \
    { "filepath",    E_DEF_STRING,          "NOT NULL" }, \
    { "filename",    E_DEF_STRING,          "NOT NULL" }, \
    { "physical",    E_DEF_BOOL,            "DEFAULT '1'" }, \
    { "imdbid",      E_DEF_STRING,          "" }, \
    { "dateAdded",   E_DEF_STRING,          "NOT NULL" }, \
    { "filesize",    E_DEF_INT,             "NOT NULL" }, \
    { "serie",       E_DEFILTERS_SHORT,     "DEFAULT '1'" }, \
    { "animation",   E_DEFILTERS_SHORT,     "DEFAULT '1'" }, \
    { "documentary", E_DEFILTERS_SHORT,     "DEFAULT '1'" }, \
    { "short",       E_DEFILTERS_SHORT,     "DEFAULT '1'" }, \
    { "",            E_END_TABLE,           "CONSTRAINT [FS_PATH_FILE] UNIQUE ([filepath] COLLATE NOCASE,[filename] COLLATE NOCASE), FOREIGN KEY(idPath) REFERENCES paths(idPath) ON DELETE CASCADE" }

#define DBTABMOVIES \
    { "idMovie",    E_DEF_INT,         "DEFAULT '0' NOT NULL PRIMARY KEY AUTOINCREMENT" }, \
    { "title",      E_DEF_STRING,      "NOT NULL" }, \
    { "titleASCII", E_DEF_STRING,      "NOT NULL" }, \
    { "year",       E_DEFILTERS_SHORT, "NOT NULL" }, \
    { "rated",      E_DEF_STRING,      "NOT NULL" }, \
    { "runtime",    E_DEF_STRING,      "NOT NULL" }, \
    { "plot",       E_DEF_STRING,      "NOT NULL" }, \
    { "language",   E_DEF_STRING,      "NOT NULL" }, \
    { "country",    E_DEF_STRING,      "NOT NULL" }, \
    { "award",      E_DEF_STRING,      "NOT NULL" }, \
    { "poster",     E_DEF_STRING,      "NOT NULL" }, \
    { "metascore",  E_DEFILTERS_SHORT, "NOT NULL" }, \
    { "imdbrating", E_DEFILTERS_SHORT, "NOT NULL" }, \
    { "imdbvotes",  E_DEF_INT,         "NOT NULL" }, \
    { "imdbid",     E_DEF_STRING,      "NOT NULL" }, \
    { "type",       E_DEF_STRING,      "NOT NULL" }, \
    { "vue",        E_DEF_BOOL,        "" }, \
    { "",           E_END_TABLE,       "CONSTRAINT [MOVIES_IMDBID] UNIQUE ([imdbid])" }

#define DBTABPERSONS \
    { "idPerson",   E_DEF_INT,      "DEFAULT '0' NOT NULL PRIMARY KEY AUTOINCREMENT" }, \
    { "name",       E_DEF_STRING,   "NOT NULL" }, \
    { "FirstName",  E_DEF_STRING,   "" }, \
    { "LastName",   E_DEF_STRING,   "" }, \
    { "",           E_END_TABLE,    "CONSTRAINT [PERSONS_NAME] UNIQUE ([name] COLLATE NOCASE)" }


#define DBTABGENRES \
    { "idGenre",    E_DEF_INT,      "DEFAULT '0' NOT NULL PRIMARY KEY AUTOINCREMENT" }, \
    { "genre",      E_DEF_STRING,   "NOT NULL" }, \
    { "", E_END_TABLE,              "CONSTRAINT [GENRES_GENRE] UNIQUE ([genre] COLLATE NOCASE)" }

#define DBTABROLES \
    { "idRole",    E_DEF_INT,       "DEFAULT '0' NOT NULL PRIMARY KEY AUTOINCREMENT" }, \
    { "role",      E_DEF_STRING,    "NOT NULL" }, \
    { "",          E_END_TABLE,     "CONSTRAINT [ROLES_ROLE] UNIQUE ([role])" }

#define DBTABPERSONINMOVIE \
    { "idMovie",    E_DEFILTERS_SHORT,  "" }, \
    { "idPerson",   E_DEFILTERS_SHORT,  "" }, \
    { "idRole",     E_DEFILTERS_SHORT,  "" }, \
    { "",           E_END_TABLE,        "CONSTRAINT [PERSONS_IN_MOVIES] UNIQUE ([idMovie],[idPerson],[idRole]), FOREIGN KEY(idMovie) REFERENCES movies(idMovie) ON DELETE CASCADE, FOREIGN KEY(idPerson) REFERENCES persons(idPerson) ON DELETE CASCADE, FOREIGN KEY(idRole) REFERENCES roles(idRole) ON DELETE CASCADE" }

#define DBTABGENREINMOVIE \
    { "idMovie",    E_DEFILTERS_SHORT,  "" }, \
    { "idGenre",    E_DEFILTERS_SHORT,  "" }, \
    { "",           E_END_TABLE,        "CONSTRAINT [GENRES_IN_MOVIES] UNIQUE ([idMovie],[idGenre]), FOREIGN KEY(idMovie) REFERENCES movies(idMovie) ON DELETE CASCADE, FOREIGN KEY(idGenre) REFERENCES genres(idGenre) ON DELETE CASCADE" }



#define DBCHANGELOG \
    { 0.1f,     "2020-01-14", "test" }, \
    { 0.2f,     "2020-01-15", "Version check" }, \
    { 0.3f,     "2020-01-16", "Date added to version" }, \
    { 0.4f,     "2020-01-21", "Date Ascii title to movies" }, \
    { 0.5f,     "2020-01-22", "Added on cascade delete contraint" }, \
    { 0.6f,     "2020-01-27", "COLLATE NOCASE in unique columns" }, \
    { 0.7f,     "2020-02-01", "New Type definition" }, \
    { 0.71f,    "2020-02-02", "minor corrections" }, \
    { 0.8f,     "2020-02-03", "New Table FileTypes" }, \
    { 0.81f,    "2020-02-03", "New Table FileTypes correction" }, \
    { 0.82f,    "2020-02-06", "moviesFS new col:animation" }, \
    { 0.83f,    "2020-02-18", "change type of anim-series columns" }, \
    { 0.84f,    "2020-02-18", "new column in moviesFS doc,short" }, \
    { 0.9f,     "2020-02-18", "Added tab PATH to store all path and added column idPath to moviesFS" }, \
    { 1.0f,     "2020-04-28", "Added tab Configuration to store the config instead of XML" }, \
    { 0.0f,     ""          , "Not a version, here to end table" }

#define DBTABCONFIGURATION    { "id",          E_DEF_INT,    "PRIMARY KEY" }, \
                              { "variable",    E_DEF_STRING, "UNIQUE NOT NULL" }, \
                              { "value",       E_DEF_STRING, "" }, \
                              { "description", E_DEF_STRING, "" }, \
                              { "",            E_END_TABLE,  "" }

#define DBCONFFIELDS \
{  0, "FS_autoreadFolder",          "0", "AutoRead when app start" }, \
{  1, "FS_extentions",              ".mp4,.avi,.mkv", "Movie files extentions"}, \
{  2, "FS_searchTypes",             "1", "Guess movie type from folder" }, \
{  3, "FS_typeSerie",               "serie", "" }, \
{  4, "FS_typeAnim",                "anim", "" }, \
{  5, "FS_typeDoc",                 "doc", "" }, \
{  6, "FS_typeShort",               "short", "" }, \
{  7, "FS_searchLanguage",          "1", ""}, \
{  8, "FS_LangFrench",              "Francais", "" } , \
{  9, "FS_LangSpanish",             "Espanol", "" } , \
{ 10, "FS_LangEglish",              "English", "" } , \
{ 11, "FS_LangItalian",             "Italia", "" } , \
{ 12, "DB_Reset",                   "0", "" } , \
{ 13, "LOG_logLevel",               "6", "DISABLED=0 ERROR=1 INFO=2 DB=3 XML=4 DEBUG=5 SQL=6" }, \
{ 14, "APP_CurrentFolder",          "", "Actual Folder"}, \
{ 15, "APP_Videoplayer",            "", "Path to player"}, \
{ 16, "OMDB_apikey",                "", "Omdb Key"}, \
{ 17, "OMDB_url",                   "http://www.omdbapi.com/", "Omdb URL"}, \
{ 18, "OMDB_apiRequest",            "apikey.aspx", "Omdb page"}, \
{ 19, "OMDB_type",                  "xml",           "data in XML or JSON"}, \
{ 20, "OMDB_plot",                  "full",          "plot:full or short"}, \
{ 20, "OMDB_listseparator",         ",", "Actual Folder"}, \
{ 21, "OMDB_testmode",              "false", "Actual Folder"}, \
{ 21, "IMDB_url",                   "https://www.imdb.com/", "Imdb URL"}, \
{ 22, "IMDB_title",                 "title", "IMDB page"}, \
{ 22, "IMDB_find",                  "find?q=", "find sintax"}, \
{ 23, "FILTER_directors",           "", "Selected filter for director"}, \
{ 23, "FILTER_USE_directors",       "0", "Selected filter for director"}, \
{ 24, "FILTER_writers",             "", "Selected filter for writer"}, \
{ 24, "FILTER_USE_writers",         "0", "Selected filter for writer"}, \
{ 25, "FILTER_actors",              "", "Selected filter for actor"}, \
{ 25, "FILTER_USE_actors",          "0", "Selected filter for actor"}, \
{ 26, "FILTER_genres",              "", "Selected filter for genre1"}, \
{ 26, "FILTER_USE_genres",          "0", "Selected filter for genre1"}, \
{ 27, "FILTER_genres2",             "", "Selected filter for genre2"}, \
{ 27, "FILTER_USE_genres2",         "0", "Selected filter for genre2"}, \
{ 28, "FILTER_yearmin",             "", "Selected filter for min year"}, \
{ 28, "FILTER_USE_yearmin",         "0", "Selected filter for min year"}, \
{ 29, "FILTER_yearmax",             "", "Selected filter for max year"}, \
{ 30, "FILTER_USE_yearmax",         "0", "Selected filter for max year"}, \
{ 31, "FILTER_imdbratingmin",       "", "Selected filter for min imdb rating"}, \
{ 32, "FILTER_USE_imdbratingmin",   "0", "Selected filter for min imdb rating"}, \
{ 33, "FILTER_imdbratingmax",       "", "Selected filter for max imdb rating"}, \
{ 34, "FILTER_USE_imdbratingmax",   "0", "Selected filter for max imdb rating"}, \
{ 35, "FILTER_serie",               "", "Selected filter for serie"}, \
{ 36, "FILTER_USE_serie",           "0", "Selected filter for serie"}, \
{ 37, "FILTER_anim",                "", "Selected filter for anim"}, \
{ 38, "FILTER_USE_anim",            "0", "Selected filter for anim"}, \
{ 39, "FILTER_doc",                 "", "Selected filter for doc"}, \
{ 40, "FILTER_USE_doc",             "0", "Selected filter for doc"}, \
{ 41, "FILTER_short",               "", "Selected filter for short"}, \
{ 42, "FILTER_USE_short",           "0", "Selected filter for short"}, \
{ 43, "XML_Tag_serie",              "serie", "XML TAG for serie"}, \
{ 44, "XML_Tag_animation",          "animation", "XML TAG for animation"}, \
{ 45, "XML_Tag_documentary",        "documentary", "XML TAG for documentary"}, \
{ 46, "XML_Tag_short",              "short", "XML TAG for short"}, \
{ 47, "XML_Path",                   "", ""}, \
{ 47, "XML_Filename",               "movieDB", ""}, \
{ 47, "XML_FilenameExt",            "xml", ""}, \
{ 48, "", "", "last value" }

#endif // _DATABASETABDEF_