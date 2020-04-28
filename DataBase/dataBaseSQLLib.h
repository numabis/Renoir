#include "database.h"

std::string SQL_SELECT = "SELECT ";

#define FORMAT                  BUTIL::Util::format
#define FORMATSQL               BUTIL::Util::SQL_format
#define FORMATCSV               BUTIL::Util::SQL_csv

#define TABID(tabColId)          (tabColId/TAB2COL)
#define COLID(tabColId)          (tabColId - TAB2COL*(tabColId/TAB2COL))
#define TABCOL(tabId,colId)      (tabId*TAB2COL + colId)  

#define STRNAME(tabColId)        (tabStruct[TABID(tabColId)][COLID(tabColId)].name)
#define COLNAME(tabColId)        (tabStruct[TABID(tabColId)][COLID(tabColId)].name)
#define COLNAMES(...)            vColIdToCol(vector<short>({__VA_ARGS__}), false)
#define STRCOLUMN(tabColId)      (tabNames[TABID(tabColId)] + "." + tabStruct[TABID(tabColId)][COLID(tabColId)].name)
#define COLUMN(tabColId)         (tabNames[TABID(tabColId)] + "." + tabStruct[TABID(tabColId)][COLID(tabColId)].name)
#define COLUMNS(...)             vColIdToCol(vector<short>({__VA_ARGS__}), true)
#define TABLE(tabId)             tabNames[tabId]
#define SQLTAB(tabId,colId)      tabStruct[tabId][colId]

#define NOCASE                   "COLLATE NOCASE "
#define SELECT(...)              FORMATCSV(string("SELECT "), COLUMNS(__VA_ARGS__)) + " "
#define SELECTALL                SQL_SELECT + " * "
#define COUNTFROM(tab)           SQL_SELECT + " * FROM " + TABLE(tab) + " "
#define FROM(tab)                "FROM " + TABLE(tab) + " "
#define JOINUSING(tab,col)       "LEFT JOIN " + TABLE(tab) + " USING(" + COLNAME(col) + ") "
#define WHERE(filter)            "WHERE " + filter + " "
#define AND(filter)              "AND " + filter + " "
#define LIKE(col,val)            COLUMN(col) + " LIKE '" + val + "' "
#define EQUAL(col,val)           COLUMN(col) + " = '" + val + "' "
#define GREATER(col,val)         COLUMN(col) + " >= '" + STR(val) + "' "
#define LESSER(col,val)          COLUMN(col) + " <= '" + STR(val) + "' "
#define NOTEQUAL(col,val)        COLUMN(col) + " <> '" + STR(val) + "' "
#define NOTEMPTY(col)            COLUMN(col) + " <> ''"
#define INSELECT(col,select)     COLUMN(col) + " IN (" + select + ")"
#define NOTINSELECT(col,select)  COLUMN(col) + " NOT IN (" + select + ")"
#define BETWEEN(col,left,right)  COLUMN(col) + " BETWEEN '" + left + "' AND '" + right + "' "
#define LIMIT(lim)               " LIMIT " + STR(lim) + " "
#define GROUPBY(col,having)      "GROUP BY " + COLUMN(col) + " HAVING count > " + STR(having) + " "
#define GROUPBY12(col,having)    "GROUP BY replace(" + COLUMN(col) + ", '" + STR(TYPE_UNKNOWN) + "', '" + STR(TYPE_MOVIE) + "') HAVING count > " + STR(having) + " "
#define COUNTCOL(col)            ", COUNT(" + COLUMN(col) + ") count "
#define ORDERBY(col,order)       "ORDER BY " + STR(COLID(col)) + " COLLATE NOCASE " + order + " "
#define ORDERBYASC               "ASC"
#define ORDERBYDSC               "DESC"

#define INSERTINTO(tab)          "INSERT INTO  " + TABLE(tab)
#define INSERTCOLS(...)          FORMATCSV("(", COLNAMES({__VA_ARGS__}), "", ") ")
#define INSERTVALS(...)          FORMATCSV("VALUES (", vector<string>({__VA_ARGS__}), "'", ") ")
#define INSERTBULK(...)          FORMATCSV("(", vector<string>({__VA_ARGS__}), "'", ") ")

#define UPDATE(tab)              "UPDATE " + TABLE(tab) + " SET "
//#define SETi(col,val,last)       FORMAT("%s = '%d'%s ", COLNAME(col), val, last)
//#define SET(col,val,last)       FORMAT("%s = '%s'%s ", COLNAME(col), val.c_str(), last)
#define SET(col,val)              COLNAME(col) + " = '" + val +"',"
#ifdef DELETE
#undef DELETE
#endif
#define DELETE(tab)             "DELETE FROM " + TABLE(tab) + " "

#define DROP(tab)               "DROP TABLE " + TABLE(tab) + " "
#define CREATE(tab,def)         "CREATE TABLE [" + TABLE(tab) + "] (" + def + ") "



#define SQLEXEC                     SACommand cursor(&conex, request.c_str(), SA_CmdSQLStmt); cursor.Execute()
#define SQLFETCH                    cursor.FetchNext()
#define SQLFIELDNAME(colName)       cursor.Field(colName)
#define SQLFIELD(tabColId)          cursor.Field((tabStruct[TABID(tabColId)][COLID(tabColId)].name).c_str())
#define SQLCLOSE                    cursor.Close()
