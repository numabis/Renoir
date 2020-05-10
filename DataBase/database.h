#pragma once

#include <queue>
#include "singleton.h"
#include "databaselite.h"
#include "mutex.h"
#include "databaseSQL.h"

#include "variables.h"
#include "movieFile.h"

#define ALL     "All"

#define SQLGENCURSOR                global_cursor
#define SQLGENDECLARE               SACommand SQLGENCURSOR;
#define SQLGENEXEC                  SQLGENCURSOR.setConnection(&conex); SQLGENCURSOR.setCommandText(request.c_str(), SA_CmdSQLStmt); SQLGENCURSOR.Execute();
#define SQLGENTEST                  !SQLGENCURSOR.isOpened() || !SQLGENCURSOR.isExecuted()
#define SQLGENFETCH                 SQLGENCURSOR.isOpened() && SQLGENCURSOR.isExecuted() && SQLGENCURSOR.FetchNext()
#define SQLGENFIELD(tabColId)       SQLGENCURSOR.Field((tabStruct[TABID(tabColId)][COLID(tabColId)].name).c_str())
#define SQLGENCLOSE                 SQLGENCURSOR.Close()

typedef struct
{
    std::string name;
    CDataType Ctype;
    std::string sqlDef;
} dbTableDef;

typedef std::map< int, std::string> typeDeftab;

/// <summary>
/// Soporte acceso base de datos SQLITE
/// </summary>
class DataBase: public BUTIL::Singleton<DataBase>, public BUTIL::DataBaseLite
{

public:
    enum filterType { NORM_COMBOT, GENRE_COMBOT, RANGE_COMBOT };
    filterType ftype[FILTERS_MAX] = { NORM_COMBOT,NORM_COMBOT,NORM_COMBOT,NORM_COMBOT,NORM_COMBOT,RANGE_COMBOT,RANGE_COMBOT,RANGE_COMBOT,RANGE_COMBOT,GENRE_COMBOT,GENRE_COMBOT,GENRE_COMBOT,GENRE_COMBOT };

    enum invertFilters { INVERT_FILTERS_GENRES, INVERT_FILTERS_GENRES2, INVERT_FILTERS_DIRECTORS, INVERT_FILTERS_ACTORS, INVERT_FILTERS_MAX };

    std::string tabRoles[ROLES_MAX] = ROLESNAMES;

    enum column                        { COL_FSID, COL_PATH, COL_FILENAME, COL_PHYSICAL, COL_DATE    , COL_SIZE   , COL_IMDBID, COL_SERIE, COL_ANIM, COL_DOC, COL_SHORT, COL_IMDBRATING, NUM_COLUMNS };

    std::vector<short> ColumnsToShow = { FS_ID   , FS_PATH , FS_FILENAME , FS_PHYSICAL , FS_DATEADDED, FS_FILESIZE, FS_IMDBID , FS_SERIE , FS_ANIM , FS_DOC , FS_SHORT , MOVIES_IMDBRATING };

    typedef struct {
        std::string str;
        int pond;
    } pondStr;

    typedef struct {
        short orderByColumn;
        bool orderByAsc;
        bool exists;
        short minOccurrences;
        short valueSelected[FILTERS_MAX];
        std::string nameSelected[FILTERS_MAX];
        std::vector<std::string> v_combos[FILTERS_MAX];
        bool invertFilters[INVERT_FILTERS_MAX];
        std::vector<pondStr> folders;
        //std::string *folderFilter;
        int folderFilterId;
    } moviesFSFilters;

    std::vector<std::string> *ptr_consoleDBG;
    states *ptr_globalState;

        /// <summary>  </summary>
	void TRANSACTION_begin(void);
    void TRANSACTION_commit(void);

	/// <summary>  </summary>
    bool             isFolderRead(MovieFolder *_folder);
    bool             setDB();
    bool             setDB(std::string path);
    bool             setDB(DB_CONFIG*);
    bool             initDB(bool _resetDB);
    bool             getUpdate();
    int              execNolock(std::string);
    bool             initTable(int _tabId);
    bool             ROLES_populate(void);
    bool             DBVERSIONS_populate(void);
    void             askUpdateDB(std::string _msg);
    int              getDefTableId(int, std::string);
    std::string      getDefTableValue(int, int);
    std::string      AllMoviesFSGetFilter();
    inline bool             isConnected(void);
    void             setConnected(bool);
    bool             getMovieData(Movie *movie);
    bool             getMovieData(MovieFile *movie);
    std::string      buildComboFilter(filterTypes);
    std::string      buildJoinFilter(filterTypes);
    short            getTabColFilters(filterTypes);
    int              PATHFS_loadFolders();
    int              loadFilter(filterTypes);

    bool getData(MovieFile *_file);
    int  getId(MovieFolder *_file);
    int  getPathId(std::string _path);
    std::string getPath(int _id);
    bool getConfiguration(confMap *_confMan);
    bool setConfiguration(std::string _var, dbConfiguration _conf);

    double DBVERSIONS_getVersion(void);

    int  MOVIES_iniAll(std::string _separator);
    bool MOVIES_getAll(Movie *movie);
    bool MOVIES_insert(Movie *movie);
    bool MOVIES_selectId(MovieFile *_file);
    bool MOVIES_selectId(Movie *_movie);

    int  MOVIESFS_getFSId(MovieFile *_file);
    bool MOVIESFS_selectFSId(MovieFile *_file);
    bool MOVIESFS_iniAll();
    bool MOVIESFS_getAll(MovieFile *file);
    bool MOVIESFS_setPhysical(bool);
    int  MOVIESFS_exists(MovieFile *file);
//    int  existsMovieFS(std::string _filePath, std::string _fileName);
    int  MOVIESFS_exists(int         _idPath,   std::string _fileName);
    bool MOVIESFS_update(MovieFile *file, bool* fields);
    int  MOVIESFS_insert(MovieFile *_file);
    void MOVIESFS_bulkInsert(MovieFile *_file);
    bool MOVIESFS_deleteAll(void);

    int  PATHFS_insert(std::string path);
    int  PATHFS_insert(MovieFolder *_folder);
    void PATHFS_bulkInsert(MovieFolder *_folder);
    int  PATHFS_update(MovieFolder *_folder);

    int              bulkInsertCommit(tabIndex _id = MAXTABLES);
    void             bulkInit(tabIndex _id);
    void             setFilterSelectedValue(filterTypes _filter, std::string _name);
    moviesFSFilters *getFilters();
    bool             deleteAllMoviesData();

private:

    std::vector<std::string> buklInserts[MAXTABLES];
    std::vector<std::string> buklUpdates[MAXTABLES];

    //std::string *configManager[CONF_MAX_VALUES];
    DB_CONFIG* ptr_dbConfig;
    DB_CONFIG      dbConfig;
    double DBVersion;
    double DBLocalVersion;
    bool update;
    //bool connected;
    moviesFSFilters filters;
    int RangeImdbRatings[2];
    dbTableDef * tabStruct[MAXTABLES];
    SQLGENDECLARE;
    friend class BUTIL::Singleton<DataBase>;

    std::string separator;
    DataBase();
    ~DataBase(void);
    
    int  insertInDefTabs(int, std::string);
    int  insertPersonsInMovies(Movie *_movie);
    int  insertGenresInMovies(Movie *_movie);

    int  PATHFS_selectId(std::string _path);
    std::string PATHFS_getPath(int _id);
    bool PATHFS_selectId(MovieFolder *_file);
    int  PATHFS_getId(MovieFolder *_file);
    bool PATHFS_existsPath(MovieFolder *_folder);
    int  PATHFS_existsPath(std::string _filePath);

    bool MOVIESFS_get(MovieFile *_file);

    bool MOVIES_get(Movie *movie);
    bool MOVIES_deleteAll(void);

    int  GENRES_getId(std::string);
    bool GENRES_getGenre(Movie *movie);
    int  GENRES_insert(std::string);
    bool GENRES_deleteAll(void);

    int  ROLES_getId(std::string);

    int  PERSONS_getId(std::string);
    bool PERSONS_getPerson(Movie *movie);
    bool PERSONS_deleteAll(void);

    int  GENREINMOVIE_insert(int _movie, int);
    bool GENREINMOVIE_deleteAll(void);

    int  PERSONINMOVIE_insert(int, int, int);
    bool PERSONINMOVIE_deleteAll(void);

    bool CONFIGURATION_populate(void);
    bool CONFIGURATION_get(confMap *);
    bool CONFIGURATION_insertOrUpdate(std::string, dbConfiguration _conf);

    void str2sql(std::string *_str);
    std::vector<std::string> vColIdToCol(std::vector<short> _colId, bool);


};
