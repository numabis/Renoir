#include <iostream>
#include <ostream>
#include <string>
#include <math.h>
//#include <map>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "curl_easy.h"
#include "curl_form.h"
#include "curl_ios.h"
#include "curl_exception.h"

#include "omdbApi.h"
#include "Convert.h"
#include "Util.h"
#include "configManager.h"
#include "movieFile.h"
//#include "ApiXML.h"

#define URLFIRST   "?"
#define URLNEXT    "&"
#define URLTITLEUNION "+"

using namespace std::string_literals;

ApiXML::ApiXML()
{
    limit = 15;
}

ApiXML::~ApiXML(void)
{
}

bool ApiXML::parseOmdbXml(MovieFile *_file)
{
    return omdbXml.readXml(_file);
}

/// Return URL 
/// + APIKEY
/// + RETURN TYPE (json/xml)
/// + PLOT TYPE (short/full)
std::string ApiXML::getComonUrl()
{
    std::string url = GETCM.getConfigStr(CONF_OMDB_QUERY_URL);

    if (url.empty())
    {
        exLOGERROR("ERROR OMDB URL EMPTY");
        MessageBox(NULL, L"ERROR OMDB URL EMPTY", (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);

        return std::string();
    }

    if (GETCM.getConfigBool(CONF_OMDB_TESTMODE) == false)
        url.append(URLFIRST + GETCM.getConfigStr(CONF_OMDB_QUERY_APIKEY) + GETCM.getConfigStr(CONF_OMDB_APIKEY));
    else
        url.append(URLFIRST + GETCM.getConfigStr(CONF_OMDB_QUERY_APIKEY) + "12345");

    std::string type = GETCM.getConfigStr(CONF_OMDB_RETURN_TYPE);
    if (type.empty() == false)
        url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_RETURN_TYPE) + type);

    std::string plot = GETCM.getConfigStr(CONF_OMDB_PLOT);
    if (plot.empty() == false)
        url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_PLOT) + plot);

    std::string MovieType = GETCM.getConfigStr(CONF_OMDB_MOVIE_TYPE);
    if (MovieType.empty() == false)
        url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_MOVIE_TYPE) + MovieType);

    return url;
}

std::stringstream ApiXML::get_response(std::string url)
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

int ApiXML::searchRequest(MovieFile *_file)
{
    int results = search(_file);
    std::vector <apiSearchValues> * v_search = omdbXml.getSearchResults();
    /*else*/ if (omdbRes == true)
    {
        _file->addSearchResults(v_search);
    }
    if (results == 1 && v_search->size()>0)
    {
        apiSearchValues movie = v_search->back();
        v_search->pop_back();
        _file->imdbId = movie.values[API_OMDBID];
        results = getByImdb(_file);
    }

    return results;
}

int ApiXML::request(MovieFile *_file)
{
    int results = 0;
    omdbRes = false;
    std::string request = getRequest(_file);

    //bool doSearchRequest = true;
    bool doSearchRequest = false;

    if (request.empty() == false)
    {
        auto omdbResp = get_response(request);
        omdbRes = omdbXml.readResponse(omdbResp.str());
        if (omdbRes == true)
        {
            doSearchRequest = false;
            _file->setXmlStr(omdbResp.str(), true);
            exLOGINFO("OmdbRequest:%s", request.c_str());
            results = 1;
        }
    }
   
    return results;
}

int ApiXML::getByImdb(MovieFile* _file)
{
    int results = 0;

    std::string url = getComonUrl();

    url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_IMDBID) + _file->imdbId);

    if (GETCM.getConfigStr(CONF_OMDB_QUERY_URL).empty())
    {
        MBOX("Wrong URL : " + url + "\nCheck config file", "Error", MB_ICONERROR | MB_OK);
        exLOGERROR("Wrong url : %s", url.c_str());
    }
    else
    {
        auto omdb = get_response(url);
        _file->setXmlStr(omdb.str(), true);
        exLOGINFO("OmdbRequest:%s", url.c_str());
        results = 1;

    }
    return results;
}

int ApiXML::getByTitle(MovieFile *file)
{
    std::string url = getComonUrl();
    std::string title = file->title;
    int results = 0;

    if (url.empty() == false)
    {
        
        BUTIL::Convert::string2url(&title);
        url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_TITLE) + title);

        if (file->year != 0) {
            url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_YEAR) + std::to_string(file->year));
        }

        auto omdb = get_response(url);

        results = omdbXml.readTotalResults(omdb.str());

        file->setXmlStr(omdb.str(), true);
        exLOGINFO("OmdbRequest:%s", url.c_str());

    }

    return results;

}

#define MIN_WORD_SIZE 3
#define MIN_WORDS 1

int ApiXML::searchByTitle(MovieFile * file)
{
    std::string query;
    std::string url = GETCM.getConfigStr(CONF_OMDB_QUERY_URL);
    std::string key,year;
    size_t minWordSz = MIN_WORD_SIZE;
    size_t minWords = MIN_WORDS;
    bool finished = false;
    int results = 0;

    std::vector<std::string> v_title;

    if (url.empty())
    {
        file->setXmlStr("", false);
        exLOGERROR("Wrong url : %s", url.c_str());
        MBOX("Wrong URL : " + url + "\nCheck config file", "Error", MB_ICONERROR | MB_OK);

        return results;
    }

    if (GETCM.getConfigBool(CONF_OMDB_TESTMODE) == false)
        key = URLFIRST + GETCM.getConfigStr(CONF_OMDB_QUERY_APIKEY) + GETCM.getConfigStr(CONF_OMDB_APIKEY);
    else
        key = URLFIRST + GETCM.getConfigStr(CONF_OMDB_QUERY_APIKEY) + "12345";

    if (file->year != 0) {
        year = URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_YEAR) + std::to_string(file->year);
    }

    BUTIL::Convert::separateValues(&v_title, file->title, " ");

    std::vector<std::string>::iterator it;

    for (it = v_title.begin(); it != v_title.end(); it++)
    {
        if (it->size() < minWordSz)
        {
            v_title.erase(it);
        }
    }

    int nbLoop = (int)exp2(v_title.size()) - 1 ;

    for (; nbLoop > 0; nbLoop--)
    {
        if (finished == true)
            break;
        int wordInd = 1;
        std::string titleSearch = "";
        for (it = v_title.begin(); it != v_title.end(); it++)
        {
            int wordInLoop = (int)exp2(wordInd-1);
            if (nbLoop & wordInLoop)
            {
                if (titleSearch.empty() == false)
                    titleSearch += URLTITLEUNION;
                titleSearch += *it;
            }
        }
        query = url + key + titleSearch + year;
        exLOGINFO("OmdbRequest:%s", query.c_str());
        auto xml = get_response(query);

        //omdbXml.setXML(xml.str());

        int results = omdbXml.readTotalResults(xml.str());

        finished = (results>0);

        if (finished)
            file->setXmlStr(xml.str(), false);
    }   
    return results;
}

std::string ApiXML::getRequest(MovieFile * _file)
{
    std::string url = getComonUrl();

    if (url.empty() == false)
    {
        if (_file->isImdbId())
        {
            url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_IMDBID) + _file->imdbId);
        }
        else
        {
            std::string title = _file->title;
            BUTIL::Convert::string2url(&title);
            url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_TITLE) + title);

            if (_file->year != 0) 
            {
                url.append(URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_YEAR) + std::to_string(_file->year));
            }
        }
    }
    return url;
}

int ApiXML::getTotalResults()
{
    return totalResults;
}

int ApiXML::getLimit()
{
    return limit;
}

void ApiXML::setLimit(int _limit)
{
    if (_limit > 0 && _limit < 10)
        _limit = 10;
    limit = _limit;
}

void ApiXML::setThreadRunning(bool _val)
{
    threadRunning = _val;
}

int ApiXML::search(MovieFile * _file)
{
    std::string url = getComonUrl();
    int partialResults = 0;
    totalResults = 0;

    if (url.empty() == false)
    {
        std::string query;
        std::string year, pageRequest;
        size_t minWordSz = MIN_WORD_SIZE;
        size_t minWords = MIN_WORDS;
        bool finished = false;


        std::vector<std::string> v_title;

        if (_file->year != 0) {
            year = URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_YEAR) + std::to_string(_file->year);
        }

        BUTIL::Convert::separateValues(&v_title, _file->title, " ");

        std::string plusWords;

        std::vector<std::string>::iterator it;

        for (it = v_title.begin(); it != v_title.end(); )
        {
            if ((*it)[0] == '+')
            {
                plusWords += *it;
                it = v_title.erase(it);
            }
            else
            {
                size_t sz = it->length();
                if (sz < minWordSz)
                {
                    it = v_title.erase(it);
                }
                else
                    it++;
            }

        }


        omdbXml.clearSearchResults();

        int page = 1;
        bool nextPage = true;
        while (nextPage)
        {
            nextPage = false;
            int nbLoop = (int)exp2(v_title.size()) - 1;

        for (; nbLoop > 0; nbLoop--)
        {
            if (finished == true)
                break;
            int wordInd = 1;
            std::string titleSearch = "";
            for (it = v_title.begin(); it != v_title.end(); it++)
            {
                int wordInLoop = (int)exp2(wordInd - 1);
                if (nbLoop & wordInLoop)
                {
                    if (titleSearch.empty() == false)
                        titleSearch += URLTITLEUNION;
                    titleSearch += *it;
                }
                wordInd++;
            }
            titleSearch += plusWords;

                pageRequest = URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_PAGE) + std::to_string(page);
                query = url + URLNEXT + GETCM.getConfigStr(CONF_OMDB_QUERY_SEARCH) + titleSearch + year + pageRequest;
                exLOGINFO("OmdbRequest:%s", query.c_str());
                auto omdbResp = get_response(query);
                bool localOmdbRes = omdbXml.readResponse(omdbResp.str());
                if (localOmdbRes == true)
                {
                    omdbRes = true;
                    int localResults = omdbXml.readTotalResults();
                    partialResults = omdbXml.parseSearchResults();
                    totalResults += partialResults;
                    if (localResults >= 10)
                    {
                        nextPage = true;
                        page++;
                    }

                }

                if (limit > 0 && totalResults > limit)
                {
                    nextPage = false;
                    finished = true;
                }
            }

        }
    }

    return totalResults;
}

