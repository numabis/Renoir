#include "bfminidump.h"
//#include "util/strutils.h"
//#include "util/timeinfo.h"
//#include "util/bffile.h"

//#include <QtCore\qdatetime.h>
//#include <QtCore\qstring.h>
//#include <qfile.h>
//#include <qtextstream.h>
//#include <QtCore\qdir.h>
#include "TimeStamp.h"
#include "Util.h"

#pragma warning( disable : 4100 ) // unreferenced formal parameter
#pragma warning( disable: 4091 4229 )

#include <locale>
#include <codecvt>

#include <windows.h>
#include <tchar.h>
#include <dbghelp.h>
#include <crtdbg.h>

#pragma comment ( lib, "dbghelp.lib" )

namespace minidump
{

static const String NAME_FILE = "ClienteBED";
static const String NAME_EXTENSION = ".dmp";
static const String NAME_FOLDER = "dmp/";
static LONG WINAPI CreateMiniDump(EXCEPTION_POINTERS* pep);
static MINIDUMP_TYPE getMinDumpType(const MiniDumpType _type);
static bool getCallbackInformation(const MiniDumpType _type, MINIDUMP_CALLBACK_INFORMATION &mci);
static BOOL CALLBACK MiniDumpCallbackCustomRelease(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput);
static bool IsDataSectionNeeded(const WCHAR* pModuleName);

struct sMiniDumpConfig
{
    String filePath;
    MiniDumpType type;

    sMiniDumpConfig()
        : filePath(),
          type(Normal)
    {

    }

    sMiniDumpConfig(const String& _filePath, MiniDumpType _type)
        : filePath(_filePath)
        , type(_type)
    {
    }

    sMiniDumpConfig(const sMiniDumpConfig &p_copy)
        : filePath(p_copy.filePath)
        , type(p_copy.type)
    {
    }

};

static sMiniDumpConfig DumpConfig;

eResult Configure(const String& _filePath, MiniDumpType _type)
{
    eResult result = eResult_Fail;
    String filePath = _filePath;
    //StrUtils::NormalizePath(filePath);
    //filePath += NAME_FOLDER;

    //if (BFFile::CreateDirectoryRecursive(filePath) == eResult_Ok)
    {
        DumpConfig.filePath = filePath + NAME_FILE;
        DumpConfig.type = _type;
        SetUnhandledExceptionFilter(CreateMiniDump);
        result = eResult_Ok;
    }

    return result;

}

void Reset()
{
    SetUnhandledExceptionFilter(nullptr);
}

static LONG WINAPI CreateMiniDump(EXCEPTION_POINTERS* pep)
{
	TimeStamp sesion = TimeStamp::now();
	std::wstring ssesion = BUTIL::Util::formatW(L"%04d%02d%02dT%02d%02d%02d", sesion.getYear(), sesion.getMonth(), sesion.getDay(), sesion.getHour(), sesion.getMinute(), sesion.getSecond());

	std::wstring fileName = BUTIL::Util::ToWideString(DumpConfig.filePath);
	fileName += ssesion;
	fileName += L".dmp";

		// Open the file 
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
    {
        // Create the minidump 
        MINIDUMP_EXCEPTION_INFORMATION mdei;

        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = pep;
        mdei.ClientPointers = FALSE;

        MINIDUMP_CALLBACK_INFORMATION mci;
        PMINIDUMP_CALLBACK_INFORMATION pmci = 0;
        if ( getCallbackInformation(DumpConfig.type, mci) )
        {
            pmci = &mci;
        }

        MINIDUMP_TYPE mdt = getMinDumpType(DumpConfig.type);

        BOOL rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
            hFile, mdt, (pep != 0) ? &mdei : 0, 0, pmci);

		/*
        if (!rv)
            DebugLog(_T("MiniDumpWriteDump failed. Error: %u \n"), GetLastError());
        else
            DebugLog(_T("Minidump created.\n"));
		*/

        // Close the file 
        CloseHandle(hFile);

    }
    else
    {
        //DebugLog(_T("CreateFile failed. Error: %u \n"), GetLastError());
    }

#if ALLOW_DEBUG_TOOLS==1
    return EXCEPTION_CONTINUE_SEARCH;
#else
    return EXCEPTION_EXECUTE_HANDLER;
#endif

}

MINIDUMP_TYPE getMinDumpType(const MiniDumpType _type)
{
    MINIDUMP_TYPE result = MiniDumpNormal;
    switch (_type)
    {
        case Normal:
        {
            result = (MINIDUMP_TYPE) (MiniDumpNormal | MiniDumpFilterModulePaths);
        }
        break;
        case MaxiDump:
        {
            result = (MINIDUMP_TYPE) (MiniDumpWithFullMemory | MiniDumpWithFullMemoryInfo | MiniDumpWithHandleData | MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules | MiniDumpFilterModulePaths);
        }
        break;
        case CustomRelease:
        {
            result = (MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory | MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo | MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules | MiniDumpFilterModulePaths);
        }
        break;
    }
    return result;
}

bool getCallbackInformation(const MiniDumpType _type, MINIDUMP_CALLBACK_INFORMATION &mci) 
{
    bool result = false;
    switch (_type)
    {
        case Normal:
        {
            result = false;
        }
        break;
        case MaxiDump:
        {
            result = false;
        }
        break;
        case CustomRelease:
        {
            result = true;
            mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallbackCustomRelease;
            mci.CallbackParam = 0;
        }
        break;
    }

    return result;
}

BOOL CALLBACK MiniDumpCallbackCustomRelease(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput)
{
    BOOL bRet = FALSE;


    // Check parameters 
    if (pInput == 0)
    {
        return FALSE;
    }
    if (pOutput == 0)
    {
        return FALSE;
    }

    // Process the callbacks 
    switch (pInput->CallbackType)
    {
        case IncludeModuleCallback:
        {
            // Include the module into the dump 
            bRet = TRUE;
        }
        break;

        case IncludeThreadCallback:
        {
            // Include the thread into the dump 
            bRet = TRUE;
        }
        break;

        case ModuleCallback:
        {
            // Are data sections available for this module ? 
            if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
            {
                // Yes, they are, but do we need them? 
                if (!IsDataSectionNeeded(pInput->Module.FullPath))
                {
                    pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
                }
            }
            bRet = TRUE;
        }
        break;

        case ThreadCallback:
        {
            // Include all thread information into the minidump 
            bRet = TRUE;
        }
        break;

        case ThreadExCallback:
        {
            // Include this information 
            bRet = TRUE;
        }
        break;

        case MemoryCallback:
        {
            // We do not include any information here -> return FALSE 
            bRet = FALSE;
        }
        break;

        case CancelCallback:
        {
            bRet = FALSE;
        }
        break;

        default:
        {
            bRet = FALSE;
        }
        break;
    }

    return bRet;

}

bool IsDataSectionNeeded(const WCHAR* pModuleName)
{
    // Check parameters 
    if (pModuleName == 0)
    {
        return false;
    }

    // Write here how filter module data section to exclude

    return false;

}

}
