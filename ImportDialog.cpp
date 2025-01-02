#include "ImportDialog.h"

ImportDialog::ImportDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Import Items", wxDefaultPosition, wxSize(400, 300))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Format choice
    wxBoxSizer* formatSizer = new wxBoxSizer(wxHORIZONTAL);
    formatSizer->Add(new wxStaticText(this, wxID_ANY, "Import Format:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_formatChoice = new wxChoice(this, wxID_ANY);
    m_formatChoice->Append("CSV Files");
    m_formatChoice->Append("CFG Files");
    m_formatChoice->SetSelection(0);
    formatSizer->Add(m_formatChoice, 1);
    mainSizer->Add(formatSizer, 0, wxEXPAND | wxALL, 5);

    // File list
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Selected Files:"), 0, wxLEFT | wxRIGHT, 5);
    m_fileList = new wxListBox(this, wxID_ANY);
    mainSizer->Add(m_fileList, 1, wxEXPAND | wxALL, 5);

    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* importBtn = new wxButton(this, wxID_ANY, "Add Files...");
    wxButton* clearBtn = new wxButton(this, wxID_ANY, "Clear Files");
    buttonSizer->Add(importBtn, 0, wxRIGHT, 5);
    buttonSizer->Add(clearBtn);
    mainSizer->Add(buttonSizer, 0, wxALL, 5);

    // OK & Cancel buttons
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);

    // Bind events
    importBtn->Bind(wxEVT_BUTTON, &ImportDialog::OnImportFiles, this);
    clearBtn->Bind(wxEVT_BUTTON, &ImportDialog::OnClearFiles, this);
    Bind(wxEVT_BUTTON, &ImportDialog::OnOK, this, wxID_OK);
}

void ImportDialog::OnImportFiles(wxCommandEvent& evt) {
    wxString wildcard = IsCSVFormat() ?
        "CSV files (*.csv)|*.csv" : "CFG files (*.cfg)|*.cfg";

    wxFileDialog openFileDialog(this, "Select files to import", "", "",
        wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxArrayString paths;
    openFileDialog.GetPaths(paths);

    for (const auto& path : paths) {
        m_selectedFiles.push_back(path.ToStdString());
        m_fileList->Append(path);
    }
}

void ImportDialog::OnClearFiles(wxCommandEvent& evt) {
    m_selectedFiles.clear();
    m_fileList->Clear();
}

void ImportDialog::OnOK(wxCommandEvent& evt) {
    if (m_selectedFiles.empty()) {
        wxMessageBox("Please select at least one file to import.", "No Files Selected",
            wxICON_WARNING | wxOK);
        return;
    }
    evt.Skip();  // Continue with dialog closure
}
