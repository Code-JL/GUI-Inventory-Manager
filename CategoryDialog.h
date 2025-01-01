#pragma once
#include <wx/wx.h>
#include <vector>
#include <string>

class CategoryDialog : public wxDialog {
public:
    CategoryDialog(wxWindow* parent, std::vector<std::string>& categories);

private:
    wxListBox* m_categoryList;
    std::vector<std::string>& m_categories;

    void OnAdd(wxCommandEvent& evt);
    void OnDelete(wxCommandEvent& evt);
    void OnRename(wxCommandEvent& evt);
    void OnSet(wxCommandEvent& evt);
};
