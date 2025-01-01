#pragma once
#include <wx/wx.h>
#include <wx/slider.h>

class FilterDialog : public wxDialog {
public:
    FilterDialog(wxWindow* parent, int& minVal, int& maxVal, std::string& category, const std::vector<std::string>& categories);

private:
    wxSlider* m_minSlider;
    wxSlider* m_maxSlider;
    int& m_minValue;
    int& m_maxValue;

    wxStaticText* m_minValueText;
    wxStaticText* m_maxValueText;

    wxChoice* m_categoryChoice;
    std::string& m_selectedCategory;

    void OnSliderUpdate(wxCommandEvent& evt);
    void OnReset(wxCommandEvent& evt);
    void OnSet(wxCommandEvent& evt);
};
