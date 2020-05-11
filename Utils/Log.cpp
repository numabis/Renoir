#include "Log.h"
#include "Convert.h"
#include "configManager.h"

#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>

    Log::Log()
	{
        //ptr_logConfig = &logConfig;
        logPath = GETCM.getHomeFolder();
        logLevel = GETCM.getConfigInt(CONF_LOG_LOGLEVEL);;
	}

    Log::~Log()
	{
	}

//    void Log::init()
//    {
//        //logConfig.logPath = GETCM.getHomeFolder();
//        logPath = GETCM.getHomeFolder();
//    }
//
//    void Log::init(LOG_CONFIG *_logConfig)
//    {
//        //ptr_logConfig = _logConfig;
//    }
//
//    void Log::init(std::string _path)
//    {
//        logPath = _path;
//    }

    void Log::setPath( std::string _path )
	{
        logPath = _path;
	}

	void Log::setPath( std::wstring _path )
	{
		setPath(BUTIL::Convert::wstring2string(_path) );
	}

	void Log::setLogLevel( int level )
	{
		if(level >= LOG_DISABLED && level <= LOG_ERROR)
            logLevel = level;
		else
            logLevel = LOG_DISABLED;
	}

	void Log::write(int level, const wchar_t *format, ...)
	{
		wchar_t Linea[1024];
	   
		va_list ap;
		va_start(ap, format);
		wvsprintf(Linea, format, ap);      
		va_end(ap);

		write(level,  std::wstring(Linea) );

	}

    void Log::writeXML(std::string _xml, std::string _pathLog)
    {
        std::ofstream fil;
        fil.open(_pathLog.c_str(), std::ios::app);
        if (fil)
        {
            fil << _xml << std::endl;
            fil.close();
        }
    }

    void Log::write(int level, const char *function, const char *format,...)
	{
		char buf[16384];
		std::ofstream fil;

		// Se comprueba si el log esta activo
		if(logLevel == LOG_DISABLED)
			return;

		// Se comprueba si se debe escribir por nivel.
		if((level == LOG_DEBUG || level == LOG_INFO) && level != logLevel)
			return;

		_tzset();
		time_t ltime = time(NULL);
		struct tm tim;
		
		localtime_s( &tim, &ltime );
		//sprintf_s( buf,sizeof(buf),"log_%04d_%02d_%02d_%02dh.txt",tim.tm_year+1900,tim.tm_mon+1,tim.tm_mday,tim.tm_hour);
		if (level != LOG_XML)
			sprintf_s( buf,sizeof(buf),"log_%04d_%02d_%02d.txt",tim.tm_year+1900,tim.tm_mon+1,tim.tm_hour<8?tim.tm_mday-1:tim.tm_mday);
		else
			sprintf_s( buf,sizeof(buf),"logXML_%04d_%02d_%02d.txt",tim.tm_year+1900,tim.tm_mon+1,tim.tm_hour<8?tim.tm_mday-1:tim.tm_mday);
		//sprintf_s(logBuff,sizeof(logBuff)
		std::string pathLog = logPath + std::string( buf );
		fil.open( pathLog.c_str(), std::ios::app );
		if( fil )
		{
			va_list ap;
			va_start(ap,format);
			vsprintf_s(buf, sizeof(buf), format,ap);
			va_end(ap);
            if (level != LOG_XML)
            {
                fil.fill('0');
                fil.width(2);
                fil << tim.tm_hour;

                fil.width();
                fil << ":";

                fil.width(2);
                fil << tim.tm_min;

                fil.width();
                fil << ":";

                fil.width(2);
                fil << tim.tm_sec;

                fil.width(0);

                fil << " [" << function << "] ";

                //fil << " = ";
            }
			fil << buf << std::endl;
			fil.close();
		}    
	}

	void Log::write2(int level, const char *format,...)
	{
		char buf[16384];
		std::ofstream fil;
	
		// Se comprueba si el log esta activo
		if(logLevel == LOG_DISABLED)
			return;
	
		// Se comprueba si se debe escribir por nivel.
		if((level == LOG_DEBUG || level == LOG_INFO) && level != logLevel)
			return;
	
		_tzset();
		time_t ltime = time(NULL);
		struct tm tim;
		
		localtime_s( &tim, &ltime );
		sprintf_s( buf,sizeof(buf),"log_%04d_%02d_%02d_%02dh.txt",tim.tm_year+1900,tim.tm_mon+1,tim.tm_mday,tim.tm_hour);
		//sprintf_s(logBuff,sizeof(logBuff)
		std::string pathLog = logPath + std::string( buf );
		fil.open( pathLog.c_str(), std::ios::app );
		if( fil )
		{
			va_list ap;
			va_start(ap,format);
			vsprintf_s(buf, sizeof(buf), format,ap);
			va_end(ap);
			
			fil.fill( '0' );
			fil.width( 2 );
			fil << tim.tm_hour;
			
			fil.width();
			fil << ":";
	
			fil.width( 2 );
			fil << tim.tm_min;
	
			fil.width();
			fil << ":";
	
			fil.width( 2 );
			fil << tim.tm_sec;
			
			fil.width(0);
			fil << " ";
			fil << buf << std::endl;
			fil.close();
		}    
	}

	void Log::write(int level, std::string txt)
	{
        write2(level, txt.c_str() );
	}

	void Log::write(int level, std::wstring txt)
	{
		std::string _convert = BUTIL::Convert::wstring2string( txt );
        write2(level,  _convert.c_str() );
	}

