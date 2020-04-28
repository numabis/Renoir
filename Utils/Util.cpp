#include <windows.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
//#include <cstring>
#include "atlstr.h"

#include "Util.h"
#include "Convert.h"
#include "sha256.h"
#include "cultureInfo.h"

namespace BUTIL
{
#pragma region "Conversión a Mayusculas/Minusculas. Varias funciones del tipo."

    /**
     * Convierte una cadena a su equivalente minuscula
     * @param in Cadenada entrada
     * @return cadena convertida
     */
    std::string Util::toLower(std::string in)
    {
        char buf[4096];
        strncpy_s(buf, sizeof(buf), in.c_str(), sizeof(buf));
        _strlwr_s(buf, sizeof(buf));
        return std::string(buf);
    }
    /**
     * Convierte una wcadena a su equivalente minuscula
     * @param in Cadenada entrada
     * @return cadena convertida
     */
    std::wstring Util::wtoLower(std::wstring in)
    {
        std::wstring out = in;
        std::transform(out.begin(), out.end(), out.begin(), towlower);
        return out;
    }

    /**
     * Convierte una cadena a su equivalente mayuscula
     * @param in Cadenada entrada
     * @return cadena convertida
     */
    std::string Util::toUpper(std::string in)
    {
        char buf[4096];
        strncpy_s(buf, sizeof(buf), in.c_str(), sizeof(buf));
        _strupr_s(buf, sizeof(buf));
        return std::string(buf);
    }
    /**
     * Convierte una w cadena a su equivalente mayuscula
     * @param in Cadenada entrada
     * @return cadena convertida
     */
    std::wstring Util::wtoUpper(std::wstring in)
    {
        std::wstring out = in;
        std::transform(out.begin(), out.end(), out.begin(), towupper);
        return out;
    }

    /**
     * Reemplaza en una cadena un secuencia por otra
     * @param in Cadenada entrada
     * @return cadena convertida
     */
    std::string Util::replace(std::string in, std::string busca, std::string reemplaza)
    {
        return in.replace(in.find(busca), busca.length(), reemplaza);
    }

    /**
     * Convierte un valor de tipo IP y Puerto "aaa.aaa.aaa.aaa:bbbbb" a su valor separado
     * @param ipPort Cadena en formato "IP:Puerto"
     * @param ip Cadena de salida solo con la "IP"
     * @param port Variable de salida solo con el puerto
     */
    void Util::toIpPort(std::string ipPort, std::string &ip, unsigned short &port)
    {
        std::string::size_type pos = ipPort.find(":", 0);
        if (pos == std::string::npos)
        {
            ip = ipPort;
            port = 0;
        }
        else
        {
            ip = ipPort.substr(0, pos);
            port = atoi(ipPort.substr(pos + 1).c_str());
        }
    }

#pragma endregion

#pragma region "Conversión a string"


    /**
     *	Convierte el formato printf a un string
     *
     *  @param	format	Formato printf
     *
     *	@return	Cadena string convertida
     */
    std::string Util::format(const char *format, ...)
    {
        char buf[4096];

        va_list ap;
        va_start(ap, format);
        vsprintf_s(buf, sizeof(buf), format, ap);
        va_end(ap);

        return std::string(buf);
    }

    std::string Util::SQL_format(const char * _request, int _nbCols, ...)
    {
        std::string request = std::string(_request);

        char* val;
        va_list vl;
        va_start(vl, _nbCols);
        for (int i = 0; i<_nbCols; i++)
        {
            val = va_arg(vl, char*);
            request += std::string(val);
            if (i < _nbCols - 1)
                request += ", ";
        }
        va_end(vl);
        return request;
    }

    std::string Util::SQL_csv(std::string _request, std::vector<std::string> _values, std::string _surround, std::string _end)
    {
        size_t nbCols = _values.size();
        for (size_t i = 0; i<nbCols; i++)
        {
            _request += _surround + _values[i] + _surround;
            if (i < nbCols - 1)
                _request += ", ";
        }
        _request += std::string(_end) + " ";
        return _request;
    }    
    std::string Util::formatSQLValues(const char * _request, int _nbCols, ...)
    {
        std::string request = std::string(_request);

        char* val;
        va_list vl;
        va_start(vl, _nbCols);
        request += "(";
        for (int i = 0; i<_nbCols; i++)
        {
            val = va_arg(vl, char*);
            request += "'" + std::string(val) + "'";
            if (i < _nbCols - 1)
                request += ", ";
        }
        request += ")";
        va_end(vl);
        return request;
    }

    /**
     * Convierte una variable de tipo double a cadena. teniendo en cuenta la suma de 0.0001
     * para el problema de la precisión.
     * @param val Valor entrada
     * @return Salida en formato string
     */
    std::string Util::toString(double val)
    {
        char buf[256];
        sprintf_s(buf, sizeof(buf), "%.2f", val + 0.0001);
        return std::string(buf);
    }

    /**
     * Convierte una variable de tipo int a cadena.
     * @param val Valor entrada
     * @return Salida en formato string
     */
    std::string Util::toString(int val)
    {
        char buf[256];
        sprintf_s(buf, sizeof(buf), "%d", val);
        return std::string(buf);
    }

    /**
     * Convierte una variable de tipo int 64bits a cadena.
     * @param val Valor entrada
     * @return Salida en formato string
     */
    std::string Util::toString(__int64 val)
    {
        char buf[256];
        sprintf_s(buf, sizeof(buf), "%I64d", val);
        return std::string(buf);
    }

    /**
     * Convierte una variable de tipo unsigned 64 bits a cadena.
     * @param val Valor entrada
     * @return Salida en formato string
     */
    std::string Util::toString(unsigned __int64 val)
    {
        char buf[256];
        sprintf_s(buf, sizeof(buf), "%I64u", val);
        return std::string(buf);
    }

    /**
     * Convierte una variable de tipo unsigned a cadena.
     * @param val Valor entrada
     * @return Salida en formato string
     */
    std::string Util::toString(unsigned int val)
    {
        char buf[256];
        sprintf_s(buf, sizeof(buf), "%u", val);
        return std::string(buf);
    }



    /////////////////////////////////////////////////////////////////////////////////////////////
    std::string Util::chomp(__int64& number)
    {
        __int64 remainder = number % 1000;
        char cad[5];
        number /= 1000;
        if (number > 0)
            sprintf_s(cad, 5, "%03I64u", remainder);
        else
        {
            if (remainder == 0)
                sprintf_s(cad, 5, "%03I64u", remainder);
            else
                sprintf_s(cad, 5, "%I64u", remainder);
        }
        return (std::string(cad));
    }


    /////////////////////////////////////////////////////////////////////////////////////////////
    std::string Util::formatNumber(__int64 number)
    {
        std::string strNumber;
        if (number == 0)
        {
            strNumber = std::string("0");
        }
        else
        {
            strNumber = chomp(number);
            while (number)
            {
                strNumber = chomp(number) + "." + strNumber;
            }
        }
        return strNumber;
    }


		/////////////////////////////////////////////////////////////////////////////////////////////
		std::string Util::formatNumber2Dec(__int64 number)
		{
			__int64 decimal = number % 100;
			char decim[5];
			sprintf_s(decim, 5, "%02I64u", decimal);
			number = number / 100;
					
			std::string result = formatNumber(number) + "," + std::string(decim);
			return result;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		std::string Util::formatNumber2Dec(double number)
		{
			char decim[5];
			double fnum = number * 100.0;
		    __int64 inum = (__int64 )(number * 100.0);
		
		    if (fnum > 0)
		        inum = ((fnum - inum) <  0.5 ? inum : ++inum);
		    else
		        inum = ((fnum - inum) > -0.5 ? inum : --inum);
		
			__int64 decimal = inum % 100;
			
			sprintf_s(decim, 5, "%02I64u", decimal);
			inum = inum / 100;
			std::string result = formatNumber(inum) + "." + std::string(decim);
			return result;
		}

        std::string Util::formatNumber1Dec(double number)
        {
            std::string result;
            if (number == 0.0)
                return result;
            char decim[5];
            double fnum = number * 10.0;
            __int64 inum = (__int64)(number * 10.0);

            if (fnum > 0)
                inum = ((fnum - inum) <  0.5 ? inum : ++inum);
            else
                inum = ((fnum - inum) > -0.5 ? inum : --inum);

            __int64 decimal = inum % 10;

            sprintf_s(decim, 5, "%01I64u", decimal);
            inum = inum / 10;
            result = formatNumber(inum) + "." + std::string(decim);
            return result;
        }
		/////////////////////////////////////////////////////////////////////////////////////////////
		//std::wstring Util::wchomp(__int64& number)
		//{
		//	__int64 remainder = number % 1000;
		//	wchar_t cad[5];
		//	number /= 1000;
		//	if (number > 0)
		//		swprintf_s(cad, 5, L"%03I64u", remainder);
		//	else
		//	{
		//		if (remainder == 0)
		//			swprintf_s(cad, 5, L"%03I64u", remainder);
		//		else
		//			swprintf_s(cad, 5, L"%Ld", remainder);
		//	}			
		//	return (std::wstring(cad));
		//}


		/////////////////////////////////////////////////////////////////////////////////////////////
		//std::wstring Util::wformatNumber(__int64 number)
		//{
		//	std::wstring strNumber;
		//	if (number == 0)
		//	{
		//		strNumber = std::wstring(L"0");
		//	}
		//	else
		//	{
		//		strNumber = wchomp(number);
		//		while( number )
		//		{
		//			strNumber = wchomp(number) + L"." + strNumber;
		//		}
		//	}
		//	return strNumber;
		//}

		/////////////////////////////////////////////////////////////////////////////////////////////
		//std::wstring Util::wformatNumber2Dec(__int64 number)
		//{
		//	wchar_t decim[5];
		//	unsigned __int64 decimal = number % 100;
		//	
		//	swprintf_s(decim, 5, L"%02I64u", decimal);
		//	number = number / 100;
		//	std::wstring result = wformatNumber(number) + L"," + std::wstring(decim);
		//	return result;
		//}

		///////////////////////////////////////////////////////////////////////////////////////////////
		//std::wstring Util::wformatNumber2Dec(double number)
		//{
		//	wchar_t decim[5];
		//
		//	double fnum = number * 100.0;
		//    __int64 inum = (__int64)(number * 100.0);
		//
		//    if (fnum > 0)
		//        inum = ((fnum - inum) <  0.5 ? inum : ++inum);
		//    else
		//        inum = ((fnum - inum) > -0.5 ? inum : --inum);
		//
		//	unsigned __int64 decimal = inum % 100;
		//	
		//	swprintf_s(decim, 5, L"%02I64u", decimal);
		//	inum = inum / 100;
		//	std::wstring result = wformatNumber(inum) + L"," + std::wstring(decim);
		//	return result;
		//}

		#pragma endregion

		std::wstring Util::formatW(const wchar_t * format, ...)
		{
			wchar_t buffer[4096];

			va_list ap;
			va_start(ap, format);
			vswprintf(buffer, 4096, format, ap);
			va_end(ap);

			return 	std::wstring(buffer);
		}

		#pragma region "Conversiones de Cadena"
		std::wstring Util::ToWideString(const std::string& str)
		{
			std::wstring result = L"";			
			int stringLength = MultiByteToWideChar(CP_ACP, 0, str.data(), static_cast<int>(str.length()), 0, 0);
			if (stringLength > 0)
			{				
				std::wstring wstr(stringLength, 0);
				MultiByteToWideChar(CP_ACP, 0,  str.data(), static_cast<int>(str.length()), &wstr[0], stringLength);
				result = wstr;
			}
			return result;
		}

		//--------------------------------------------------------------------------------------
		std::string Util::toString( std::wstring& source )
		{
/*			std::string str(source.length()+1, 0);
			WideCharToMultiByte(CP_ACP, 0, source.c_str(), source.length(), &str[0], -1, NULL, NULL);			
			return str;	*/		

			std::string result;
			BSTR unicodestr = 0;
			
			unicodestr = ::SysAllocString(source.c_str());
			int lenW = ::SysStringLen(unicodestr);
			int lenA = ::WideCharToMultiByte(CP_ACP, 0, unicodestr, lenW, 0, 0, NULL, NULL);
			if (lenA > 0)
			{
				char *ansistr;
				ansistr = new char[lenA + 1]; // allocate a final null terminator as well
				::WideCharToMultiByte(CP_ACP, 0, unicodestr, lenW, ansistr, lenA, NULL, NULL);
				ansistr[lenA] = 0; // Set the null terminator yourself
				result = std::string(ansistr);
				delete[] ansistr;
			}
			else
			{
				result = "";
			}		
			::SysFreeString(unicodestr);
			return result;
		}

		#pragma endregion

		#pragma region "Conversión ISO 8601"

	//SADateTime Util::ISO8601_SADateTime( std::string _fecha)
	//{
	//	int year = atoi( _fecha.substr(0,4).c_str() );
	//	int month = atoi( _fecha.substr(5,2).c_str() );
	//	int day = atoi( _fecha.substr(8,2).c_str() );
	//	int hour = atoi( _fecha.substr(11,2).c_str() );
	//	int minute = atoi( _fecha.substr(14,2).c_str() );
	//	int second = atoi( _fecha.substr(17,2).c_str() );
	//
	//	return SADateTime( year, month, day, hour, minute, second );
	//}

	//std::string Util::SADateTime_ISO8601( SADateTime &_fecha )
	//{
	//	std::stringstream str;
	//
	//	str.fill( '0' );
	//
	//	str.width(4);
	//	str << _fecha.GetYear();
	//	
	//	str.width(0);
	//	str << "-";
	//	
	//	str.width(2);
	//	str << _fecha.GetMonth();
	//	
	//	str.width(0);
	//	str << "-";
	//	
	//	str.width(2);
	//	str << _fecha.GetDay();
	//
	//	str.width(0);
	//	str << "T";
	//
	//	str.width(2);
	//	str << _fecha.GetHour();
	//	
	//	str.width(0);
	//	str << ":";
	//
	//	str.width(2);
	//	str << _fecha.GetMinute();
	//	
	//	str.width(0);
	//	str << ":";
	//
	//	str.width(2);
	//	str << _fecha.GetSecond();
	//
	//	return str.str();
	//}



	#pragma endregion

    /**
     *  Redondea el 3 decimal
     *  
     *  @param  value   Valor a redondear
     *  @return Valor redondeado
     *  
     */                             
	double Util::round( double value )
	{
		value += 0.005;
		__int64 val = (__int64)value*100;
		return (double)val/100.00;
	}

    /**
     *  Redondea el 3 decimal
     *  
     *  @param  value   Valor a redondear
     *  @return Valor redondeado
     *  
     */                             
	float Util::round( float value )
	{
		value += 0.005f;
		__int64 val = (__int64)value*100;
		return (float)val/100.00f;
	}

    /**
     *  Recupera un valor en euros a céntimos
     *  
     *  @param  _value          Valor en Euros o Dolares
     *  
     *  @return Valor en céntimos o centavos.
     *  
     */                        
	__int64 Util::cent( double _value )
	{
        _value = _value + 0.001;
		return static_cast<__int64>( _value*100.00 );
	}

	std::string Util::toHex( std::string  _value )
	{
		char buf[4];
		std::string ret = "";
		for( size_t i=0; i<_value.length(); i++ )
		{
			sprintf_s( buf, sizeof(buf), "%02X", _value[i] );
			ret += buf;
		}
		return ret;

	}

	std::string  Util::fromHex( std::string  _value )
	{
		std::string ret = "";
		for( size_t i=0; i<_value.length(); i+=2 )
		{
			int hex;
			sscanf_s( _value.substr(i,2).c_str(), "%02X", &hex );
			ret += hex;
		}
		return ret;
	}

    int Util::stringToInt(std::string _value, std::string _thousands)
    {
        int ret;
        size_t found = _value.find_last_of(_thousands);
        if (found != std::string::npos)
            _value.erase(found,1);
        ret = atoi(_value.c_str());
        return ret;
    }

	double Util::intToDouble(int _valor)
	{
		return (((double)_valor / 100.0) + 0.0001);
	}

	double Util::int64ToDouble(__int64 _valor)
	{
		return (((double)_valor / 100.0) + 0.0001);
	}

	int Util::doubleToInt(double _valor)
	{
		return (int)((_valor + 0.0001) * 100.0);
	}

	__int64 Util::doubleToInt64(double _valor)
	{
		return (__int64)((_valor + 0.0001) * 100.0);
	}

	std::wstring Util::formatearTexto(std::wstring _fichero)
	{
		std::wstring texto = L"";
		FILE *f =  _wfopen(_fichero.c_str(), L"rt");
		if (f != NULL)
		{
			fseek (f , 0 , SEEK_END);
			long size = ftell (f);
			rewind (f);

			wchar_t * buffer =(wchar_t *)malloc(size * sizeof(wchar_t));
			memset(buffer, 0, size * sizeof(wchar_t));

			fread(buffer, sizeof(wchar_t), size, f);
			fclose(f);

			texto = buffer;
			for (int i=0; i<(int)texto.size(); i++)
			{
				if(texto[i] == '\n')
				{
					texto[i]= '|';
				}
			}

			for (int i=0; i<(int)texto.size(); i++)
			{
				if (texto[i] == '\r')
					texto.erase(i, 1);
			}

			free(buffer);
		}

		return texto;
	}
    
	bool Util::sortVectorInt(int objA, int objB)
	{
		return (objA < objB);
    }

    int Util::ErrorMessageBox(char *tipo, char *ptRecurso, bool *retorno)
    {
        //if (configuracionCarga.recursosNecesarios == true)
        *retorno = false;
        //if (configuracionCarga.recursosAvisos)
        //{
        char str[100];
        int retMessageBox;
        //ret = false;
        sprintf(str, "%s: %s", tipo, ptRecurso);

        std::wstring message(str, str + strlen(str));
        retMessageBox = MessageBox(NULL, message.c_str(), (LPCWSTR)L"Error", MB_ICONERROR | MB_OKCANCEL);

        if (retMessageBox == IDCANCEL)
            *retorno = false;

        return retMessageBox;
        //} 
        //return  MB_OK;
    }
    //    void Util::normalize(std::string *str)
    //    {
    //        std::string new_string;
    //        for (size_t i = 0; i < str->length(); ++i) {
    //            switch ((*str)[i]) {
    //            case 'é':
    //            case 'è':
    //                (*str)[i]='e';
    //                break;
    //            }
    //        }
    //    }
    //void Util::fileNameToDetails(filetype* _file)
    //{
    //    std::size_t size = _file->filename.size();
    //    if (size == 0)
    //        return;
    //
    //    size_t yearPos = findYear2(_file->filename, &(_file->year));
    //
    //    std::size_t foundOpen;
    //
    //    foundOpen = min(_file->filename.find_first_of("([{"), yearPos-1);
    //    //if (foundOpen < 3 || foundOpen < _file->filename.size() / 2)
    //    //    foundOpen = std::string::npos;
    //    if (foundOpen < size)
    //        _file->title = _file->filename.substr(0, foundOpen);
    //    else
    //    {
    //        _file->title = _file->filename;
    //        _file->title[_file->filename.find_last_of(".")] = '\0';
    //    }
    //
    //    BUTIL::Convert::normalize(&(_file->title));
    //}

    //int Util::findYear(const std::string str)
    //{
    //    std::size_t foundOpen, foundClose;
    //    std::string::size_type sz;
    //    foundClose = str.find_first_of(")");
    //    foundOpen = str.find_first_of("(");
    //    //test-+(qwer)(1234)1985.[3](is).avi
    //    //0123456789012345678901234567890123456789
    //    //0         1         2         3         
    //    int min;
    //    while (foundClose != std::string::npos && foundOpen != std::string::npos)
    //    {
    //        if (foundClose - foundOpen == 5)
    //        {
    //            std::string year = str.substr(foundOpen + 1, 4);
    //            if (isYear(year))
    //            {
    //                return stoi(year, &sz);
    //                //std::size_t foundOpen = _file->filename.find_first_of("(") + 1;
    //                //_file->title = rtrim_copy(_file->filename.substr(0, foundOpen - 1));
    //                break;
    //            }
    //        }
    //        min = foundOpen < foundClose ? foundOpen : foundClose;
    //        foundClose = str.find_first_of(")", foundClose + 1);
    //        foundOpen = str.find_first_of("(", foundOpen + 1);
    //
    //    }
    //    return 0;
    //}

    //int Util::findYear2(const std::string str, short* _year)
    //{
    //    std::size_t findYear;
    //    std::string::size_type sz;
    //    findYear = str.find_first_of("12");
    //    //test-+(qwer)(1234)1985.[3](is).avi
    //    //0123456789012345678901234567890123456789
    //    //0         1         2         3         
    //    while (findYear != std::string::npos)
    //    {
    //        std::size_t previous = str.find_last_of(".-_([{", findYear);
    //        std::size_t next = str.find_first_of(".-_)]}", findYear);
    //
    //        if (findYear < 1 || previous != findYear - 1 || next != findYear + 4)
    //        {
    //            findYear = str.find_first_of("12", findYear + 1);
    //            continue;
    //        }
    //        if (1)
    //        {
    //            std::string year = str.substr(findYear, 4);
    //            if (isYear(year))
    //            {
    //                *_year = stoi(year, &sz);
    //                return findYear;
    //                //std::size_t foundOpen = _file->filename.find_first_of("(") + 1;
    //                //_file->title = rtrim_copy(_file->filename.substr(0, foundOpen - 1));
    //                break;
    //            }
    //        }
    //        findYear = str.find_first_of("12", findYear + 1);
    //
    //    }
    //    return std::string::npos;
    //}

    bool Util::isDigits(const std::string str)
    {
        return str.find_first_not_of("0123456789") == std::string::npos;
    }

    bool Util::isYear(const std::string str)
    {
        if ((str[0] != '1' || str[1] != '9') && (str[0] != '2' || str[1] != '0'))
            return false;
        return str.find_first_not_of("0123456789") == std::string::npos;
    }

    // trim from start (in place)
    inline void Util::ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    inline void Util::rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    inline void Util::trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    inline std::string Util::ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    inline std::string Util::rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    inline std::string Util::trim_copy(std::string s) {
        trim(s);
        return s;
    }
    
    //Returns the last Win32 error, in string format. Returns an empty string if there is no error.
    //std::string Util::GetLastErrorAsString()
    char* Util::GetLastErrorAsString(int _lastError)
    {
        if (_lastError == 0)
            return LPSTR(); //No error message has been recorded

                            //        LPSTR messageBuffer = nullptr;
        char *messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, _lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        return messageBuffer;
    }

    char* Util::GetLastErrorAsString()
    {
        //Get the error message, if any.
        return GetLastErrorAsString(::GetLastError());

        //std::string message(messageBuffer, size);

        //Free the buffer.
        //LocalFree(messageBuffer);

        //return message;
    }

    int Util::msgBox(std::ostringstream _ostr, std::string _title, int _btns)
    {
        return MessageBox(NULL, CString(_ostr.str().c_str()), CString(_title.c_str()), _btns);
    }
    int Util::msgBox(std::string _str, std::string _title, int _btns)
    {
        return MessageBox(NULL, CString(_str.c_str()), CString(_title.c_str()), _btns);
    }
}

