#include "MainFrame.h"
#include <wx/persist/toplevel.h>
#include <wx/display.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    // Get screen size
    wxDisplay display;
    wxRect screenSize = display.GetGeometry();

    // Calculate 60% of screen dimensions
    int width = screenSize.GetWidth() * 0.6;
    int height = screenSize.GetHeight() * 0.6;

    // Set size, START of MainFrame
    SetSize(width, height);

    // Create main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create left panel with listbox (20% width)
    wxPanel* leftPanel = new wxPanel(this);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    // Create and populate listbox
    m_listBox = new wxListBox(leftPanel, wxID_ANY);
    m_listBox->Append("Sword");
    m_listBox->Append("Shield");
    m_listBox->Append("Potion");
    m_listBox->Append("Bow");
    m_listBox->Append("Arrow");

    leftSizer->Add(m_listBox, 1, wxEXPAND | wxALL, 5);
    leftPanel->SetSizer(leftSizer);
    mainSizer->Add(leftPanel, 20, wxEXPAND);

    // Create right panel (80% width)
    wxPanel* rightPanel = new wxPanel(this);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    // Add counter
    m_itemCount = new wxStaticText(rightPanel, wxID_ANY, "Quantity: 0");
    rightSizer->Add(m_itemCount, 0, wxALL, 5);

    // Add title
    m_itemTitle = new wxStaticText(rightPanel, wxID_ANY, "Select an item", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxFont titleFont = m_itemTitle->GetFont();
    titleFont.SetPointSize(14);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_itemTitle->SetFont(titleFont);
    rightSizer->Add(m_itemTitle, 0, wxALL | wxEXPAND, 5);

    // Add description
    m_itemDescription = new wxStaticText(rightPanel, wxID_ANY, "Item description will appear here");
    rightSizer->Add(m_itemDescription, 0, wxALL | wxEXPAND, 5);

    rightPanel->SetSizer(rightSizer);
    mainSizer->Add(rightPanel, 80, wxEXPAND);

    SetSizer(mainSizer);

    // Bind events
    m_listBox->Bind(wxEVT_LISTBOX, &MainFrame::OnListBoxSelect, this);


    // Enable persistence, END of MainFrame
    SetName("MainFrame");
    wxPersistentRegisterAndRestore(this, "MainFrame");
}


void MainFrame::OnListBoxSelect(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        wxString itemName = m_listBox->GetString(selection);
        m_itemTitle->SetLabel(itemName);

        // Example descriptions
        wxString description;
        if (itemName == "Sword") {
            description = "A sharp blade for close combat";
        }
        else if (itemName == "Shield") {
            description = "Protective gear to block attacks";
        }
        else if (itemName == "Potion") {
            description = "Restores 50 HP when consumed";
        }
        else if (itemName == "Bow") {
            description = "Long-range weapon for precise attacks";
        }
        else if (itemName == "Arrow") {
            description = "Ammunition for bow. Deals 25 damage";
        }

        m_itemDescription->SetLabel(description);
        m_itemCount->SetLabel(wxString::Format("Quantity: %d", selection + 1));  // Example counter
    }
}
