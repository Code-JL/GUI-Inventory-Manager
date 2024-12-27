#include "MainFrame.h"
#include <wx/persist/toplevel.h>
#include <wx/display.h>

void MainFrame::InitializeItems() {
    m_items.push_back(Item("Sword", "A sharp blade for close combat", 5, "path/to/sword.png"));
    m_items.push_back(Item("Shield", "Protective gear to block attacks", 3, "path/to/shield.png"));
    m_items.push_back(Item("Potion", "Restores 50 HP when consumed", 10, "path/to/potion.png"));
    m_items.push_back(Item("Bow", "Long-range weapon for precise attacks", 2, "path/to/bow.png"));
    m_items.push_back(Item("Arrow", "Ammunition for bow. Deals 25 damage", 50, "path/to/arrow.png"));
}

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    InitializeItems();

    wxDisplay display;
    wxRect screenSize = display.GetGeometry();
    int width = screenSize.GetWidth() * 0.6;
    int height = screenSize.GetHeight() * 0.6;
    SetSize(width, height);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* leftPanel = new wxPanel(this);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    m_listBox = new wxListBox(leftPanel, wxID_ANY);

    // Populate listbox from items
    for (const auto& item : m_items) {
        m_listBox->Append(item.getName());
    }

    leftSizer->Add(m_listBox, 1, wxEXPAND | wxALL, 5);
    leftPanel->SetSizer(leftSizer);
    mainSizer->Add(leftPanel, 20, wxEXPAND);

    wxPanel* rightPanel = new wxPanel(this);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    m_itemCount = new wxStaticText(rightPanel, wxID_ANY, "Quantity: 0");
    rightSizer->Add(m_itemCount, 0, wxALL, 5);

    m_itemTitle = new wxStaticText(rightPanel, wxID_ANY, "Select an item", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxFont titleFont = m_itemTitle->GetFont();
    titleFont.SetPointSize(14);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_itemTitle->SetFont(titleFont);
    rightSizer->Add(m_itemTitle, 0, wxALL | wxEXPAND, 5);

    m_itemDescription = new wxStaticText(rightPanel, wxID_ANY, "Item description will appear here");
    rightSizer->Add(m_itemDescription, 0, wxALL | wxEXPAND, 5);

    rightPanel->SetSizer(rightSizer);
    mainSizer->Add(rightPanel, 80, wxEXPAND);

    SetSizer(mainSizer);

    m_listBox->Bind(wxEVT_LISTBOX, &MainFrame::OnListBoxSelect, this);

    SetName("MainFrame");
    wxPersistentRegisterAndRestore(this, "MainFrame");
}

void MainFrame::OnListBoxSelect(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        const Item& selectedItem = m_items[selection];

        m_itemTitle->SetLabel(selectedItem.getName());
        m_itemDescription->SetLabel(selectedItem.getDescription());
        m_itemCount->SetLabel(wxString::Format("Quantity: %d", selectedItem.getAmount()));
    }
}
