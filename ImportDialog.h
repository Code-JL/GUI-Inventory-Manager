#pragma once
#include <wx/wx.h>
#include <vector>
#include <string>

class ImportDialog : public wxDialog {
public:
    ImportDialog(wxWindow* parent);
    std::vector<std::string> GetSelectedFiles() const { return m_selectedFiles; }
    bool IsCSVFormat() const { return m_formatChoice->GetSelection() == 0; }

private:
    wxChoice* m_formatChoice;
    wxListBox* m_fileList;
    std::vector<std::string> m_selectedFiles;

    void OnImportFiles(wxCommandEvent& evt);
    void OnClearFiles(wxCommandEvent& evt);
    void OnOK(wxCommandEvent& evt);
};
