#pragma once

#include <wx/wx.h>
#include <wx/listbox.h>
#include "Item.h"
#include "Save.h"
#include "FilterDialog.h"
#include <vector>

// Main application window class
class MainFrame : public wxFrame {
public:
    // Constructor
    MainFrame(const wxString& title);

    // Override to provide window name for persistence
    virtual wxString GetName() const override {
        return "MainFrame";
    }

private:
    // Data members
    std::vector<Item> m_items;          // Container for inventory items

    // UI Elements - Main display
    wxListBox* m_listBox;               // List of items
    wxStaticText* m_itemCount;          // Display item quantity
    wxStaticText* m_itemTitle;          // Display item name
    wxTextCtrl* m_itemDescription;      // Display/edit item description

    // UI Elements - Buttons
    wxButton* m_incrementBtn;           // Add quantity button
    wxButton* m_decrementBtn;           // Remove quantity button
    wxButton* m_setAmountBtn;           // Set specific quantity button
    wxButton* m_setNameBtn;             // Rename item button
    wxButton* m_setImageBtn;            // Change item image button
    // Search components
    wxTextCtrl* m_searchBox;
    wxButton* m_filterButton;
    int m_minQuantity = 0;
    int m_maxQuantity = 999;


    // Event handlers for item manipulation
    void OnIncrement(wxCommandEvent& evt);
    void OnDecrement(wxCommandEvent& evt);
    void OnSetAmount(wxCommandEvent& evt);
    void OnSetName(wxCommandEvent& evt);
    void OnSetImage(wxCommandEvent& evt);
    void OnListBoxSelect(wxCommandEvent& evt);

    //Event handlers for search
    void OnSearchInput(wxCommandEvent& evt);
    void OnFilterButton(wxCommandEvent& evt);
    void UpdateItemList();

    // Menu bar event handlers
    void OnSave(wxCommandEvent& evt);    // Save inventory to file
    void OnLoad(wxCommandEvent& evt);    // Load inventory from file
    void OnExit(wxCommandEvent& evt);    // Handle application exit
    void OnAbout(wxCommandEvent& evt);   // Show about dialog

    // Initialization
    void InitializeItems();              // Setup initial inventory items
};
