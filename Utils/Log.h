#pragma once

#include <string>
#include "variables.h"

//#define LOGINFO(...)	Log.write(Log::LOG_INFO,  __FUNCTION__, ##__VA_ARGS__);
//#define LOGXML(...)		Log.write(Log::LOG_XML,   __FUNCTION__, ##__VA_ARGS__);
//#define LOGNAME()		Log.write(Log::LOG_INFO,  __FUNCTION__, "");
//#define LOGERROR(...)	Log.write(Log::LOG_ERROR, __FUNCTION__, ##__VA_ARGS__);
//#define LOGDEBUG(...)	Log.write(Log::LOG_DEBUG, __FUNCTION__, ##__VA_ARGS__);

class Log
{

public:

    Log();
    ~Log();
       
	enum 
	{
        LOG_DISABLED,
        LOG_ERROR,
        LOG_INFO,
        LOG_DB,
        LOG_SQL,
        LOG_XML,
        LOG_DEBUG
	};

    void init(LOG_CONFIG *logConfig);
    void init(std::string _path);
    void init();
    void setPath(std::string);
    void setPath( std::wstring );
	void setLogLevel( int = 0 );

	void write(int,  std::string );
	void write2(int, const char *,...);
	void write(int, const wchar_t *format, ...);
    //void writeXML(std::string xml);
    void writeXML(std::string xml, std::string fileName);
    void write(int level, const char *function, const char *format, ...);
    void write(int, std::wstring );

private:

    LOG_CONFIG *ptr_logConfig;
    LOG_CONFIG logConfig;

};
