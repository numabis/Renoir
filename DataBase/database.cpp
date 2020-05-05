#include "database.h"

#include <KnownFolders.h>
#include "databaseTabDef.h"
#include "databaseSQLLib.h"
#include "exlog.h"
#include "util.h"
#include "xml.h"
#include "Convert.h"
#include <sstream>
#include <fstream>
#include <functional>   // std::greater

#define EXECPCIONMBOX(EXCEPCION)    MBOX(std::string(__FUNCTION__) + std::string(":") + std::string(EXCEPCION.ErrText()), "Error", MB_ICONERROR | MB_OK);
#define ERRORMBOX(MERROR)           MBOX(std::string(__FUNCTION__) + std::string(":") + MERROR, "Error", MB_ICONERROR | MB_OK);

using std::string;
using std::vector;
using BUTIL::Util;

#pragma region TABLE_DEFINITIONS

typedef struct {
    float version;
    std::string dateAdded;
    std::string description;
} dbVersion;

dbVersion valuesDbVersions[] = { DBCHANGELOG };
dbConfiguration dbConf[] = { DBCONFFIELDS };

vector<string>           tabNames = TABNAMELIST;
std::string getSqlType[E_MAX_TYPES] = TABDATATYPES;

dbTableDef tblConfiguration[] = { DBTABCONFIGURATION };
dbTableDef tblDbVersions[] = { DBTABVERSION };
dbTableDef tblpathFS[] = { DBTABPATHFS };
dbTableDef tblMoviesFS[] = { DBTABMOVIESFS };
dbTableDef tblMovies[] = { DBTABMOVIES };
dbTableDef tblPersons[] = { DBTABPERSONS };
dbTableDef tblGenres[] = { DBTABGENRES };
dbTableDef tblRoles[] = { DBTABROLES };
dbTableDef tblPersonsInMovies[] = { DBTABPERSONINMOVIE };
dbTableDef tblGenresInMovies[] = { DBTABGENREINMOVIE };

vector<dbTableDef *> ptr_tabNames = TABSCRUCTLIST;

#define BULKMAXROW  1
#pragma endregion TABLE_DEFINITIONS

#pragma region CONTRUCTORS
DataBase::DataBase()
{
    update = false;

    filters.exists = true;
    filters.orderByAsc = true;
    filters.orderByColumn = COL_FILENAME + 1;
    filters.minOccurrences = MIN_OCCURRENCES;

    for (short filter = 0; filter < FILTERS_MAX; filter++)
    {
        if (ftype[filter] == NORM_COMBOT)
            filters.valueSelected[filter] = TYPE_UNSET;
        else if (ftype[filter] == GENRE_COMBOT)
        {
            filters.valueSelected[filter] = TYPE_ALL;
        }
        filters.nameSelected[filter] = "All";
    }

    filters.valueSelected[FILTERS_YEARMIN] = MIN_YEAR;
    filters.nameSelected[FILTERS_YEARMIN] = std::to_string(MIN_YEAR);
    filters.valueSelected[FILTERS_YEARMAX] = MAX_YEAR;
    filters.nameSelected[FILTERS_YEARMAX] = std::to_string(MAX_YEAR);
    filters.valueSelected[FILTERS_IMDBRATINGMIN] = MIN_IMDBRATING;
    filters.nameSelected[FILTERS_IMDBRATINGMIN] = std::to_string(MIN_IMDBRATING);
    filters.valueSelected[FILTERS_IMDBRATINGMAX] = MAX_IMDBRATING;
    filters.nameSelected[FILTERS_IMDBRATINGMAX] = std::to_string(MAX_IMDBRATING);

    for (short invert = INVERT_FILTERS_GENRES; invert < INVERT_FILTERS_MAX; invert++)
        filters.invertFilters[invert] = false;

    for (int tab = 0; tab < MAXTABLES; tab++)
    {
        tabStruct[tab] = ptr_tabNames[tab];
    }
    filters.folderFilterId = -1;
    ptr_dbConfig = &dbConfig;
}
DataBase::~DataBase(void)
{
   //for (int i=0;i<10;i++)
   //    delete filters->v_combos[FILTERS_MAX];
}
#pragma endregion 

#pragma region INIT_DB
bool DataBase::isConnected(void)
{
    return connected;
}
bool DataBase::setDB(std::string _path)
{
    exLOGDEBUG("");

    dbConfig.dbPath = _path;
    dbConfig.dbName = DBNAME;
    return setDB(&dbConfig);

}
bool DataBase::setDB(DB_CONFIG* _dbConfig)
{
    exLOGDEBUG("");
    DWORD success = false;
    DWORD error = ERROR_BAD_PATHNAME;
    ptr_dbConfig = _dbConfig;
    std::string file;

    if (ptr_dbConfig->dbPath.empty() == false)
        file = ptr_dbConfig->dbPath;

    if (ptr_dbConfig->dbPath.empty() == false)
    {
        if ((file.compare(file.size() - 1, 1, "/") != 0) && (file.compare(file.size() - 1, 1, "\\") != 0))
            file += "\\";

        DWORD ftyp = GetFileAttributesA(file.c_str());
        error = ::GetLastError();
        if (ftyp == INVALID_FILE_ATTRIBUTES)
        {
            exLOGDEBUG("Create Dir %s", file.c_str());
            success = CreateDirectoryA(file.c_str(), NULL);
            error = ::GetLastError();
        }

        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        {
            error = ERROR_SUCCESS;
            exLOGDEBUG("Ok");
            success = true;
        }
    }

    if (error != ERROR_ALREADY_EXISTS && error != ERROR_SUCCESS) {
        exLOGDEBUG("Error");
        ERRORMBOX(BUTIL::Util::GetLastErrorAsString(error) + std::string("path") + file);
        exLOGERROR("path", file.c_str());
        connected = false;
        return connected;
    }

    file += ptr_dbConfig->dbName;

    exLOGINFO("DB set in %s", file.c_str());

    connected = false;
    //if (setDBFile(file,"OFF","OFF"))
    if (setDBFile(file, "PERSIST", "NORMAL"))
        connected = initDB(false);

    return connected;
}
bool DataBase::initDB(bool _resetDB)
{
    exLOGDEBUG("");
    bool success = true;
    for (int tab = 0; tab<MAXTABLES; tab++)
        if (_resetDB || !tabExists(TABLE(tab)))
        {
            exLOGDEBUG("Creating tables");
            if (initTable(tab) == false)
            {
                std::ostringstream ostr;
                ostr << "Error creating table " << TABLE(tab).c_str();
                MBOX(ostr.str(), "Error", MB_ICONERROR | MB_OK);
                return false;
                success = false;
            } else
                exLOGINFO("Created tab [%s]", tabNames[tab].c_str());

            if (tab == TAB_ROLES)
                ROLES_populate();
            if (tab == TAB_DBVERSIONS)
                DBVERSIONS_populate();
            if (tab == TAB_CONFIGURATION)
                CONFIGURATION_populate();
        }

    DBVERSIONS_getVersion();

    return success;
}
bool DataBase::initTable(int _tabId)
{
    int colId;
    string createTable, tabDef;

    if (tabExists(TABLE(_tabId)))
        execNolock(DROP(_tabId));

    if (!tabExists(TABLE(_tabId)))
    {
        colId = 0;
        dbTableDef *columnDef = &tabStruct[_tabId][colId];
        while (columnDef->Ctype != E_END_TABLE)
        {
            if (colId != 0)
                tabDef += ", ";
            tabDef += "[" + columnDef->name + "] " + getSqlType[columnDef->Ctype] + " " + columnDef->sqlDef;
            columnDef = &tabStruct[_tabId][++colId];
        }
        if (columnDef->sqlDef.empty() == false)
            tabDef += ", " + columnDef->sqlDef;
        createTable = CREATE(_tabId, tabDef);

        return exec(createTable);
    }
    return true;
}
double DataBase::DBVERSIONS_getVersion(void)
{
    exLOGDEBUG("");
    DBVersion = 0.0f;
    //wchar_t msg[256];
    std::string request;

    dbVersion* dbVersion = valuesDbVersions;
    while (dbVersion->version != 0.0f)
    {
        DBLocalVersion = dbVersion->version;
        dbVersion++;
    }
    request = SELECT(DBVER_VERSION) + FROM(TAB_DBVERSIONS) + ORDERBY(DBVER_DATEADDED, ORDERBYDSC) + LIMIT(1);

    try
    {
        SQLEXEC;
        if (SQLFETCH)
        {
            DBVersion = SQLFIELD(DBVER_VERSION).asDouble();
        }
        else
        {
            //wsprintf(msg, L"Error in DB Version, you should reset the databse, reset?");
            askUpdateDB("Error in DB Version, you should reset the databse, reset?");
        }
        SQLCLOSE;
    }
    catch (SAException &e)
    {
        //std::string sqlError = (std::string)e.ErrText();
        //wsprintf(msg, L"DB ERROR : %s\nYou shoud update DB, update?", sqlError.c_str());
        __debugbreak();
        askUpdateDB("DB ERROR : " + std::string(e.ErrText()) + "\nYou shoud update DB, update?");
        DBVersion = -1.0f;
        exLOGERROR("%s - SQL : %s", e.ErrText(), request.c_str());
        //SQLCLOSE;
    }

    if (abs(DBVersion - DBLocalVersion) > 0.001)
    {
        std::ostringstream ostr;
        ostr << "DB WARNING : DB is obsolete, update (" << DBVersion << "->" << DBLocalVersion << ") ? ";
        askUpdateDB(ostr.str());
    }

    return DBVersion;
}
void DataBase::askUpdateDB(std::string _msg)
{
    if (MBOX(_msg, "Error", MB_ICONERROR | MB_YESNO) == IDYES)
    {
        update = true;
        ptr_dbConfig->resetDB = true;
    }
}

bool DataBase::ROLES_populate()
{
    exLOGDEBUG("");
    bool ret = true;
    std::string request;
    short role;
    for (role = ROLES_DIRECTOR; role < ROLES_MAX; role++)
    {
        string request = INSERTINTO(TAB_ROLES) + INSERTCOLS(ROLES_ID, ROLES_ROLE) + INSERTVALS(STR(role), tabRoles[role]);

        int tmpRet = execNolock(request);
        if (tmpRet != ERROR_SUCCESS)
            ret = false;
        exLOGSQL("SQL %s : %s", ret?"ok":"ko",request.c_str());
    }
    exLOGDB("DB insert to %s : %d", TABLE(TAB_ROLES).c_str(), role);
    return ret;
}
bool DataBase::DBVERSIONS_populate(void)
{
    exLOGDEBUG("");
    bool ret = true;
    dbVersion* dbVersion = valuesDbVersions;
    std::string request;
    exLOGDB("DB insert to %s", TABLE(TAB_DBVERSIONS).c_str());

    while (dbVersion->version != 0.0f)
    {
        str2sql(&dbVersion->description);

        string request = INSERTINTO(TAB_DBVERSIONS);
        request += INSERTCOLS(DBVER_VERSION, DBVER_DATEADDED, DBVER_DESCRIPTION);
        request += INSERTVALS(STR(dbVersion->version),dbVersion->dateAdded,dbVersion->description);
        
        int tmpRet = execNolock(request);
        if (tmpRet != ERROR_SUCCESS)
            ret = false;

        dbVersion++;
        exLOGSQL("SQL %s : %s", ret ? "ok" : "ko", request.c_str());
    }
    return ret;
}

bool DataBase::getUpdate()
{
    exLOGDEBUG("");
    return update;
}
DataBase::moviesFSFilters *DataBase::getFilters()
{
    return &filters;
}
#pragma endregion 

#pragma region DB_STRUCT
void DataBase::str2sql(std::string *_str)
{
    size_t quote = _str->find_first_of("'");
    while (quote != std::string::npos)
    {
        if (_str->substr(quote + 1, 1) != "'")
        {
            _str->replace(quote, 1, "''");
        }
        quote = _str->find_first_of("'", quote + 2);
    }
    quote = _str->find_first_of("&");
    while (quote != std::string::npos)
    { //&amp;quot;
        std::string tmp = _str->substr(quote, 6);
        if (_str->substr(quote, 6) == "&quot;")
            _str->replace(quote, 6, "''");
        quote = _str->find_first_of("&", quote + 2);
    }
}

int DataBase::execNolock(std::string _request)
{
    int ret = execUnlock(_request);
    if (ret == ERROR_SUCCESS)
    {
        return ERROR_SUCCESS;
    }

    std::string errorMsg = BUTIL::Util::GetLastErrorAsString(ret);
    __debugbreak();
    switch (ret)
    {
    case ERROR_ACCESS_DENIED:
        errorMsg = "Error: database is locked";
        break;
    case ERROR_WRITE_PROTECT:
        errorMsg = "columns are not unique";
    }
    exLOGSQL("ERROR SQL %s : %s", errorMsg.c_str(), _request.c_str());

    MBOX(errorMsg, "Error", MB_ICONERROR | MB_OK);
    return ret;
}

void DataBase::TRANSACTION_begin(void)
{
	lock();
	execNolock("BEGIN TRANSACTION");
}
void DataBase::TRANSACTION_commit(void)
{
	execNolock("COMMIT TRANSACTION");
	unlock();
}
vector<string> DataBase::vColIdToCol(vector<short> _colId, bool _dot)
{
    vector<string> colNames;
    std::vector<short>::iterator it;

    for (it = _colId.begin(); it != _colId.end(); it++)
    {
        if (_dot)
            colNames.push_back(COLUMN(*it));
        else
            colNames.push_back(COLNAME(*it));
    }
    return colNames;
}
#pragma endregion 

#pragma region TAB_CONFIGURATION
bool DataBase::CONFIGURATION_populate(void)
{
    exLOGDEBUG("");
    bool ret = true;
    dbConfiguration* conf = dbConf;
    std::string request;

    while (conf->variable != "")
    {
        string request = INSERTINTO(TAB_CONFIGURATION);
        request += INSERTCOLS(CONF_INDEX, CONF_VAR, CONF_VALUE, CONF_DESCRIPTION);
        request += INSERTVALS(STR(conf->index), conf->variable, conf->value, conf->description);

        int tmpRet = execNolock(request);
        if (tmpRet != ERROR_SUCCESS)
            ret = false;

        conf++;
        exLOGSQL("SQL %s : %s", ret ? "ok" : "ko", request.c_str());
    }
    return ret;

}
bool DataBase::getConfiguration(std::vector<dbConfiguration> * configManager)
{
    return CONFIGURATION_get(configManager);
}

bool DataBase::setConfiguration(dbConfiguration * row)
{
    return CONFIGURATION_insertOrUpdate(row->index, row->variable, row->value);
}

bool DataBase::CONFIGURATION_get(std::vector<dbConfiguration> * _configManager)
{
    bool ret = false;
    if (!connected)
        return ret;

    string request = SELECT(CONF_INDEX, CONF_VALUE) + FROM(TAB_CONFIGURATION);

    try
    {
        SQLEXEC;

        int row = CONF_CURRENTFOLDER;
        int index;
        
        while (SQLFETCH)
        {
            ret = true;
            index = SQLFIELD(CONF_INDEX).asShort();
            _configManager->at(index).value = (std::string)SQLFIELD(CONF_VALUE).asString();
        }
        SQLCLOSE;
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);
        exLOGERROR("SQL ko : %s", request.c_str());
        exLOGERROR(e.ErrText());
        return ret;
    }
    exLOGSQL("SQL ok : %s", request.c_str());
    return ret;
}
bool DataBase::CONFIGURATION_insertOrUpdate(int _id, std::string _var, std::string _val)
{
    int ret = -1;

    std::string request;

    request = COUNTFROM(TAB_CONFIGURATION) + WHERE(EQUAL(CONF_INDEX, STR(_id)));
    ret = execScalar(request);
    if (ret >= 0)
    { // UPDATE
        request = UPDATE(TAB_CONFIGURATION);
        request += SET(CONF_VALUE, _val);
        request += WHERE(EQUAL(CONF_INDEX, STR(_id)));
    }
    else
    { // INSERT
        request = INSERTINTO(TAB_CONFIGURATION);
        request += INSERTCOLS(CONF_INDEX, CONF_VAR, CONF_VALUE);
        request += INSERTVALS(STR(_id), _var,  _val);
    }
    return (execNolock(request) == ERROR_SUCCESS);
}
#pragma endregion 

#pragma region TAB_PATHFS
int  DataBase::getId(MovieFolder *_file)
{
    return PATHFS_getId(_file);
}
int  DataBase::getPathId(std::string _path)
{
    return PATHFS_selectId(_path);
}
bool DataBase::isFolderRead(MovieFolder *_folder)
{
    std::string request;
    bool ret = false;

    if (_folder->getIdPath() < 0)
        return PATHFS_selectId(_folder);

    request = SELECT(PATH_ISREAD);
    request += FROM(TAB_PATHFS);
    if (_folder->getIdPath()>=0)
        request += WHERE(EQUAL(PATH_ID, STR(_folder->getIdPath())));
    else
    request += WHERE(EQUAL(PATH_PATH, _folder->getPath())) + NOCASE;

    try
    {
        SQLEXEC;
        if (SQLFETCH)
        {
            _folder->setIsRead(SQLFIELD(PATH_ISREAD).asBool());
            ret = true;
        }
        SQLCLOSE;
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);
        exLOGERROR(e.ErrText());
        ret = false;
    }

    return ret;

}
 int DataBase::PATHFS_getId(MovieFolder *_folder)
{
    if (_folder->getIdPath() > 0)
        return _folder->getIdPath();

    PATHFS_selectId(_folder);

    return _folder->getIdPath();
}
 int DataBase::PATHFS_selectId(std::string _path)
 {
     int ret = -1;
     std::string request;
     str2sql(&_path);

     request = SELECT(PATH_ID);
     request += FROM(TAB_PATHFS);
     request += WHERE(EQUAL(PATH_PATH, _path)) + NOCASE;

     try
     {
         SQLEXEC;
         if (SQLFETCH)
         {
             ret = SQLFIELD(PATH_ID).asShort();
         }
         SQLCLOSE;
     }
     catch (SAException &e)
     {
         __debugbreak();
         EXECPCIONMBOX(e);
         exLOGERROR(e.ErrText());
         ret = -1;
     }

     return ret;
 }
bool DataBase::PATHFS_selectId(MovieFolder *_folder)
{
    bool ret = false;
    std::string request;
    std::string path = _folder->getPath();
    str2sql(&path);

    request = SELECT(PATH_ID, PATH_ISREAD, PATH_ISSUB, PATH_HIDE);
    request += FROM(TAB_PATHFS);
    request += WHERE(EQUAL(PATH_PATH, path)) + NOCASE;

    try
    {
        SQLEXEC;
        if (SQLFETCH)
        {
            _folder->setIdPath(SQLFIELD(PATH_ID).asShort());
            _folder->setIsRead(SQLFIELD(PATH_ISREAD).asBool());
            _folder->setIsSub(SQLFIELD(PATH_ISSUB).asBool());
            _folder->setIsHide(SQLFIELD(PATH_HIDE).asBool());
            ret = true;
        }
        SQLCLOSE;
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);
        exLOGERROR(e.ErrText());
        ret = false;
    }

    return ret;

}
bool DataBase::PATHFS_existsPath(MovieFolder *_folder)
{
    int pathId = PATHFS_existsPath(_folder->getPath());
    if (pathId > 0) _folder->setIdPath(pathId);
    return (pathId > 0);
}
int  DataBase::PATHFS_existsPath(std::string _filePath)
{
    // ret = movieID if success / -1 if error
    str2sql(&_filePath);
    int ret = 0;

    string request = SELECT(PATH_ID) +
        FROM(TAB_PATHFS) +
        WHERE(LIKE(PATH_PATH, _filePath));

    return execScalarUnlock(request);

}
int  DataBase::PATHFS_insert(std::string _path)
{
    if (_path[_path.size() -1] != '\\')
        _path += "\\";
   
    MovieFolder folder;
    folder.setPath(_path);
    PATHFS_getId(&folder);
    if (folder.getIdPath() >= 0)
        return folder.getIdPath();
    return PATHFS_insert(&folder);
}
int  DataBase::PATHFS_insert(MovieFolder *_folder)
{// ret = 0 if success / GetLastError() if error
    int ret = PATHFS_existsPath(_folder);
    if (ret > 0)
        return PATHFS_update(_folder);
    ret = -1;

    std::string path = _folder->getPath();
    str2sql(&path);
    
    string request = INSERTINTO(TAB_PATHFS) + INSERTCOLS(PATH_PATH, PATH_ISREAD, PATH_ISSUB, PATH_HIDE) + INSERTVALS(path, STR(_folder->isRead()), STR(_folder->isSub()), STR(_folder->isHide()));

    ret = (execNolock(request) == ERROR_SUCCESS);

    if (ret)
    {
        ret = PATHFS_getId(_folder);
        exLOGINFO("Inserted new path %s as %s", path.c_str(), _folder->isSub() ? "Sub" : "Head");
    }
    else
        __debugbreak();
    exLOGSQL("SQL %s : %s", ret ? "ok" : "ko", request.c_str());
    return ret;
}
void DataBase::PATHFS_bulkInsert(MovieFolder *_folder)
{
    if (PATHFS_existsPath(_folder))
        return ;

    std::string path = _folder->getPath();
    str2sql(&path);

    //string request = INSERTINTO(TAB_PATHFS) + INSERTCOLS(PATH_PATH, PATH_ISREAD, PATH_ISSUB, PATH_HIDE) + INSERTVALS(path, STR(_folder->getIsRead()), STR(_folder->getIsSub()), STR(_folder->getIsHide()));

    string request;
    if (buklInserts[TAB_PATHFS].size() == 0)
        request = INSERTINTO(TAB_PATHFS) + INSERTCOLS(PATH_PATH, PATH_ISREAD, PATH_ISSUB, PATH_HIDE) + INSERTVALS(path, STR(_folder->isRead()), STR(_folder->isSub()), STR(_folder->isHide()));
    else
        request = INSERTBULK(path, STR(_folder->isRead()), STR(_folder->isSub()), STR(_folder->isHide()));


    buklInserts[TAB_PATHFS].push_back(request);
}

int DataBase::PATHFS_update(MovieFolder *_folder)
{// ret = 0 if success / GetLastError() if error
    std::string path = _folder->getPath();
    str2sql(&path);
    int ret = -1;

    string request = UPDATE(TAB_PATHFS);

    request += SET(PATH_ISREAD, STR(_folder->isRead()));
    request += SET(PATH_ISSUB, STR(_folder->isSub()));
    request += SET(PATH_HIDE, STR(_folder->isHide()));
    
    size_t sz = request.size();

    if (request[sz - 1] == ',')
        request[sz - 1] = ' ';

    if (_folder->getIdPath() != -1)
    {
        request += WHERE(EQUAL(PATH_ID, STR(_folder->getIdPath())));
    }
    else
    {
        request += WHERE(EQUAL(PATH_PATH, path));
    }

//    if (countSets > 0)
        ret = (execNolock(request) == ERROR_SUCCESS);

    exLOGSQL("SQL %s : %s", ret ? "ok" : "ko", request.c_str());
    return ret;
}
int  DataBase::PATHFS_loadFolders()
{
    if (!connected)
        return -1;

    std::string tmp;

    string request;
    string select, from, groupby, orderby;

    //SELECT path, count(idpath)
    //    FROM paths join moviesFS using (idpath)
    //    group by idpath
    //    ORDER BY 1 COLLATE NOCASE DESC
    enum { COLVAL = 1, COLCOUNT };

    select = SELECT(PATH_PATH) + COUNTCOL(PATH_ID);
    from = FROM(TAB_PATHFS) + JOINUSING(TAB_MOVIESFS,PATH_ID);
    groupby = GROUPBY(PATH_ID, 0);
    orderby = ORDERBY(PATH_PATH, ORDERBYASC);

    request = select + from + groupby + orderby;

    filters.folders.clear();

    try
    {
        SQLEXEC;
        std::string folder;
        while (SQLFETCH)
        {
            pondStr field;
            field.str = (std::string)SQLFIELD(PATH_PATH).asString();
            field.pond = SQLFIELDNAME(COLCOUNT).asShort();
            filters.folders.push_back(field);
            //folder = SQLFIELDNAME(PATH_PATH).asString();
            //filters.folders.push_back(folder);
        }
        SQLCLOSE;
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);
        exLOGERROR("SQL ko : %s", request.c_str());
        exLOGERROR(e.ErrText());
        return -1;
    }
    exLOGSQL("SQL ok : %s", request.c_str());
    return filters.folders.size();
}
#pragma endregion 

#pragma region TAB_MOVIESFS
bool DataBase::getData(MovieFile *_file)
{
    return MOVIESFS_get(_file);
}
string DataBase::AllMoviesFSGetFilter()
{
    //string all = "1";

    //string folderPath = *filters.folderFilter + "%";
    //BUTIL::Convert::string2sqlLike(&folderPath);
    //string folderList = SELECT(PATH_ID) + FROM(TAB_PATHFS) + WHERE(LIKE(PATH_PATH, folderPath));
    //string folderFilter = INSELECT(FS_IDPATH, folderList);
    string conditions = WHERE(EQUAL(FS_IDPATH, STR(filters.folderFilterId)));

    string yearMinFilter = GREATER(MOVIES_YEAR, filters.valueSelected[FILTERS_YEARMIN]);
    string yearMaxFilter = LESSER(MOVIES_YEAR, filters.valueSelected[FILTERS_YEARMAX]);
    string imdbRatingMinFilter = GREATER(MOVIES_IMDBRATING, filters.valueSelected[FILTERS_IMDBRATINGMIN]);
    string imdbRatingMaxFilter = LESSER(MOVIES_IMDBRATING, filters.valueSelected[FILTERS_IMDBRATINGMAX]);

    if (filters.valueSelected[FILTERS_YEARMIN] > MIN_YEAR)
        conditions += AND(yearMinFilter);
    if (filters.valueSelected[FILTERS_YEARMAX] < MAX_YEAR && filters.valueSelected[FILTERS_YEARMAX]>=0)
        conditions += AND(yearMaxFilter);
    if (filters.valueSelected[FILTERS_IMDBRATINGMIN] > MIN_IMDBRATING && filters.valueSelected[FILTERS_IMDBRATINGMIN] >= 0)
        conditions += AND(imdbRatingMinFilter);
    if (filters.valueSelected[FILTERS_IMDBRATINGMAX] < MAX_IMDBRATING && filters.valueSelected[FILTERS_IMDBRATINGMAX] >= 0)
        conditions += AND(imdbRatingMaxFilter);

    for (short filter = FILTERS_DIRECTORS; filter < FILTERS_GENRES; filter++)
    {
        if (filters.valueSelected[filter] >= 0)
        {
            string selectFilter = SELECT(PERSONINMOVIE_IDMOVIE) + FROM(TAB_PERSONINMOVIE);
            string personFilter;
            selectFilter += WHERE(EQUAL(PERSONINMOVIE_IDPERSON, STR(filters.valueSelected[filter])));

            selectFilter += AND(EQUAL(PERSONINMOVIE_IDROLE, STR(filter)));

            short invertFilter = (filter == FILTERS_DIRECTORS) ? INVERT_FILTERS_DIRECTORS : INVERT_FILTERS_ACTORS;
            if (filters.invertFilters[invertFilter] == false)
                personFilter = INSELECT(MOVIES_IDMOVIE, selectFilter);
            else
                personFilter = NOTINSELECT(MOVIES_IDMOVIE, selectFilter);

            conditions += AND(personFilter);
        }
    }

    if (filters.valueSelected[FILTERS_GENRES] >= 0)
    {
        string selectFilter = SELECT(GENREINMOVIE_IDMOVIE) + FROM(TAB_GENREINMOVIE);
        string genreFilter;

        selectFilter += WHERE(EQUAL(GENREINMOVIE_IDGENRE, STR(filters.valueSelected[FILTERS_GENRES])));

        if (filters.invertFilters[INVERT_FILTERS_GENRES] == false)
            genreFilter = INSELECT(MOVIES_IDMOVIE, selectFilter);
        else
            genreFilter = NOTINSELECT(MOVIES_IDMOVIE, selectFilter);
        conditions += AND(genreFilter);
    }
    if (filters.valueSelected[FILTERS_GENRES2] >= 0)
    {
        string selectFilter = SELECT(GENREINMOVIE_IDMOVIE) + FROM(TAB_GENREINMOVIE);
        string genreFilter;
        selectFilter += WHERE(EQUAL(GENREINMOVIE_IDGENRE, STR(filters.valueSelected[FILTERS_GENRES2])));

        if (filters.invertFilters[INVERT_FILTERS_GENRES2] == false)
            genreFilter = INSELECT(MOVIES_IDMOVIE, selectFilter);
        else
            genreFilter = NOTINSELECT(MOVIES_IDMOVIE, selectFilter);
        conditions += AND(genreFilter);
    }
    if (filters.valueSelected[FILTERS_SERIE] <  TYPE_ALL)
    {
        if (filters.valueSelected[FILTERS_SERIE] == TYPE_NOTMOVIE)
            conditions += AND(EQUAL(FS_SERIE, STR(TYPE_NOTMOVIE)));
        else 
            conditions += AND(LESSER(FS_SERIE, TYPE_MOVIE));
    }
    if (filters.valueSelected[FILTERS_ANIM] <  TYPE_ALL)
    {
        if (filters.valueSelected[FILTERS_ANIM] == TYPE_NOTMOVIE)
            conditions += AND(EQUAL(FS_ANIM, STR(TYPE_NOTMOVIE)));
        else
            conditions += AND(LESSER(FS_ANIM, TYPE_MOVIE));
    }
    if (filters.valueSelected[FILTERS_DOC] <  TYPE_ALL)
    {
        if (filters.valueSelected[FILTERS_DOC] == TYPE_NOTMOVIE)
            conditions += AND(EQUAL(FS_DOC, STR(TYPE_NOTMOVIE)));
        else
            conditions += AND(LESSER(FS_DOC, TYPE_MOVIE));
    }
    if (filters.valueSelected[FILTERS_SHORT] <  TYPE_ALL)
    {
        if (filters.valueSelected[FILTERS_SHORT] == TYPE_NOTMOVIE)
            conditions += AND(EQUAL(FS_SHORT, STR(TYPE_NOTMOVIE)));
        else
            conditions += AND(LESSER(FS_SHORT, TYPE_MOVIE));
    }
    if (filters.exists == true)
    {
        string existsFilter = EQUAL(FS_PHYSICAL, STR(filters.exists));
        conditions += AND(existsFilter);
    }
    return conditions;
}
bool   DataBase::MOVIESFS_iniAll()
{
    int count = 0;
    if (!connected)
        return false;
    std::string request , conditions;
    try
    {
        if (SQLGENTEST)
        {
            string select = SELECT(ColumnsToShow);
            string from = FROM(TAB_MOVIESFS);
            string join = JOINUSING(TAB_MOVIES, FS_IMDBID);
            string conditions = AllMoviesFSGetFilter();
            string order = filters.orderByAsc ? ORDERBYDSC : ORDERBYASC;
            string orderby = ORDERBY(filters.orderByColumn, order);

            request = select + from + join + conditions + orderby;

            ptr_consoleDBG->push_back(request);
            //debug

            SQLGENEXEC;

            //if (SQLGENFETCH)
            //{
            //    SQLGENCURSOR.Field. ;
            //    count++;
            //}

            exLOGSQL("SQL ok : %s", request.c_str());
            return true;
        }
    }
    catch (SAException &e)
    {
        __debugbreak();

        std::ostringstream ostr;
        ostr << "iniAllMoviesFS : " << e.ErrText();
        MBOX(ostr.str(), "Error", MB_ICONERROR | MB_OK);

        exLOGERROR("SQL ko : %s", request.c_str());
        exLOGERROR(e.ErrText());
        SQLGENCLOSE;
        return false;
    }

    return true;
}
bool   DataBase::MOVIESFS_getAll(MovieFile *_file)
{
    if (!connected)
        return false;
    static int count = 0;
    try
    {
        if (SQLGENFETCH)
        {
            _file->moviesFSId = SQLGENFIELD(FS_ID).asShort();
            _file->setPath((std::string)SQLGENFIELD(FS_PATH).asString());
            _file->setFilename((std::string)SQLGENFIELD(FS_FILENAME).asString());
            _file->exists = SQLGENFIELD(FS_PHYSICAL).asBool();
            _file->imdbId = SQLGENFIELD(FS_IMDBID).asString();
            _file->dateAdded = SQLGENFIELD(FS_DATEADDED).asString();
            _file->dateAdded = _file->dateAdded.substr(0, 10);
            _file->size = SQLGENFIELD(FS_FILESIZE).asULong();
            _file->typeIs[TYPE_SERIE] = (typesValues)SQLGENFIELD(FS_SERIE).asShort();
            _file->typeIs[TYPE_ANIM] = (typesValues)SQLGENFIELD(FS_ANIM).asShort();
            _file->typeIs[TYPE_DOC] = (typesValues)SQLGENFIELD(FS_DOC).asShort();
            _file->typeIs[TYPE_SHORT] = (typesValues)SQLGENFIELD(FS_SHORT).asShort();
            _file->imdbRating = SQLGENFIELD(MOVIES_IMDBRATING).asShort();
            if (_file->imdbRating != 0)
            {
                while (_file->imdbRating > 100)
                    _file->imdbRating /= 10;
                while (_file->imdbRating < 10)
                    _file->imdbRating *= 10;
            }
            //short year = SQLGENFIELD(MOVIES_YEAR).asShort();
            //if (year < RangeYears[MIN])
            //    RangeYears[MIN] = year;
            //if (year > RangeYears[MAX])
            //    RangeYears[MAX] = year;
            count++;
            return true;
        }

        SQLGENCLOSE;
        exLOGDB("DB select from %s : %d", TABLE(TAB_MOVIESFS).c_str(), count);
        count = 0;
        return false;
    }
    catch (SAException &e)
    {
        __debugbreak();

        //std::ostringstream ostr;
        //ostr << __FUNCTION__ << " : " << e.ErrText();
        //MBOX(ostr.str(), "Error", MB_ICONERROR | MB_OK);
        //MBOX("" + std::string(__FUNCTION__) + std::string(":") + std::string(e.ErrText()), "Error", MB_ICONERROR | MB_OK);
        EXECPCIONMBOX(e);

        exLOGERROR(e.ErrText());
        SQLGENCLOSE;
        return false;
    }

    return true;
}
int   DataBase::MOVIESFS_getFSId(MovieFile *_file)
{
    if (_file->moviesFSId > 0)
        return _file->moviesFSId;

    MOVIESFS_selectFSId(_file);

    return _file->moviesFSId;
}
bool DataBase::MOVIESFS_get(MovieFile *_file)
{
    std::string tmpDate;
    std::string request;
    //SACommand cursorMovies;
    bool ret = false;

    if (_file->moviesFSId > 0)
    {
        request = SELECT(FS_IMDBID, FS_SERIE, FS_ANIM, FS_DOC, FS_SHORT) + FROM(TAB_MOVIESFS) + WHERE(EQUAL(FS_ID, STR(_file->moviesFSId)));
        try
        {

            SQLEXEC;
            if (SQLFETCH)
            {
                _file->imdbId = SQLFIELD(FS_IMDBID).asString();
                for (int type = 0; type < TYPE_MAX; type++)
                {
                    int field = FS_SERIE + type;
                    //short value = SQLFIELD(FS_SERIE + type).asShort();
                    //_file->typeIs[type] = (typesValues)value;
                    _file->typeIs[type] = (typesValues)SQLFIELD(field).asShort();

                }
                ret = true;
            }

            SQLCLOSE;
        }
        catch (SAException &e)
        {
            __debugbreak();
            EXECPCIONMBOX(e);
            exLOGERROR("SQL ko : %s", request.c_str());
            exLOGERROR(e.ErrText());
            return false;
        }
        exLOGSQL("SQL ok : %s", request.c_str());
    }

    return ret;
}
bool   DataBase::MOVIESFS_selectFSId(MovieFile *_file)
{
    std::string request;
    bool ret = false;
    string filename = _file->getFilename();
    str2sql(&filename);
    //string path = _file->getPath();
    //str2sql(&path);

    request = SELECT(FS_ID);
    request += FROM(TAB_MOVIESFS);

    if (_file->isImdbId())
        request += WHERE(EQUAL(FS_IMDBID, _file->imdbId));
    else
    {
        //request += WHERE(EQUAL(FS_PATH, path)) + AND(EQUAL(FS_FILENAME, filename));
        request += WHERE(EQUAL(FS_IDPATH, STR(_file->getIdPath()))) + AND(EQUAL(FS_FILENAME, filename));
    }

    try
    {
        SQLEXEC;
        if (SQLFETCH)
        {
            _file->moviesFSId = SQLFIELD(FS_ID).asShort();
            ret = true;
        }
        SQLCLOSE;
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e); 
        //wchar_t msg[256];
        //std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
        //wsprintf(msg, L"selectMoviesFSId : %s", sqlError.c_str());
        //MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        
        exLOGERROR(e.ErrText());
        ret = false;
    }

    return ret;

}
bool   DataBase::MOVIESFS_setPhysical(bool _exists)
{
    std::string request = UPDATE(TAB_MOVIESFS);
    string exists = _exists ? "1" : "0";
    request += SET(FS_PHYSICAL, exists);

    size_t sz = request.size();
    if (request[sz - 1] == ',')
        request[sz - 1] = ' ';

    //string folderPath = *filters.folderFilter + "%";
    //string folderList = SELECT(PATH_ID) + FROM(TAB_PATHFS) + WHERE(LIKE(PATH_PATH, folderPath));
    //string folderFilter = INSELECT(FS_IDPATH, folderList);
    //request += WHERE(folderFilter);
    request += WHERE(EQUAL(FS_IDPATH, STR(filters.folderFilterId)));

    int ret = execNolock(request);
    return (ret == ERROR_SUCCESS);
}
 int   DataBase::MOVIESFS_exists(MovieFile *_file)
{
    if (_file->getIdPath() == -1)
        PATHFS_getId(_file);
    int fsId = MOVIESFS_exists(_file->getIdPath(), _file->getFilename());
    if (fsId >= 0) 
        _file->moviesFSId = fsId;
    return fsId;
}
// int   DataBase::existsMovieFS(std::string _filePath, std::string _fileName)
// {// ret = movieID if success / -1 if error
//     str2sql(&_filePath);
//     str2sql(&_fileName);
//     int ret = 0;
//
//     string request = SELECT(FS_ID) +
//         FROM(TAB_MOVIESFS) +
//         WHERE(LIKE(FS_PATH, _filePath)) +
//         AND(LIKE(FS_FILENAME, _fileName));
//
//     return execScalarUnlock(request);
// }
 int   DataBase::MOVIESFS_exists(int _idPath, std::string _fileName)
 {// ret = movieID if success / -1 if error
     str2sql(&_fileName);
     int ret = 0;

     string request = SELECT(FS_ID) +
         FROM(TAB_MOVIESFS) +
         WHERE(LIKE(FS_IDPATH, STR(_idPath))) +
         AND(LIKE(FS_FILENAME, _fileName));

     return execScalarUnlock(request);
 }
 bool   DataBase::MOVIESFS_update(MovieFile *_file, bool* fields)
{// ret = 0 if success / GetLastError() if error
    string filename = _file->getFilename();
    str2sql(&filename);
    std::string tmpPath = _file->getPath();
    //str2sql(&_file->path);
    str2sql(&tmpPath);
    bool ret = false;

    if (_file->getIdPath() == -1)
    {
        if (PATHFS_getId(_file) == -1)
        {
            PATHFS_insert(_file);
        }
    }
    if (_file->getIdPath() != -1)
    {
        fields[FS_IDPATH - FS_ID] = true;
    }

    string exists = _file->exists ? "1" : "0";
    string size = _file->exists ? STR(_file->size) : "0";
    string isSerie = STR(_file->typeIs[TYPE_SERIE]);
    string isAnim = STR(_file->typeIs[TYPE_ANIM]);
    string isDoc = STR(_file->typeIs[TYPE_DOC]);
    string isShort = STR(_file->typeIs[TYPE_SHORT]);
    string idPath = STR(_file->getIdPath());

    string request = UPDATE(TAB_MOVIESFS);
    short countSets = 0;
    for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
        if (fields[i]) countSets++;
        
    if (_file->moviesFSId != -1)
    {
        if (fields[FS_PATH - FS_ID])     request += SET(FS_PATH, filename);
        if (fields[FS_FILENAME - FS_ID]) request += SET(FS_FILENAME, filename);
    }
    if (fields[FS_PHYSICAL - FS_ID])     request += SET(FS_PHYSICAL, exists);
    if (fields[FS_IMDBID - FS_ID])       request += SET(FS_IMDBID, _file->imdbId);
    if (fields[FS_DATEADDED - FS_ID])    request += SET(FS_DATEADDED, _file->dateAdded);
    if (fields[FS_FILESIZE - FS_ID])     request += SET(FS_FILESIZE, size);
    if (fields[FS_SERIE - FS_ID])        request += SET(FS_SERIE, isSerie);
    if (fields[FS_ANIM - FS_ID])         request += SET(FS_ANIM, isAnim);
    if (fields[FS_DOC - FS_ID])          request += SET(FS_DOC, isDoc);
    if (fields[FS_SHORT - FS_ID])        request += SET(FS_SHORT, isShort);
    if (fields[FS_IDPATH - FS_ID])       request += SET(FS_IDPATH, idPath);

    size_t sz = request.size();

    if (request[sz - 1] == ',')
        request[sz - 1] = ' ';

    if (_file->moviesFSId != -1)
    {
        request += WHERE(EQUAL(FS_ID, STR(_file->moviesFSId)));
    }
    else if (_file->getIdPath() == -1)
    {
        request += WHERE(EQUAL(FS_IDPATH, STR(_file->getIdPath())));
        request += AND(EQUAL(FS_FILENAME, filename));
    }
    else
    {
        request += WHERE(EQUAL(FS_PATH, tmpPath));
        request += AND(EQUAL(FS_FILENAME, filename));
    }

    if (countSets > 0)
        ret = (execNolock(request) == ERROR_SUCCESS);

    return ret;

}
 int   DataBase::MOVIESFS_insert(MovieFile *_file)
{// ret = 0 if success / GetLastError() if error

    if (PATHFS_getId(_file) == - 1)
        PATHFS_insert(_file);

    std::string filename = _file->getFilename();
    std::string path = _file->getPath();
    str2sql(&filename);
    str2sql(&path);
    bool ret;

    string request = INSERTINTO(TAB_MOVIESFS);
    request += INSERTCOLS(FS_IDPATH, FS_PATH, FS_FILENAME, FS_IMDBID, FS_DATEADDED, FS_FILESIZE);
    request += INSERTVALS(STR(_file->getIdPath()), path, filename, _file->imdbId, _file->dateAdded, STR(_file->size));

    if (_file->getIdPath() == -1)
    {
        request += COLUMN(FS_IDPATH) + "==-1";
    }



    ret = (execNolock(request) == ERROR_SUCCESS);

    return ret;
}
void   DataBase::MOVIESFS_bulkInsert(MovieFile *_file)
{
    if (PATHFS_getId(_file) == -1)
        PATHFS_insert(_file);

    std::string filename = _file->getFilename();
    std::string path = _file->getPath();
    str2sql(&filename);
    str2sql(&path);

    std::string wildcard = "¿";

    std::string selectIdPath = SELECT(PATH_ID) + FROM(TAB_PATHFS) + WHERE(EQUAL(PATH_PATH, path));

    selectIdPath = "(" + selectIdPath + ")";

    int row = buklInserts[TAB_MOVIESFS].size();

    string request;
    if (row == 0) {
        request = INSERTINTO(TAB_MOVIESFS);
        request += INSERTCOLS(FS_IDPATH, FS_PATH, FS_FILENAME, FS_IMDBID, FS_DATEADDED, FS_FILESIZE);
        request += "VALUES ";
        buklInserts[TAB_MOVIESFS].push_back(request);
        request = INSERTBULK(wildcard, path, filename, _file->imdbId, _file->dateAdded, STR(_file->size));
    }
    else
    {
        request = INSERTBULK(wildcard, path, filename, _file->imdbId, _file->dateAdded, STR(_file->size));
    }

    //request += INSERTVALS(wildcard, path, filename, _file->imdbId, _file->dateAdded, STR(_file->size));

    size_t pos = request.find_first_of(wildcard);
    request.replace(pos-1, wildcard.size()+2, selectIdPath);

    buklInserts[TAB_MOVIESFS].push_back(request);
}
int    DataBase::bulkInsertCommit(tabIndex _id)
{
    if (buklInserts[_id].size() == 0)
        return 0;

    std::vector<std::string>::iterator it;

    //std::string insert, request, values;
    std::string insert, request;
    int row = 0;
    int count = 0;
    execNolock("BEGIN TRANSACTION\n");
    for (it = buklInserts[_id].begin(); it != buklInserts[_id].end(); it++)
    {
        if (it == buklInserts[_id].begin())
        {
            insert = *it;
        }
        else
        {
            request += *it;
            //request += insert + *it + ";";
            row++;
            if (row >= BULKMAXROW || (it+1) == buklInserts[_id].end())
            {
                //execNolock("BEGIN TRANSACTION\n");
                request = insert + request + ";";
                //request = "BEGIN TRANSACTION;\n";
                if (execNolock(request) == ERROR_SUCCESS)
                    count += row;
                //execNolock("COMMIT TRANSACTION");

                row = 0;
                request = "";
            } 
            else
                request += ",\n";
        }

    }
//    consoleDBG->push_back("Commit: Start");
    execNolock("COMMIT TRANSACTION");
//    consoleDBG->push_back("Commit: Done");


    buklInserts[_id].clear();
    return count;
}
void   DataBase::bulkInit(tabIndex _id)
{
    buklInserts[_id].clear();
}
bool   DataBase::MOVIESFS_deleteAll(void)
{
    bool ret = false;
    std::string request = DELETE(TAB_MOVIESFS);
    ret = (execNolock(request) == ERROR_SUCCESS);

    request = DELETE(TAB_PATHFS);
    ret &= (execNolock(request) == ERROR_SUCCESS);
    return ret;
}
#pragma endregion 

#pragma region TAB_MOVIES
int DataBase::MOVIES_iniAll(std::string _separator)
{
    std::string  request;
    separator = _separator;
    int count = 0;
    try
    {
        request = std::string("SELECT COUNT(*) AS ret ") + FROM(TAB_MOVIES);
        count = execScalar(request);
        if (SQLGENTEST)
        {
            request = SELECT(MOVIES_TITLE, MOVIES_YEAR, MOVIES_RATED, MOVIES_IDMOVIE, MOVIES_RUNTIME,
                MOVIES_IDMOVIE, MOVIES_IDMOVIE, MOVIES_IDMOVIE, MOVIES_IDMOVIE, MOVIES_PLOT,
                MOVIES_LANGUAGE, MOVIES_COUNTRY, MOVIES_AWARD, MOVIES_POSTER, MOVIES_METASCORE,
                MOVIES_IMDBRATING, MOVIES_IMDBVOTES, MOVIES_IMDBID, MOVIES_TYPE);
            request += FROM(TAB_MOVIES);
            SQLGENEXEC;
            exLOGSQL("SQL ok : %s", request.c_str());
            return count;
        }
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);

//        wchar_t msg[256];
//        std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
//        wsprintf(msg, L"iniAllMovies : %s", sqlError.c_str());
//        __debugbreak();
//        MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        exLOGERROR("SQL ko : %s", request.c_str());
        exLOGERROR(e.ErrText());
        SQLGENCLOSE;
        return -1;
    }

    return count;
}
bool DataBase::MOVIES_getAll(Movie *_movie)
{
    static int count = 0;
    std::string tmpDate;
    try
    { 
        if (SQLGENFETCH)
        {
            _movie->setIdMovie(SQLGENFIELD(MOVIES_IDMOVIE).asShort());
            _movie->setTitle(SQLGENFIELD(MOVIES_TITLE).asString());
            _movie->setTitle(SQLGENFIELD(MOVIES_TITLE).asString());
            _movie->setYear(SQLGENFIELD(MOVIES_YEAR).asShort());
            _movie->setRated(SQLGENFIELD(MOVIES_RATED).asString());
            _movie->setRuntime(SQLGENFIELD(MOVIES_RUNTIME).asString());
            _movie->setPlot(SQLGENFIELD(MOVIES_PLOT).asString());
            _movie->setLanguage(SQLGENFIELD(MOVIES_LANGUAGE).asString());
            _movie->setCountry(SQLGENFIELD(MOVIES_COUNTRY).asString());
            _movie->setAwards(SQLGENFIELD(MOVIES_AWARD).asString());
            _movie->setPoster(SQLGENFIELD(MOVIES_POSTER).asString());
            _movie->setMetascore(SQLGENFIELD(MOVIES_METASCORE).asShort());
            _movie->setImdbRating(SQLGENFIELD(MOVIES_IMDBRATING).asShort());
            _movie->setImdbId(SQLGENFIELD(MOVIES_IMDBID).asString());
            _movie->setImdbVotes(SQLGENFIELD(MOVIES_IMDBVOTES).asShort());
            _movie->setType(SQLGENFIELD(MOVIES_TYPE).asString());
            GENRES_getGenre(_movie);
            PERSONS_getPerson(_movie);
            count++;
            return true;
        }
        SQLGENCLOSE;
        exLOGDB("DB select from %s : %d", TABLE(TAB_MOVIES).c_str(), count);
        count = 0;
        return false;
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);

//        wchar_t msg[256];
//        std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
//        wsprintf(msg, L"getAllMovies : %s", sqlError.c_str());
//        __debugbreak();
//        MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);

        exLOGERROR(e.ErrText());
        SQLGENCLOSE;
        return false;
    }

    return true;
}

bool DataBase::MOVIES_insert(Movie *_m)
{
    std::string request;

    //std::string from = FROM(MOVIES);

    //begin();
    if (_m->getIdMovie() >= 0)
    {
        request = DELETE(TAB_MOVIES);
        request += WHERE(EQUAL(MOVIES_IDMOVIE, STR(_m->getIdMovie())));
        if (execNolock(request) == ERROR_SUCCESS)
            _m->setIdMovie(-1);
    }

    if (_m->getIdMovie() < 0)
    {
        std::string plot = _m->getPlot();
        str2sql(&plot);
        std::string title = _m->getTitle();
        str2sql(&title);
        std::string titleASCII = _m->getTitle();
        str2sql(&titleASCII);

        string cols = INSERTCOLS(MOVIES_TITLE, MOVIES_TITLEASCII, MOVIES_YEAR,
            MOVIES_RATED, MOVIES_RUNTIME, MOVIES_PLOT, MOVIES_LANGUAGE,
            MOVIES_COUNTRY, MOVIES_AWARD, MOVIES_POSTER, MOVIES_METASCORE,
            MOVIES_IMDBRATING, MOVIES_IMDBVOTES, MOVIES_IMDBID, MOVIES_TYPE);
        string vals = INSERTVALS(title, titleASCII, std::to_string(_m->getYear()),
            _m->getRated(), _m->getRuntime(), plot, _m->getLanguage(),
            _m->getCountry(), _m->getAwards(), _m->getPoster(), std::to_string(_m->getMetascore()),
            std::to_string(_m->getImdbRating()), std::to_string(_m->getImdbVotes()), _m->getImdbId(), _m->getType());

        request = INSERTINTO(TAB_MOVIES);
        request += cols;
        request += vals;
 
        if (execNolock(request) != ERROR_SUCCESS) {
            __debugbreak();
            ERRORMBOX(BUTIL::Util::GetLastErrorAsString());

//            wchar_t msg[256];
//            char* sqlError = BUTIL::Util::GetLastErrorAsString();
//            wsprintf(msg, L"insertMovie : %s", BUTIL::Convert::charToWchar(sqlError));
//            MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);

            exLOGERROR("SQL ko: %s", BUTIL::Util::GetLastErrorAsString());
            exLOGERROR("SQL ko: %s", request.c_str());
            return false;
        } else
            exLOGSQL("SQL ok : %s", request.c_str());
    }
    else
    {
        //execNolock("update tbl_ultimas_jugadas set Fecha=DATETIME(CURRENT_TIMESTAMP, 'localtime'), Datos='%s' where Sesion='%s' and Partida=%d and Link='%c'");
    }
    insertGenresInMovies(_m);
    insertPersonsInMovies(_m);
    //    insertDirectors(_movie);
    //    insertWriters(_movie);
    //    insertActors(_movie);
    //commit();

    return true;

}

bool DataBase::MOVIES_selectId(MovieFile *_file)
{
    return MOVIES_selectId(_file->getMovie());
}
bool DataBase::MOVIES_selectId(Movie *_movie)
{
    bool ret = false;
    //if (_movie->getIdMovie() >= 0)
    //    return true;

    std::string table = tabNames[TAB_MOVIES];
    std::string request = SELECT(MOVIES_IDMOVIE) + FROM(TAB_MOVIES);

    if (_movie->getImdbId().size() != 0)
    {
        request += WHERE(EQUAL(MOVIES_IMDBID, _movie->getImdbId())) ;
    }
    else if (_movie->getYear() != 0)
    {
        std::string title = _movie->getTitleASCII();
        if (title.size() == 0)
            std::string title = _movie->getTitle();
        BUTIL::Convert::string2sqlLike(&title);

        request += WHERE(LIKE(MOVIES_TITLEASCII, title)) + AND(EQUAL(MOVIES_YEAR, STR(_movie->getYear())));

    }
    else if (_movie->getTitleASCII().size() != 0)
    {
        //std::string title = _movie->getTitleASCII();
        std::string title = _movie->getTitle();
        if (title.size() == 0)
            std::string title = _movie->getTitle();
        BUTIL::Convert::string2sqlLike(&title);
        std::string like = LIKE(MOVIES_TITLEASCII, title);
        request += WHERE(LIKE(MOVIES_TITLEASCII, title));

    }
    else
        return false;

    try
    {
        SQLEXEC;
        if (SQLFETCH)
        {
            _movie->setIdMovie(SQLFIELD(MOVIES_IDMOVIE).asShort());
            ret = true;
        }
        SQLCLOSE;
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);
//        wchar_t msg[256];
//        std::wstring wstr_sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
//        const wchar_t *wchar_sqlError = BUTIL::Convert::charToWchar((LPSTR)wstr_sqlError.c_str());
//        wsprintf(msg, L"selectMovieId : %s", BUTIL::Convert::charToWchar((LPSTR)wstr_sqlError.c_str()));
//        MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);

        exLOGERROR(e.ErrText());
        ret = false;
    }

    return ret;
}
bool DataBase::getMovieData(MovieFile *_file)
{
    //Movie *movie = _file->getMovie();
    bool ret = getMovieData(_file->getMovie());
    if (ret)
    {
        _file->imdbId = _file->getMovie()->getImdbId();
        _file->imdbRating = _file->getMovie()->getImdbRating();
        _file->title = _file->getMovie()->getTitleASCII();

        for (int type = TYPE_SERIE; type < TYPE_MAX; type++)
        {
            if (_file->typeIs[type] == TYPE_UNKNOWN)
                _file->typeIs[type] = _file->getMovie()->getTypeIs((movieInfoTypes)type) ? TYPE_NOTMOVIE : TYPE_MOVIE;
        }

        //if (_file->typeIs[TYPE_ANIM] == FILTER_UNKNOWN)
        //    _file->typeIs[TYPE_ANIM] = _file->getMovie()->getTypeIs(TYPE_ANIM)? FILTER_NOTMOVIE : FILTER_MOVIE;
        //if (_file->typeIs[TYPE_SERIE] == FILTER_UNKNOWN)
        //    _file->typeIs[TYPE_SERIE] = _file->getMovie()->getTypeIs(TYPE_SERIE) ? FILTER_NOTMOVIE : FILTER_MOVIE;
        //if (_file->typeIs[TYPE_DOC] == FILTER_UNKNOWN)
        //    _file->typeIs[TYPE_DOC] = _file->getMovie()->getTypeIs(TYPE_DOC) ? FILTER_NOTMOVIE : FILTER_MOVIE;
        //if (_file->typeIs[TYPE_SHORT] == FILTER_UNKNOWN)
        //    _file->typeIs[TYPE_SHORT] = _file->getMovie()->getTypeIs(TYPE_SHORT) ? FILTER_NOTMOVIE : FILTER_MOVIE;
    }
    return ret;
}
bool DataBase::getMovieData(Movie *_movie)
{
    bool ret = MOVIES_get(_movie);
    if (ret == true)
    {
        GENRES_getGenre(_movie);
        PERSONS_getPerson(_movie);
    }
    return ret;
}
bool DataBase::MOVIES_get(Movie *_movie)
{
    std::string tmpDate;
    std::string request;
    //SACommand cursorMovies;
    bool ret = false;

    if (_movie->getIdMovie() > 0)
    {
        request = SELECTALL + FROM(TAB_MOVIES) + WHERE(EQUAL(MOVIES_IDMOVIE, STR(_movie->getIdMovie())));
        try
        {

            SQLEXEC;
            if (SQLFETCH)
            {
                _movie->setIdMovie(SQLFIELD(MOVIES_IDMOVIE).asShort());
                _movie->setTitle(SQLFIELD(MOVIES_TITLE).asString());
                _movie->setYear(SQLFIELD(MOVIES_YEAR).asShort());
                _movie->setRated(SQLFIELD(MOVIES_RATED).asString());
                _movie->setRuntime(SQLFIELD(MOVIES_RUNTIME).asString());
                _movie->setPlot(SQLFIELD(MOVIES_PLOT).asString());
                _movie->setLanguage(SQLFIELD(MOVIES_LANGUAGE).asString());
                _movie->setCountry(SQLFIELD(MOVIES_COUNTRY).asString());
                _movie->setAwards(SQLFIELD(MOVIES_AWARD).asString());
                _movie->setPoster(SQLFIELD(MOVIES_POSTER).asString());
                _movie->setMetascore(SQLFIELD(MOVIES_METASCORE).asShort());
                _movie->setImdbRating(SQLFIELD(MOVIES_IMDBRATING).asShort());
                _movie->setImdbId(SQLFIELD(MOVIES_IMDBID).asString());
                _movie->setImdbVotes(SQLFIELD(MOVIES_IMDBVOTES).asShort());
                _movie->setType(SQLFIELD(MOVIES_TYPE).asString());

                ret = true;
            }

            SQLCLOSE;
            //unlock();
        }
        catch (SAException &e)
        {
            __debugbreak();
            EXECPCIONMBOX(e);
//            wchar_t msg[256];
//            std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
//            wsprintf(msg, L"getMovie : %s", BUTIL::Convert::charToWchar((LPSTR)sqlError.c_str()));
//            __debugbreak();
//            MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
            exLOGERROR("SQL ko : %s", request.c_str());
            exLOGERROR(e.ErrText());
            return false;
        }
        exLOGSQL("SQL ok : %s", request.c_str());
    }

    return ret;
}

bool DataBase::deleteAllMoviesData(void)
{
    bool ret = true;
    TRANSACTION_begin();
    if (MOVIES_deleteAll() == false)
        ret = false;
    if (GENREINMOVIE_deleteAll() == false)
        ret = false;
    if (PERSONINMOVIE_deleteAll() == false)
        ret = false;
    TRANSACTION_commit();
    return ret;
}
bool DataBase::MOVIES_deleteAll(void)
{
    bool ret = false;
    std::string request = DELETE(TAB_MOVIES);
    return (execNolock(request) == ERROR_SUCCESS);
}

#pragma endregion 

#pragma region TAB_PERSONS_GENRES_ROLES

string DataBase::buildComboFilter(filterTypes _filterby)
{
    string comboFilter;
    string selectIds, personFilter[ROLES_MAX], genreFilter, genre2Filter;
    string serieFilter, animFilter, docFilter, shortFilter;
    string yearMin, yearMax, imdbRatingMin, imdbRatingMax;

    for (short role = ROLES_DIRECTOR; role < ROLES_MAX; role++)
    {
        selectIds = SELECT(PERSONINMOVIE_IDMOVIE) + FROM(TAB_PERSONINMOVIE);
        selectIds += WHERE(EQUAL(PERSONINMOVIE_IDPERSON, STR(filters.valueSelected[role])));
        selectIds += AND(EQUAL(PERSONINMOVIE_IDROLE, STR(role)));

        short invertF = (role == ROLES_DIRECTOR) ? INVERT_FILTERS_DIRECTORS : INVERT_FILTERS_ACTORS;
        if (filters.invertFilters[invertF] == false)
            personFilter[role] = INSELECT(MOVIES_IDMOVIE, selectIds);
        else
            personFilter[role] = NOTINSELECT(MOVIES_IDMOVIE, selectIds);

        if ((_filterby != role) && (filters.valueSelected[role] >= 0))
            comboFilter += AND(personFilter[role]);
        if (_filterby == role)
            comboFilter += AND(EQUAL(PERSONINMOVIE_IDROLE, STR(role)));
    }
    selectIds = SELECT(GENREINMOVIE_IDMOVIE) + FROM(TAB_GENREINMOVIE) + WHERE(EQUAL(GENREINMOVIE_IDGENRE, STR(filters.valueSelected[FILTERS_GENRES])));
    if (filters.invertFilters[INVERT_FILTERS_GENRES] == false)
        genreFilter = INSELECT(MOVIES_IDMOVIE, selectIds);
    else
        genreFilter = NOTINSELECT(MOVIES_IDMOVIE, selectIds);

    selectIds = SELECT(GENREINMOVIE_IDMOVIE) + FROM(TAB_GENREINMOVIE) + WHERE(EQUAL(GENREINMOVIE_IDGENRE, STR(filters.valueSelected[FILTERS_GENRES2])));
    if (filters.invertFilters[INVERT_FILTERS_GENRES2] == false)
        genre2Filter = INSELECT(MOVIES_IDMOVIE, selectIds);
    else
        genre2Filter = NOTINSELECT(MOVIES_IDMOVIE, selectIds);

    serieFilter = EQUAL(FS_SERIE, STR(filters.valueSelected[FILTERS_SERIE]));
    animFilter = EQUAL(FS_ANIM, STR(filters.valueSelected[FILTERS_ANIM]));
    docFilter = EQUAL(FS_DOC, STR(filters.valueSelected[FILTERS_DOC]));
    shortFilter = EQUAL(FS_SHORT, STR(filters.valueSelected[FILTERS_SHORT]));
    yearMin = GREATER(MOVIES_YEAR, filters.valueSelected[FILTERS_YEARMIN]);
    yearMax = LESSER(MOVIES_YEAR, filters.valueSelected[FILTERS_YEARMAX]);
    imdbRatingMin = GREATER(MOVIES_IMDBRATING, filters.valueSelected[FILTERS_IMDBRATINGMIN]);
    imdbRatingMax = LESSER(MOVIES_IMDBRATING, filters.valueSelected[FILTERS_IMDBRATINGMAX]);
    if ((_filterby != FILTERS_GENRES) && (filters.valueSelected[FILTERS_GENRES] >= 0))
        comboFilter += AND(genreFilter);
    if ((_filterby != FILTERS_GENRES2) && (filters.valueSelected[FILTERS_GENRES2] >= 0))
        comboFilter += AND(genre2Filter);

    if ((_filterby != FILTERS_SERIE) && filters.valueSelected[FILTERS_SERIE] < TYPE_ALL)
        comboFilter += AND(serieFilter);
    if ((_filterby != FILTERS_ANIM) && filters.valueSelected[FILTERS_ANIM]  < TYPE_ALL)
        comboFilter += AND(animFilter);
    if ((_filterby != FILTERS_DOC) && filters.valueSelected[FILTERS_DOC]  < TYPE_ALL)
        comboFilter += AND(docFilter);
    if ((_filterby != FILTERS_SHORT) && filters.valueSelected[FILTERS_SHORT]  < TYPE_ALL)
        comboFilter += AND(shortFilter);

    if (_filterby != FILTERS_YEARMIN && filters.valueSelected[FILTERS_YEARMIN] >= 0)
        comboFilter += AND(yearMin);
    if (_filterby != FILTERS_YEARMAX && filters.valueSelected[FILTERS_YEARMAX] >= 0)
        comboFilter += AND(yearMax);

    if (_filterby != FILTERS_IMDBRATINGMIN && filters.valueSelected[FILTERS_IMDBRATINGMIN] >= 0)
        comboFilter += AND(imdbRatingMin);
    if (_filterby != FILTERS_IMDBRATINGMAX && filters.valueSelected[FILTERS_IMDBRATINGMAX] >= 0)
        comboFilter += AND(imdbRatingMax);


    return comboFilter;
}
string DataBase::buildJoinFilter(filterTypes _filterby)
{
    string join, joinMovies, joinGenreInMovie, joinPersonInMovie;
    joinMovies = JOINUSING(TAB_MOVIES, MOVIES_IMDBID);
    joinGenreInMovie = JOINUSING(TAB_GENREINMOVIE, GENREINMOVIE_IDMOVIE);
    joinGenreInMovie += JOINUSING(TAB_GENRES, GENRES_ID);
    joinPersonInMovie = JOINUSING(TAB_PERSONINMOVIE, PERSONINMOVIE_IDMOVIE);
    joinPersonInMovie += JOINUSING(TAB_PERSONS, PERSONS_ID);

    join = joinMovies;

    if (_filterby == FILTERS_DIRECTORS || _filterby == FILTERS_WRITERS || _filterby == FILTERS_ACTORS) 
        join += joinPersonInMovie;
    if (_filterby == FILTERS_GENRES || _filterby == FILTERS_GENRES2)
        join += joinGenreInMovie;

    return join;
}
short DataBase::getTabColFilters(filterTypes _filterby)
{
    switch (_filterby)
    {
    case FILTERS_DIRECTORS:
    case FILTERS_WRITERS:
    case FILTERS_ACTORS:
       return PERSONS_NAME;
        break;
    case FILTERS_GENRES:
    case FILTERS_GENRES2:
        return GENRES_GENRE;
        break;
    case FILTERS_SERIE:
        return FS_SERIE;
        break;
    case FILTERS_ANIM:
        return FS_ANIM;
        break;
    case FILTERS_DOC:
        return FS_DOC;
        break;
    case FILTERS_SHORT:
        return FS_SHORT;
        break;
    case FILTERS_YEARMIN:
    case FILTERS_YEARMAX:
        return MOVIES_YEAR;
        break;
    case FILTERS_IMDBRATINGMIN:
    case FILTERS_IMDBRATINGMAX:
        return MOVIES_IMDBRATING;
        break;
    default:
        return -1;
        break;
    }
    return -1;
}
int  DataBase::loadFilter(filterTypes _filterby)
{
    if (!connected)
        return -1;

    enum { TAB, COL, TABCOL };
    enum { COLVAL = 1, COLCOUNT };

    short fTabCol;
    int count = 0, aggregate = 0;
    int orderByCol = COLCOUNT;
    bool orderByDesc = true;
    std::string tmp;

    string request;
    string select, from, join, conditions, orderby, goupeby;
    string existsFilter, filesExist, imdbIdFilter;// , ForceFileExistsFilter;

    fTabCol = getTabColFilters(_filterby);


    existsFilter = EQUAL(FS_PHYSICAL, STR(filters.exists));
    imdbIdFilter = NOTEMPTY(FS_IMDBID);


    if (_filterby == FILTERS_YEARMIN || _filterby == FILTERS_IMDBRATINGMIN)
    {
        orderByCol = COLVAL;
        orderByDesc = true;
    }
    if (_filterby == FILTERS_YEARMAX || _filterby == FILTERS_IMDBRATINGMAX)
    {
        orderByCol = COLVAL;
        orderByDesc = false;
    }

    select = SELECT(fTabCol) + COUNTCOL(fTabCol);
    from = FROM(TAB_MOVIESFS);
    join = buildJoinFilter(_filterby);

    //string folderPath = *filters.folderFilter + "%";
    //BUTIL::Convert::string2sqlLike(&folderPath);
    //string folderList = SELECT(PATH_ID) + FROM(TAB_PATHFS) + WHERE(LIKE(PATH_PATH, folderPath));
    string folderList = SELECT(PATH_ID) + FROM(TAB_PATHFS) + WHERE(EQUAL(FS_IDPATH, STR(filters.folderFilterId)));

    string folderFilter = INSELECT(FS_IDPATH, folderList);
    //string conditions = WHERE(folderFilter);

    conditions = WHERE(imdbIdFilter) + AND(folderFilter) + buildComboFilter(_filterby);
    if (filters.exists)
        conditions += AND(existsFilter);
    string order = filters.orderByAsc ? ORDERBYDSC : ORDERBYASC;
    orderby = ORDERBY(orderByCol, order);
    if (ftype[_filterby] != GENRE_COMBOT)
    {
        goupeby = GROUPBY(fTabCol, filters.minOccurrences - 1);
        //replace(moviesFS.serie, '1', '2')
    }
    else
    {
        goupeby = GROUPBY12(fTabCol, filters.minOccurrences - 1);
    }
    

    request = select + from + join + conditions + goupeby + orderby;

    filters.v_combos[_filterby].clear();
    filters.v_combos[_filterby].push_back(ALL);

    try
    {
        aggregate = 0;
        SQLEXEC;
        while (SQLFETCH)
        {
            if (ftype[_filterby] != GENRE_COMBOT)
                tmp = SQLFIELDNAME(COLVAL).asString();
            else
            {
                short val = SQLFIELDNAME(COLVAL).asShort();
                tmp = (val== TYPE_MOVIE || val== TYPE_UNKNOWN) ? "No" : "Yes";
            }
            count = SQLFIELDNAME(COLCOUNT).asShort();
            aggregate += count;
            if (_filterby == FILTERS_IMDBRATINGMIN || _filterby == FILTERS_IMDBRATINGMAX)
            {
                if (tmp.size() == 2)
                    tmp.insert(1, ".");
            }
            tmp += " (";
            if (ftype[_filterby] == NORM_COMBOT || ftype[_filterby] == GENRE_COMBOT)
                tmp += std::to_string(count);
            else
                tmp += std::to_string(aggregate);
            tmp += ")";
            

            filters.v_combos[_filterby].push_back(tmp);
        }
        SQLCLOSE;
    }
    catch (SAException &e)
    {
        __debugbreak();
        //EXECPCIONMBOX(e);
        ERRORMBOX(TABLE(TABID(fTabCol)) + std::string(":") + std::string(e.ErrText()));
        //SQLCLOSE;
        //wchar_t msg[256];
        //std::string sqlError = (std::string)e.ErrText();
        //const char* c_sqlError = sqlError.c_str();
        //wsprintf(msg, L"loadFilter %s : %s", BUTIL::Convert::charToWchar((LPSTR)TABLE(TABID(fTabCol))), BUTIL::Convert::charToWchar((LPSTR)c_sqlError));
        //__debugbreak();
        //MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        exLOGERROR("SQL ko : %s", request.c_str());
        exLOGERROR(e.ErrText());
        return -1;
    }
    exLOGSQL("SQL ok : %s", request.c_str());
    return count;
}
int DataBase::GENRES_getId(std::string _genre)
{// ret 0 if correct / GetLastError()
    return getDefTableId(TAB_GENRES, _genre);
}
int DataBase::ROLES_getId(std::string _role)
{// ret 0 if correct / GetLastError()
    return getDefTableId(TAB_ROLES, _role);
}
int DataBase::PERSONS_getId(std::string _person)
{// ret 0 if correct / GetLastError()
    return getDefTableId(TAB_PERSONS, _person);
}
int DataBase::getDefTableId(int _table, std::string _value)
{// ret 0 if correct / GetLastError()
    int ret = -1;
    std::string request;
    short tabColId = TABCOL(_table,DEFTABS_ID);
    short tabColValue = TABCOL(_table,DEFTABS_VALUE);
    try
    {
        if (_value.size() != 0)
        {

            size_t open = _value.find_first_of("(");
            if (open != std::string::npos)
            {
                _value.erase(open - 1);
            }
            str2sql(&_value);
            request = SELECT(tabColId) + FROM(_table) + WHERE(EQUAL(tabColValue, _value)) ;

            SQLEXEC;
            if (SQLFETCH)
            {
                ret = SQLFIELD(tabColId).asShort();
            }
            SQLCLOSE;
        }
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);
//        ERRORMBOX(TABLE(TABID(fTabCol)) + std::string(":") + std::string(e.ErrText()));
//        wchar_t msg[256];
//        std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
//        wsprintf(msg, L"getDefTableId : %s", BUTIL::Convert::charToWchar((LPSTR)sqlError.c_str()));
//        __debugbreak();
//        MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        exLOGERROR("SQL ko : %s", request.c_str());
        exLOGERROR(e.ErrText());
        return false;
    }
    exLOGSQL("SQL ok : %s", request.c_str());
    return ret;
}
std::string DataBase::getDefTableValue(int _table, int _id)
{// ret 0 if correct / GetLastError()
    int ret = -1;
    std::string value;
    std::string request;
    short tabColId = TABCOL(_table,DEFTABS_ID);
    short tabColValue = TABCOL(_table,DEFTABS_VALUE);
    try
    {
        if (_id >= 0)
        {
            request = SELECT(tabColValue) + FROM(_table) + WHERE(EQUAL(tabColId, STR(_id)));

            SQLEXEC;
            if (SQLFETCH)
            {
                value = SQLFIELD(tabColValue).asString();
            }
            SQLCLOSE;
        }
    }
    catch (SAException &e)
    {
        __debugbreak();
        EXECPCIONMBOX(e);
//        ERRORMBOX(TABLE(TABID(fTabCol)) + std::string(":") + std::string(e.ErrText()));
//        wchar_t msg[256];
//        std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
//        wsprintf(msg, L"getDefTableId : %s", BUTIL::Convert::charToWchar((LPSTR)sqlError.c_str()));
//        __debugbreak();
//        MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        exLOGERROR("SQL ko : %s", request.c_str());
        exLOGERROR(e.ErrText());
        return value;
    }
    exLOGSQL("SQL ok : %s", request.c_str());
    return value;
}
bool DataBase::GENRES_getGenre(Movie *_movie)
{
    std::string request;
    bool ret = false;

    if (_movie->getIdMovie() >= 0)
    {
        request = SELECT(GENRES_GENRE) + FROM(TAB_GENRES);
        request += JOINUSING(TAB_GENREINMOVIE, GENRES_ID);
        request += WHERE(EQUAL(GENREINMOVIE_IDMOVIE, STR(_movie->getIdMovie())));
        request += ORDERBY(GENRES_GENRE, ORDERBYASC);

        try
        {
            SQLEXEC;

            std::vector<std::string> v_genres;

            while (SQLFETCH)
            {
                ret = true;
                v_genres.push_back((std::string)SQLFIELD(GENRES_GENRE).asString());
            }

            _movie->setGenres(v_genres);


            SQLCLOSE;
            //unlock();
        }
        catch (SAException &e)
        {
            __debugbreak();
            EXECPCIONMBOX(e);
//            ERRORMBOX(TABLE(TABID(fTabCol)) + std::string(":") + std::string(e.ErrText()));
//            wchar_t msg[256];
//            std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
//            std::wstring wRequest = BUTIL::Convert::string2wstring(request);
//            wsprintf(msg, L"getGenres : %s (%s)", (LPCWSTR)sqlError.c_str(), (LPCWSTR)wRequest.c_str());
//            __debugbreak();
//            MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
            exLOGERROR("SQL ko : %s", request.c_str());
            exLOGERROR(e.ErrText());
            return false;
        }
        exLOGSQL("SQL ok : %s", request.c_str());
    }
    return ret;
}
bool DataBase::PERSONS_getPerson(Movie *_movie)
{
    std::string request;
    bool ret = false;
    short roleId;

    if (_movie->getIdMovie() >= 0)
    {
        std::vector<std::string> v_persons;

        for (roleId = ROLES_DIRECTOR; roleId < ROLES_MAX; roleId++)
        {
            request = SELECT(PERSONS_NAME) + FROM(TAB_PERSONS);
            request += JOINUSING(TAB_PERSONINMOVIE, PERSONS_ID);
            request += WHERE(EQUAL(PERSONINMOVIE_IDMOVIE, STR(_movie->getIdMovie())));
            request += AND(EQUAL(PERSONINMOVIE_IDROLE, STR(roleId)));
            request += ORDERBY(PERSONS_NAME, ORDERBYASC);

            v_persons.clear();

            try
            {
                SQLEXEC;
                while (SQLFETCH)
                {
                    ret = true;
                    v_persons.push_back((std::string)SQLFIELD(PERSONS_NAME).asString());
                }
                SQLCLOSE;
            }
            catch (SAException &e)
            {
                __debugbreak();
                EXECPCIONMBOX(e);
//                ERRORMBOX(TABLE(TABID(fTabCol)) + std::string(":") + std::string(e.ErrText()));
//                wchar_t msg[256];
//                std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
//                wsprintf(msg, L"getPersons : %s", BUTIL::Convert::charToWchar((LPSTR)sqlError.c_str()));
//                __debugbreak();
//                MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
                exLOGERROR("SQL ko : %s", request.c_str());
                exLOGERROR(e.ErrText());
                return false;
            }
            _movie->setRoles(v_persons, (ROLES_PERSONS)roleId);

        }
        //unlock();

        exLOGSQL("SQL ok : %s", request.c_str());
    }
    return ret;
}
int DataBase::insertInDefTabs(int _table, std::string _value)
{
    int ret = -1;
    if (_table<TAB_PERSONS || _table>TAB_ROLES)
        return ret;
    string request;
    str2sql(&_value);
    short tabColId = TABCOL(_table, DEFTABS_VALUE);

    request = COUNTFROM(_table) + WHERE(EQUAL(tabColId, _value));
    ret = execScalar(request);
    if (ret >= 0)
        return ERROR_SUCCESS;

    request = INSERTINTO(_table) + INSERTCOLS(tabColId) + INSERTVALS(_value.c_str());

    ret = execNolock(request);
    exLOGDB("SQL %s insert into %s : %s", ret== ERROR_SUCCESS ? "ok" : "ko", TABLE(_table).c_str(), _value.c_str());
    return ret;
}
int DataBase::GENRES_insert(std::string _genre)
{// ret 0 if correct / GetLastError()
    std::string request;
    int ret;
    str2sql(&_genre);

    request = COUNTFROM(TAB_GENRES) + WHERE(EQUAL(GENRES_GENRE, _genre));
    ret = execScalar(request);
    if (ret >= 0)
        return ERROR_SUCCESS;

    request = INSERTINTO(TAB_GENRES) + INSERTCOLS(GENRES_GENRE) + INSERTVALS(_genre);

    ret = execNolock(request);
    exLOGDB("SQL %s insert into %s : %s", ret == ERROR_SUCCESS ? "ok" : "ko", TABLE(TAB_GENRES).c_str(), _genre.c_str());

    return ret;
}
int DataBase::PERSONINMOVIE_insert(int _movieId, int _personId, int _roleId)
{
    int ret = -1;
    std::string request;

    request =  COUNTFROM(TAB_PERSONINMOVIE) + WHERE(EQUAL(PERSONINMOVIE_IDMOVIE, STR(_movieId)));
    request += AND(EQUAL(PERSONINMOVIE_IDPERSON, STR(_personId))) + AND(EQUAL(PERSONINMOVIE_IDROLE, STR(_roleId)));

    ret = execScalar(request);

    if (ret >= 0)
        return ERROR_SUCCESS;

    request = INSERTINTO(TAB_PERSONINMOVIE);
    request += INSERTCOLS(PERSONINMOVIE_IDMOVIE, PERSONINMOVIE_IDPERSON, PERSONINMOVIE_IDROLE);
    request += INSERTVALS(STR(_movieId), STR(_personId), STR(_roleId));

    return execNolock(request);
}
int DataBase::GENREINMOVIE_insert(int _movieId, int _genreId)
{// ret 0 if correct / GetLastError()
    int ret = -1;

    std::string request;

    request = COUNTFROM(TAB_GENREINMOVIE) + WHERE(EQUAL(GENREINMOVIE_IDMOVIE, STR(_movieId))) + AND(EQUAL(GENREINMOVIE_IDGENRE, STR(_genreId)));
    ret = execScalar(request);
    if (ret >= 0)
        return ERROR_SUCCESS;

    request = INSERTINTO(TAB_GENREINMOVIE);
    request += INSERTCOLS(GENREINMOVIE_IDMOVIE, GENREINMOVIE_IDGENRE);
    request += INSERTVALS(STR(_movieId), STR(_genreId));

    return execNolock(request);
}
int DataBase::insertGenresInMovies(Movie *_movie)
{// ret : inserted genres / -1 if error
    int count = -1;
    if (_movie->getIdMovie())
    {
        int movieId = _movie->getIdMovie();
        std::vector<std::string> *ptr_v_genres = _movie->getvGenres();
        //if (ST_DIFF(ST_READING_FILE))
        //    begin();
        for (std::vector<std::string>::iterator it = ptr_v_genres->begin(); it != ptr_v_genres->end(); ++it)
        {
            int genreId = GENRES_getId(*it);
            if (genreId < 0)
            {
                insertInDefTabs(TAB_GENRES, *it);
                genreId = GENRES_getId(*it);
            }
            if (genreId >= 0) 
            {
                GENREINMOVIE_insert(movieId, genreId);
                if (count == -1)
                    count = 1;
                else
                    count++;
            }
        }
        //if (ST_DIFF(ST_READING_FILE))
        //    commit();
    }
    //exLOGDB("SQL %d insertions into %s", count, TABLE(TAB_GENREINMOVIE));

    return 0;
}
int DataBase::insertPersonsInMovies(Movie *_movie)
{// ret : inserted genres / -1 if error
    int count = -1;

    if (_movie->getIdMovie() >= 0)
    {
        std::vector<std::string> * v_persons;
        //if (ST_DIFF(ST_READING_FILE))
        //    begin();
        for (short roleId = ROLES_DIRECTOR; roleId < ROLES_MAX; roleId++)
        {
            v_persons = _movie->getvRoles((ROLES_PERSONS)roleId);

            for (std::vector<std::string>::iterator it = v_persons->begin(); it != v_persons->end(); ++it)
            {
                int personId = PERSONS_getId(*it);
                if (personId < 0)
                {
                    insertInDefTabs(TAB_PERSONS, *it);
                    personId = PERSONS_getId(*it);
                }

                if (personId >= 0)
                {
                    PERSONINMOVIE_insert(_movie->getIdMovie(), personId, roleId);
                    if (count == -1)
                        count = 1;
                    else
                        count++;
                }
            }
        }
        //if (ST_DIFF(ST_READING_FILE))
        //    commit();

    }
    //exLOGDB("SQL %d insertions into %s", count, TABLE(PERSONINMOVIE));

    return count;
}
void DataBase::setFilterSelectedValue(filterTypes _filter, string _value)
{
    size_t found = _value.find_first_of("(");
    string tmp = _value.substr(0, found);
    filters.nameSelected[_filter] = tmp;
    int id=0;
    char first;
    switch (_filter)
    {
    case FILTERS_DIRECTORS:
    case FILTERS_WRITERS:
    case FILTERS_ACTORS:
        id = PERSONS_getId(_value);
        break;
    case FILTERS_GENRES:
    case FILTERS_GENRES2:
        id = GENRES_getId(_value);
        break;
    case FILTERS_SERIE:
    case FILTERS_ANIM:
    case FILTERS_DOC:
    case FILTERS_SHORT:
        first = _value[0];
        switch (first)
        {
        case 'Y': //Yes
            id = TYPE_NOTMOVIE;
            break;
        case 'N': //No
            id = TYPE_MOVIE;
            break;
        default: //ALL
            id = TYPE_ALL;
        }
        break;
    case FILTERS_YEARMAX:
    case FILTERS_YEARMIN:
        id = atoi(tmp.c_str());
        if (id == 0) id = -1;
        break;
    case FILTERS_IMDBRATINGMAX:
    case FILTERS_IMDBRATINGMIN:
        if (tmp.empty() == false && tmp[1] == '.')
            tmp.erase(1, 1);
        id = atoi(tmp.c_str());
        if (id == 0) id = -1;
        break;
    }
    filters.valueSelected[_filter] = id;
}
bool DataBase::PERSONS_deleteAll(void)
{
    return false;
}
bool DataBase::PERSONINMOVIE_deleteAll(void)
{
    exLOGDEBUG("");
    bool ret = false;

    std::string request = DELETE(TAB_PERSONINMOVIE);
    return (execNolock(request) == ERROR_SUCCESS);
}
bool DataBase::GENRES_deleteAll(void)
{
    return false;
}
bool DataBase::GENREINMOVIE_deleteAll(void)
{
    exLOGDEBUG("");
    bool ret = false;

    std::string request = DELETE(TAB_GENREINMOVIE);
    ret = (execNolock(request) == ERROR_SUCCESS);
    return ret;
}
#pragma endregion TAB_PERSONS_GENRES_ROLES


