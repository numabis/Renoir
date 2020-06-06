#include "ApiJson.h"
#include "Convert.h"
#include "definitions.h"
#include "database.h"
//#include "omdb.h"
#include "Util.h"
#include "configManager.h"

#include <fstream>

#define NODE_ROOT "root"
#define NODE_MOVIE "movie"
#define NODE_RESULT "result"
#define NODE_ERROR "error"
#define ATTR_RESPONSE "response"
#define ATTR_TOTALRESULTS "totalResults"

std::string movieJsonAttr[] = { "title", "year", "rated", "released", "runtime", "genre", "director", "writer", "actors", "plot", "language", "country", "awards", "poster", "metascore", "imdbRating", "imdbVotes", "imdbID", "type" };
std::string ApiSearchAttr[API_NBATTRIBUTES] = { APIATRTIBUTES };
//std::vector<Movie> vectorMovies;

ApiJson::ApiJson()
{
    init();
}

ApiJson::ApiJson(std::string _json)
{
    init();
    jsonStr = _json;
}

void ApiJson::init(void)
{
    response = false;
    totalResults = 0;
}


void ApiJson::setJson(std::string _json)
{
    jsonStr = _json;
    jsonStruct = json::parse(_json);
}
/*{ 
"searchType":"Movie",
"expression":"CORTO MALTESE La ballade de la mer salee",
"results":[
           {"id":"tt1098199",
           "resultType":"Title",
           "image":"https://imdb-api.com/images/original/MV5BNDM0YzFiMzItZDMxOC00YjIyLThiNTktZWU1MGYwMmRhNWY3XkEyXkFqcGdeQXVyNjU3ODUxMTc@._V1_Ratio0.7273_AL_.jpg",
           "title":"Corto Maltese: The Ballad of the Salt Sea","description":"(2002)"}
          ],"errorMessage":""} */

ApiJson::~ApiJson(void)
{
}

bool ApiJson::readOmdbError(MovieFile *_file)
{ // //<root response="False"><error>Movie not found!</error></root>
    return false;
}

bool ApiJson::parseJson()
{
    bool ret = false;
    return ret;

}
void ApiJson::clearSearchResults(void)
{
    searchResults.clear();
}
int ApiJson::parseSearchResults(std::string _json)
{
    int res = 0;
    jsonStr = _json;
    jsonStruct = json::parse(_json);
    
    //std::string searchType = jsonStruct["searchType"];
    //std::string expression = jsonStruct["expression"];
    json results = jsonStruct["results"];
    res = results.size();
    //json val = results.value();
    for (auto result = results.begin(); result != results.end(); ++result)
    {
        // the value
        json &val = result.value();
        
        bool addResult = true;
        apiSearchValues tmpResults;
        for (auto field = val.begin(); field != val.end(); ++field)
        {
            std::string key = field.key();
            std::string val = field.value();
            
            transform(key.begin(), key.end(), key.begin(), ::tolower);
            
            for (int attr = 0; attr < API_NBATTRIBUTES; attr++)
            {
                if (key.compare(ApiSearchAttr[attr]) == 0)
                {
                    
                    if (attr == API_IMDBID)
                    {
                        std::vector<apiSearchValues>::iterator it;
                        for (it = searchResults.begin(); it != searchResults.end(); it++)
                        {
                            if (it->values[API_OMDBID] == val)
                            {
                                addResult = false;
                                break;
                            }
                        }
                        tmpResults.values[API_OMDBID] = val;
                    }
                        
                    else if (attr == API_IMAGE)
                        tmpResults.values[API_POSTER] = val;
                    else if (attr == API_DESCRIPTION)
                        tmpResults.values[XMLS_TYPE] = val;
                    else
                        tmpResults.values[attr] = val;

                    tmpResults.isFromJson = true;
                    break;
                }
            }
            if (addResult == false)
                break;
        }     

        if (addResult)
        {
            searchResults.push_back(tmpResults);
        }
    }

    return res;
}
int ApiJson::parseSearchResults()
{
    int parcialResults = 0;

    return parcialResults;
}

int ApiJson::readTotalResults(std::string _json)
{
    jsonStr = _json;

    return readTotalResults();
}

int ApiJson::readTotalResults()
{
    totalResults = 0;
    return totalResults;
}

bool ApiJson::readResponse(std::string _json)
{
    jsonStr = _json;
    return readResponse();
}

bool ApiJson::readResponse()
{
    response = false;
    return response;
}

bool ApiJson::getResponse()
{
    return response;
}

int ApiJson::getTotalResults()
{
    return totalResults;
}

std::vector <apiSearchValues> * ApiJson::getSearchResults()
{
    return &searchResults;
}

std::string ApiJson::movieToJson(Movie _movie)
{
    std::stringstream str;
    std::string jsonValues[XML_NBATTRIBUTES];
    _movie.movie2string(jsonValues);
    return str.str();
}

bool ApiJson::loadMoviesFromDB()
{
    MovieFile file;
    return false;
}

bool  ApiJson::readJSON(std::string _json)
{
    exLOGERROR("JSON not implemented");
    return false;
}

bool ApiJson::readApiJson(MovieFile *_file)
{
    bool ret = false;
    return ret;
}
