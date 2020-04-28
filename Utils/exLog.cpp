#include "exLog.h"

#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Convert.h"

namespace BUTIL
{
	/*
	 * Constructor. Solo inicializa la clase Mutex.
	 *      
	 * Inicializa la ruta por defecto a la misma donde esta el cliente
	 * y el log desactivado.          
	 */
	exLog::exLog()
	{
		LOG_path = "./";
		logLevel = exLOG_DISABLED;
	}

	/**
	 * Destructor.
	 */
    exLog::~exLog()
	{
	}

//    void Log::setConfig(LOG_CONFIG *_logConfig)
//    {
//        logConfig = _logConfig;
//    }

	/**
	 * Asigna el path donde guardara los archivos log.
	 *      
	 * @param _path Ruta donde guardar el archivo log.
	 */
	void exLog::setPath( std::string _path )
	{
		lock();
		LOG_path = _path;
		unlock();

	}

	/**
	 * Asigna el path donde guardara los archivos log.
	 *      
	 * @param _path Ruta donde guardar el archivo log.
	 */
	void exLog::setPath( std::wstring _path )
	{
		setPath( Convert::wstring2string(_path) );
	}


	/**
	 * Establece el nivel de profundidad del log.
	 *      
	 * @param _estado true para activar la escritura fisica.
	 */
	void exLog::setLogLevel( int level )
	{
		if(level >= exLOG_DISABLED && level < exLOG_MAX)
			logLevel = level;
		else
			logLevel = exLOG_DISABLED;
	}

	/**
	 * Escribe una entrada log formateada
	 *      
	 * @param format Formato standar printf
	 */
	void exLog::exlog(int level, const wchar_t *format, ...)
	{
		wchar_t Linea[1024];
	   
		va_list ap;
		va_start(ap, format);
		wvsprintf(Linea, format, ap);      
		va_end(ap);

		exlog(level,  std::wstring(Linea) );

	}

	/**
	 * Escribe una entrada log formateada
	 *      
	 * @param format Formato standar printf
	 */
	void exLog::exlog(int level, const char *function, const char *format,...)
	{
		char buf[16384];
        char filename[20];
		std::ofstream fil;

		// Se comprueba si el log esta activo
		if(logLevel == exLOG_DISABLED)
			return;

		// Se comprueba si se debe escribir por nivel.
		//if((level == exLOG_DEBUG || level == exLOG_INFO) && level != logLevel)
        if (level > logLevel)
			return;

		lock();
		
		_tzset();
		time_t ltime = time(NULL);
		struct tm tim;
		
		localtime_s( &tim, &ltime );
        sprintf_s(filename, sizeof(filename), "%04d_%02d_%02d.txt", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_hour<8 ? tim.tm_mday - 1 : tim.tm_mday);
        switch (level)
        {
        case exLOG_XML:
            sprintf_s(buf, sizeof(buf), "logXML_%s", filename);
            break;
        case exLOG_SQL:
            sprintf_s(buf, sizeof(buf), "logSQL_%s", filename);
            break;
        default:
            sprintf_s(buf, sizeof(buf), "Log_%s", filename);
        }

        std::string pathLog = LOG_path + std::string( buf );
		fil.open( pathLog.c_str(), std::ios::app );
        buf[0] = '\0';
		if( fil )
		{
			va_list ap;
			va_start(ap,format);
			vsprintf_s(buf, sizeof(buf), format,ap);
            if (level != exLOG_SQL)
            {
                if (*buf != '\0')
                    ptr_consoleDBG->push_back(std::string(buf));
                else
                    ptr_consoleDBG->push_back(std::string(function));
            }
			va_end(ap);
            if (level != exLOG_XML)
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
		unlock();

	}

	void exLog::exlog2(int level, const char *format,...)
	{
		char buf[16384];
		std::ofstream fil;
	
		// Se comprueba si el log esta activo
		if(logLevel == exLOG_DISABLED)
			return;
	
		// Se comprueba si se debe escribir por nivel.
		if((level == exLOG_DEBUG || level == exLOG_INFO) && level != logLevel)
			return;
	
		lock();
		
		_tzset();
		time_t ltime = time(NULL);
		struct tm tim;
		
		localtime_s( &tim, &ltime );
		sprintf_s( buf,sizeof(buf),"log_%04d_%02d_%02d_%02dh.txt",tim.tm_year+1900,tim.tm_mon+1,tim.tm_mday,tim.tm_hour);
		//sprintf_s(logBuff,sizeof(logBuff)
		std::string pathLog = LOG_path + std::string( buf );
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
		unlock();
	}

	/**
	 * Escribe una entrada log 
	 *      
	 * @param txt Cadena a imprimir
	 */
	void exLog::exlog(int level, std::string txt)
	{
		exlog2(level, txt.c_str() );
	}

	/**
	 * Escribe una entrada log 
	 *      
	 * @param txt Cadena a imprimir
	 */
	void exLog::exlog(int level, std::wstring txt)
	{
		std::string _convert = Convert::wstring2string( txt );
		exlog2(level,  _convert.c_str() );
	}


	/**
	 * Escribe un volcado de memoria binaria al archivo log en bloques de 16 bytes
	 *      
	 * @param  pt      Puntero buffer memoria
	 * @param  size    Longitud del buffer
	 * @param  ascii   Si esta activo, muestra una linea con el equivalente
	 *                 ascii de la memoria.        
	 */
	void exLog::exlogBin(int level, std::string id, BYTE *pt, int size, bool ascii )
	{
		bool primero=true;
		static char buf[256];
		static char buf2[1024];

		for( int i=0; i<size; i+=32 )
		{
			sprintf_s( buf, sizeof(buf), "%08d ", i );
			
			for( int j=0; j<32; j++ )
			{
				if( (i+j) >= size )
					break;

				sprintf_s( buf2, sizeof(buf2), "%02X", (unsigned char) *(pt+i+j) );
				strcat_s( buf, sizeof(buf), buf2 );
			}

			if( primero )
			{
				primero=false;
				sprintf_s( buf2, sizeof(buf2), " (size=%d) (%s)", size, id.c_str() );
				strcat_s( buf, sizeof(buf), buf2 );
			}

			exlog2( level, buf );

			if( ascii )
			{
				sprintf_s( buf, sizeof(buf), "%08d ", i );
				for( int j=0; j<32; j++ )
				{
					if( (i+j) >= size )
						break;

					if( isprint( (char)*(pt+i+j) ) )
					{
						sprintf_s( buf2, sizeof(buf2), "%c ", (unsigned char) *(pt+i+j) );
						strcat_s( buf, sizeof(buf), buf2 );
					}
					else
						strcat_s( buf, sizeof(buf), "  " );
				}
				exlog2(level, buf );
			}
		}
	}
}
