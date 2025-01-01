#include "FilterDialog.h"

FilterDialog::FilterDialog(wxWindow* parent, int& minVal, int& maxVal, std::string& category, const std::vector<std::string>& categories)
    : wxDialog(parent, wxID_ANY, "Quantity Filter")
    , m_minValue(minVal)
    , m_maxValue(maxVal)
    , m_selectedCategory(category)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add category filter at the top
    wxBoxSizer* categorySizer = new wxBoxSizer(wxHORIZONTAL);
    categorySizer->Add(new wxStaticText(this, wxID_ANY, "Category:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

    m_categoryChoice = new wxChoice(this, wxID_ANY);
    m_categoryChoice->Append("All Categories");
    for (const auto& cat : categories) {
        m_categoryChoice->Append(cat);
    }
    m_categoryChoice->SetSelection(0);

    categorySizer->Add(m_categoryChoice, 1);
    mainSizer->Add(categorySizer, 0, wxEXPAND | wxALL, 5);

    // Create min slider with value display
    wxBoxSizer* minSizer = new wxBoxSizer(wxHORIZONTAL);
    m_minSlider = new wxSlider(this, wxID_ANY, minVal, 0, 999);
    m_minValueText = new wxStaticText(this, wxID_ANY, wxString::Format("%d", minVal));

    minSizer->Add(new wxStaticText(this, wxID_ANY, "Minimum Quantity:"), 0, wxALIGN_CENTER_VERTICAL);
    minSizer->Add(m_minSlider, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
    minSizer->Add(m_minValueText, 0, wxALIGN_CENTER_VERTICAL);

    // Create max slider with value display
    wxBoxSizer* maxSizer = new wxBoxSizer(wxHORIZONTAL);
    m_maxSlider = new wxSlider(this, wxID_ANY, maxVal, 0, 999);
    m_maxValueText = new wxStaticText(this, wxID_ANY, wxString::Format("%d", maxVal));

    maxSizer->Add(new wxStaticText(this, wxID_ANY, "Maximum Quantity:"), 0, wxALIGN_CENTER_VERTICAL);
    maxSizer->Add(m_maxSlider, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
    maxSizer->Add(m_maxValueText, 0, wxALIGN_CENTER_VERTICAL);

    mainSizer->Add(minSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(maxSizer, 0, wxEXPAND | wxALL, 5);

    // Add buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* resetButton = new wxButton(this, wxID_ANY, "Reset");
    buttonSizer->Add(new wxButton(this, wxID_OK, "Set"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
    buttonSizer->Add(resetButton, 0, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

    SetSizer(mainSizer);

    // Bind events
    m_minSlider->Bind(wxEVT_SLIDER, &FilterDialog::OnSliderUpdate, this);
    m_maxSlider->Bind(wxEVT_SLIDER, &FilterDialog::OnSliderUpdate, this);
    resetButton->Bind(wxEVT_BUTTON, &FilterDialog::OnReset, this);
    Bind(wxEVT_BUTTON, &FilterDialog::OnSet, this, wxID_OK);
}

void FilterDialog::OnSliderUpdate(wxCommandEvent& evt) {
    m_minValueText->SetLabel(wxString::Format("%d", m_minSlider->GetValue()));
    m_maxValueText->SetLabel(wxString::Format("%d", m_maxSlider->GetValue()));
}

void FilterDialog::OnReset(wxCommandEvent& evt) {
    m_minSlider->SetValue(0);
    m_maxSlider->SetValue(999);
    m_categoryChoice->SetSelection(0);
    m_minValueText->SetLabel("0");
    m_maxValueText->SetLabel("999");
}

void FilterDialog::OnSet(wxCommandEvent& evt) {
    m_minValue = m_minSlider->GetValue();
    m_maxValue = m_maxSlider->GetValue();
    m_selectedCategory = m_categoryChoice->GetSelection() == 0 ? "" :
        m_categoryChoice->GetString(m_categoryChoice->GetSelection()).ToStdString();
    EndModal(wxID_OK);
}
