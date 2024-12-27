#pragma once
#include <wx/wx.h>
#include <wx/listbox.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

    // Return a unique name for this window instance
    virtual wxString GetName() const override {
        return "MainFrame";
    }

private:
    wxListBox* m_listBox;
    wxStaticText* m_itemCount;
    wxStaticText* m_itemTitle;
    wxStaticText* m_itemDescription;

    void OnListBoxSelect(wxCommandEvent& evt);

};

