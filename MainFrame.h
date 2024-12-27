#pragma once
#include <wx/wx.h>
#include <wx/listbox.h>
#include "Item.h"
#include "Save.h"
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
    wxTextCtrl* m_itemDescription;
    

    wxButton* m_incrementBtn;
    wxButton* m_decrementBtn;
    wxButton* m_setAmountBtn;
    wxButton* m_setNameBtn;
    wxButton* m_setDescriptionBtn;
    wxButton* m_setImageBtn;

    void OnIncrement(wxCommandEvent& evt);
    void OnDecrement(wxCommandEvent& evt);
    void OnSetAmount(wxCommandEvent& evt);
    void OnSetName(wxCommandEvent& evt);
    void OnSetImage(wxCommandEvent& evt);
    void OnListBoxSelect(wxCommandEvent& evt);

    // Add to private section:
    void OnSave(wxCommandEvent& evt);
    void OnLoad(wxCommandEvent& evt);

    void InitializeItems();

};
