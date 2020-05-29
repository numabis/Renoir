#include <windows.h>
#include <algorithm>
#include "Convert.h"
#include <sstream>

namespace BUTIL
{
    /**
     *  Convierte una cadena wstring a string
     *  
     *  @param  s   Cadena a convertir
     *  
     *  @return Cadena convertida
     *  
     */                                  
	std::string Convert::wstring2string(std::wstring s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0); 
		char* buf = new char[len];
		WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0); 
		std::string r(buf);
		delete[] buf;
		return r;
	}

    /**
     *  Convierte una cadena string a wstring
     *  
     *  @param  s   Cadena a convertir
     *  
     *  @return Cadena convertida
     *  
     */                                  
	std::wstring Convert::string2wstring(std::string s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
    //std::string Convert::toString(int val)
    //{
    //    char buf[256];
    //    sprintf_s(buf, sizeof(buf), "%d", val);
    //    return std::string(buf);
    //}
    //CString Convert::string2CString(std::string str)
    //{
    //    CString ret;
    //    ret.Format(L"%s", str);
    //    return ret;
    //}

    void Convert::wstring2url(std::wstring *_str)
    {
        std::string str = wstring2string(*_str);
        string2url(&str);
        *_str = string2wstring(str);
    }
    //void Convert::removeDoubleQuote(std::string *_str)
    //{
    //    int found = 0;
    //    while (_str->find_first_of("&", found))
    //    {
    //        if (found)
    //    }
    //}
    void Convert::string2url(std::string *_str)
    {
        for (size_t i = 0; i < _str->length(); ++i) {
            switch ((*_str)[i]) {
            case ' ':
            //case '\'':
            case '-':
            case '_':
            case '.':
            case ',':
                (*_str)[i] = '+';
                break;
            case 'é':
            case 'è':
            case 'ê':
                (*_str)[i] = 'e';
                break;
            case 'á':
            case 'à':
                (*_str)[i] = 'a';
                break;
            case 'ú':
                (*_str)[i] = 'u';
                break;
            case 'ó':
                (*_str)[i] = 'o';
                break;
            case 'í':
                (*_str)[i] = 'i';
                break;
            case '&':
                _str->replace(i, 1, "%26");
                break;
            }
        }
    }

    void Convert::string2sqlLike(std::string *_str)
    {
        for (size_t i = 0; i < _str->length(); ++i) {
            switch ((*_str)[i]) {
            case ' ':
            case '-':
            case '_':
            case ':':
            case ',':
            case '\'':
            case 'é':
            case 'è':
            case 'ê':
            case 'á':
            case 'à':
            case 'ú':
            case 'ó':
            case 'í':
                (*_str)[i] = '%';
                break;
//            case 'é':
//            case 'è':
//            case 'ê':
//                (*_str)[i] = 'e';
//                break;
//            case 'á':
//            case 'à':
//                (*_str)[i] = 'a';
//                break;
//            case 'ú':
//                (*_str)[i] = 'u';
//                break;
//            case 'ó':
//                (*_str)[i] = 'o';
//                break;
//            case 'í':
//                (*_str)[i] = 'i';
//                break;
            }
        }
        //*_str += "%";
    }

    void Convert::normalize(std::string *_str)
    { // +		"franÃ§ois ¨©ª ¡¢¬ Ã­Ã®Ã¯ÃºÃ»Ã¼Â¿"
        // françois        èéêàáâì í î ï ú û ü ¿

        for (size_t i = 0; i < _str->length(); ++i) {
            switch ((*_str)[i]) {
            case 'Ã':
                _str->erase(i, 1);
                switch ((*_str)[i]) {
                case -96: //' ':
                case -95: //'¡':
                case -94: //'¢':
                    (*_str)[i] = 'a';
                    break;
                case -89: // '§'
                    (*_str)[i] = 'c';
                    break;
                case -88: // '¨'
                case -87: // '©'
                case -86: //'ª':
                case -85: //'«':
                    (*_str)[i] = 'e';
                    break;
                case -84: //'¬':
                case -83: //'­'
                case -82: //'®':
                case -81: //'¯':
                    (*_str)[i] = 'i';
                    break;
                case -77: //'³':
                    (*_str)[i] = 'o';
                    break;
                case -70: //'º':
                case -69: //'»':
                case -68: // '¼'
                    (*_str)[i] = 'u';
                    break;
                }
                break;
            case '.':
            case '+':
            case '-':
            case '_':
            case ':':
            case ',':
                (*_str)[i] = ' ';
                break;
            case 'é':
            case 'è':
            case 'ê':
                (*_str)[i] = 'e';
                break;
            case 'á':
            case 'à':
                (*_str)[i] = 'a';
                break;
            case 'ú':
                (*_str)[i] = 'u';
                break;
            case 'ó':
                (*_str)[i] = 'o';
                break;
            case 'í':
                (*_str)[i] = 'i';
                break;
            case 'ç':
                (*_str)[i] = 'c';
                break;
            }
        }
    }

    LPCWSTR Convert::charToWchar(LPSTR _str)
    {
        int nChars = MultiByteToWideChar(CP_ACP, 0, _str, -1, NULL, 0);
        const WCHAR *str = new WCHAR[nChars];
        MultiByteToWideChar(CP_UTF8, 0, _str, -1, (LPWSTR)str, nChars);

        return str;
        delete[] str;
    }
    void Convert::wcharToChar(LPCWSTR _src, LPSTR *_dest)
    {
        const wchar_t* pt;
        //char buffer[255];
        int i = 0, length;

        pt = _src;
        while (*pt) {
            length = wctomb((char*)(_dest + i), *pt);
            if (length<1) break;
            ++pt;
            i++;
        }
        _dest[i] = '\0';
    }

    //LPSTR Convert::wcharToChar(LPCWSTR _src)
    //{
    //    LPSTR dest;
    //    const wchar_t* pt;
    //    //char buffer[255];
    //    int i = 0, length;
    //
    //    pt = _src;
    //    while (*pt) {
    //        length = wctomb((char*)(dest + i), *pt);
    //        if (length<1) break;
    //        ++pt;
    //        i++;
    //    }
    //    dest[i] = '\0';
    //    return dest;
    //}

    void Convert::toLower(std::string *_str)
    {
        for (size_t i = 0; i < _str->length(); ++i)
        {
            if ((*_str)[i] >= 'A' && (*_str)[i] <= 'Z')
                (*_str)[i] += 32;
        }
    }
    bool Convert::stob(std::string s, bool throw_on_error)
    {
        auto result = false;    // failure to assert is false

        std::istringstream is(s);
        // first try simple integer conversion
        is >> result;

        if (is.fail())
        {
            // simple integer failed; try boolean
            is.clear();
            is >> std::boolalpha >> result;
        }

        if (is.fail() && throw_on_error)
        {
            throw std::invalid_argument(s.append(" is not convertable to bool"));
        }

        return result;
    }

    void Convert::separateValues(std::vector<std::string> *v_list, std::string _list, std::string _sep)
    {
        v_list->clear();
        size_t list_sz = _list.size();
        size_t list_end = list_sz;
        size_t found = _list.find_last_of(_sep);
        size_t next = found;
        while (_list[next + 1] == ' ') next++;
        while (found != std::string::npos)
        {
            list_end = _list.find_last_of("(");
            if (list_end == std::string::npos)
                list_end = list_sz;
            else
                list_end -= 1;
            v_list->push_back(_list.substr(next + 1, list_end - next - 1));
            list_sz = found;
            _list.resize(found);
            found = _list.find_last_of(_sep);
            next = found;
            while (_list[next + 1] == ' ') next++;
        }
        list_end = _list.find_last_of("(");
        if (list_end == std::string::npos)
            list_end = list_sz;
        else
            list_end--;
        v_list->push_back(_list.substr(0, list_end));
    }
}

