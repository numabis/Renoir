#pragma once

#include <string>
#include <vector>
#include "Mutex.h"
#include "Singleton.h"
//#include "definitions.h"

#define exLOGERROR(...)	BUTIL::exLog::getInstance().exlog(BUTIL::exLog::exLOG_ERROR, __FUNCTION__, ##__VA_ARGS__);
#define exLOGINFO(...)	BUTIL::exLog::getInstance().exlog(BUTIL::exLog::exLOG_INFO,  __FUNCTION__, ##__VA_ARGS__);
#define exLOGNAME()		BUTIL::exLog::getInstance().exlog(BUTIL::exLog::exLOG_INFO,  __FUNCTION__, "");
#define exLOGDB(...)	BUTIL::exLog::getInstance().exlog(BUTIL::exLog::exLOG_DB,    __FUNCTION__, ##__VA_ARGS__);
#define exLOGSQL(...)	BUTIL::exLog::getInstance().exlog(BUTIL::exLog::exLOG_SQL,    __FUNCTION__, ##__VA_ARGS__);
#define exLOGXML(...)	BUTIL::exLog::getInstance().exlog(BUTIL::exLog::exLOG_XML,   __FUNCTION__, ##__VA_ARGS__);
#define exLOGDEBUG(...)	BUTIL::exLog::getInstance().exlog(BUTIL::exLog::exLOG_DEBUG, __FUNCTION__, ##__VA_ARGS__);

#define ST_SAVE(MY_ST) states preState = *ptr_globalState; *ptr_globalState = MY_ST; //consoleDBG->push_back(BUTIL::exLog::stNames[MY_ST]);
#define ST_SET(MY_ST) *ptr_globalState = MY_ST; //consoleDBG->push_back(stNames[MY_ST]);
#define ST_RESTORE()  *ptr_globalState = preState; //consoleDBG->push_back(stNames[preState]);
#define ST_EQUAL(MY_ST) *ptr_globalState == MY_ST
#define ST_DIFF(MY_ST) *ptr_globalState != MY_ST
#define ST_PREEQUAL(MY_ST) preState == MY_ST
#define ST_PREDIFF(MY_ST) preState != MY_ST

namespace BUTIL
{
	/** Clase encapsula sistema LOG a disco */  
	class exLog: Mutex, public Singleton<exLog>
	{
	public:

        // Niveles del log
		enum 
		{
			exLOG_DISABLED,
            exLOG_ERROR,
            exLOG_INFO,
            exLOG_DB,
            exLOG_XML,
            exLOG_DEBUG,
            exLOG_SQL,
            exLOG_MAX

        };

        //states globalState;


//        void setConfig(LOG_CONFIG *logConfig);
        void setPath(std::string);
        void setPath( std::wstring );
        void setLogLevel(int * level = 0);

		void exlog(int,  std::string );
		void exlog2(int, const char *,...);
		void exlog(int, const wchar_t *format, ...);
		void exlogBin(int, std::string, BYTE *, int, bool=false );
		void exlog(int level, const char *function, const char *format,...);

		void exlog(int, std::wstring );

        std::vector<std::string> *ptr_consoleDBG;

	private:
		friend class Singleton<exLog>;
		std::string LOG_path;					/**< Ruta por defecto para los log */
		
		// Indicador del nivel de profundidad del log del log. (Se configura en config.xml)
		int *logLevel;
        int logDisable;

//        LOG_CONFIG *logConfig;

		exLog();
		~exLog();
	};
}
