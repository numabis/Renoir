#include <filesystem>
#include "fileSystem.h"
#include "database.h"
#include "Convert.h"
#include "util.h"
#include "Mutex.h"
#include "configManager.h"

fileSystem::fileSystem()
{
    stopThreads = false;
}

fileSystem::~fileSystem(void)
{
}

void fileSystem::init()
{
}

static DWORD WINAPI readFolderThread(void* Param)
{
    fileSystem* This = (fileSystem*)Param;
    return This->readFolderStart();
}

void fileSystem::terminateThreads()
{
    if (isRunning(hThreadReadFolder))
    {
        ST_SET(ST_STANDBY);
        stopThreads = true;
    }
}

bool fileSystem::isRunning(HANDLE  _hThread)
{
    DWORD exitCode = 0;
    if (_hThread)
        GetExitCodeThread(_hThread, &exitCode);
    if (exitCode == STILL_ACTIVE)
        return true;
    return false;
}

HANDLE fileSystem::readFolder()
{
    DWORD  hThreadIdArray;
    stopThreads = false;
    hThreadReadFolder = CreateThread(NULL, 0, readFolderThread, (void*) this, 0, &hThreadIdArray);
    return hThreadReadFolder;
}

std::vector<MovieFile> * fileSystem::getVfiles()
{
    return &v_files;
}

BUTIL::Mutex* fileSystem::getMutex(void)
{
    return &mtx_readfiles;
}

int fileSystem::readFolderStart(ManageXML *_xmlFiles)
{
    ST_SAVE(ST_READING_FODLER)

    namespace fs = std::experimental::filesystem;

    WIN32_FILE_ATTRIBUTE_DATA fInfo;
    SYSTEMTIME        stSystemTime;
    FILETIME* min;
    std::vector<std::string> extList = GETCM.getConfigVect(CONF_FS_EXTENTIONS, ",");
    std::string strType[TYPE_MAX];
    strType[TYPE_SERIE] = GETCM.getConfigStr(CONF_FS_TYPESERIE);
    strType[TYPE_ANIM] = GETCM.getConfigStr(CONF_FS_TYPEANIM);
    strType[TYPE_DOC] = GETCM.getConfigStr(CONF_FS_TYPEDOC);
    strType[TYPE_SHORT] = GETCM.getConfigStr(CONF_FS_TYPESHORT);

    //progressCounter[CNT_READ] = 0;
    //progressCounter[CNT_TOTAL] = 0;
    //progressCounter[CNT_MAX_READ] = 0;

    int localProgressCounter = 0;

    std::string currentFolder = GETCM.getConfigStr(CONF_APP_CURRENTFOLDER);

    //for (const auto & entry : fs::recursive_directory_iterator(currentFolder))
    //{
    //    if (std::find(extList.begin(), extList.end(), entry.path().extension().string()) != extList.end())
    //    {
    //        //progressCounter[CNT_TOTAL]++;
    //        localProgressCounter++;
    //    }
    //}

    //if (GETDB.getPathId(currentFolder) == -1)
    //{
    //    MovieFolder homeFolder(currentFolder, true);
    //    homeFolder.insertDB();
    //}

    GETDB.TRANSACTION_begin();
    for (const auto & entry : fs::recursive_directory_iterator(currentFolder))
    {
        if (stopThreads)
        {
            stopThreads = false;
            break;
        }
        
        if (std::find(extList.begin(), extList.end(), entry.path().extension().string()) != extList.end())
        {
            MovieFile file;
            file.setFilename(entry.path().filename().string());
            file.setPath(entry.path().parent_path().string());
            file.setIsSub(file.pathCompare(currentFolder)!=0);
            file.setIsRead(true);
            file.exists = true;
            //progressCounter[CNT_READ]++;
            localProgressCounter++;

            if (GetFileAttributesExA(entry.path().string().c_str(), GetFileExInfoStandard, &fInfo))
            {
                min = &fInfo.ftCreationTime;
                if (min->dwHighDateTime > fInfo.ftLastAccessTime.dwHighDateTime)
                    min = &fInfo.ftLastAccessTime;
                if (min->dwHighDateTime > fInfo.ftLastWriteTime.dwHighDateTime)
                    min = &fInfo.ftLastWriteTime;
                if (FileTimeToSystemTime(min, &stSystemTime))
                {
                    // YYYY-MM-DD hh:mm:ss.mmm
                    file.dateAdded = BUTIL::Util::format("%04d-%02d-%02dT%02d:%02d", stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute);
                    file.size = (fInfo.nFileSizeHigh + fInfo.nFileSizeLow) / (1024 * 1024);
                }
            }
            else
            {
                int error = GetLastError();
                std::string errorMsg = BUTIL::Util::GetLastErrorAsString();
                exLOGERROR("GetFileAttributesEx Error: %s", errorMsg.c_str());
            }

            if (GETCM.getConfigBool(CONF_FS_SEARCHTYPES))
            {
                for (short type = 0; type < TYPE_MAX; type++)
                {
                    if(file.guessType(strType[type]))
                        file.typeIs[type] = TYPE_NOTMOVIE;
                }
            }
            file.fileNameToDetails();

            int exists = GETDB.MOVIESFS_exists(&file);

            if (exists > 0)
                GETDB.getData(&file);

            file.initMovieValue();
            if (file.isImdbId() == false)
                GETDB.getMovieData(&file);

            if (exists <= 0)
            {
                GETDB.MOVIESFS_insert(&file);
            }
            else
            {
                //GETDB.getData(&file);
                bool fields[FS_MAX_FIELDS - FS_ID];
                for (short i = 0; i < FS_MAX_FIELDS - FS_ID; i++)
                    fields[i] = false;
                fields[FS_PHYSICAL - FS_ID] = true;
                fields[FS_DATEADDED - FS_ID] = true;
                fields[FS_FILESIZE - FS_ID] = true;
                fields[FS_IDPATH - FS_ID] = true;

                for (short type = 0; type < TYPE_MAX; type++)
                {
                    if (file.typeIs[type] != TYPE_UNKNOWN)
                        fields[type + FS_SERIE - FS_ID] = true;
                }

                GETDB.MOVIESFS_update(&file, fields);
            }
            file.dateAdded = file.dateAdded.substr(0, 10);
            mtx_readfiles.lock();
            v_files.push_back(file);
            mtx_readfiles.unlock();
        }
    }
    GETDB.TRANSACTION_commit();
    MovieFolder homeFolder(currentFolder, true);
    homeFolder.updateDB();
    ST_RESTORE();
    //return progressCounter[CNT_READ];
    return localProgressCounter;
}

void fileSystem::setProgressCounter(size_t * _progressCounter)
{
    progressCounter = _progressCounter;
}
