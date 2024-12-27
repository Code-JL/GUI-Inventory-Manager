#pragma once
#include <wx/wx.h>
#include <wx/persist.h>
#include <wx/persist/toplevel.h>

class App : public wxApp {
public:
    virtual bool OnInit();
    virtual int OnExit();
private:
    wxPersistenceManager* m_persistMgr;
};
