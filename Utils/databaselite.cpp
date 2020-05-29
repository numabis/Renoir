#include "databaseLite.h"
#include "exLog.h"
#include "Convert.h"
#include <queue>
#include <iostream>
#include <fstream>

namespace BUTIL
{
	#pragma region "constructor/destructor"

	/**
	 * Constructor.
	 * Abre la base de datos indicada, si no existe la crea
	 * @param _fichero Nombre del fichero a abrir o crear.
	 */
    DataBaseLite::DataBaseLite( void )
	{
        connected = false;
	}

	
	/**
	 * Destructor.
	 * Cierra la base de datos SQLITE
	 */
    DataBaseLite::~DataBaseLite(void)
	{
		lock();
		
		if( conex.isConnected() == true )
			conex.Disconnect();

        connected = false;

		conex.setClient( SA_Client_NotSpecified );
		conex.Destroy();

		unlock();
	}

	/**
	 * Asigna la base de datos
	 * 
	 * @param	_fichero	Ruta y nombre del fichero s3db
	 *
	 */
	bool DataBaseLite::setDBFile( std::string _file )
	{
        return setDBFile(_file,std::string("PERSIST"), std::string("NORMAL"));
	}

    bool DataBaseLite::setDBFile(std::string _file, std::string _journal, std::string _synchronous)
    {
        exLOGDEBUG("");
        lock();
        const char * cfile = _file.c_str();
        const wchar_t * wcfile = BUTIL::Convert::charToWchar((LPSTR)cfile);
        SAString SAWfile(cfile);
        delete[] wcfile;
        //        SAString *SAfile;
        //SAfile.GetWideChars() = wcfile;
        //SAfile = new SAString(wcfile);

        try
        {
            if (conex.isConnected() == true) {
                conex.Disconnect();
                connected = false;
            }
            //conex.Connect(file, "", "", SA_SQLite_Client);
            conex.Connect(SAWfile, "", "", SA_SQLite_Client);
            //exec("PRAGMA journal_mode = PERSIST;");	// TRUNCATE/PERSIST
            exec("PRAGMA journal_mode = %s;", _journal.c_str());	// TRUNCATE/PERSIST
            exec("PRAGMA foreign_keys = ON;");	// TRUNCATE/PERSIST
            exec("PRAGMA synchronous = %s;", _synchronous.c_str());

            if (conex.isConnected() == true)
            {
                exLOGINFO("Connected to DB");
                connected = true;
            } else
                exLOGINFO("Error Connecting to DB");
        }
        catch (SAException &e)
        {
            //__debugbreak();
            //MessageBox(NULL, std::wstring(__FUNCTION__) + std::wstring(":") + std::wstring(e.ErrText()), L"Error", MB_ICONERROR | MB_OK);
            //EXECPCIONMBOX(e);
            //exLOGERROR("SQL ko : %s", request.c_str());
            //exLOGERROR(e.ErrText());


            wchar_t msg[512];
            std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
            std::wstring wfile = BUTIL::Convert::string2wstring(_file);
            wsprintf(msg, L"SQLAPI ERROR : %s - %s", sqlError.c_str(), wfile.c_str());

            exLOGERROR(((std::string)e.ErrText()).c_str());
            MessageBox(NULL, (LPCWSTR)msg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);


        }

        unlock();
        return connected;
    }
	#pragma endregion

	#pragma region "Funciones basicas. exec protegido mutex"

	/**
	 * Ejecuta una sentencia SQL con protección multi-threads
	 * @param _cmd Sentencia SQL valida.
	 * @return true si la operación es realizada correctamente
	 */
	bool DataBaseLite::exec( std::string _cmd )
	{
        if (!connected)
            return false;
		bool ret = false;
		lock();
		ret = (execUnlock( _cmd ) == ERROR_SUCCESS);
		unlock();
		return ret;
	}

	/**
	 * Ejecuta una sentencia SQL con protección multi-threads
	 * @param format Soporta formato igual que printf
	 * @return true si la operación es realizada correctamente
	 */
	bool DataBaseLite::exec( const char *format,... )
	{
        if (!connected)
            return false;

		bool ret = false;
		char buf[4096];

		va_list ap;
		va_start(ap,format);
		vsprintf_s(buf, sizeof(buf), format,ap);
		va_end(ap);

		lock();
		ret=(execUnlock( std::string( buf ) ) == ERROR_SUCCESS);
		unlock();
		return ret;

	}

	#pragma endregion

	#pragma region "Funciones basicas. exec sin proteger con mutex"
	/**
	 * Ejecuta una sentencia SQL. No tiene protección multi-threads, reservada para usos especiales.
	 * @param _cmd Sentencia SQL valida.
	 * @return true si la operación es correcta
	 */
	int DataBaseLite::execUnlock( std::string _cmd )
	{
        if (!connected)
            return ERROR_BAD_DEVICE;

		//int ret = ERROR_SUCCESS;
        errorCode = ERROR_SUCCESS;

		try
		{
			SACommand cmd ( &conex, _cmd.c_str() , SA_CmdSQLStmtRaw);
			cmd.Execute();
			cmd.Close();
			//ret = ERROR_SUCCESS;
            errorCode = ERROR_SUCCESS;
		}catch( SAException &e )
		{
            //ret = e.ErrNativeCode();
            errorCode = e.ErrNativeCode();
            errorMsg = e.ErrText();
            //wchar_t msg[512];
            //std::wstring sqlError = BUTIL::Convert::string2wstring((std::string)e.ErrText());
            //std::wstring wfile = BUTIL::Convert::string2wstring(_cmd);
            //wsprintf(msg, L"SQLAPI ERROR : %s - %s", sqlError.c_str(), wfile.c_str());

            //exLOGERROR();
            if (_cmd.size() > 255)
                _cmd = _cmd.substr(0, 254);

            exLOGERROR("ERROR SQL   : %s", e.ErrText());
            exLOGERROR("ERROR QUERY : %s", _cmd.c_str());
		}

		return errorCode;
	}

	/**
	 * Ejecuta una sentencia SQL. No tiene protección multi-threads, reservada para usos especiales.
	 * @param format Sentencia SQL valida. Igual que printf
	 * @return true si la operación es correcta
	 */
	bool DataBaseLite::execUnlock( const char *format,... )
	{
        if (!connected)
            return false;

		bool ret = false;
		char buf[4096];

		va_list ap;
		va_start(ap,format);
		vsprintf_s(buf, sizeof(buf), format,ap);
		va_end(ap);

		return (execUnlock( std::string( buf ) ) == ERROR_SUCCESS);
	}
	#pragma endregion

	#pragma region "Funciones basicas. exec Scalar protegido con mutex. select * y cuenta los registros implicados"
	/**
	 * Realiza una consulta de tipo count(*). Protección multithread
	 * @param format Sentencia SQL de tipo select count(*) as ret from xxx
	 * @return el número de registros encontrados
	 */
	int DataBaseLite::execScalar( const char *format,...  )
	{
        if (!connected)
            return ERROR_BAD_DEVICE;

		char buf[4096];

		va_list ap;
		va_start(ap,format);
		vsprintf_s(buf, sizeof(buf), format,ap);
		va_end(ap);

		lock();
		int ret = execScalarUnlock( std::string( buf ) );
		unlock();

		return ret;
	}


	/**
	 * Realiza una consulta de tipo count(*). Protección multithread
	 * @param _cmd Sentencia SQL de tipo select count(*) as ret from xxx
	 * @return el número de registros encontrados
	 */
	int DataBaseLite::execScalar( std::string _cmd )
	{
        if (!connected)
            return ERROR_BAD_DEVICE;

		int ret = 0;
		
		lock();
		ret = execScalarUnlock( _cmd );
		unlock();

		return ret;
	}
	#pragma endregion

	#pragma region "Funciones basicas. exec Scalar sin proteger con mutex. select * y cuenta los registros implicados"
	/**
	 * Realiza una consulta de tipo count(*). Sin protección multithread
	 * @param format Sentencia SQL de tipo select count(*) as ret from xxx
	 * @return el número de registros encontrados
	 */
	int DataBaseLite::execScalarUnlock( const char *format,...  )
	{
        if (!connected)
            return ERROR_BAD_DEVICE;

		char buf[4096];

		va_list ap;
		va_start(ap,format);
		vsprintf_s(buf, sizeof(buf), format,ap);
		va_end(ap);

		return execScalarUnlock( std::string( buf ) );
	}


	/**
	 * Realiza una consulta de tipo count(*). Sin protección multithread
	 * @param _cmd Sentencia SQL de tipo select count(*) as ret from xxx
	 * @return el número de registros encontrados
	 */
	int DataBaseLite::execScalarUnlock( std::string _cmd )
	{
        if (!connected)
            return ERROR_BAD_DEVICE;

		int ret = -1;
		
		try
		{
			SACommand cmd ( &conex, _cmd.c_str() , SA_CmdSQLStmt );
			cmd.Execute();

			if( cmd.FetchNext() && cmd.FieldCount()>0 )
			{
				ret = cmd.Field(1).asLong();
			}

			cmd.Close();

		}
		catch( SAException &e )
		{
            ret = -1;
            const char *msg = e.ErrText();
            exLOGERROR(msg);
            //if (ret == ERROR_LOCK_VIOLATION)
            //{
                //const char *msg = e.ErrText();
                MessageBox(NULL, (LPCWSTR)BUTIL::Convert::charToWchar((LPSTR)msg), (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
            //}
		}

		return ret;
	}

	#pragma endregion

	#pragma region "comprueba si existe una tabla. Utiliza internamete mutex"
	/**
	 * Realiza la comprobación para conocer si existe una tabla en la base de datos.
	 * @param _tabla nombre de la tabla
	 * @return true si existe la tabla
	 */
	bool DataBaseLite::tabExists( std::string _table)
	{
        if (!connected)
            return false;

		std::string str = "SELECT count(*) as ret FROM sqlite_master WHERE type='table' AND name='" + _table + "'";
			
		if( execScalar( str ) > 0 )
			return true;

		return false;
	}

    //int DataBaseLite::collate()
    //{
    //    int collation = sqlite3_create_collation();
    //}
    

	#pragma endregion
}


