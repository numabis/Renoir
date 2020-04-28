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

#define DBTABCONFIGURATION    { "variable",    E_DEF_STRING, "UNIQUE NOT NULL PRIMARY KEY" }, \
                              { "value",       E_DEF_STRING, "" }, \
                              { "description", E_DEF_STRING, "" }, \
                              { "",            E_END_TABLE,  "CONSTRAINT[VARIABLE] UNIQUE([variable])" }

#define DBCONFFIELDS \
{ "selectedFolder", "", "Actual Folder"}, \
{ "videoplayer", "", "Path to player"}, \
{ "OMDB_apikey", "", "Omdb Key"}, \
{ "OMDB_url", "http://www.omdbapi.com/", "Omdb URL"}, \
{ "OMDB_apiRequest", "apikey.aspx", "Omdb page"}, \
{ "OMDB_type", "xml", "data in XML or JSON"}, \
{ "OMDB_plot", "full", "plot:full or short"}, \
{ "OMDB_listseparator", ",", "Actual Folder"}, \
{ "OMDB_testmode", "false", "Actual Folder"}, \
{ "IMDB_url", "https://www.imdb.com/", "Imdb URL"}, \
{ "IMDB_title", "title", "IMDB page"}, \
{ "IMDB_find", "find?q=", "find sintax"}, \
{ "Filter_directors", "", "Selected filter for director"}, \
{ "Filter_writers", "", "Selected filter for writer"}, \
{ "Filter_actors", "", "Selected filter for actor"}, \
{ "Filter_genres", "", "Selected filter for genre1"}, \
{ "Filter_genres2", "", "Selected filter for genre2"}, \
{ "Filter_yearmin", "", "Selected filter for min year"}, \
{ "Filter_yearmax", "", "Selected filter for max year"}, \
{ "Filter_imdbratingmin", "", "Selected filter for min imdb rating"}, \
{ "Filter_imdbratingmax", "", "Selected filter for max imdb rating"}, \
{ "Filter_serie", "", "Selected filter for serie"}, \
{ "Filter_anim", "", "Selected filter for anim"}, \
{ "Filter_doc", "", "Selected filter for doc"}, \
{ "Filter_short", "", "Selected filter for short"}, \
{ "xmlTag_serie", "serie", "XML TAG for serie"}, \
{ "xmlTag_animation", "animation", "XML TAG for animation"}, \
{ "xmlTag_documentary", "documentary", "XML TAG for documentary"}, \
{ "xmlTag_short", "short", "XML TAG for short"}, \
{ "", "", "last value" }

#endif // _DATABASETABDEF_