#include "CategoryDialog.h"

CategoryDialog::CategoryDialog(wxWindow* parent, std::vector<std::string>& categories)
    : wxDialog(parent, wxID_ANY, "Manage Categories")
    , m_categories(categories)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create category list
    m_categoryList = new wxListBox(this, wxID_ANY);
    for (const auto& category : categories) {
        m_categoryList->Append(category);
    }
    mainSizer->Add(m_categoryList, 1, wxEXPAND | wxALL, 5);

    // Create buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* addBtn = new wxButton(this, wxID_ANY, "Add");
    wxButton* deleteBtn = new wxButton(this, wxID_ANY, "Delete");
    wxButton* renameBtn = new wxButton(this, wxID_ANY, "Rename");
    wxButton* setBtn = new wxButton(this, wxID_ANY, "Set");

    buttonSizer->Add(addBtn, 0, wxALL, 5);
    buttonSizer->Add(deleteBtn, 0, wxALL, 5);
    buttonSizer->Add(renameBtn, 0, wxALL, 5);
    buttonSizer->Add(setBtn, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
    SetSizer(mainSizer);

    // Bind events
    addBtn->Bind(wxEVT_BUTTON, &CategoryDialog::OnAdd, this);
    deleteBtn->Bind(wxEVT_BUTTON, &CategoryDialog::OnDelete, this);
    renameBtn->Bind(wxEVT_BUTTON, &CategoryDialog::OnRename, this);
    setBtn->Bind(wxEVT_BUTTON, &CategoryDialog::OnSet, this);
}

void CategoryDialog::OnAdd(wxCommandEvent& evt) {
    wxString input = wxGetTextFromUser("Enter new category name:", "Add Category");
    if (!input.IsEmpty()) {
        m_categoryList->Append(input);
        m_categories.push_back(input.ToStdString());
    }
}

void CategoryDialog::OnDelete(wxCommandEvent& evt) {
    int selection = m_categoryList->GetSelection();
    if (selection != wxNOT_FOUND) {
        m_categories.erase(m_categories.begin() + selection);
        m_categoryList->Delete(selection);
    }
}

void CategoryDialog::OnRename(wxCommandEvent& evt) {
    int selection = m_categoryList->GetSelection();
    if (selection != wxNOT_FOUND) {
        wxString input = wxGetTextFromUser("Enter new name:", "Rename Category",
            m_categoryList->GetString(selection));
        if (!input.IsEmpty()) {
            m_categoryList->SetString(selection, input);
            m_categories[selection] = input.ToStdString();
        }
    }
}

void CategoryDialog::OnSet(wxCommandEvent& evt) {
    EndModal(wxID_OK);
}
