#pragma once
#include <wx/wx.h>
#include <wx/listbox.h>
#include "Item.h"
#include <vector>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

    virtual wxString GetName() const override {
        return "MainFrame";
    }

private:
    std::vector<Item> m_items;
    wxListBox* m_listBox;
    wxStaticText* m_itemCount;
    wxStaticText* m_itemTitle;
    wxStaticText* m_itemDescription;
    
    void OnListBoxSelect(wxCommandEvent& evt);
    void InitializeItems();
};
