#include "variables.h"
#include "movieFolder.h"
#include "database.h"
#include "configManager.h"
#include "Util.h"

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