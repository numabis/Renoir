#pragma once

#include "definitions.h"
#include "manageXML.h"
#include "thread.h"
//#include <afxwin.h>

/// Soporte acceso base de datos SQLITE

class fileSystem /*: public BUTIL::Thread*/
{

public:

    std::vector<std::string> *ptr_consoleDBG;
    states *ptr_globalState;

    fileSystem();
    ~fileSystem(void);
    void init(FS_CONFIG*);
//    HANDLE countFolder(void);
    HANDLE readFolder(void);
//    int countFolderStart(void);
    void terminateThreads();
    bool isRunning(HANDLE  hThread);
    //    bool isRunningCountFolderThread();
//    int readFolder(ManageXML *_xmlFiles = NULL);
    int readFolderStart(ManageXML *_xmlFiles = NULL);
    std::string getFolderPath();
    void setFolderPath(std::string);
    void setProgressCounter(size_t *);
    BUTIL::Mutex* getMutex(void);
    std::vector<MovieFile> * getVfiles();

private:
    HANDLE  hThreadReadFolder;
    bool stopThreads;
    size_t *progressCounter;
    FS_CONFIG *fsConfig;
    std::vector<MovieFile> v_files;
    BUTIL::Mutex mtx_readfiles;
//    HANDLE  hThread;
};
