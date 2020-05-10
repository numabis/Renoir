#pragma once

#include <string>
#include "Mutex.h"
#include "SQLAPI.h"

namespace BUTIL
{
	/** Clase base para conexión y procesos LITESQL */
	class DataBaseLite: public BUTIL::Mutex
	{
	public:
        DataBaseLite(void);
		~DataBaseLite(void);

		bool setDBFile( std::string );
        bool setDBFile(std::string _file, std::string _journal, std::string _synchronous);

		bool tabExists( std::string ) ;

		int execScalar( std::string );
		int execScalar( const char *format, ... );

		bool exec( std::string );
		bool exec( const char *format, ... );

        int  errorCode;
        std::string errorMsg;


//    private:
        bool connected;

        //int collate();
	protected:
		int execScalarUnlock( std::string );
		int execScalarUnlock( const char *format, ... );

		int execUnlock( std::string );
		bool execUnlock( const char *format, ... );

		SAConnection conex;				/**< Conexión base de datos utilizando SQLAPI++ */
	};
}

