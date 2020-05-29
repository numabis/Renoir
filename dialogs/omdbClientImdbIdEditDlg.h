#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream

#include "omdbClient.h"
#include "database\database.h"
#include "fileSystem\fileSystem.h"
#include "utils\manageXML.h"
#include "utils\log.h"
#include "utils\xml.h"

#define _REFRESH_UI_EVENT_	1000		// Timer de actualización del UI
#define _REFRESH_UI_TIMER	1000		// Tiempo de actualización del UI em ms


class omdbClientImdbIdEdit : public CDialogEx/*, public CListCtrl*/
{
public:
    omdbClientImdbIdEdit(CWnd* pParent /*=NULL*/);
    virtual BOOL OnInitDialog();
//    omdbClientImdbIdEdit(CWnd* pParent = NULL);
    enum { IDD = IDD_IMDBID_EDIT };

    void setImdb(std::string _str);
    
protected:

    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();

private:
    CWnd* m_parent;
    std::string imdbId;
    CEdit *editImdbId;

    void OnInitDialogVars();
    void OnInitDialogValues();
    void OnInitDialogData();
};
