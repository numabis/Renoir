#include "variables.h"
#include "movieFile.h"
#include "database.h"
#include "Util.h"

#pragma region MovieFile

MovieFile::MovieFile()
{
    //moviesFSId = -1;
    //LstPosition = -1;
    //pathId = -1;
    //pathIsSub = false;
    //pathIsHidden = false;
    //size = 0;
    //year = 0;
    //imdbRating = 0;
    //for (int type = TYPE_SERIE; type<TYPE_MAX; type++)
    //    typeIs[type] = TYPE_UNKNOWN;
    resetFile();
}
MovieFile::MovieFile(Movie _movie)
{
    resetFile();
    //moviesFSId = -1;
    LstPosition = 0;
    //size = 0;
    setMovie(_movie);
}

MovieFile::MovieFile(FS_CONFIG *_config)
{
    resetFile();
    fsConfig = _config;
}

void MovieFile::setFsConfig(FS_CONFIG *_config)
{
    fsConfig = _config;
}

bool MovieFile::fileIsMovie()
{
    if (fsConfig == NULL)
        return false;
    if (std::find(fsConfig->extentions.begin(), fsConfig->extentions.end(), ext) != fsConfig->extentions.end())
        return true;
    return false;
}
bool MovieFile::fileIsMovie(std::string _ext)
{
    ext = _ext;
    return fileIsMovie();
}

void MovieFile::resetFile()
{
    resetFolder();
    moviesFSId = -1;
    LstPosition = -1;
    size = 0;
    year = 0;
    imdbRating = 0;
    filename = "";
    title = "";
    imdbId = "";
    for (int type = TYPE_SERIE; type<TYPE_MAX; type++)
        typeIs[type] = TYPE_UNKNOWN;
    resetMovie();
}
void MovieFile::resetMovie()
{
    Movie empty;
    movie.copyMovie(empty);
    movie.setIdMovie(-1);
}
void MovieFile::setMovie(Movie _movie)
{
    movie.copyMovie(_movie);
    this->title = movie.getTitleASCII();
    this->imdbId = movie.getImdbId();
    this->year = movie.getYear();
    this->imdbRating = movie.getImdbRating();
}

void MovieFile::initMovieValue()
{
    movie.setIdMovie(-1);
    movie.setTitle(title);
    movie.setYear(year);
    movie.setImdbId(imdbId);
    movie.setImdbRating(imdbRating);
}
void MovieFile::initMovieValue(std::string* _values, std::string _sep)
{
    movie.initValues(_values, _sep);
    this->title = movie.getTitleASCII();
    this->imdbId = movie.getImdbId();
    this->year = movie.getYear();
    this->imdbRating = movie.getImdbRating();
}
MovieFile::~MovieFile()
{
}

void MovieFile::setFilename(std::string _filename)
{
    filename = _filename;
}
std::string MovieFile::getFilename(void)
{
    return filename;
}
void MovieFile::setExt(std::string _ext)
{
    transform(_ext.begin(), _ext.end(), _ext.begin(), ::tolower);
    ext = _ext;
}
std::string MovieFile::getExt(void)
{
    return ext;
}

Movie* MovieFile::getMovie()
{
    return &movie;
}

void MovieFile::fileNameToDetails()
{
    std::size_t size = this->filename.size();
    if (size == 0)
        return;

    year = 0;

    size_t yearPos = findYear(this->filename, &(this->year));

    std::size_t foundOpen;

    foundOpen = min(this->filename.find_first_of("([{"), yearPos - 1);
    if (foundOpen <= 2 )
        foundOpen = max(this->filename.find_first_of("([{"), yearPos - 1);
    if (foundOpen <= 2)
        foundOpen = size;
    int extPos = this->filename.find_last_of(".");
    if (foundOpen < size)
    {
        this->title = this->filename.substr(0, foundOpen);
    }
    else
    {
        this->title = this->filename.substr(0, extPos);
        //this->title[extPos] = '\0';
    }

    this->ext = this->filename.substr(extPos);
    normalize(&(this->title));
    if (this->title[this->title.size()-1] == ' ')
        this->title.erase(this->title.size()-1);
}

std::string MovieFile::getFullPath(void)
{
    return getPath() + "\\" + filename;
}

std::string MovieFile::getFullPath(std::string _filename)
{
    return getPath() + "\\" + _filename;
}

int MovieFile::findYear(const std::string str, short* _year)
{
    std::size_t findYear;
    std::string::size_type sz;
    findYear = str.find_first_of("12");
    //test-+(qwer)(1234)1985.[3](is).avi
    //0123456789012345678901234567890123456789
    //0         1         2         3         
    while (findYear != std::string::npos)
    {
        std::size_t previous = str.find_last_of(".-_([{", findYear);
        std::size_t next = str.find_first_of(".-_)]}", findYear);

        if (findYear < 3 || previous != findYear - 1 || next != findYear + 4)
        {
            findYear = str.find_first_of("12", findYear + 1);
            continue;
        }
        if (1)
        {
            std::string year = str.substr(findYear, 4);
            if (BUTIL::Util::isYear(year))
            {
                *_year = stoi(year, &sz);
                return findYear;
                //std::size_t foundOpen = _file->filename.find_first_of("(") + 1;
                //_file->title = rtrim_copy(_file->filename.substr(0, foundOpen - 1));
                break;
            }
        }
        findYear = str.find_first_of("12", findYear + 1);

    }
    return std::string::npos;
}

void MovieFile::normalize(std::string *_str)
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

bool MovieFile::isImdbId()
{
    if (imdbId.size() != 0 && imdbId.substr(0, 2) == OMDBTT)
        return true;
    return false;
}

std::string MovieFile::getOmdbXml()
{
    return omdbXml;
}

void MovieFile::setOmdbXml(std::string _omdb, bool _value)
{
    omdbXml = _omdb;
    isOmdbXml = _value;
}
bool MovieFile::isOmdbXmlSetted()
{
    return isOmdbXml;
}

#ifdef USEJSON
std::string getOmdbJson()
{
    return omdbJSON;
}
void setOmdbJson(std::string _json)
{
    omdbJSON = _json;
}
#endif

#pragma endregion MovieFile

#pragma region MovieFolder
MovieFolder::MovieFolder(std::string _str)
{
    resetFolder();
    path = _str;
}
MovieFolder::MovieFolder(void)
{
    resetFolder();
}
MovieFolder::MovieFolder(std::string _path, bool _isRead)
{
    path = _path;
    id = -1;
    read = _isRead;
    subfolder = false;
    hide = false;
}
MovieFolder::MovieFolder(std::string _path, int _id, bool _isRead, bool _isSub, bool _hide)
{
    path = _path;
    id = _id;
    read = _isRead;
    subfolder = _isSub;
    hide = _hide;
}
MovieFolder::~MovieFolder(void)
{

}
void MovieFolder::resetFolder(void)
{
    path = "";
    id = -1;
    subfolder = false;
    hide = false;
    read = false;
    for (int type = TYPE_SERIE; type<TYPE_MAX; type++)
        typeIs[type] = TYPE_UNKNOWN;
}
void MovieFolder::setFolder(MovieFile _file)
{
    id = _file.id;
    path = _file.path;
    read = _file.read;
    hide = _file.hide;
    subfolder = _file.subfolder;
}
void MovieFolder::setFolder(MovieFolder _folder)
{
    id = _folder.id;
    path = _folder.path;
    read = _folder.read;
    hide = _folder.hide;
    subfolder = _folder.subfolder;
}
void MovieFolder::setPath(std::string _str)
{
    transform(_str.begin(), _str.end(), _str.begin(), ::tolower);
    if (_str[_str.size() - 1] != '\\')
        _str += "\\";
    path = _str;
}
void MovieFolder::setIdPath(short _id)
{
    id = _id;
}
void MovieFolder::setIsSub(bool _is)
{
    subfolder = _is;
}
void MovieFolder::setIsHide(bool _is)
{
    hide = _is;
}
void MovieFolder::setIsRead(bool _is)
{
    read = _is;
}

std::string MovieFolder::getPath(void)
{
    return path;
}
short MovieFolder::getIdPath(void)
{
    return id;
}
bool MovieFolder::isSub(void)
{
    return subfolder;
}
bool MovieFolder::isHide(void)
{
    return hide;
}
bool MovieFolder::isRead(void)
{
    return read;
}

int MovieFolder::pathCompare(std::string _str)
{
    return path.compare(_str);
}
bool MovieFolder::guessType(std::string _type)
{
    size_t pos = path.find(_type);
    if (pos > 0 && pos != std::string::npos)
        return true;
    return false;
}
void MovieFolder::guessType(void)
{
    for (int t = 0; t < TYPE_MAX; t++)
    {
        typesValues type = (typesValues)t;
        size_t pos = path.find(type);
        if (pos > 0 && pos != std::string::npos)
            typeIs[type] = TYPE_NOTMOVIE;
        else
            typeIs[type] = TYPE_MOVIE;
    }
}
bool MovieFolder::isPathEmpty()
{
    return path.empty();
}
bool MovieFolder::updateIsRead(FOLDER_STAT)
{
    //DataBase::getInstance().PATHFS_insert(this);
    return false;
}
int MovieFolder::updateDB()
{
    return GETDB.PATHFS_update(this);
}
int MovieFolder::insertDB()
{
    return GETDB.PATHFS_insert(this);
}
#pragma endregion MovieFolder