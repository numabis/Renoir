#include <iostream>
#include <ostream>
#include <string>
#include <math.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "curl_easy.h"
#include "curl_form.h"
#include "curl_ios.h"
#include "curl_exception.h"

#include "ImdbApi.h"
#include "Convert.h"
#include "Util.h"
#include "configManager.h"
#include "movieFile.h"

#define URLFIRST   "?"
#define URLNEXT    "&"
#define URLTITLEUNION "+"

using namespace std::string_literals;

ImdbApi::ImdbApi()
{
    limit = 15;
}

ImdbApi::~ImdbApi(void)
{
}

bool ImdbApi::readImdbApiResult(MovieFile *_file)
{
    //return IMDbAPIXml.readIMDbAPIXml(_file);
    return false;
}

// https://imdb-api.com/en/API/SearchMovie/k_9ZnN7HB8/
std::string ImdbApi::getSearchUrl()
{ 
    std::string url = GETCM.getConfigStr(CONF_IMDBAPI_QUERY_URL) + "/" + GETCM.getConfigStr(CONF_IMDBAPI_QUERY_REQUEST) + "/" + GETCM.getConfigStr(CONF_IMDBAPI_QUERY_SEARCH_MOVIE) + "/" + GETCM.getConfigStr(CONF_IMDBAPI_APIKEY) + "/";

    if (url.empty())
    {
        exLOGERROR("ERROR ImdbApi URL EMPTY");
        MessageBox(NULL, L"ERROR ImdbApi URL EMPTY", (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);

        return std::string();
    }

    return url;
}
// https://imdb-api.com/en/API/Title/k_9ZnN7HB8/tt1375666/
std::string ImdbApi::getMovieUrl()
{
    std::string url = GETCM.getConfigStr(CONF_IMDBAPI_QUERY_URL) + "/" + GETCM.getConfigStr(CONF_IMDBAPI_QUERY_REQUEST) + "/" + GETCM.getConfigStr(CONF_IMDBAPI_QUERY_GETMOVIE) + "/" + GETCM.getConfigStr(CONF_IMDBAPI_APIKEY) + "/";

    if (url.empty())
    {
        exLOGERROR("ERROR ImdbApi URL EMPTY");
        MessageBox(NULL, L"ERROR ImdbApi URL EMPTY", (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);

        return std::string();
    }

    return url;
}

std::stringstream ImdbApi::get_response(std::string url)
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

int ImdbApi::searchRequest(MovieFile *_file)
{
    imdbJson.clearSearchResults();
    int results = searchByTitle(_file);
    std::vector <apiSearchValues> * v_search = imdbJson.getSearchResults();
    /*else*/ if (imdbApiRes == true)
    {
        _file->addSearchResults(v_search);
    }
    if (results == 1 && v_search->size()>0)
    {
        apiSearchValues movie = v_search->back();
        v_search->pop_back();
        _file->imdbId = movie.values[API_OMDBID];
        //results = getByImdb(_file);
    }

    return results;
}

int ImdbApi::request(MovieFile *_file)
{
    int results = 0;
    imdbApiRes = false;
    std::string request = getRequest(_file);


    if (request.empty() == false)
    {
        auto ImdbApiResp = get_response(request);
        //IMDbAPIRes = IMDbAPIXml.readResponse(IMDbAPIResp.str());
        if (imdbApiRes == true)
        {
            //_file->setIMDbAPIXml(IMDbAPIResp.str(), true);
            exLOGINFO("ImdbApiRequest:%s", request.c_str());
            results = 1;
        }
    }

    return results;
}
// https://imdb-api.com/en/API/Title/k_9ZnN7HB8/tt1375666/
int ImdbApi::getByImdb(MovieFile* _file)
{
    int results = 0;

    std::string url = getMovieUrl();

    url += _file->imdbId;

    if (url.empty())
    {
        MBOX("Wrong URL : " + url + "\nCheck config file", "Error", MB_ICONERROR | MB_OK);
        exLOGERROR("Wrong url : %s", url.c_str());
    }
    else
    {
        auto ImdbApi = get_response(url);
        _file->setJsonStr(ImdbApi.str(), true);
        exLOGINFO("ImdbApiRequest:%s", url.c_str());
        results = 1;

    }
    return results;
}

//int ImdbApi::getByTitle(MovieFile *file)
//{
//    std::string url = getComonUrl();
//    std::string title = file->title;
//    int results = 0;
//
//    if (url.empty() == false)
//    {
//        
//        BUTIL::Convert::string2url(&title);
//        url.append(URLNEXT + GETCM.getConfigStr(CONF_IMDbAPI_QUERY_TITLE) + title);
//
//        if (file->year != 0) {
//            url.append(URLNEXT + GETCM.getConfigStr(CONF_IMDbAPI_QUERY_YEAR) + std::to_string(file->year));
//        }
//
//        auto IMDbAPI = get_response(url);
//
//        results = IMDbAPIXml.readTotalResults(IMDbAPI.str());
//
//        file->setIMDbAPIXml(IMDbAPI.str(), true);
//        exLOGINFO("IMDbAPIRequest:%s", url.c_str());
//
//    }
//
//    return results;
//
//}

#define MIN_WORD_SIZE 3
#define MIN_WORDS 1

int ImdbApi::searchByTitle(MovieFile * file)
{
    std::string query;
    std::string url = getSearchUrl();
    std::string title,year;
    size_t minWordSz = MIN_WORD_SIZE;
    size_t minWords = MIN_WORDS;
    bool finished = false;
    int results = 0;

    std::vector<std::string> v_title;

    if (url.empty())
    {
        file->setJsonStr("", false);
        exLOGERROR("Wrong url : %s", url.c_str());
        MBOX("Wrong URL : " + url + "\nCheck config file", "Error", MB_ICONERROR | MB_OK);

        return results;
    }

    title = file->title;


    BUTIL::Convert::string2url(&title);
    query = url + title;// +"%20" + year;
    if (file->year != 0) {
        query += "%20" + std::to_string(file->year);
    }
    exLOGINFO("ImdbApiRequest:%s", query.c_str());
    auto jsonStr = get_response(query);

    std::string jsonResp = jsonStr.str();

    if (jsonResp[0] == '{')
    {
        file->setJsonStr(jsonStr.str(), true);
        results = imdbJson.parseSearchResults(jsonStr.str());
        if (results>0)
            imdbApiRes = true;
    }



    return results;
}

std::string ImdbApi::getRequest(MovieFile * _file)
{
    std::string url = getMovieUrl();

    if (url.empty() == false)
    {
        if (_file->isImdbId())
        {
            url.append(_file->imdbId);
        }
    }
    return url;
}

int ImdbApi::getTotalResults()
{
    return totalResults;
}

int ImdbApi::getLimit()
{
    return limit;
}

void ImdbApi::setLimit(int _limit)
{
    if (_limit > 0 && _limit < 10)
        _limit = 10;
    limit = _limit;
}

void ImdbApi::setThreadRunning(bool _val)
{
    threadRunning = _val;
}

int ImdbApi::search(MovieFile * _file)
{
    std::string url = getSearchUrl();
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
            year = std::to_string(_file->year);
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


        //IMDbAPIXml.clearSearchResults();

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

            query = url + titleSearch + " " + year;
            exLOGINFO("ImdbApiRequest:%s", query.c_str());
            auto ImdbApiResp = get_response(query);
            bool localImdbApiRes = false; // IMDbAPIXml.readResponse(IMDbAPIResp.str());
            if (localImdbApiRes == true)
            {
                imdbApiRes = true;
                //int localResults = IMDbAPIXml.readTotalResults();
                //partialResults = IMDbAPIXml.parseSearchResults();
                totalResults += partialResults;

            }

            if (limit > 0 && totalResults > limit)
            {
                finished = true;
            }
        }
    }
    return totalResults;
}
