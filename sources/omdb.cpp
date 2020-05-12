#include <iostream>
#include <ostream>
#include <string>
//#include <map>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "curl_easy.h"
#include "curl_form.h"
#include "curl_ios.h"
#include "curl_exception.h"

#include "omdb.h"
#include "Convert.h"
#include "configManager.h"

#define WOMDBKEY     L"apikey="
#define WOMDBURL     L"http://www.omdbapi.com/?"
#define WOMDBTITLE   L"t="
#define WOMDBTYPE    L"type=" //movie, series, episode
#define WOMDBYEAR    L"y="
#define WOMDBRET     L"r=" //  json, xml
#define WOMDBIMDB    L"i="

#define OMDBKEY "apikey="s
#define OMDBURL "http://www.omdbapi.com/?"
#define OMDBTITLE   "t="s
#define OMDBTYPE    "type="s //movie, series, episode
#define OMDBYEAR    "y="s
#define OMDBRET     "r="s //  json, xml
#define OMDBIMDB    "i="s


Omdb::Omdb()
{

}

Omdb::~Omdb(void)
{
}

//void Omdb::init()
//{
//    //omdbConfig = GETCM.getOmdbConfig();
//    omdbConfig.apikey = GETCM.getConfigStr(CONF_OMDB_APIKEY);
//    omdbConfig.omdbUrl = GETCM.getConfigStr(CONF_OMDB_URL);
//    omdbConfig.apiRequestUrl = GETCM.getConfigStr(CONF_OMDB_APIREQUEST);
//    omdbConfig.type = GETCM.getConfigStr(CONF_OMDB_TYPE);
//    omdbConfig.plot = GETCM.getConfigStr(CONF_OMDB_PLOT);
//    omdbConfig.separator = GETCM.getConfigStr(CONF_OMDB_LISTSEPARATOR);
//    omdbConfig.testmode = (GETCM.getConfigInt(CONF_OMDB_TESTMODE) == 1);
//    //omdbConfig.typeXml = GETCM.getConfigStr("CONF_OMDB_APIKEY");
//    //omdbConfig.plotShort = GETCM.getConfigStr("CONF_OMDB_APIKEY");
//}

//void Omdb::init(OMDB_CONFIG *_config)
//{
//    omdbConfig = _config;
//}

std::stringstream Omdb::get_response(std::wstring url)
{
    return get_response(WS2S(url));
}

std::stringstream Omdb::get_response(std::string url)
{
    std::stringstream str;
    curl::curl_ios<std::stringstream> writer(str);
    curl::curl_easy easy(writer);

    easy.add<CURLOPT_URL>(url.data());
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);

    try
    {
        easy.perform();
    }
    catch (curl::curl_easy_exception error)
    {
        auto errors = error.get_traceback();
        error.print_traceback();
    }

    return str;
}

void Omdb::searchByImdb(MovieFile* _file)
{
    using namespace std::string_literals;

    std::string url = GETCM.getConfigStr(CONF_OMDB_URL);
    url.append("?" + OMDBKEY + GETCM.getConfigStr(CONF_OMDB_APIKEY));
    url.append("&" + OMDBRET + GETCM.getConfigStr(CONF_OMDB_TYPE));
    url.append("&" + OMDBIMDB + _file->imdbId);

    if (GETCM.getConfigStr(CONF_OMDB_URL).empty())
    {
        wchar_t wmsg[256];
        //char* msg = BUTIL::Util::GetLastErrorAsString(error);
        _swprintf(wmsg, L"Wrong URL : %s\nCheck config file", BUTIL::Convert::charToWchar((LPSTR)url.c_str()));
        MessageBox(NULL, wmsg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        exLOGERROR("Wrong url : %s", url.c_str());
    }
    else
    {
        auto omdb = get_response(url);
        _file->setOmdbXml(omdb.str(), true);
        exLOGINFO("OmdbRequest:%s", url.c_str());

    }
}

#ifdef WSTRINGS
std::wstring Omdb::search(std::wstring _imdbtt)
{
    using namespace std::string_literals;

    std::string url = omdbConfig->url;
    //url.append(L"?" + WOMDBKEY + omdbConfig->apikey);
    //url.append(L"&" + WOMDBRET + omdbConfig->type);
    //url.append(L"&" + WOMDBIMDB + _imdbtt);
    //
    auto resp = get_response(url);
    return BUTIL::Convert::string2wstring(resp.str());

}
#endif
void Omdb::search(MovieFile *file)
{
    if (file->isImdbId())
        searchByImdb(file);
    else
        searchByName(file);
}
#ifdef WSTRINGS
std::wstring Omdb::search(std::wstring _title, short _year)
{
    using namespace std::string_literals;
    //std::wstring url = omdbConfig->url;
    //url.append(L"?" + OMDBKEY + omdbConfig->apikey);
    //url.append(L"&" + OMDBRET + omdbConfig->type);
    //BUTIL::Convert::wstring2url(&_title);
    //url.append(L"&" + OMDBTITLE + _title);
    //if (_year != 0) {
    //    url.append(L"&" + OMDBYEAR + std::to_wstring(_year));
    //}


    //url.append("&" + OMDBKEY + OmdbConfig->apikey);
    //std::string urlFormat = BUTIL::Convert::string2url(url);

    auto json = get_response(url);

    return json.str();

}
#endif

void Omdb::searchByName(MovieFile *file)
{
    //std::string _title;
    //short _year;

    using namespace std::string_literals;
    std::string url = GETCM.getConfigStr(CONF_OMDB_URL);

    if (GETCM.getConfigBool(CONF_OMDB_TESTMODE) == false)
        url.append("?" + OMDBKEY + GETCM.getConfigStr(CONF_OMDB_APIKEY));
    else
        url.append("?" + OMDBKEY + "");
    url.append("&" + OMDBRET + GETCM.getConfigStr(CONF_OMDB_TYPE));
    BUTIL::Convert::string2url(&file->title);
    url.append("&" + OMDBTITLE + file->title);
    if (file->year != 0) {
        url.append("&" + OMDBYEAR + std::to_string(file->year));
    }

    if (GETCM.getConfigStr(CONF_OMDB_URL).empty())
    {
        wchar_t wmsg[256];
        //char* msg = BUTIL::Util::GetLastErrorAsString(error);
        _swprintf(wmsg, L"Wrong URL : %s\nCheck config file", BUTIL::Convert::charToWchar((LPSTR)url.c_str()));
        MessageBox(NULL, wmsg, (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
        exLOGERROR("Wrong url : %s", url.c_str());
    } 
    else
    {
        auto omdb = get_response(url);
        file->setOmdbXml(omdb.str(), true);
        exLOGINFO("OmdbRequest:%s", url.c_str());

    }
}

