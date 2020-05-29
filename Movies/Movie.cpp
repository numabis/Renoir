#include "Movie.h"
#include "movieFile.h"
#include "util.h"
#include "convert.h"
#include "database.h"
#include "configManager.h"
#include <algorithm>
#include <curl/curl.h>

using std::string;
using std::vector;

Movie::Movie()
{
    idMovie     = -1 ;
    year        =  0 ;
    imdbRating  =  0 ;
    imdbVotes   =  0 ;
    metascore   =  0 ;
    for (int type= 0; type<TYPE_MAX;type++)
        typeIs[type] = false;
    separator   = DEFAULTSEP ;
}

Movie::Movie(std::string* _xml)
{
    idMovie = -1;
    initValues(_xml);
}

Movie::Movie(std::string* _xml, std::string _sep)
{
    idMovie = -1;
    initValues(_xml, _sep);
}

Movie::Movie(MovieFile _file)
{
    initValuesFromFile(_file);
}

Movie::~Movie()
{
}

void Movie::initValues(std::string*_data, std::string _sep)
{
    separator = _sep;
    initValues(_data);
}

void Movie::initValuesFromFile(MovieFile _file)
{
    idMovie = -1;
    title = _file.title;
    year = _file.year;
    imdbId = _file.imdbId;
    imdbRating = 0;
    imdbVotes = 0;
    metascore = 0;
    separator = DEFAULTSEP;
    for (int type = 0; type<TYPE_MAX; type++)
        typeIs[type] = false;
}

void Movie::setSerie()
{
    if (type.compare(strType[TYPE_SERIE]) == 0 || type.compare(EPISODE) == 0)
        typeIs[TYPE_SERIE] = true;
    else
        typeIs[TYPE_SERIE] = false;
}

void Movie::setTypes(movieInfoTypes _type)
{
    vector<string>::iterator it;

    it = find(v_genres.begin(), v_genres.end(), strType[_type]);
    if (it != v_genres.end())
        typeIs[_type] = true;
    else
        typeIs[_type] = false;
}

//void Movie::setAnim()
//{
//    setTypes(TYPE_ANIM);
//}
//void Movie::setDoc()
//{
//    setTypes(TYPE_DOC);
//}
//void Movie::setShort()
//{
//    setTypes(TYPE_SHORT);
//}

void Movie::initValues(std::string*_data)
{
    idMovie = -1;
    title = _data[XML_TITLE];
    titleASCII = _data[XML_TITLE]; ;
    BUTIL::Convert::normalize(&titleASCII);
    year = atoi(_data[XML_YEAR].c_str());
    rated = _data[XML_RATED];
    runtime = _data[XML_RUNTIME];
    separateValues(&v_genres, _data[XML_GENRE]);
    separateValues(&v_roles[ROLES_DIRECTOR], _data[XML_DIRECTORS]);
    separateValues(&v_roles[ROLES_WRITER  ], _data[XML_WRITERS  ]);
    separateValues(&v_roles[ROLES_ACTOR   ], _data[XML_ACTORS   ]);
    plot = _data[XML_PLOT];
    language = _data[XML_LANGUAGE];
    country = _data[XML_COUNTRY];
    awards = _data[XML_AWARDS];
    poster = _data[XML_POSTER];
    metascore = atoi(_data[XML_METASCORE].c_str());
    setImdbRating(STR2INT(_data[XML_IMDBRATING], "."));
    imdbVotes = STR2INT(_data[XML_IMDBVOTES], ",");
    imdbId = _data[XML_IMDBID];
    type = _data[XML_TYPE];
    setSerie();
    for (int type = TYPE_ANIM; type < TYPE_MAX; type++)
        setTypes((movieInfoTypes)type);
}

void Movie::movie2string(std::string* _data)
{
    _data[XML_TITLE] = title;
    _data[XML_YEAR] = std::to_string(year);
    _data[XML_RATED] = rated;
    _data[XML_RUNTIME] = runtime;
    _data[XML_GENRE] = vector2std(v_genres, getSeparator());
    _data[XML_DIRECTORS] = vector2std(v_roles[ROLES_DIRECTOR], getSeparator());
    _data[XML_WRITERS] = vector2std(v_roles[ROLES_WRITER], getSeparator());
    _data[XML_ACTORS] = vector2std(v_roles[ROLES_ACTOR], getSeparator());
    _data[XML_PLOT] = plot;
    _data[XML_LANGUAGE] = language;
    _data[XML_COUNTRY] = country;
    _data[XML_AWARDS] = awards;
    _data[XML_POSTER] = poster;
    _data[XML_METASCORE] = std::to_string(metascore);
    _data[XML_IMDBRATING] = BUTIL::Util::formatNumber1Dec(imdbRating/10.0);
    _data[XML_IMDBVOTES] = std::to_string(imdbVotes);
    _data[XML_IMDBID] = imdbId;
    _data[XML_TYPE] = type;
}

bool Movie::compare(Movie _movie)
{
    if (imdbId.size() != 0 && _movie.imdbId.size() != 0)
    {
        if (imdbId == _movie.imdbId)
            return true;
        else
            return false;
    }

    if (titleASCII.size() != 0 && _movie.titleASCII.size() != 0)
    {
        if (titleASCII == _movie.titleASCII)
        {
            if (year != 0 && _movie.year != 0)
            {
                if (year != _movie.year)
                    return false;
            }
            return true;
        }
    }

    if (title.size() != 0 && _movie.title.size() != 0)
    {
        if (title == _movie.title)
        {
            if (year != 0 && _movie.year != 0)
            {
                if (year != _movie.year)
                    return false;
            }

            return true;
        }
        else
            return false;
    }
    return false;

}

int Movie::getIdMovie() {
    if (idMovie < 0)
        GETDB.MOVIES_selectId(this);
    return idMovie;
}
std::string Movie::getInfo(movieInfo _info)
{
    switch (_info)
    {
    case INFO_TITLE:
        return getTitle();
        break;
    case INFO_RUNTIME:
        return getRuntime();
        break;
    case INFO_GENRES:
        return getGenres();
        break;
    case INFO_DIRECTORS:
        return getRoles(ROLES_DIRECTOR);
        break;
    case INFO_WRITERS:
        return getRoles(ROLES_WRITER);
        break;
    case INFO_ACTORS:
        return getRoles(ROLES_ACTOR);
        break;
    case INFO_PLOT:
        return getPlot();
        break;
    };
    return "";
}
std::string Movie::getTitle()
{
    return title;
}
std::string Movie::getTitleASCII()
{
    BUTIL::Convert::normalize(&titleASCII);
    return titleASCII;
}
int Movie::getYear()
{
    return year;
}
std::string Movie::getRated()
{
    return rated;
}
std::string Movie::getRuntime()
{
    return runtime;
}
std::string Movie::getGenres()
{
    return vector2std(v_genres, NEWLINE);
}
std::vector<std::string> *Movie::getvGenres()
{
    return &v_genres;
}
std::string Movie::getRoles(ROLES_PERSONS _role)
{
    return vector2std(v_roles[_role], NEWLINE);
}
std::vector<std::string>* Movie::getvRoles(ROLES_PERSONS _role)
{
    return &v_roles[_role];
}
std::string Movie::getPlot()
{
    return plot;
}
std::string Movie::getLanguage() 
{
    return language;
}
std::string Movie::getCountry() 
{
    return country;
}
std::string Movie::getAwards() 
{
    return awards;
}
std::string Movie::getPoster() 
{
    return poster;
}
int Movie::getMetascore() 
{
    return metascore;
}
int Movie::getImdbRating() 
{
    if (imdbRating != 0)
    {
        while (imdbRating > 100)
            imdbRating /= 10;
        while (imdbRating < 10)
            imdbRating *= 10;
    }
    return imdbRating;
}
int Movie::getImdbVotes() 
{
    return imdbVotes;
}
std::string Movie::getImdbId()
{
    return imdbId;
}
std::string Movie::getType() 
{
    return type;
}

std::string Movie::vector2std(std::vector<std::string> _vector, std::string _separator)
{
    size_t i;
    std::string ret;
    size_t nb = _vector.size();
    for (i = 0; i < nb; i++)
        if (i < nb - 1)
            ret += _vector[i] + _separator;
        else
            ret += _vector[i];
    return ret;
    return _vector[0];
}

void Movie::separateValues(std::vector<std::string> *v_list, std::string _list) {
    v_list->clear();
    size_t list_sz = _list.size();
    size_t list_end = list_sz;
    size_t found = _list.find_last_of(getSeparator());
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
        found = _list.find_last_of(getSeparator());
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

void Movie::setIdMovie(int _idMovie)
{
    idMovie = _idMovie;
}
void Movie::setTitle(std::string _title)
{
    title = _title;
    setTitleASCII(_title);
}
void Movie::setTitle(SAString _title)
{
    title = _title;
    setTitleASCII(_title);
}
void Movie::setTitleASCII(std::string _titleASCII)
{
    titleASCII = _titleASCII;
    BUTIL::Convert::normalize(&titleASCII);
}
void Movie::setTitleASCII(SAString _titleASCII)
{
    titleASCII = _titleASCII;
    BUTIL::Convert::normalize(&titleASCII);
}
void Movie::setYear(int _year)
{
    year = _year;
}
void Movie::setRated(std::string _rated)
{
    rated = _rated;
}
void Movie::setRated(SAString _rated)
{
    rated = _rated;
}
void Movie::setRuntime(std::string _runtime)
{
    runtime = _runtime;
}
void Movie::setRuntime(SAString _runtime)
{
    runtime = _runtime;
}
void Movie::setGenres(std::vector<std::string> _vector)
{
    v_genres = _vector;
    for (int type = TYPE_ANIM; type < TYPE_MAX; type++)
        setTypes((movieInfoTypes)type);
}
void Movie::setRoles(std::vector<std::string> _vector, ROLES_PERSONS _role)
{
    v_roles[_role] = _vector;
}
void Movie::setPlot(std::string _plot)
{
    plot = _plot;
}
void Movie::setPlot(SAString _plot)
{
    plot = _plot;
}
void Movie::setLanguage(std::string _language)
{
    language = _language;
}
void Movie::setLanguage(SAString _language)
{
    language = _language;
}
void Movie::setCountry(std::string _country)
{
    country = _country;
}
void Movie::setCountry(SAString _country)
{
    country = _country;
}
void Movie::setAwards(std::string _awards)
{
    awards = _awards;
}
void Movie::setAwards(SAString _awards)
{
    awards = _awards;
}
void Movie::setPoster(std::string _poster)
{
    poster = _poster;
}
void Movie::setPoster(SAString _poster)
{
    poster = _poster;
}
void Movie::setMetascore(int _metascore)
{
    metascore = _metascore;
}
void Movie::setImdbRating(int _imdbRating)
{
    if (_imdbRating != 0)
    {
        while (_imdbRating > 100)
            _imdbRating /= 10;
        while (_imdbRating < 10)
            _imdbRating *= 10;
    }
    imdbRating = _imdbRating;
}
void Movie::setImdbVotes(int _imdbVotes)
{
    imdbVotes = _imdbVotes;
}
void Movie::setType(std::string _type)
{
    type = _type;
}
void Movie::setType(SAString _type)
{
    type = _type;
    setSerie();
}
void Movie::setImdbId(std::string _imdbID)
{
    imdbId = _imdbID;
}
void Movie::setImdbId(SAString _imdbID)
{
    imdbId = _imdbID;
}
void Movie::setSeparator(std::string _sep)
{
    separator = _sep;
}
std::string Movie::getSeparator(void)
{
    if (separator.size() != 0)
        return separator;
    else
        return DEFAULTSEP;
}
void Movie::copyMovie(Movie _movie) {
    title = _movie.title;
    titleASCII = _movie.titleASCII;
    year = _movie.year;
    rated = _movie.rated;
    runtime = _movie.runtime;
    v_genres = _movie.v_genres;
    for (int role= ROLES_DIRECTOR; role<ROLES_MAX;role++)
        v_roles[role] = _movie.v_roles[role];
    plot = _movie.plot;
    language = _movie.language;
    country = _movie.country;
    awards = _movie.awards;
    poster = _movie.poster;
    metascore = _movie.metascore;
    imdbRating = _movie.imdbRating;
    imdbVotes = _movie.imdbVotes;
    imdbId = _movie.imdbId;
    type = _movie.type;
}

void Movie::updateMovie(Movie _movie) {
    if (_movie.rated.empty() == false)
        rated = _movie.rated;
    if (_movie.runtime.empty() == false)
        runtime = _movie.runtime;
    if (_movie.plot.empty() == false)
        plot = _movie.plot;
    if (_movie.language.empty() == false)
        language = _movie.language;
    if (_movie.country.empty() == false)
        country = _movie.country;
    if (_movie.awards.empty() == false)
        awards = _movie.awards;
    if (_movie.poster.empty() == false)
        poster = _movie.poster;
    if (_movie.imdbId.empty() == false)
        imdbId = _movie.imdbId;
    if (_movie.type.empty() == false)
        type = _movie.type;
    if (_movie.v_genres.size() != 0)
        v_genres = _movie.v_genres;
    for (int role= ROLES_DIRECTOR; role<ROLES_MAX;role++)
    if (_movie.v_roles[role].size() != 0)
        v_roles[role] = _movie.v_roles[role];
    if (_movie.metascore != 0)
        metascore = _movie.metascore;
    if (_movie.imdbRating != 0)
        imdbRating = _movie.imdbRating;
    if (_movie.imdbVotes != 0)
        imdbVotes = _movie.imdbVotes;
}

bool Movie::getTypeIs(movieInfoTypes _type)
{
    return typeIs[_type];
}

size_t Movie::callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata)
{
    FILE* stream = (FILE*)userdata ;
    if (!stream)
    {
        //printf("!!! No stream\n");
        exLOGERROR("No stream");
        return 0;
    }

    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;
}

bool Movie::fileExists(std::string _path)
{
    int error;
    DWORD ftyp = GetFileAttributesA(_path.c_str());
    error = ::GetLastError();
    if (error == INVALID_FILE_ATTRIBUTES)
    {
//        consoleLOG->push_back("fileExists error " + error);
        return false;
    }

//    consoleLOG->push_back("fileExists ftyp " + ftyp);
    return ((ftyp & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY);
}

std::string Movie::url2fileName(std::string _url)
{
    size_t found = _url.find_last_of("/");
    if (found != string::npos)
    {
        //fileName += "\\" + _url.substr(found + 1, string::npos);
        return CACHEPATH + _url.substr(found + 1, string::npos);
    }
    return "";
}
bool Movie::downloadJpeg(string _url, string _fileName)
{
    //DWORD error = ERROR_BAD_PATHNAME;
    //DWORD success = false;
    bool success = false;
    std::string path = CACHEPATH;
    if (_fileName.empty() == false)
    {
        if (BUTIL::Util::folderCreates(path) == false)
        {
            int error = ::GetLastError();
            ERRORMBOX(BUTIL::Util::GetLastErrorAsString(error) + _fileName);
            exLOGERROR("%s - %s", BUTIL::Util::GetLastErrorAsString(error),_fileName.c_str());
            return false;
        }

        FILE* fp = fopen(_fileName.c_str(), "wb");
        if (!fp)
        {
//            consoleLOG->push_back("Failed to create file on the disk " + _fileName);
            exLOGERROR("Failed to create file on the disk : %s", _fileName.c_str());
            return false;
        }

        CURL* curlCtx = curl_easy_init();
        curl_easy_setopt(curlCtx, CURLOPT_URL, _url.c_str());
        curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, &callbackfunction);
        curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);

        CURLcode rc = curl_easy_perform(curlCtx);
        if (rc)
        {
//            consoleLOG->push_back("Failed to download " + _url);
            exLOGERROR("Failed to download (%d) : %s", rc, _url);
            return false;
        }

        long res_code = 0;
        curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
        if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
        {
//            consoleLOG->push_back("Response code " + res_code);
            exLOGERROR("Response code : %d", res_code);
            return false;
        }

        curl_easy_cleanup(curlCtx);

        fclose(fp);

        posterPath = _fileName;
    }

    return true;
}

bool Movie::getJpeg()
{
    if (poster == "N/A")
    {
//        consoleLOG->push_back("Poster N/A");
        return false;
    }
    if (poster == "")
    {
        //        consoleLOG->push_back("Poster N/A");
        return false;
    }
    string fileName = url2fileName(poster);
//    consoleLOG->push_back("fileName : " + fileName);
    if (fileExists(fileName) == true)
    {
//        consoleLOG->push_back("fileExists");
        posterPath = fileName;
        return true;
    }
    if (poster.empty() == false)
    {
//        consoleLOG->push_back("downloadJpeg");
        return downloadJpeg(poster, fileName);
    }
//    else
//        consoleLOG->push_back("poster " + poster);
    return false;
}

std::string Movie::getJpegPath(void)
{
    return posterPath;
}
//bool Movie::getIsAnim()
//{
//    return getTypeIs(TYPE_ANIM);
//}
//bool Movie::getIsSerie()
//{
//    return getTypeIs(TYPE_SERIE);
//}
//bool Movie::getIsDoc()
//{
//    return getTypeIs(TYPE_DOC);
//}
//bool Movie::getIsShort()
//{
//    return getTypeIs(TYPE_SHORT);
//}
