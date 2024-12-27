#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

    // Return a unique name for this window instance
    virtual wxString GetName() const override {
        return "MainFrame";
    }
};

