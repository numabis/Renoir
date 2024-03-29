#ifndef CONVERT_H
#define CONVERT_H
#pragma once
#include <string>
#include <vector>

namespace BUTIL
{
	class Convert
	{
	public:
		static std::string wstring2string( std::wstring  );
        static std::wstring string2wstring(std::string);
        static void string2url(std::string *str);
        //static void removeDoubleQuote(std::string *str);
        static void wstring2url(std::wstring *str);
        static void string2sqlLike(std::string *str);
        static void normalize(std::string *str);
        static LPCWSTR charToWchar(LPSTR _str);
        //static LPSTR wcharToChar(LPCWSTR _src);
        static void wcharToChar(LPCWSTR _src, LPSTR *_dest);
        static void toLower(std::string *_str);
        static bool stob(std::string s, bool throw_on_error = true);
        static void separateValues(std::vector<std::string>* v_list, std::string _list, std::string _sep);
        //static CString string2CString(std::string);
        //static std::string toString(int);
	};
}

#endif //CONVERT_H