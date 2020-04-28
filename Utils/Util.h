#pragma once

#include <string>
#include "variables.h"

namespace BUTIL
{
	/** Clase funciones estaticas para conversiones */
	class Util
	{
	public:
        static bool isDigits(const std::string str);
        static bool isYear(const std::string str);
        static inline void ltrim(std::string &s);
        // trim from end (in place)
        static inline void rtrim(std::string &s);
        // trim from both ends (in place)
        static inline void trim(std::string &s);
        // trim from start (copying)
        static inline std::string ltrim_copy(std::string s);
        // trim from end (copying)
        static inline std::string rtrim_copy(std::string s);
        static inline std::string trim_copy(std::string s);
        static std::string SQL_format(const char *, int, ...);
        static std::string SQL_csv(std::string _format, std::vector<std::string> _values, std::string _surround="", std::string _end = "");
        static std::string formatSQLValues(const char *, int, ...);
        static std::string format( const char *format, ... );
		
		static std::wstring formatW(const wchar_t * format, ...);

		static std::string toLower( std::string );
		static std::string toUpper( std::string );
		static std::wstring wtoLower( std::wstring );
		static std::wstring wtoUpper( std::wstring );
		static std::string replace(std::string, std::string, std::string );

		static std::string toString ( double );
		static std::string toString ( int );
		static std::string toString ( unsigned int );
		static std::string toString ( __int64 );
		static std::string toString ( unsigned __int64 );
		static std::string toString( std::wstring& source );

		static void toIpPort( std::string, std::string &, unsigned short &);

		/** Formatea un número en una cadena de texto con separación de miles
		*	@param val Valor entrada
		*	@return Salida en formato string formateada */
		static std::string formatNumber(__int64 number);
		/** Formatea un número en una cadena de texto con separación de miles
		*	@param val Valor entrada
		*	@return Salida en formato string formateada */
		//static std::wstring wformatNumber(__int64 number);

		/** Formatea un número donde las ´dos últimas cifras serán decimales en una cadena de texto con separación de miles y decimales
		*	@param val Valor entrada
		*	@return Salida en formato string formateada */
		static std::string formatNumber2Dec(__int64 number);
		/** Formatea un número con dos decimales en una cadena de texto con separación de miles y decimales
		 *	@param val Valor entrada
		 *	@return Salida en formato string formateada 
		 */
		static std::string formatNumber2Dec(double number);

           static std::string formatNumber1Dec(double number);
		/** Formatea un número en una cadena de texto con separación de miles
		*	@param val Valor entrada
		*	@return Salida en formato string formateada */
		//static std::wstring wformatNumber2Dec(__int64 number);

		/** Formatea un número con dos decimales en una cadena de texto con separación de miles y decimales
		 *	@param val Valor entrada
		 *	@return Salida en formato string formateada 
		 */
		//static std::wstring wformatNumber2Dec(double number);

		/* Convierte una Cadena tipo string a wstring  */
		static std::wstring ToWideString(const std::string& str);

		static __int64 cent( double );
		static double round( double );
		static float round( float );

		static std::string toHex( std::string _value );
		static std::string fromHex( std::string _value );
		
		static double intToDouble(int);
		static double int64ToDouble(__int64);
		static int doubleToInt(double);
		static __int64 doubleToInt64(double);
        static int stringToInt(std::string, std::string);
		static std::wstring formatearTexto(std::wstring _fichero);

        static bool sortVectorInt(int objA, int objB);
		
		static int ErrorMessageBox(char *tipo,char *ptRecurso,bool *retorno);
        //static std::string GetLastErrorAsString();
        static char* GetLastErrorAsString();
        static char* GetLastErrorAsString(int);

        static int msgBox(std::string _str, std::string _title, int _btns);
        static int msgBox(std::ostringstream ostr, std::string _title, int _btns);

    private:
			/** Función auxiliar de formatNumber para partir números.
			*	@param Número que queda por formatear 
			*	@returns Parte de número formateada. */
			static std::string chomp(__int64& number);
			/** Función auxiliar de wformatNumber para partir números.
			*	@param Número que queda por formatear 
			*	@returns Parte de número formateada. */
			//static std::wstring wchomp(__int64& number);
	};
}
