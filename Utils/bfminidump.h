#pragma once

//#pragma warning( disable : 4100 ) // unreferenced formal parameter
#include <string>

typedef std::string String;

enum eResult
{
    eResult_Fail = 0,
    eResult_Ok,
    eResult_InProgress,
    // --- ---
    eResult_Invalid
};


namespace minidump
{
    enum MiniDumpType
    {
        Normal,
        MaxiDump,
        CustomRelease
    };

    eResult Configure(const String& _filePath, MiniDumpType _type);
    void Reset();
}

